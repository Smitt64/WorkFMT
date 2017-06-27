#include "fmtapplication.h"
#include "mainwindow.h"
#include "fmtcore.h"
#include "tablesgroupprovider.h"
#include "oracleauthdlg.h"
#include "fmtgeninterface.h"
#include "fmtgentablessql.h"
#include "fmtgencpptemplate.h"
#include "loggingcategories.h"
#include <fmtdbghelp.h>
#include <QDebug>
#include <functional>
#include <QStyleFactory>

#ifdef Q_OS_WIN
#include <windows.h>
typedef FmtDbgHelp *(*t_CreateDbgHelpObject)(QObject *);
LONG CALLBACK ExceptionFilter(PEXCEPTION_POINTERS pExInfo)
{
    FmtApplication *FmtApp = (FmtApplication*)qApp;
    return FmtApp->UnhandledExceptionFilter(pExInfo);
}
#endif

QScopedPointer<QFile> m_logFile;
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

FmtApplication::FmtApplication(int &argc, char **argv)  :
    QApplication(argc, argv)
{
#ifdef Q_OS_WIN
    hOldFilter = NULL;
    pDbgHelp = NULL;
#endif
    QApplication::setApplicationName("WorkFMT");
    QApplication::setApplicationVersion(GetVersionNumberString());
    pSettings = new QSettings("fmtopt.ini", QSettings::IniFormat, this);
    /*QDir logDir(applicationDirPath());
    if (!logDir.cd("logs"))
    {
        logDir.mkdir("logs");
        logDir.cd("logs");
    }

    QLoggingCategory::setFilterRules("Info=false");
    QString logFileName = QString("%1.txt").arg(logDir.absoluteFilePath(QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss_zzz")));
    m_logFile.reset(new QFile(logFileName));
    m_logFile.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);*/

    Q_INIT_RESOURCE(fmt);
    qRegisterMetaType<RecentList>("RecentList");

    qDebug() << QStyleFactory::keys();
}

FmtApplication::~FmtApplication()
{
#ifdef Q_OS_WIN
    if (hOldFilter)
        SetUnhandledExceptionFilter(hOldFilter);
    hOldFilter = NULL;
#endif
}

void FmtApplication::init()
{
    pTnsModel = new OracleTnsListModel(this);
    FmtInit();

    QDir trDir(applicationDirPath());
    if (trDir.cd("translations"))
    {
        QString translatorFile = QString("qt_%1").arg("ru");
        if (qt_translator.load(translatorFile, trDir.absolutePath()))
        {
            qCInfo(logCore()) << "Translator installed: " << translatorFile;
            installTranslator(&qt_translator);
        }
        else
            qCWarning(logCore()) << "Error loading translator " << translatorFile;
    }
    else
        qCWarning(logCore()) << "Can't find translations folder";

    initDbgHelp();

    registerFmtGenInterface<FmtGenTablesSql>("FmtGenTablesSql");
    registerFmtGenInterface<FmtGenCppTemplate>("FmtGenCppTemplate");
}

void FmtApplication::initDbgHelp()
{
#ifdef Q_OS_WIN
    QDir dbgDir(applicationDirPath());
    dbgLib.setFileName(dbgDir.absoluteFilePath("FmtDbgHelp.dll"));
    if (dbgLib.load())
    {
        qCInfo(logDbgHelp()) << "FmtDbgHelp.dll loaded";
        t_CreateDbgHelpObject creator = (t_CreateDbgHelpObject)dbgLib.resolve("CreateDbgHelpObject");

        if (!creator)
            qCWarning(logDbgHelp()) << "Can't resolve symbol 'CreateDbgHelpObject':" << dbgLib.errorString();
        else
        {
            pDbgHelp = creator(this);
            if (pDbgHelp)
            {
                //AddVectoredExceptionHandler(0, (LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionFilter);
                SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionFilter);
                qCInfo(logDbgHelp()) << "Help object created, exception filter installed";
            }
            else
                qCWarning(logDbgHelp()) << "Can't create help object";
        }
    }
    else
        qCInfo(logDbgHelp()) << "Error loading file " << dbgLib.fileName();
