#include "fmtrslmodule.h"
#include "rsscript/registerobjlist.hpp"
#include "rslexecutor.h"
#include "mdisubinterface.h"
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
            ThrowParamTypeError(prm_Table);
    }
    else
        ThrowParamTypeError(prm_Table);

    type = GetFuncParamType(prm_Title);
    if (type == QVariant::String)
        title = GetFuncParam(prm_Title).toString();

    QList<FmtField*> pFldList;
    int stat = SelectTableFieldsDlg(table, title, &pFldList);

    if (!stat)
    {
        QVariantList list;
        for (FmtField *fld : pFldList)
            list.append(QVariant::fromValue((QObject*)fld));

        SetReturnVal(QVariant::fromValue(list));
    }
}

void fmtappRegister()
{
    RegisterObjList::inst()->AddObject<FmtWorkWindow>(false);

    RegisterObjList::inst()->AddStdProc("fmtAppHasTableWindow", fmtAppHasTableWindow);
    RegisterObjList::inst()->AddStdProc("fmtAppCurrentTableWindow", fmtAppCurrentTableWindow);
    RegisterObjList::inst()->AddStdProc("fmtappOpenTableWindow", fmtappOpenTableWindow);

    RegisterObjList::inst()->AddStdProc("fmtSelectTableFieldsDailog", fmtSelectTableFieldsDailog);
}
