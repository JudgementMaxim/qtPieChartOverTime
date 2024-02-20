#ifndef BULLETPIECHART_H
#define BULLETPIECHART_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QDate>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>  // Include QChart
#include "filelogger.h"

class BulletPieChart : public QObject
{
    Q_OBJECT
private:
    QJsonDocument jsonDoc;
    FileLogger logger;


public:
    QPieSeries *series = new QPieSeries();
    QChart *chart = new QChart();
    QString filename = "bulletpiechart.cpp";

    explicit BulletPieChart(QObject *parent = nullptr);
    float gOtFS(QString seller);            // getOvertimeFromSeller
    float gSOt();                           // getSumOverTime
    float gMOtFS(QString seller, int month); // getMonthlyOvertimeFromSeller
    QStringList getSellers();
    void printSeriesContents(QPieSeries* series);
    QPieSeries* creatIndividualSeries(QString seller);


    QJsonDocument openJSON();

    QPieSeries *createBaseSeries();
signals:
};

#endif // BULLETPIECHART_H
