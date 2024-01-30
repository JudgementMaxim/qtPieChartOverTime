#include "bulletpiechart.h"
#include <QFile>
#include <QDir>



BulletPieChart::BulletPieChart(QObject *parent) : QObject(parent)
{
    // Call the function to open the JSON file and store its content in jsonDoc
    openJSON();
}

float BulletPieChart::gOtFS(QString seller)
{
    // Retrieve the JSON document from the file
    QJsonDocument jsonDoc = openJSON();
    int overtime = 0;
    int overtimeTemp;

    // Extract the array of entries from the JSON document
    QJsonArray jsonArray = jsonDoc.array();

    // Iterate through each entry in the array
    for (const QJsonValue& sellersValue : jsonArray) {
        QJsonObject entry = sellersValue.toObject();
        QString sellerSearched = entry["Verkaeufer"].toString();

        // Check if the current entry matches the specified seller
        if (sellerSearched == seller) {
            // Accumulate the working hours for the matched seller (converted to minutes)
            //qDebug() << "Minuten:" <<entry["Arbeitszeit"].toInt();
            overtimeTemp = entry["Arbeitszeit"].toInt();
            overtime += overtimeTemp;
            //qDebug() << overtime;
        } else {
            // This will return 0 on the first iteration if the seller doesn't match,
            // consider revising this logic based on your requirements
            //qDebug() <<  seller <<" Verkäufer nicht gefunden. " << sellerSearched << " Vergleichswert";
        }
    }
    // Return the total accumulated overtime hours (converted to hours)
    return overtime / 60.0;
}

float BulletPieChart::gSOt()
{
    int overtime = 0;
    QJsonDocument jsonDoc = openJSON();
    QJsonArray jsonArray = jsonDoc.array();

    // Iterate through each entry in the array
    for (const QJsonValue& sellersValue : jsonArray) {
        QJsonObject entry = sellersValue.toObject();
        int overtimeIndividual = entry["Arbeitszeit"].toInt();

        // Sum up the individual overtime hours
        overtime += overtimeIndividual;
    }

    // Return the total overtime hours (converted to hours)
    return overtime / 60.0;
}

float BulletPieChart::gMOtFS(QString seller, int month)
{
    int overtime = 0;
    int overtimeTemp;

    QJsonDocument jsonDoc = openJSON();
    QJsonArray jsonArray = jsonDoc.array();

    // Iterate through each entry in the array
    for (const QJsonValue& sellersValue : jsonArray) {
        QJsonObject entry = sellersValue.toObject();
        QString sellerSearched = entry["Verkaeufer"].toString();
        QString dateString = entry["Datum"].toString();
        QDate date = QDate::fromString(dateString, "yyyy-MM-dd");
        int monthSearched = date.month();

        // Check if the current entry matches the specified seller and month
        if (sellerSearched == seller && monthSearched == month) {
            // Accumulate the working hours for the matched seller (converted to minutes)
            qDebug() << "Minuten:" <<entry["Arbeitszeit"].toInt();
            overtimeTemp = entry["Arbeitszeit"].toInt();
            overtime += overtimeTemp;
            //qDebug() << overtime;
        }else if(sellerSearched == seller) {
            //qDebug() << "Verkäufer " << seller << " gefunden aber nicht monat " << month;
        }
        else if(monthSearched == month) {
            //qDebug() << "Verkäufer " << seller << " nicht gefunden aber monat " << month;
        }
        else{
            //qDebug() << "Verkäufer " << seller << " nicht gefunden und nicht monat " << month;
        }
    }

    // Return the total accumulated overtime hours for the specified seller and month (converted to hours)
    return overtime / 60.0;
}

QStringList BulletPieChart::getSellers()
{
    QStringList sellers;

    // Retrieve the JSON document from the file
    QJsonDocument jsonDoc = openJSON();

    // Extract the array of entries from the JSON document
    QJsonArray jsonArray = jsonDoc.array();

    // Iterate through each entry in the array
    for (const QJsonValue& sellersValue : jsonArray) {
        QJsonObject entry = sellersValue.toObject();
        QString sellerSearched = entry["Verkaeufer"].toString();

        // Check if the seller is not already in the list before appending
        if (!sellers.contains(sellerSearched)) {
            sellers.append(sellerSearched);
        }
    }

    // Return the list of unique sellers
    return sellers;
}



QJsonDocument BulletPieChart::openJSON()
{
    // Open the JSON file in read-only mode
    QFile dataJSON("datensaetze.json");

    if (!dataJSON.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open JSON file. Error: " << dataJSON.errorString();
        qDebug() << "Current Working Directory: " << QDir::currentPath();
        // Handle the error or return an empty QJsonDocument
        return QJsonDocument();
    }

    // Read the contents of the file into a QByteArray
    QByteArray data = dataJSON.readAll();

    // Parse the JSON data into a QJsonDocument
    jsonDoc = QJsonDocument::fromJson(data);

    // Close the JSON file
    dataJSON.close();

    // Return the parsed JSON document
    return jsonDoc;
}

// ... (existing code)

QChart* BulletPieChart::creatIndividualChart(QString seller)
{
    series->clear(); // Clear existing data in the series

    for (int i = 0; i < 12; i++) {
        float hours = gMOtFS(seller, i);
        qDebug() << "Seller: " << seller << ", Month: " << i + 1 << ", Hours: " << hours; // Check data
        series->append(QString::number(i + 1), hours); // Assuming month is represented by numbers 1 to 12
    }

    chart->addSeries(series);
    chart->setTitle("Overtime Hours per Month for " + seller);

    return chart;
}

void BulletPieChart::printSeriesContents(QPieSeries* series) {
    qDebug() << "Series Contents:";
    for (const auto& slice : series->slices()) {
        qDebug() << "Label:" << slice->label() << ", Value:" << slice->value();
    }
}

QChart* BulletPieChart::createBaseChart()
{
    series->clear(); // Clear existing data in the series

    QStringList sellers = getSellers();

    int labelIndex = 0; // Counter for unique labels

    for (const QString& seller : sellers) {
        float hours = gOtFS(seller);
        qDebug() << "Seller: " << seller << ", Hours: " << hours; // Check data

        // Append slice with a unique label
        series->append(seller, hours);
        labelIndex++;
    }

    printSeriesContents(series);
    //aus irgendeinem grund wird immer die daten - 1 angezeigt
    //und chart anzeigen klappt immernoch nicht
    // Clear the existing chart
    chart->removeAllSeries();
    chart->addSeries(series);

    // Set up axes
    chart->createDefaultAxes();

    // Set chart title
    chart->setTitle("Overtime Hours per Person");

    return chart;
}





