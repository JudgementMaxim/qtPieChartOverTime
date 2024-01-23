#include "bulletpiechart.h"
#include <QFile>

BulletPieChart::BulletPieChart(QObject *parent) : QObject(parent)
{
    openJSON();


}

float BulletPieChart::gOtFS(QString seller)
{
    QJsonDocument jsonDoc = openJSON();
    QJsonArray jsonArray = jsonDoc.array();


    for(const QJsonValue& sellersValue : jsonArray){
        QJsonObject entry = sellersValue.toObject();
        QString sellerSearched = entry["Verkaeufer"].toString();

        if(sellerSearched == seller){
            return  entry["Arbeitszeit"].toInt()/60;
        }
    }
}

float BulletPieChart::gSOt()
{
    int overtime = 0;
    QJsonDocument jsonDoc = openJSON();
    QJsonArray jsonArray = jsonDoc.array();


    for(const QJsonValue& sellersValue : jsonArray){
        QJsonObject entry = sellersValue.toObject();
        int overtimeIndividual = entry["Arbeitszeit"].toInt();

        overtime += overtimeIndividual;
    }
    return overtime/60;
}

float BulletPieChart::gMOtFS(QString seller, int month)
{
    QJsonDocument jsonDoc = openJSON();
    QJsonArray jsonArray = jsonDoc.array();


    for(const QJsonValue& sellersValue : jsonArray){
        QJsonObject entry = sellersValue.toObject();
        QString sellerSearched = entry["Verkaeufer"].toString();
        QString dateString  = entry["Datum"].toString();
        QDate date = QDate::fromString(dateString, "yyyy-MM-dd");
        int monthSearched = date.month();

        if(sellerSearched == seller && monthSearched == month){
            return  entry["Arbeitszeit"].toInt()/60;
        }
    }
}

QStringList BulletPieChart::getSellers()
{
    QStringList sellers;

    QJsonDocument jsonDoc = openJSON();
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue& sellersValue : jsonArray) {
        QJsonObject entry = sellersValue.toObject();
        QString sellerSearched = entry["Verkaeufer"].toString();

        // Check if the seller is not already in the list before appending
        if (!sellers.contains(sellerSearched)) {
            sellers.append(sellerSearched);
        }
    }

    return sellers;
}

QJsonDocument BulletPieChart::openJSON()
{
    QFile dataJSON("datensaetze.json");
    dataJSON.open(QIODevice::ReadOnly);

    QByteArray data = dataJSON.readAll();
    jsonDoc = QJsonDocument::fromJson(data);

    dataJSON.close();

    return jsonDoc;
}
