#include "fmtcommandspage.h"
#include "usercommandsgroup.h"
#include <QVBoxLayout>

FmtCommandsPage::FmtCommandsPage(QWidget *parent) :
    CommandsOptions(parent)
{
    m_pUserCmd = new UserCommandsGroup();

    QVBoxLayout *pVbox = dynamic_cast<QVBoxLayout*>(layout());
    pVbox->addWidget(m_pUserCmd);
}

FmtCommandsPage::~FmtCommandsPage()
{

}

int FmtCommandsPage::save()
{
    m_pUserCmd->save(settings());
    return CommandsOptions::save();
}

void FmtCommandsPage::restore()
{
    m_pUserCmd->load(settings());
    CommandsOptions::restore();
}
