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

class BulletPieChart : public QObject
{
    Q_OBJECT
private:
    QJsonDocument jsonDoc;

    void createBaseChart();

    void creatIndividualChart(QString seller);
public:
//    QPieSeries *series = new QPieSeries();
//    QChart *chart = new QChart();
//    QChartView *view = new QChartView(chart);

    explicit BulletPieChart(QObject *parent = nullptr);
    float gOtFS(QString seller);            // getOvertimeFromSeller
    float gSOt();                           // getSumOverTime
    float gMOtFS(QString seller, int month); // getMonthlyOvertimeFromSeller
    QStringList getSellers();

    QJsonDocument openJSON();

signals:
};

#endif // BULLETPIECHART_H
