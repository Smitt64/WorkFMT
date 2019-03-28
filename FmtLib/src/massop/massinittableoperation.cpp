#include "massinittableoperation.h"
#include "inittables/massinittableparams.h"
#include "inittables/massinittablesprogress.h"
#include "massoperationwizard.h"

MassInitTableOperation::MassInitTableOperation(QObject *parant) :
    MassOpInterface(parant),
    pParamsPage(new MassInitTableParams()),
    pParamsProgress(new MassInitTablesProgress())
{
}

MassInitTableOperation::~MassInitTableOperation()
{

}

void MassInitTableOperation::initPages()
{
    pParamsPage->setTables(pWizard->tables());
    addPage(pParamsPage.data());
    addPage(pParamsProgress.data());
}

void MassInitTableOperation::deinitPages()
{
    MassOpInterface::deinitPages();
}
