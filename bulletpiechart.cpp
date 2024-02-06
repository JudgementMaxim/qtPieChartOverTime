#include "bulletpiechart.h" // Einschließen der Header-Datei für die Definition der BulletPieChart-Klasse
#include <QFile> // Einschließen der Header-Datei für die Datei-Ein-/Ausgabe
#include <QDir> // Einschließen der Header-Datei für das Arbeiten mit Verzeichnissen

BulletPieChart::BulletPieChart(QObject *parent) : QObject(parent), logger("bulletpiechart.cpp")
{
    openJSON(); // Beim Erstellen eines BulletPieChart-Objekts wird die Methode openJSON() aufgerufen
}

// Methode zur Berechnung der Gesamtüberstunden eines Verkäufers
float BulletPieChart::gOtFS(QString seller)
{
    QJsonDocument jsonDoc = openJSON(); // Öffnen der JSON-Datei und Laden des Inhalts
    int overtime = 0; // Variable zur Speicherung der Überstunden

    QJsonArray jsonArray = jsonDoc.array(); // Konvertierung des JSON-Dokuments in ein JSON-Array

    // Iteration durch jedes JSON-Objekt im Array
    for (const QJsonValue& sellersValue : jsonArray) {
        QJsonObject entry = sellersValue.toObject(); // Konvertierung des JSON-Werts in ein JSON-Objekt
        QString sellerSearched = entry["Verkaeufer"].toString(); // Extrahieren des Verkäufers aus dem Eintrag

        // Wenn der Verkäufer mit dem gesuchten Verkäufer übereinstimmt, werden die Überstunden addiert
        if (sellerSearched == seller) {
            int overtimeTemp = entry["Arbeitszeit"].toInt(); // Extrahieren der Arbeitszeit aus dem Eintrag
            overtime += overtimeTemp; // Addieren der Arbeitszeit zu den Überstunden
        }
    }

    return overtime / 60.0; // Rückgabe der Gesamtüberstunden in Stunden
}

// Methode zur Berechnung der Gesamtüberstunden aller Verkäufer
float BulletPieChart::gSOt()
{
    int overtime = 0; // Variable zur Speicherung der Gesamtüberstunden
    QJsonDocument jsonDoc = openJSON(); // Öffnen der JSON-Datei und Laden des Inhalts
    QJsonArray jsonArray = jsonDoc.array(); // Konvertierung des JSON-Dokuments in ein JSON-Array

    // Iteration durch jedes JSON-Objekt im Array
    for (const QJsonValue& sellersValue : jsonArray) {
        QJsonObject entry = sellersValue.toObject(); // Konvertierung des JSON-Werts in ein JSON-Objekt
        int overtimeIndividual = entry["Arbeitszeit"].toInt(); // Extrahieren der Arbeitszeit aus dem Eintrag
        overtime += overtimeIndividual; // Addieren der Arbeitszeit zu den Gesamtüberstunden
    }

    return overtime / 60.0; // Rückgabe der Gesamtüberstunden in Stunden
}

// Methode zur Berechnung der Monatsüberstunden eines bestimmten Verkäufers
float BulletPieChart::gMOtFS(QString seller, int month)
{
    int overtime = 0; // Variable zur Speicherung der Monatsüberstunden

    QJsonDocument jsonDoc = openJSON(); // Öffnen der JSON-Datei und Laden des Inhalts
    QJsonArray jsonArray = jsonDoc.array(); // Konvertierung des JSON-Dokuments in ein JSON-Array

    // Iteration durch jedes JSON-Objekt im Array
    for (const QJsonValue& sellersValue : jsonArray) {
        QJsonObject entry = sellersValue.toObject(); // Konvertierung des JSON-Werts in ein JSON-Objekt
        QString sellerSearched = entry["Verkaeufer"].toString(); // Extrahieren des Verkäufers aus dem Eintrag
        QString dateString = entry["Datum"].toString(); // Extrahieren des Datums aus dem Eintrag
        QDate date = QDate::fromString(dateString, "yyyy-MM-dd"); // Konvertierung des Datums in ein QDate-Objekt
        int monthSearched = date.month(); // Extrahieren des Monats aus dem Datum

        // Wenn der Verkäufer und der Monat übereinstimmen, werden die Überstunden addiert
        if (sellerSearched == seller && monthSearched == month) {
            int overtimeTemp = entry["Arbeitszeit"].toInt(); // Extrahieren der Arbeitszeit aus dem Eintrag
            overtime += overtimeTemp; // Addieren der Arbeitszeit zu den Monatsüberstunden
        }
    }

    return overtime / 60.0; // Rückgabe der Monatsüberstunden in Stunden
}