#endif
}

#ifdef Q_OS_WIN
LONG FmtApplication::UnhandledExceptionFilter(PEXCEPTION_POINTERS pExInfo)
{
    Q_UNUSED(pExInfo)
    QString ExceptionCodeShort, ExceptionCodeText;
    QMetaObject::invokeMethod(pDbgHelp, "ExceptionCodeShort", Qt::DirectConnection,
                                Q_RETURN_ARG(QString, ExceptionCodeShort),
                                Q_ARG(int, pExInfo->ExceptionRecord->ExceptionCode));
    QMetaObject::invokeMethod(pDbgHelp, "ExceptionCodeText", Qt::DirectConnection,
                                Q_RETURN_ARG(QString, ExceptionCodeText),
                                Q_ARG(int, pExInfo->ExceptionRecord->ExceptionCode));

    qCCritical(logDbgHelp()) << "Exception:" << QString("0x%1").arg(pExInfo->ExceptionRecord->ExceptionCode, 8, 16, QLatin1Char('0')) << ExceptionCodeShort;
    qCCritical(logDbgHelp()) << ExceptionCodeText;
    qCCritical(logDbgHelp()) << "Address:" << QString("0x%1").arg((DWORD)pExInfo->ExceptionRecord->ExceptionAddress, 8, 16, QLatin1Char('0'));
    QDir dbgDir(applicationDirPath());
    QString dumpFile = dbgDir.absoluteFilePath("FmtApplication.dmp");

    bool retval = false;
    QMetaObject::invokeMethod(pDbgHelp, "WritMiniDump", Qt::DirectConnection,
                                Q_RETURN_ARG(bool, retval),
                                Q_ARG(QString, dumpFile),
                                Q_ARG(void*, pExInfo));
    qCInfo(logDbgHelp()) << "Create mini dump " << dumpFile << ">>" << retval;

    QString module;
    QString function;
    QString file;
    int line;
    QMetaObject::invokeMethod(pDbgHelp, "GetExceptionInfo", Qt::DirectConnection,
                              QGenericReturnArgument(), Q_ARG(void*, pExInfo),
                              Q_ARG(QString&,module),
                              Q_ARG(QString&,function),
                              Q_ARG(QString&,file),
                              Q_ARG(int&,line));
    qCInfo(logDbgHelp()) << "Exception in symbol:" << QString("%1!%2 Line %3").arg(QFileInfo(module).fileName(), function)
                            .arg(line);
    if (file.length() > 0)
        qCInfo(logDbgHelp()) << "Exception in file:" << file;

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

OracleTnsListModel *FmtApplication::getOracleTnsModel()
{
    return pTnsModel;
}

QMainWindow *FmtApplication::addMainWindow()
{
    MainWindow *w = new MainWindow;
    w->show();

    return w;
}

QSettings *FmtApplication::settings()
{
    return pSettings;
}

bool FmtApplication::notify(QObject *receiver, QEvent *e)
{
    bool hr = true;

    try
    {
        hr = QApplication::notify(receiver, e);
    }
    catch(std::exception &e)
    {
        qCCritical(logCore()) << "Exception: " << e.what();
    }
    catch(int e)
    {
        qCCritical(logCore()) << "Exception code: " << e;
    }

    return hr;
}

void FmtApplication::initLogging(const QString &rules)
{
    QDir logDir(applicationDirPath());
    if (!logDir.cd("logs"))
    {
        logDir.mkdir("logs");
        logDir.cd("logs");
    }

    if (!rules.isEmpty())
    {
        QString r = rules;
        QLoggingCategory::setFilterRules(r.replace(";", "\n"));
    }

    QString logFileName = QString("%1.txt").arg(logDir.absoluteFilePath(QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss_zzz")));
    m_logFile.reset(new QFile(logFileName));
    m_logFile.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTextStream out(m_logFile.data());
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }

    out << context.category << ": "
        << msg << endl;
    out.flush();
}
