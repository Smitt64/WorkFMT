#include "mdisubinterface.h"
#include "connectioninfo.h"

MdiSubInterface::MdiSubInterface(QWidget *parent) :
    QDialog(parent),
    pParentWnd(Q_NULLPTR),
    pConnection(Q_NULLPTR)
{

}

MdiSubInterface::~MdiSubInterface()
{

}

void MdiSubInterface::setParentWnd(QMdiSubWindow *wnd)
{
    pParentWnd = wnd;
}

ConnectionInfo *MdiSubInterface::connection() const
{
    return pConnection;
}

void MdiSubInterface::setConnection(ConnectionInfo *Connection)
{
    pConnection = Connection;
}

/*QString MdiSubInterface::makeWindowTitle()
{
    QString schemeName = pTable->connection()->schemeName();
    return QString("%1@%2")
            .arg(schemeName.trimmed())
            .arg(pTable->name());
}*/
