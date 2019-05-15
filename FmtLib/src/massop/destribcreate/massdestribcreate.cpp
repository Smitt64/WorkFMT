#include "massdestribcreate.h"
#include "massdestribparamspage.h"
#include "massdestribparammodel.h"
#include "massdestribprogress.h"

MassDestribCreate::MassDestribCreate(QObject *parent) :
    MassOpInterface(parent),
    mParams(new MassDestribParamsPage()),
    mProgress(new MassDestribProgress())
{
    pModel = new MassDestribParamModel(this);
}

MassDestribCreate::~MassDestribCreate()
{

}

void MassDestribCreate::initPages()
{
    addPage(mParams.data());
    addPage(mProgress.data());
}

void MassDestribCreate::deinitPages()
{
    MassOpInterface::deinitPages();
}

MassDestribParamModel *MassDestribCreate::model()
{
    return pModel;
}
