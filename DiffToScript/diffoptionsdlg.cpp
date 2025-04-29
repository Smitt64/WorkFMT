#include "diffoptionsdlg.h"
#include "tablemacoptionspage.h"
#include <QIcon>

DiffOptionsDlg::DiffOptionsDlg(QSettings *settings, QWidget *parent) :
    OptionsDlg(settings, parent)
{
    m_TableMacOptionsPage = new TableMacOptionsPage(this);
    setDefaultStyle("windowsvista");

    addPage(tr("Макросы"), QIcon("://img/msgr3en.dll_14_128_0-0.png"), m_TableMacOptionsPage);
    addRslPage();
}

DiffOptionsDlg::~DiffOptionsDlg()
{

}
