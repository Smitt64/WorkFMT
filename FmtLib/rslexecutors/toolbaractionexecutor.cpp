#include "toolbaractionexecutor.h"
#include "mdisubinterface.h"
#include "mainwindow.h"
#include "fmtworkwindow.h"
#include "connectioninfo.h"
#include "rsscript/registerobjlist.hpp"
#include <codeeditor/codehighlighter.h>
#include "toolsruntime.h"
#include <errordlg.h>
#include <errorsmodel.h>

ToolbarActionExecutor::ToolbarActionExecutor(MainWindow *parent)
    : RslExecutor{parent}
{
    pMainWindow = parent;
}

ToolbarActionExecutor::~ToolbarActionExecutor()
{

}

void ToolbarActionExecutor::onInspectModuleSymbol(Qt::HANDLE sym)
{
    QString name = getSymbolName(sym);

    if (!name.compare("{CurrentConnection}", Qt::CaseInsensitive))
    {
        ConnectionInfo *info = pMainWindow->currentConnection();

        if (info)
            globalSet(sym, QVariant::fromValue((QObject*)info));
    }
    else if (!name.compare("{FmtTable}", Qt::CaseInsensitive))
    {
        FmtWorkWindow *wnd = pMainWindow->currentWorkWindow();

        if (wnd)
        {
            FmtSharedTablePtr table = wnd->table();
            globalSet(sym, QVariant::fromValue((QObject*)table.data()));
        }
    }
}

void ToolbarActionExecutor::PlayRepProc()
{
    call("ExecAction", {});
}

void ToolbarActionExecutor::playRep(const QString &filename, const QString &output, RslExecutorProc proc)
{
    QMap<QString,QString> meta = rslGetMacroInfo(filename);

    RslExecutor::playRep(filename, output, proc);
    QStringList err = errors();
    
    if (!err.isEmpty())
    {
        ErrorsModel model;

        for (int i = 0; i < err.size(); ++i)
            model.addError(err[i]);

        ErrorDlg dlg(ErrorDlg::ModeInformation, pMainWindow);
        dlg.setErrors(&model);
        dlg.exec();
    }

    if (meta.contains("ShowReport"))
    {
        if (!meta["ShowReport"].compare("true"))
        {
            QString title = meta["Title"];

            if (title.isEmpty())
                title = meta["Description"];

            if (title.isEmpty())
                title = tr("Результат выполнения");

            QString code = toolReadTextFileContent(outputFileName(), "IBM 866");
            toolShowCodeDialog(pMainWindow, title, HighlighterPlain, code);
        }
    }
}
