#include "widget.h"

#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent), view(nullptr), logger("logfile.txt")
{
    resize(400, 900);
    setWindowTitle("Kuchendiagram");

    logger.log("Widget constructor called.", filename);

    QPieSeries *series = new QPieSeries;
    series = bpc.createBaseSeries();

    QChart *chart = new QChart;
    chart->addSeries(series);
    if (!chart) {
        logger.log("Error: Failed to create base chart.", filename);
        return;
    }
    logger.log("Base chart created successfully.", filename);

    chart->setAnimationOptions(QChart::AllAnimations);

    view = new QChartView(chart, this);
    if (!view) {
        logger.log("Error: Failed to create chart view.", filename);
        delete chart;
        return;
    }
    logger.log("Chart view created successfully.", filename);

    applyBaseChart(chart);

    view->setRenderHint(QPainter::Antialiasing);

    connect(series, &QPieSeries::clicked, this, &Widget::handleSliceClicked);
    connect(series, &QPieSeries::hovered, this, &Widget::handleSliceHovered);




    logger.log("Widget constructor completed.", filename);
}

void Widget::applyBaseChart(QChart *chart)
{
    if (!chart) {
        logger.log("Error: Invalid chart pointer in applyBaseChart().", filename);
        return;
    }

    logger.log("Applying settings to the base chart.", filename);

    chart->setTitle(chart->title());
    chart->legend()->setVisible(true);
    chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Widget::handleSliceClicked(QPieSlice *slice)
{
    if (!view || !slice) {
        logger.log("Error: Invalid view or slice pointer in handleSliceClicked().", filename);
        return;
    }

    QString category = slice->label();
    qreal value = slice->value();

    logger.log("Clicked on slice: " + category + ", Value: " + QString::number(value), filename);

    clearChartView(view->chart());

    logger.log("Creating individual chart for category: " + category, filename);
    QPieSeries *individualSeries = new QPieSeries;
    individualSeries = bpc.creatIndividualSeries(category);

    QChart *individualChart = new QChart;
    individualChart->addSeries(individualSeries);
    if (!individualChart) {
        logger.log("Error: Failed to create individual chart.", filename);
        return;
    }

    QList<QAbstractSeries*> seriesList = individualChart->series();
    for (QAbstractSeries *series : seriesList) {
        view->chart()->addSeries(series);
    }
    connect(individualSeries, &QPieSeries::hovered, this, &Widget::handleSliceHovered);

    logger.log("New series added to the chart.", filename);
}

void Widget::handleSliceHovered(QPieSlice *slice)
{
    slice->setExploded(!slice->isExploded());
    slice->setLabelVisible(!slice->isLabelVisible());
}



void Widget::resizeEvent(QResizeEvent *event)
{
    if (!view) {
        logger.log("Error: Invalid view pointer in resizeEvent().", filename);
        return;
    }

    logger.log("Resizing event triggered.", filename);

    view->resize(size());
}

void Widget::clearChartView(QChart *chart)
{
    if (!chart) {
        logger.log("Error: Invalid chart pointer in clearChartView().", filename);
        return;
    }

    logger.log("Clearing chart view...", filename);

    QList<QAbstractSeries*> seriesList = chart->series();
    for (QAbstractSeries *series : seriesList) {
        chart->removeSeries(series);
        delete series;
    }

    logger.log("Chart view cleared.", filename);
}

Widget::~Widget()
{
    logger.log("Widget destructor called.", filename);
    delete myForm;
}
