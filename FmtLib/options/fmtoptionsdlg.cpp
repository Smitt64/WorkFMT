#include "fmtoptionsdlg.h"
#include <QSettings>
#include "generaloptions.h"
#include "tablegroupoptions.h"
#include <QIcon>

FmtOptionsDlg::FmtOptionsDlg(ConnectionInfo *connection, QSettings *settings, QWidget *parent) :
    OptionsDlg(settings, parent)
{
    setDefaultStyle("windowsvista");
    addPage(tr("Общие"), QIcon(":/img/explorer.exe_14_103-6.png"), new GeneralOptions());
    addStylePage(QString(), "style");
    addCodeEditorPage("CodeEditor", "theme");
    addRslPage();

    if (connection)
        addPage(tr("Группы таблиц"), QIcon(":/img/sapi.cpl_14_138-1.png"), new TableGroupOptions(connection));

    addCommandsPage();
    addLogPage("WorkFmt");
}

FmtOptionsDlg::~FmtOptionsDlg()
{

}
