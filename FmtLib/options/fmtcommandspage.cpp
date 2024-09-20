#include "fmtcommandspage.h"
#include "usercommandsgroup.h"
#include <QVBoxLayout>

FmtCommandsPage::FmtCommandsPage(QWidget *parent) :
    CommandsOptions(parent)
{
    m_pUserCmd = new UserCommandsGroup(this);
    m_pUserCmd->setVisible(true);

    QVBoxLayout *pVbox = dynamic_cast<QVBoxLayout*>(layout());
    pVbox->addWidget(m_pUserCmd);

    adjustSize();
}

FmtCommandsPage::~FmtCommandsPage()
{

}
