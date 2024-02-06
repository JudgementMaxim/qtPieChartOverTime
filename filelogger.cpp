#include "FileLogger.h"
#include <iostream>
#include <QDir>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

FileLogger::FileLogger(const QString &sourceFileName)
{
    qDebug() << "FileLogger constructor called for" << sourceFileName;

    QDir().mkpath("log");

    QString currentDateTime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    logFileName = "log/debug_log_" + currentDateTime + ".txt";

    qDebug() << "Log file path:" << logFileName;

    logFile.setFileName(logFileName);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        textStream.setDevice(&logFile);
        log("FileLogger initialized.", sourceFileName);
    }
    else
    {
        qWarning() << "Failed to open log file for writing.";
    }
}

FileLogger::~FileLogger()
{
    qDebug() << "FileLogger destructor called";
    log("FileLogger destroyed.", "Destructor");
    logFile.close();
}

void FileLogger::log(const QString &message, const QString &sourceFileName)
{
    QTextStream ts(&logFile);
    ts << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz] ")
       << "[" << sourceFileName << "] " << message << Qt::endl;
}

void FileLogger::openLogFile()
{
    QString absolutePath = QDir::current().absoluteFilePath(logFileName);

#ifdef Q_OS_WIN
    bool success = QProcess::startDetached("notepad.exe", QStringList() << absolutePath);
#elif defined(Q_OS_UNIX) || defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    bool success = QProcess::startDetached("xdg-open", QStringList() << absolutePath);
#else
    qWarning() << "Opening files not supported on this platform.";
    bool success = false;
#endif

    if (!success) {
        qWarning() << "Failed to open log file.";
    }
}

