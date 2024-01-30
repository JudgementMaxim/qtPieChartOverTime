#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "form.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void applyBaseChart(QChart *chart);  // Change the parameter to a pointer

    ~Widget();

private:
    Form *myForm;
    QChartView *view;
    void applyBaseChart();

protected:


    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};
#endif // WIDGET_H
