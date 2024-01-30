// widget.cpp
#include "widget.h"
#include "bulletpiechart.h"
#include "form.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    myForm = new Form(this);

    // Assuming you have a QChart object created in your BulletPieChart class
    BulletPieChart bpc;
    QChart* chart = bpc.createBaseChart();

    view = new QChartView(chart);
    applyBaseChart(); // Apply additional settings to the chart

    view->setParent(myForm);
}

void Widget::applyBaseChart()
{

    QChart *chart = view->chart();

    chart->setTitle("My Chart");

    chart->legend()->setVisible(true);
}

Widget::~Widget()
{
    delete myForm;
    // The QChartView and QChart are automatically deleted when myForm is deleted,
    // since they are set as children of myForm.
}
