#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <fmtcore.h>
#include <QTextStream>
#include "dbmainwindow.h"

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(int argc, char *argv[], QObject *parent = 0);

signals:
    void finished();

public slots:
    void run();
    void exportTable();

private slots:
    void processError(const QString &str);
    void exportTableStart(const QString &str);

private:
    QStringList args;
    QCommandLineParser parser;
    QScopedPointer<QCommandLineOption> connectionStringOption;
    QScopedPointer<QCommandLineOption> exportOption;
    QScopedPointer<QCommandLineOption> dirOption;
    QScopedPointer<QCommandLineOption> dbtOption;

    QScopedPointer<QTextStream> sterr;
    QScopedPointer<QTextStream> stout;

    DbMainWindow *w;
};

#endif // TASK_H
