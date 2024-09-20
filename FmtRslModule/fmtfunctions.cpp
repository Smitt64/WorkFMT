#include "fmtrslmodule.h"
#include "rsscript/registerobjlist.hpp"
#include "rslexecutor.h"
#include "mdisubinterface.h"
#include <connectioninfo.h>
#include <fmtworkwindow.h>
#include <mainwindow.h>
#include <QApplication>
#include <fmttable.h>
#include <fmtcore.h>

static MainWindow *GetMainWindow()
{
    return qobject_cast<MainWindow*>(QApplication::activeWindow());
}

// ----------------------------------------------------------------------------

static void fmtAppHasTableWindow()
{
    enum
    {
        prm_Table = 0,
    };

    MainWindow *mwnd = GetMainWindow();
    if (!mwnd)
        return;

    QMdiSubWindow *sub = nullptr;

    int type = GetFuncParamType(prm_Table);
    if (type == QVariant::String)
    {
        QVariant val = GetFuncParam(prm_Table);
        sub = mwnd->hasTableWindow(val.toString());
    }
    else if (type == QVariant::Int)
    {
        int id = GetFuncParam(prm_Table).toInt();
        sub = mwnd->hasTableWindow(id);
    }
    else if (type == QVariant::UserType)
    {
        FmtTable *table = GetFuncParam<FmtTable*>(0);

        if (!table)
            ThrowParamTypeError(prm_Table);

        sub = mwnd->hasTableWindow(table->id());
    }
    else
        ThrowParamTypeError(prm_Table);

    if (!sub)
        return;

    FmtWorkWindow *w = qobject_cast<FmtWorkWindow*>(sub->widget());

    if (!w)
        return;

    SetReturnVal(QVariant::fromValue((QObject*)w));
}

static void fmtAppCurrentTableWindow()
{
    MainWindow *mwnd = GetMainWindow();

    if (!mwnd)
        return;
        
    FmtWorkWindow *w = mwnd->currentWorkWindow();

    if (!w)
        return;
    
    SetReturnVal(QVariant::fromValue((QObject*)w));
}

static void fmtappOpenTableWindow()
{
    enum
    {
        prm_Table = 0,
    };

    MainWindow *mwnd = GetMainWindow();
    if (!mwnd || !mwnd->currentConnection())
        return;

    int type = GetFuncParamType(prm_Table);

    QSharedPointer<FmtTable> table;
    if (type == QVariant::String)
    {
        QString val = GetFuncParam(prm_Table).toString();
        QMdiSubWindow *wnd = mwnd->hasTableWindow(val);

        if (!wnd)
        {        
            table.reset(new FmtTable(mwnd->currentConnection()));

            if (!table->load(val))
                ThrowParamTypeError(prm_Table);
        }
        else
        {
            FmtWorkWindow *w = qobject_cast<FmtWorkWindow*>(wnd->widget());

            if (w)
            {
                SetReturnVal(QVariant::fromValue((QObject*)w));
                return;
            }
        }
    }
    else if (type == QVariant::Int)
    {
        int id = GetFuncParam(prm_Table).toInt();
        QMdiSubWindow *wnd = mwnd->hasTableWindow(id);

        if (!wnd)
        {     
            table.reset(new FmtTable(mwnd->currentConnection()));

            if (!table->load(id))
                ThrowParamTypeError(prm_Table);
        }
        else
        {
            FmtWorkWindow *w = qobject_cast<FmtWorkWindow*>(wnd->widget());

            if (w)
            {
                SetReturnVal(QVariant::fromValue((QObject*)w));
                return;
            }
        }
    }
    else
        ThrowParamTypeError(prm_Table);
    
    FmtWorkWindow *w = nullptr;
    mwnd->CreateDocument(table, &w)->show();

    SetReturnVal(QVariant::fromValue((QObject*)w));
}

