#ifndef MDISUBINTERFACE_H
#define MDISUBINTERFACE_H

#include "fmtlib_global.h"
#include <QtWidgets>
#include "fmttable.h"

class ConnectionInfo;
class FMTLIBSHARED_EXPORT MdiSubInterface : public QDialog
{
    Q_OBJECT
public:
    MdiSubInterface(QWidget *parent = Q_NULLPTR);
    virtual ~MdiSubInterface();

    void setParentWnd(QMdiSubWindow *wnd);
    QMdiSubWindow *mdiWindow() { return pParentWnd; }

    virtual QString makeWindowTitle() = 0;

    ConnectionInfo *connection() const;
    void setConnection(ConnectionInfo *Connection);

signals:
    void needUpdateTableList();

protected:
    QMdiSubWindow *pParentWnd;
    ConnectionInfo *pConnection;
};

#endif // MDISUBINTERFACE_H
