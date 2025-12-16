#include "addfromwordoperation.h"
#include "regparmwizard.h"
#include "wordpreviewregpage.h"
#include "rsl/reginfoobj.h"

AddFromWordOperation::AddFromWordOperation(QObject *parent)
    : OperationBase(parent)
{

}

void AddFromWordOperation::AddFromWordOperation::run()
{
    WordPreviewRegPage *PreviewPage = wizzard()->wordPreviewRegPage();
    QSharedPointer<RegInfoObjModel> model = PreviewPage->model();
    RegInfoObjList RegsInfoList = model->getAllObjects();

    for (const auto &RegInfo : qAsConst(RegsInfoList))
        processRegInfo(RegInfo);

    emit finished();
}

void AddFromWordOperation::processRegInfo(const QSharedPointer<RegInfoObj> &obj)
{

}