static void fmtSelectTableFieldsDailog()
{
    enum
    {
        prm_Table = 0,
        prm_Title,
    };

    QString title;
    FmtTable *table = nullptr;
    MainWindow *mwnd = GetMainWindow();

    int type = GetFuncParamType(prm_Table);
    if (type == QVariant::UserType)
    {
        table = GetFuncParam<FmtTable*>(0);

        if (!table)
            ThrowParamTypeError<FmtTable>(prm_Table);
    }
    else
        ThrowParamTypeError<FmtTable>(prm_Table);

    type = GetFuncParamType(prm_Title);
    if (type == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    QList<FmtField*> pFldList;
    int stat = SelectTableFieldsDlg(table, title, &pFldList);

    if (stat)
    {
        QVariantList list;
        for (FmtField *fld : pFldList)
            list.append(QVariant::fromValue((QObject*)fld));

        SetReturnVal(QVariant::fromValue(list));
    }
}

static void fmtappConnections()
{
    MainWindow *mwnd = GetMainWindow();

    if (!mwnd)
        return;

    QVariantList list;
    for (ConnectionInfo *conn : mwnd->connections())
        list.append(QVariant::fromValue((QObject*)conn));

    SetReturnVal(QVariant::fromValue(list));
}

static void fmtappConnectionWindows()
{
    enum
    {
        prm_Connection = 0,
    };
    MainWindow *mwnd = GetMainWindow();

    if (!mwnd)
        return;

    ConnectionInfo *info = nullptr;
    int type = GetFuncParamType(prm_Connection);
    if (type == QVariant::UserType)
    {
        info = GetFuncParam<ConnectionInfo*>(prm_Connection);

        if (!info)
            ThrowParamTypeError(prm_Connection);
    }
    else
        ThrowParamTypeError(prm_Connection);

    QVariantList list;
    QList<QWidget*> windows = mwnd->windows(info);
    for (QWidget *workwnd : windows)
        list.append(QVariant::fromValue((QObject*)workwnd));

    SetReturnVal(QVariant::fromValue(list));
}

static void fmtappOpenConnection()
{
    MainWindow *mwnd = GetMainWindow();
    ConnectionInfo *info = mwnd->openConnection();

    if (info)
        SetReturnVal(QVariant::fromValue((QObject*)info));
}

static void fmtappAddConnection()
{
    enum
    {
        prm_Connection = 0,
    };
    MainWindow *mwnd = GetMainWindow();

    if (!mwnd)
        return;

    ConnectionInfo *info = nullptr;
    int type = GetFuncParamType(prm_Connection);
    if (type == QVariant::UserType)
    {
        info = GetFuncParam<ConnectionInfo*>(prm_Connection);

        if (!info)
            ThrowParamTypeError(prm_Connection);
    }
    else
        ThrowParamTypeError(prm_Connection);

    bool hr = mwnd->addConnection(info);
    SetReturnVal(hr);
}

static void fmtappIsExistsConnection()
{
    enum
    {
        prm_Connection = 0,
    };
    MainWindow *mwnd = GetMainWindow();

    if (!mwnd)
        return;

    ConnectionInfo *info = nullptr;
    int type = GetFuncParamType(prm_Connection);
    if (type == QVariant::UserType)
    {
        info = GetFuncParam<ConnectionInfo*>(prm_Connection);

        if (!info)
            ThrowParamTypeError(prm_Connection);
    }
    else
        ThrowParamTypeError(prm_Connection);

    bool hr = mwnd->isExistsConnection(info);
    SetReturnVal(hr);
}

static void fmtappParseConnectionString()
{
    enum
    {
        prm_connString = 0,
        prm_user,
        prm_pass,
        prm_service
    };

    QString connString;

    int type = GetFuncParamType(prm_connString);
    if (type == QVariant::String)
    {
        QVariant val = GetFuncParam(prm_connString);
        connString = val.toString();
    }
    else
        ThrowParamTypeError(prm_connString, "string");

    QString user;
    QString pass;
    QString service;

    bool hr = ParseConnectionString(connString, user, pass, service);

    SetFuncParam(prm_user, user);
    SetFuncParam(prm_pass, pass);
    SetFuncParam(prm_service, service);
    SetReturnVal(hr);
}

void fmtappRegister()
{
    RegisterObjList::inst()->AddObject<FmtWorkWindow>(false);

    RegisterObjList::inst()->AddStdProc("fmtAppHasTableWindow", fmtAppHasTableWindow);
    RegisterObjList::inst()->AddStdProc("fmtAppCurrentTableWindow", fmtAppCurrentTableWindow);
    RegisterObjList::inst()->AddStdProc("fmtappOpenTableWindow", fmtappOpenTableWindow);
    RegisterObjList::inst()->AddStdProc("fmtappConnections", fmtappConnections);
    RegisterObjList::inst()->AddStdProc("fmtappConnectionWindows", fmtappConnectionWindows);
    RegisterObjList::inst()->AddStdProc("fmtappOpenConnection", fmtappOpenConnection);
    RegisterObjList::inst()->AddStdProc("fmtappAddConnection", fmtappAddConnection);
    RegisterObjList::inst()->AddStdProc("fmtappIsExistsConnection", fmtappIsExistsConnection);
    RegisterObjList::inst()->AddStdProc("fmtappParseConnectionString", fmtappParseConnectionString);

    RegisterObjList::inst()->AddStdProc("fmtSelectTableFieldsDailog", fmtSelectTableFieldsDailog);
}
