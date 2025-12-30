#include "mainwindow.h" // -------------------
#include "fmtapplication.h"
#include "fmtcore.h"
#include "toolsruntime.h"
#include "mdiproxystyle.h"
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QSettings>
#include <QDir>
#include "fmtribbonmainwindow.h"
#include "IconThemeManager.h"

static void ProcessRsreqOption(FmtRibbonMainWindow *w, const QString &constringsdir);
static void ProcessLoggingOption(FmtApplication *app, QCommandLineParser *parser, QCommandLineOption &logOption, QCommandLineOption &logruleOption);

static void InitIconTheme()
{
    IconThemeManager::initialize("vs_theme");

    /*#ifdef QT_DEBUG
    IconThemeManager::addCustomPath("d:\\Work\\ResEditor\\RsResEditor\\res\\icons");
#endif*/
}

int main(int argc, char **argv)
{
    //SetSysColor(COLOR_WINDOW, RGB(0x33, 0x33, 0x33));
    QDir settingsDir = QDir(argv[0]);

    InitIconTheme();
    FmtApplication a(argc, argv);

    QStyle *windows = QStyleFactory::create("WindowsVista");
    QScopedPointer<MDIProxyStyle> OfficeStyle(new MDIProxyStyle());
    OfficeStyle->setBaseStyle(windows);
    a.setStyle(OfficeStyle.data());
    //a.setPalette(OfficeStyle->standardPalette());

    QCommandLineParser parser;
    QCommandLineOption helpOption = parser.addHelpOption();
    Q_UNUSED(helpOption)
    parser.addVersionOption();

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

    //qDebug() << a.arguments();
    parser.process(a.arguments());

    //MainWindow *w = (MainWindow*)a.addMainWindow();
    FmtRibbonMainWindow *w = a.addMainWindow<FmtRibbonMainWindow>();
    ProcessLoggingOption(&a, &parser, logOption, logruleOption);
    a.init();
    //a.applyStyle();

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

static void ProcessRsreqOption(FmtRibbonMainWindow *w, const QString &constringsdir)
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
        if (parser->isSet(logruleOption))
            rules = parser->value(logruleOption);

        toolInitLogging("WorkFmt", rules);
    }
}
