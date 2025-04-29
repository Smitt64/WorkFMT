#include <iostream>
#include "wizard/diffwizard.h"
#include "mainwindow.h"
#include "task.h"
#include "fmtapplication.h"
#include "cmdparser.h"
#include "taskoptions.h"
#include "difftoscripttest.h"
#include <QMessageBox>
#include <QFileInfo>
//#include <QApplication>
#include <QTimer>
#include <QTextStream>
#include <QFile>
#include <QCommandLineParser>
#include <QTest>
#include <qloggingcategory.h>
#include <rsscript/registerobjlist.hpp>

int main(int argc, char *argv[])
{
    FmtApplication app(argc, argv);
    QApplication::setApplicationName("DiffToScript");
/*#ifdef QT_DEBUG
    QLoggingCategory::setFilterRules(QStringLiteral("Fmt=false\nSql=false\nCore=false\n*.info=false"));
    QTest::qExec(new DiffToScriptTest);
    QLoggingCategory::setFilterRules(QStringLiteral("Fmt=false\nSql=false\nCore=false\n*.info=true"));
    return 0;
#endif*/

    rslAddStaticMacroDir(".\\mac\\difftoscript");

    if (argc > 1)
    {
        QFileInfo current(QCoreApplication::applicationFilePath());
        QApplication::addLibraryPath(current.path());
        QApplication::addLibraryPath(current.path() + "\\sqldrivers");
        QApplication::addLibraryPath(current.path() + "\\platforms");

        CmdParser cmdParser(&app);
        CommandLineParseResult result = cmdParser.parse();

        Task *pTask = new Task();
        if (result.statusCode == CommandLineParseResult::Status::Error)
        {
            QTextStream(stdout) << result.errorString <<Qt::endl;
            return 1;
        }
        QObject::connect(pTask, &Task::finished, &app, [&app, &pTask]()
        {
            app.exit(pTask->result());
        });

        pTask->optns = cmdParser.opts;

        QTimer::singleShot(0, pTask, SLOT(run()));
        return app.exec();
    }
    else
    {
        QDir trDir(QApplication::applicationDirPath());
        if (!trDir.cd("translations"))
            trDir = QDir::current();
        else
            trDir = QApplication::applicationDirPath();

        QTranslator qt_translator;
        if (trDir.cd("translations"))
        {
            QString translatorFile = QString("qt_%1").arg("ru");
            if (qt_translator.load(translatorFile, trDir.absolutePath()))
            {
                //qCInfo(logCore()) << "Translator installed: " << translatorFile;
                QApplication::installTranslator(&qt_translator);
            }
            else
                QMessageBox::critical(nullptr, "Ошибка!", QString("Не удалось загрузить перевод %1").arg(translatorFile));
        }

        DiffWizard wzrd;
        wzrd.show();
        return app.exec();
    }

    return 0;
}
