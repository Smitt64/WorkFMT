#include "fmtrslmodule.h"
#include "rsscript/registerobjlist.hpp"
#include "rslexecutor.h"
#include "mdisubinterface.h"
#include <fmtworkwindow.h>
#include <mainwindow.h>
#include <QApplication>
#include <fmttable.h>

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

    SetReturnVal(QVariant::fromValue(w));
}

static void fmtAppCurrentTableWindow()
{
    MainWindow *mwnd = GetMainWindow();

    if (!mwnd)
        return;
        
    FmtWorkWindow *w = mwnd->currentWorkWindow();

    if (!w)
        return;
    
    SetReturnVal(QVariant::fromValue(w));
}

void fmtappRegister()
{
    RegisterObjList::inst()->AddObject<FmtWorkWindow>();

    RegisterObjList::inst()->AddStdProc("fmtAppHasTableWindow", fmtAppHasTableWindow);
    RegisterObjList::inst()->AddStdProc("fmtAppCurrentTableWindow", fmtAppCurrentTableWindow);
}
