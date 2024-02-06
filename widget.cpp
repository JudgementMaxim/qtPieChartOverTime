// widget.cpp
#include "widget.h"
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    resize(400, 900);

    // Assuming you have a QChart object created in your BulletPieChart class
    QChart *chart = bpc.createBaseChart();
    chart->setAnimationOptions(QChart::AllAnimations);

    view = new QChartView(chart, this);
    applyBaseChart(); // Apply additional settings to the chart

    connect(static_cast<QPieSeries *>(chart->series().at(0)), &QPieSeries::clicked, this, &Widget::handleSliceClicked);
}

void Widget::applyBaseChart()
{
    view->chart()->setTitle(view->chart()->title());
    view->chart()->legend()->setVisible(true);

    // Set size policy for the chart to expand in both directions
    view->chart()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Widget::handleSliceClicked(QPieSlice *slice)
{
    // Slot to handle the clicked signal
    if (slice) {
        // Access information about the clicked slice
        QString category = slice->label();
        qreal value = slice->value();

        // Print or use the information as needed
        qDebug() << "Clicked on slice: " << category << ", Value: " << value;

        // Clear the existing chart view
        clearChartView(view);

        // Create a new individual chart based on the clicked slice's category
        QChart *individualChart = bpc.creatIndividualChart(category);
        if (individualChart) {  // Check if the individual chart is valid
            // Set the new chart to the view
            view->setChart(individualChart);
            applyBaseChart();
            return;  // Exit the function to avoid potential issues
        }

        // Handle error conditions if the chart creation fails
        qDebug() << "Error: Unable to create individual chart.";
    }
}



void Widget::resizeEvent(QResizeEvent *event)
{
    view->resize(size());
}

void Widget::clearChartView(QChartView *chartView)
{
    QChart *chart = chartView->chart();

    // Remove all existing series from the chart
    QList<QAbstractSeries *> allSeries = chart->series();
    for (QAbstractSeries *series : allSeries)
    {
        chart->removeSeries(series);
        delete series; // Optional, if you want to delete the series
    }
}

Widget::~Widget()
{
    delete myForm;
    // The QChartView and QChart are automatically deleted when myForm is deleted,
    // since they are set as children of myForm.
}
