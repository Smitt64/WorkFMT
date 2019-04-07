#include "massdestribcreate.h"
#include "massdestribparamspage.h"
#include "massdestribparammodel.h"

MassDestribCreate::MassDestribCreate(QObject *parent) :
    MassOpInterface(parent),
    mParams(new MassDestribParamsPage())
{
    pModel = new MassDestribParamModel(this);
}

MassDestribCreate::~MassDestribCreate()
{

}

void MassDestribCreate::initPages()
{
    addPage(mParams.data());
}

void MassDestribCreate::deinitPages()
{
    MassOpInterface::deinitPages();
}

MassDestribParamModel *MassDestribCreate::model()
{
    return pModel;
}
