// widget.h
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "form.h"
#include <QtCharts/QChartView>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void applyBaseChart(); // No need to pass QChart as a parameter

private:
    Form *myForm;
    QChartView *view;
};

#endif // WIDGET_H
