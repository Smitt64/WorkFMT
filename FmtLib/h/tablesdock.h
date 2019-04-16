#ifndef TABLESDOCK_H
#define TABLESDOCK_H

#include <QtWidgets/QtWidgets>
#include <QAbstractItemModel>
#include "fmtlib_global.h"

class FmtTablesModel;
class TablesDockWidget;
class ConnectionInfo;
class FMTLIBSHARED_EXPORT TablesDock : public QDockWidget
{
    Q_OBJECT
public:
    TablesDock(const QString &title, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::Widget);
    virtual ~TablesDock();
    void setModel(FmtTablesModel *model);
    TablesDockWidget *tablesWidget();

    void setConnection(ConnectionInfo *info);
    void setItemDelegate(QAbstractItemDelegate *delegate);
    void closeConnection();
    void setEventFilter(QObject *objEventFilter);

public slots:
    void forceRepaint();

signals:
    void tableDbClicked(quint32);

private slots:
    void tabCloseRequested(const int &index);

private:
    void addFilterTab();
    QTabWidget *pTabBar;
    ConnectionInfo *pInfo;
    QAbstractItemDelegate *pDelegate;
    QPushButton *pAddButton;
    QFrame *pBtnContainer;
    QHBoxLayout *pLayout;
    QList<TablesDockWidget*> pWidget;
    QObject *pEventFilter;
};

#endif // TABLESDOCK_H
