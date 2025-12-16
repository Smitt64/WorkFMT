#include "operationbase.h"

OperationBase::OperationBase(QObject *parent) :
    QObject(parent),
    QRunnable()
{

}

void OperationBase::setWizzard(RegParmWizard *wzrd)
{
    m_pWizzard = wzrd;
}

RegParmWizard *OperationBase::wizzard()
{
    return m_pWizzard;
}
