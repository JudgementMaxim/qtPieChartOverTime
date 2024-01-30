// widget.cpp
#include "widget.h"
#include "bulletpiechart.h"
#include "form.h"
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{


    // Assuming you have a QChart object created in your BulletPieChart class
    BulletPieChart bpc;
    QChart* chart = bpc.creatIndividualChart("Jane Doe");

    view = new QChartView(chart,this);
    applyBaseChart(); // Apply additional settings to the chart

    // Set parent for view



}

void Widget::applyBaseChart()
{
    view->chart()->setTitle(view->chart()->title());
    view->chart()->legend()->setVisible(true);

    // Set size policy for the chart to expand in both directions
    view->chart()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


}

void Widget::resizeEvent(QResizeEvent *event)
{
    view->resize(size());
}

Widget::~Widget()
{
    delete myForm;
    // The QChartView and QChart are automatically deleted when myForm is deleted,
    // since they are set as children of myForm.
}
