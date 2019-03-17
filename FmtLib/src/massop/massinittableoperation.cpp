#include "massinittableoperation.h"
#include "inittables/massinittableparams.h"
#include "massoperationwizard.h"

MassInitTableOperation::MassInitTableOperation(QObject *parant) :
    MassOpInterface(parant),
    pParamsPage(new MassInitTableParams())
{
}

MassInitTableOperation::~MassInitTableOperation()
{

}

void MassInitTableOperation::initPages()
{
    pParamsPage->setTables(pWizard->tables());
    addPage(pParamsPage.data());
}

void MassInitTableOperation::deinitPages()
{
    MassOpInterface::deinitPages();
}
