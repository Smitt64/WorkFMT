#include "massopenfunctemplate.h"
#include "massopenfucnparams.h"

MassOpenFuncTemplate::MassOpenFuncTemplate(QObject *parent) :
    MassOpInterface(parent),
    pParamPage(new MassOpenFucnParams())
{

}

MassOpenFuncTemplate::~MassOpenFuncTemplate()
{

}

void MassOpenFuncTemplate::initPages()
{
    addPage(pParamPage.data());
}

void MassOpenFuncTemplate::deinitPages()
{
    MassOpInterface::deinitPages();
}
