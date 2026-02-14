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
    : RslExecutor{parent},
    pInfo(nullptr)
{
    pMainWindow = parent;

    FmtWorkWindow *wnd = pMainWindow->currentWorkWindow();
    pTable = wnd->table();
    pInfo = pMainWindow->currentConnection();
}

ToolbarActionExecutor::ToolbarActionExecutor(FmtSharedTablePtr Table, QWidget *parent)
    : RslExecutor{parent},
    pInfo(nullptr)
{
    pTable = Table;
    pInfo = pTable->connection();
}

ToolbarActionExecutor::~ToolbarActionExecutor()
{

}

void ToolbarActionExecutor::onInspectModuleSymbol(Qt::HANDLE sym)
{
    QString name = getSymbolName(sym);

    if (!name.compare("{CurrentConnection}", Qt::CaseInsensitive))
    {
        if (pInfo)
            globalSet(sym, QVariant::fromValue((QObject*)pInfo));
    }
    else if (!name.compare("{FmtTable}", Qt::CaseInsensitive))
    {
        globalSet(sym, QVariant::fromValue((QObject*)pTable.data()));
    }
}

void ToolbarActionExecutor::PlayRepProc()
{
    call("ExecAction", {});
}

void ToolbarActionExecutor::playRep(const QString &filename, const QString &output, RslExecutorProc proc)
{
    QWidget *pWidget = qobject_cast<QWidget*>(parent());
    QMap<QString,QString> meta = rslGetMacroInfo(filename);

    RslExecutor::playRep(filename, output, proc);
    QStringList err = errors();
    
    if (!err.isEmpty())
    {
        ErrorsModel model;

        for (int i = 0; i < err.size(); ++i)
            model.addError(err[i]);

        ErrorDlg dlg(ErrorDlg::ModeInformation, pWidget);
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

            int Highlighter = toolHighlighterByName(meta["Highlighter"]);

            QString code = toolReadTextFileContent(outputFileName(), "IBM 866");
            toolShowCodeDialog(pWidget, title, Highlighter, code);
        }
    }
}
