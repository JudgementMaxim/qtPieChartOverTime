#ifndef BULLETPIECHART_H
#define BULLETPIECHART_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QDate>

class BulletPieChart : public QObject
{
    Q_OBJECT
private:
    QJsonDocument jsonDoc;
public:
    explicit BulletPieChart(QObject *parent = nullptr);
    float gOtFS(QString seller);            //getOvertimeFromSeller
    float gSOt();                           //getSumOverTime
    float gMOtFS(QString seller,int month); //getMonthlyOvertimeFromSeller
    QStringList getSellers();

    QJsonDocument openJSON();
signals:

};

#endif // BULLETPIECHART_H
