#include "comparefmtwizard.h"
#include "compareparamspage.h"
#include <QIcon>

CompareFmtWizard::CompareFmtWizard(QWidget *parent) :
    QWizard(parent)
{
    m_ParamsPage = new CompareParamsPage(this);
    addPage(m_ParamsPage);

    setWindowIcon(QIcon(":/img/SychronizeListHS.png"));
}

CompareFmtWizard::~CompareFmtWizard()
{

}
