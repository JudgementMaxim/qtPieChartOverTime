#ifndef FILELOGGER_H
#define FILELOGGER_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QDesktopServices>  // Include QDesktopServices for opening files
#include <QUrl>  // Include QUrl for URL handling
#include <iostream>

class FileLogger
{
public:
    FileLogger(const QString &sourceFileName);
    ~FileLogger();
    void log(const QString &message, const QString &sourceFileName);
    void openLogFile();

private:
    QFile logFile;
    QTextStream textStream;
    QString logFileName;
};

#endif // FILELOGGER_H