// Methode zum Abrufen einer Liste aller Verkäufer
QStringList BulletPieChart::getSellers()
{
    QStringList sellers; // Liste zur Speicherung der Verkäufer
    QJsonDocument jsonDoc = openJSON(); // Öffnen der JSON-Datei und Laden des Inhalts
    QJsonArray jsonArray = jsonDoc.array(); // Konvertierung des JSON-Dokuments in ein JSON-Array

    // Iteration durch jedes JSON-Objekt im Array
    for (const QJsonValue& sellersValue : jsonArray) {
        QJsonObject entry = sellersValue.toObject(); // Konvertierung des JSON-Werts in ein JSON-Objekt
        QString sellerSearched = entry["Verkaeufer"].toString(); // Extrahieren des Verkäufers aus dem Eintrag

        // Wenn der Verkäufer noch nicht in der Liste enthalten ist, wird er hinzugefügt
        if (!sellers.contains(sellerSearched)) {
            sellers.append(sellerSearched); // Hinzufügen des Verkäufers zur Liste
        }
    }

    return sellers; // Rückgabe der Liste der Verkäufer
}

// Methode zum Öffnen der JSON-Datei und Laden des Inhalts
QJsonDocument BulletPieChart::openJSON()
{
    QFile dataJSON("datensaetze.json"); // Erstellen eines QFile-Objekts mit dem Dateinamen

    // Überprüfen, ob die JSON-Datei erfolgreich geöffnet wurde
    if (!dataJSON.open(QIODevice::ReadOnly)) {
        logger.log("Failed to open JSON file. Error: " + dataJSON.errorString(),filename); // Fehlermeldung protokollieren
        logger.log("Current Working Directory: " + QDir::currentPath(),filename); // Protokollieren des aktuellen Arbeitsverzeichnisses
        return QJsonDocument(); // Rückgabe eines leeren JSON-Dokuments im Fehlerfall
    }

    QByteArray data = dataJSON.readAll(); // Lesen des gesamten Inhalts der Datei
    jsonDoc = QJsonDocument::fromJson(data); // Konvertierung der Daten in ein JSON-Dokument

    dataJSON.close(); // Schließen der Datei nach dem Lesen

    return jsonDoc; // Rückgabe des JSON-Dokuments
}

// Methode zum Erstellen eines Diagramms für die Überstunden eines bestimmten Verkäufers pro Monat
QChart* BulletPieChart::creatIndividualChart(QString seller)
{
    series->clear(); // Löschen der vorhandenen Datenreihen im Diagramm

    // Iteration durch jeden Monat
    for (int i = 0; i < 12; i++) {
        float hours = gMOtFS(seller, i); // Abrufen der Monatsüberstunden für den Verkäufer
        logger.log("Seller: " + seller + ", Month: " + QString::number(i + 1) + ", Hours: " + QString::number(hours),filename); // Protokollieren der Daten
        series->append(QString::number(i + 1), hours); // Hinzufügen der Daten zur Datenreihe
    }

    chart->addSeries(series); // Hinzufügen der Datenreihe zum Diagramm
    chart->setTitle("Overtime Hours per Month for " + seller); // Festlegen des Diagrammtitels

    return chart; // Rückgabe des erstellten Diagramms
}

// Methode zum Protokollieren der Inhalte einer Datenreihe
void BulletPieChart::printSeriesContents(QPieSeries* series) {
    logger.log("Series Contents:",filename); // Protokollieren der Überschrift

    // Iteration durch jede Kuchenscheibe in der Datenreihe
    for (const auto& slice : series->slices()) {
        logger.log("Label: " + slice->label() + ", Value: " + QString::number(slice->value()),filename); // Protokollieren der Kuchenscheibe
    }
}

// Methode zum Erstellen eines Basisdiagramms für die Gesamtüberstunden pro Verkäufer
QChart* BulletPieChart::createBaseChart()
{
    series->clear(); // Löschen der vorhandenen Datenreihen im Diagramm
    QStringList sellers = getSellers(); // Abrufen der Liste aller Verkäufer
    int labelIndex = 0; // Index für die Etiketten im Diagramm

    // Iteration durch jeden Verkäufer
    for (const QString& seller : sellers) {
        float hours = gOtFS(seller); // Abrufen der Gesamtüberstunden für den Verkäufer
        logger.log("Seller: " + seller + ", Hours: " + QString::number(hours),filename); // Protokollieren der Daten

        series->append(seller, hours); // Hinzufügen der Daten zur Datenreihe
        labelIndex++; // Inkrementieren des Index für die Etiketten
    }

    printSeriesContents(series); // Protokollieren der Inhalte der Datenreihe

    chart->removeAllSeries(); // Entfernen aller vorhandenen Datenreihen aus dem Diagramm
    chart->addSeries(series); // Hinzufügen der Datenreihe zum Diagramm
    chart->createDefaultAxes(); // Erstellen der Standardachsen
    chart->setTitle("Overtime Hours per Person"); // Festlegen des Diagrammtitels

    return chart; // Rückgabe des erstellten Diagramms
}
