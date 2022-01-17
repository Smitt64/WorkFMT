#ifndef SUBWINDOWSMODEL_H
#define SUBWINDOWSMODEL_H

#include <QObject>
#include <QtCore>
#include <QMdiArea>
#include "fmtlib_global.h"

class ConnectionInfo;
class SubWindowsTreeItem
{
    friend class SubWindowsModel;
public:
    explicit SubWindowsTreeItem(const QList<QVariant> &data, SubWindowsTreeItem *parentItem = nullptr);
    ~SubWindowsTreeItem();

    void appendChild(SubWindowsTreeItem *child);

    SubWindowsTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column, int role = Qt::DisplayRole) const;
    int row() const;
    SubWindowsTreeItem *parentItem();

private:
    ConnectionInfo *info;
    QList<SubWindowsTreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    SubWindowsTreeItem *m_parentItem;

    QMdiSubWindow *wnd;
};

class FMTLIBSHARED_EXPORT SubWindowsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SubWindowsModel(QObject *parent = nullptr);
    virtual ~SubWindowsModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    void addConnection(ConnectionInfo *info);
    QModelIndex addWindow(ConnectionInfo *info, QMdiSubWindow *wnd);

    QModelIndex findWindow(ConnectionInfo *info, QMdiSubWindow *wnd);
    QMdiSubWindow *window(const QModelIndex &index);

public slots:
    void removeConnection(ConnectionInfo *info);

signals:
    void windowsUpdated();

private slots:
    void windowClosed(QObject*);

private:
    SubWindowsTreeItem *rootItem;
};

#endif // SUBWINDOWSMODEL_H
