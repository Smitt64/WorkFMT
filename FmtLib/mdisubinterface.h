#ifndef MDISUBINTERFACE_H
#define MDISUBINTERFACE_H

#include "fmtlib_global.h"
#include <QtWidgets>
#include "fmttable.h"

class FMTLIBSHARED_EXPORT MdiSubInterface : public QDialog
{
    Q_OBJECT
public:
    MdiSubInterface(QWidget *parent = Q_NULLPTR);
    virtual ~MdiSubInterface();

    void setParentWnd(QMdiSubWindow *wnd);
    QMdiSubWindow *mdiWindow() { return pParentWnd; }

    QString makeWindowTitle(const FmtSharedTablePtr &pTable);

signals:
    void needUpdateTableList();

protected:
    QMdiSubWindow *pParentWnd;
};

#endif // MDISUBINTERFACE_H
