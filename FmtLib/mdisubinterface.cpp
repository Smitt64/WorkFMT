#include "mdisubinterface.h"
#include "connectioninfo.h"

MdiSubInterface::MdiSubInterface(QWidget *parent) :
    QDialog(parent),
    pParentWnd(Q_NULLPTR)
{

}

MdiSubInterface::~MdiSubInterface()
{

}

void MdiSubInterface::setParentWnd(QMdiSubWindow *wnd)
{
    pParentWnd = wnd;
}

QString MdiSubInterface::makeWindowTitle(const FmtSharedTablePtr &pTable)
{
    QString schemeName = pTable->connection()->schemeName();
    return QString("%1@%2")
            .arg(schemeName.trimmed())
            .arg(pTable->name());
}
