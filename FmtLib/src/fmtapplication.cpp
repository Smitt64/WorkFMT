#include "fmtapplication.h"
#include "mainwindow.h"
#include "fmtcore.h"
#include "tablesgroupprovider.h"
#include "oracleauthdlg.h"
#include "fmtgeninterface.h"
#include "fmtgentablessql.h"
#include "fmtgencpptemplate.h"
#include "fmtgenhotfix.h"
#include "loggingcategories.h"
#include "fmtgeninputservicecpptemplate.h"
#include "fmtgencppclasstemplate.h"
#include "massop/massinittableoperation.h"
#include "massop/btrvtemplate/massopbtrvtemplate.h"
#include "massop/massopenfunc/massopenfunctemplate.h"
#include "massop/destribcreate/massdestribcreate.h"
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
    hOldFilter = Q_NULLPTR;
    pDbgHelp = Q_NULLPTR;
#endif
    QApplication::setApplicationName("WorkFMT");
    QApplication::setApplicationVersion(GetVersionNumberString());
    pSettings = new QSettings("fmtopt.ini", QSettings::IniFormat, this);
    addLibraryPath(QFileInfo(QCoreApplication::applicationFilePath()).path());
    addLibraryPath(QDir::current().path());
    qRegisterMetaType<RecentList>("RecentList");

    //qDebug() << QStyleFactory::keys();
    m_fLogging = false;
}

FmtApplication::~FmtApplication()
{
#ifdef Q_OS_WIN
    if (hOldFilter)
        SetUnhandledExceptionFilter(hOldFilter);
    hOldFilter = Q_NULLPTR;
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

    registerFmtGenInterface<FmtGenCppTemplate>("FmtGenCppTemplate", tr("Шаблоны btrv"));
    registerFmtGenInterface<FmtGenTablesSql>("FmtGenTablesSql", tr("Скрипт TablesSql"));
    registerFmtGenInterface<FmtGenInputServiceCppTemplate>("FmtGenInputServiceCppTemplate", tr("Объектный сервис ввода"));
    registerFmtGenInterface<FmtGenCppClassTemplate>("FmtGenCppClassTemplate", tr("Класс для импорта в RSL"));
    registerFmtGenInterface<FmtGenHotFix>("FmtGenHotFix", tr("PL/SQL скрип наполнения fmt"));

    registerMassOpInterface<MassInitTableOperation>("MassInitTableOperation", tr("Создание таблиц/индексов"));
    registerMassOpInterface<MassOpBtrvTemplate>("MassOpBtrvTemplate", tr("Шаблоны btrv"));
    registerMassOpInterface<MassOpenFuncTemplate>("MassOpenFuncTemplate", tr("Функция открытия файлов"));
    registerMassOpInterface<MassDestribCreate>("MassDestribCreate", tr("Дистрибутивное наполнение"));
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

    DWORD ExceptionAddress = *reinterpret_cast<DWORD*>(pExInfo->ExceptionRecord->ExceptionAddress);
    qCCritical(logDbgHelp()) << "Address:" << QString("0x%1").arg(ExceptionAddress, 8, 16, QLatin1Char('0'));
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
        if (e->type() == QEvent::KeyPress)
            ShowObjectStackMsg(qApp->focusWidget(), (QKeyEvent*)e);
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

void FmtApplication::ShowObjectStackMsg(QObject *receiver, QKeyEvent *e)
{
    if (e->key() == Qt::Key_F12 && ((e->modifiers() & Qt::ControlModifier) == Qt::ControlModifier))
    {
        if (lastCtrlF12Msg.isEmpty())
        {
            QString msg;
            QObject *cur = qobject_cast<QObject*>(receiver);

            while(cur)
            {
                QString className = cur->metaObject()->className();

                //if (className[0] != QChar('Q'))
                {
                    if (cur == receiver)
                        msg = QString("%1").arg(className);
                    else
                        msg = QString("%1/").arg(className) + msg;
                }
                cur = cur->parent();
            }
            QWidget *parentWidget = qobject_cast<QWidget*>(receiver);
            QMessageBox dlg(parentWidget);
            dlg.setWindowTitle(tr("Иерархия объектов"));
            dlg.setText(msg);
            dlg.exec();

            lastCtrlF12Msg = msg;
        }
        else
            lastCtrlF12Msg = "";
        //e->accept();
    }
}

bool FmtApplication::initLogging(const QString &rules)
{
    QDir logDir(applicationDirPath());
    if (!logDir.cd("logs"))
    {
        logDir.mkdir("logs");
        logDir.cd("logs");
    }

    QString logFileName = QString("%1.txt").arg(logDir.absoluteFilePath(QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss_zzz")));
    m_logFile.reset(new QFile(logFileName));

    if (m_logFile.data()->open(QFile::Append | QFile::Text))
    {
        setLoggingRules(rules);
        qInstallMessageHandler(messageHandler);
        m_fLogging = true;
    }
    else
    {
        m_logFile.reset(Q_NULLPTR);
        m_fLogging = false;
    }

    return m_fLogging;
}

void FmtApplication::setLoggingRules(const QString &rules)
{
    if (!rules.isEmpty())
    {
        QString r = rules;
        QLoggingCategory::setFilterRules(r.replace(";", "\n"));
    }
}

void FmtApplication::disableLogging()
{
    if (!m_logFile.isNull() && m_logFile->isOpen())
    {
        m_logFile->close();
        m_logFile.reset(Q_NULLPTR);
        qInstallMessageHandler(0);
        m_fLogging = false;
    }
}

bool FmtApplication::isLoggingEnabled() const
{
    return m_fLogging;
}

QString FmtApplication::logginFileName() const
{
    if (!m_logFile.isNull() && m_logFile->isOpen())
        return m_logFile->fileName();

    return QString();
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
