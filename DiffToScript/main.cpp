#include <iostream>

#include "mainwindow.h"
#include "task.h"
#include "fmtapplication.h"
#include "cmdparser.h"
#include "taskoptions.h"
#include "difftoscripttest.h"

#include <QFileInfo>
//#include <QApplication>
#include <QTimer>
#include <QTextStream>
#include <QFile>
#include <QCommandLineParser>
#include <QTest>
#include <qloggingcategory.h>


int main(int argc, char *argv[])
{
    Task *pTask = new Task();

    FmtApplication app2(argc, argv);
#ifdef QT_DEBUG
    QLoggingCategory::setFilterRules(QStringLiteral("Fmt=false\nSql=false\nCore=false\n*.info=false"));
    QTest::qExec(new DiffToScriptTest);
    QLoggingCategory::setFilterRules(QStringLiteral("Fmt=false\nSql=false\nCore=false\n*.info=true"));
    return 0;
#endif


    QFileInfo current(QCoreApplication::applicationFilePath());
    QApplication::addLibraryPath(current.path());
    QApplication::addLibraryPath(current.path() + "\\sqldrivers");
    QApplication::addLibraryPath(current.path() + "\\platforms");

    CmdParser cmdParser(&app2);
    CommandLineParseResult result = cmdParser.parse();

    if (result.statusCode == CommandLineParseResult::Status::Error)
    {
        QTextStream(stdout) << result.errorString <<Qt::endl;
        return 1;
    }
    QObject::connect(pTask, SIGNAL(finished()), &app2, SLOT(quit()));

    pTask->optns = cmdParser.opts;

    QTimer::singleShot(0, pTask, SLOT(run()));

    return app2.exec();
}