#ifndef FMTINDECESMODEL_H
#define FMTINDECESMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QSharedPointer>
#include "fmtlib_global.h"

class FmtTable;
class FmtIndex;
class FmtSegment;
class FmtIndecesModelItem;
class FMTLIBSHARED_EXPORT FmtIndecesModel : public QAbstractItemModel
{
    Q_OBJECT
    friend class FmtTable;
    friend class FmtIndex;
    friend class FmtSegment;
public:
    FmtIndecesModel(FmtTable *parent);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex indexForItem(FmtIndecesModelItem *item) const;

    void addIndex();

    void setPropertyChanged(FmtIndecesModelItem *item, const quint16 &column);

signals:
    void indexAdded();

protected:
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:
    void RemoveFmtIndex(FmtIndex *pIndex);
    void RemoveFmtSegment(FmtSegment *pIndex);
    FmtIndecesModelItem *getItem(const QModelIndex &index) const;
    FmtTable *pTable;
    FmtIndecesModelItem *rootItem;
};

#endif // FMTINDECESMODEL_H
