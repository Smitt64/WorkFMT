#ifndef FMTAPPLICATION_H
#define FMTAPPLICATION_H

#include <QObject>
#include <QApplication>
#include <QMainWindow>
#include <QSettings>
#include <QTranslator>
#include "fmtlib_global.h"
#include "OracleTnsListModel.h"
#ifdef Q_OS_WIN
#include <QLibrary>
#include <windows.h>
#endif

#ifdef Q_OS_WIN
class FmtDbgHelp;
#endif
class FMTLIBSHARED_EXPORT FmtApplication : public QApplication
{
    Q_OBJECT
    friend class tagUnhandledExceptionWrp;
public:
    explicit FmtApplication(int &argc, char **argv);
    virtual ~FmtApplication();

    QMainWindow *addMainWindow();

    QSettings *settings();
    virtual bool notify(QObject *receiver, QEvent *e);

    OracleTnsListModel *getOracleTnsModel();

    void init();
    void applyStyle();

private:
    void initDbgHelp();
    void ShowObjectStackMsg(QObject *receiver, QKeyEvent *e);
#ifdef Q_OS_WIN
    friend LONG CALLBACK ExceptionFilter(PEXCEPTION_POINTERS pExInfo);
    LPTOP_LEVEL_EXCEPTION_FILTER hOldFilter;
    LONG UnhandledExceptionFilter(PEXCEPTION_POINTERS pExInfo);
    QLibrary dbgLib;
    QObject *pDbgHelp;
#endif
    QString lastCtrlF12Msg;
    QSettings *pSettings;
    OracleTnsListModel *pTnsModel;
    QTranslator qt_translator;
};

#endif // FMTAPPLICATION_H
