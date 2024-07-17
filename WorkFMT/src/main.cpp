#include "mainwindow.h"
#include "fmtapplication.h"
#include "fmtcore.h"
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QSettings>
#include <QDir>
#include <QStyleFactory>

static void ProcessRsreqOption(MainWindow *w, const QString &constringsdir);
static void ProcessLoggingOption(FmtApplication *app, QCommandLineParser *parser, QCommandLineOption &logOption, QCommandLineOption &logruleOption);

int main(int argc, char *argv[])
{
    FmtApplication a(argc, argv);

    QCommandLineParser parser;
    QCommandLineOption helpOption = parser.addHelpOption();
    Q_UNUSED(helpOption)
    parser.addVersionOption();

    qDebug() << QStyleFactory::keys();
    QStyle *s = QStyleFactory::create("WindowsModernStyleBlue");
    qDebug() << s;
    a.setStyle(s);

    QCommandLineOption connectionStringOption(QStringList() << "cs" << "con-str",
                                        QApplication::translate("main", "Подключиться к базе данных использую строку подключения <constring>"),
                                        QApplication::translate("main", "constring"));

    QCommandLineOption rsreqOption(QStringList() << "rq" << "rsreq",
                                        QApplication::translate("main", "Подключиться к базе данных использую файл rsreq.ini <directory>"),
                                        QApplication::translate("main", "directory"));

    QCommandLineOption logOption(QStringList() << "l" << "logging",
                                        QApplication::translate("main", "Включить логирование событий"));

    QCommandLineOption logruleOption(QStringList() << "lr" << "logrule",
                                        QApplication::translate("main", "Правила логирования событий <loggingrules>"),
                                        QApplication::translate("main", "loggingrules"));

    parser.addOption(connectionStringOption);
    parser.addOption(rsreqOption);
    parser.addOption(logOption);
    parser.addOption(logruleOption);

    parser.process(a.arguments());

    MainWindow *w = (MainWindow*)a.addMainWindow();
    ProcessLoggingOption(&a, &parser, logOption, logruleOption);
    a.init();

    // строка подключения
    if (parser.isSet(connectionStringOption))
    {
        QStringList constrings = parser.values(connectionStringOption);
        foreach (const QString &cstr, constrings) {
            w->OpenConnection(cstr);
        }
    }

    // строка подключения файл rsreq.ini
    if (parser.isSet(rsreqOption))
    {
        QStringList constringsdir = parser.values(rsreqOption);
        foreach (const QString &cstr, constringsdir) {
            ProcessRsreqOption(w, cstr);
        }
    }

    return a.exec();
}

static void ProcessRsreqOption(MainWindow *w, const QString &constringsdir)
{
    QDir d(constringsdir);

    QFile f(d.absoluteFilePath("rsreq.ini"));
    if (f.open(QIODevice::ReadOnly))
    {
        QString fdata = f.readAll();
        f.close();

        QString constrtype1regexp = ConstrType1RegExp();

        QRegularExpression rexpr(constrtype1regexp);
        QRegularExpressionMatch match = rexpr.match(fdata);

        if (match.hasMatch())
            w->OpenConnection(match.captured(0));
    }
}

static void ProcessLoggingOption(FmtApplication *app, QCommandLineParser *parser, QCommandLineOption &logOption, QCommandLineOption &logruleOption)
{
    if (parser->isSet(logOption))
    {
        QString rules;
        if (parser->isSet(logruleOption)) rules = parser->value(logruleOption);
        app->initLogging(rules);
    }
}
