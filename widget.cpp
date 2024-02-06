#include "widget.h" // Inkludiere die Header-Datei für das Widget

// Inkludiere notwendige Header-Dateien
#include <QVBoxLayout>

// Konstruktor für das Widget
Widget::Widget(QWidget *parent)
    : QWidget(parent), view(nullptr), logger("logfile.txt") // Initialisiere Member-Variablen
{
    resize(400, 900); // Setze die Größe des Widgets

    // Erstelle ein Basisdiagramm (Base Pie Chart)
    QChart *chart = bpc.createBaseChart();
    if (!chart) { // Überprüfe, ob das Diagramm erstellt wurde
        logger.log("Error: Failed to create base chart.", filename); // Logge einen Fehler
        return; // Beende die Funktion
    }
    chart->setAnimationOptions(QChart::AllAnimations); // Setze die Animations-Optionen für das Diagramm

    view = new QChartView(chart, this); // Erstelle eine Ansicht für das Diagramm
    if (!view) { // Überprüfe, ob die Ansicht erstellt wurde
        logger.log("Error: Failed to create chart view.", filename); // Logge einen Fehler
        delete chart; // Lösche das Diagramm
        return; // Beende die Funktion
    }
    applyBaseChart(chart); // Wende Basisdiagramm-Einstellungen an

    // Verbinde das Signal für einen Klick auf ein Diagrammschnittstück mit der entsprechenden Funktion
    connect(static_cast<QPieSeries *>(chart->series().at(0)), &QPieSeries::clicked, this, &Widget::handleSliceClicked);
}

// Funktion zum Anwenden von Einstellungen auf das Basisdiagramm
void Widget::applyBaseChart(QChart *chart)
{
    if (!chart) { // Überprüfe, ob das Diagramm gültig ist
        logger.log("Error: Invalid chart pointer.", filename); // Logge einen Fehler
        return; // Beende die Funktion
    }

    // Wende Einstellungen auf das Diagramm an
    chart->setTitle(chart->title());
    chart->legend()->setVisible(true);
    chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

// Funktion zur Behandlung eines Klicks auf ein Diagrammschnittstück
void Widget::handleSliceClicked(QPieSlice *slice)
{
    if (!view || !slice) { // Überprüfe, ob die Ansicht und das Schnittstück gültig sind
        logger.log("Error: Invalid view or slice pointer.", filename); // Logge einen Fehler
        return; // Beende die Funktion
    }

    // Erhalte die Kategorie und den Wert des angeklickten Schnittstücks
    QString category = slice->label();
    qreal value = slice->value();

    // Logge den Klick
    logger.log("Clicked on slice: " + category + ", Value: " + QString::number(value), filename);

    // Lösche die aktuelle Ansicht des Diagramms
    clearChartView(view, logger);

    // Erstelle ein individuelles Diagramm für die angeklickte Kategorie
    QChart *individualChart = bpc.creatIndividualChart(category);

    if (!individualChart) { // Überprüfe, ob das individuelle Diagramm erstellt wurde
        logger.log("Error: Failed to create individual chart.", filename); // Logge einen Fehler
        return; // Beende die Funktion
    }

    view->setChart(individualChart); // Setze das individuelle Diagramm in die Ansicht ein
}

// Funktion zum Behandeln von Größenänderungsereignissen
void Widget::resizeEvent(QResizeEvent *event)
{
    if (!view) { // Überprüfe, ob die Ansicht gültig ist
        logger.log("Error: Invalid view pointer.", filename); // Logge einen Fehler
        return; // Beende die Funktion
    }

    view->resize(size()); // Passe die Größe der Ansicht an die Widget-Größe an
}

// Funktion zum Löschen aller Serien aus einem Diagramm
void Widget::clearChartView(QChartView *chartView, FileLogger &logger)
{
    if (!chartView || !chartView->chart()) { // Überprüfe, ob die Ansicht und das Diagramm gültig sind
        logger.log("Error: Invalid chart view or chart pointer.", filename); // Logge einen Fehler
        return; // Beende die Funktion
    }

    chartView->chart()->removeAllSeries(); // Entferne alle Serien aus dem Diagramm
}

// Destruktor für das Widget
Widget::~Widget()
{
    delete myForm; // Lösche das Formular (myForm)
}
