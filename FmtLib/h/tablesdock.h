#ifndef TABLESDOCK_H
#define TABLESDOCK_H

#include <QtWidgets/QtWidgets>
#include <QAbstractItemModel>
#include "fmtlib_global.h"

class FmtTablesModel;
class TablesDockWidget;
class FMTLIBSHARED_EXPORT TablesDock : public QDockWidget
{
    Q_OBJECT
public:
    TablesDock(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0);
    virtual ~TablesDock();
    void setModel(FmtTablesModel *model);
    TablesDockWidget *tablesWidget();

signals:
    void tableDbClicked(quint32);

private:
    TablesDockWidget *pWidget;
};

#endif // TABLESDOCK_H
