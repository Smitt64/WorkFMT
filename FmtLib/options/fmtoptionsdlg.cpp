#include "fmtoptionsdlg.h"
#include <QSettings>
#include "generaloptions.h"
#include <QIcon>

FmtOptionsDlg::FmtOptionsDlg(QSettings *settings, QWidget *parent) :
    OptionsDlg(settings, parent)
{
    setDefaultStyle("windowsvista");
    addPage(tr("Общие"), QIcon(":/img/explorer.exe_14_103-6.png"), new GeneralOptions());
    addStylePage(QString(), "style");
    addCodeEditorPage("CodeEditor", "theme");
    addRslPage();
}

FmtOptionsDlg::~FmtOptionsDlg()
{

}
