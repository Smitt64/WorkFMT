#include "massopbtrvtemplate.h"
#include "massopbtrvtemplateresultpage.h"

MassOpBtrvTemplate::MassOpBtrvTemplate(QObject *parent) :
    MassOpInterface(parent),
    pResultPage(new MassOpBtrvTemplateResultPage())
{

}

MassOpBtrvTemplate::~MassOpBtrvTemplate()
{

}

void MassOpBtrvTemplate::initPages()
{
    addPage(pResultPage.data());
}

void MassOpBtrvTemplate::deinitPages()
{
    MassOpInterface::deinitPages();
}
