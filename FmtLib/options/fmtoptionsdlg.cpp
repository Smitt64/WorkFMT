#include "fmtoptionsdlg.h"
#include <QSettings>

FmtOptionsDlg::FmtOptionsDlg(QSettings *settings, QWidget *parent) :
    OptionsDlg(settings, parent)
{
    setDefaultStyle("windowsvista");
    addStylePage(QString(), "style");
    addCodeEditorPage();
}

FmtOptionsDlg::~FmtOptionsDlg()
{

}
