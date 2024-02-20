// widget.h

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "form.h"
#include "bulletpiechart.h"
#include "filelogger.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void applyBaseChart(QChart *chart); // Change the parameter to a pointer

    ~Widget();

    void clearChartView(QChart *chart);
private:
    Form *myForm;
    QChartView *view;
    BulletPieChart bpc;
    QString filename = "widget.cpp";
    void applyBaseChart();
    void clearChartView(QChart *chart, FileLogger &logger); // Pass logger as a parameter
    FileLogger logger;



private slots:
    void handleSliceClicked(QPieSlice *slice);
    void handleSliceHovered(QPieSlice *slice);


protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // WIDGET_H
