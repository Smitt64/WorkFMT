#ifndef FMTFILDSMODEL_H
#define FMTFILDSMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QSharedPointer>
#include "fmtlib_global.h"

class FmtTable;
class FmtField;
class FMTLIBSHARED_EXPORT FmtFildsModel : public QAbstractItemModel
{
    Q_OBJECT
    friend class FmtTable;
    friend class FmtField;
public:
    enum
    {
        fld_Name,
        fld_Type,
        fld_Size,
        fld_Comment,
        fld_DbName,
        fld_Custom,
        fld_Offset,
        fld_Outlen,
        fld_Decpoint,
        fld_Hidden,
        fld_Id,
        fld_FmtId,
        fld_TypeIndex
    };
    FmtFildsModel(FmtTable *parent);

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent (const QModelIndex &index) const;
    virtual int rowCount (const QModelIndex &parent = QModelIndex()) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    void beginRemoveField(int row);
    void endRemoveField();
    //void setFieldChanged(const int &row, const int &column);
    FmtTable *fmtTable();
signals:
    void AddedToLast(FmtField *fld, const QModelIndex &index);

private:
    void setFieldPropertyChanged(const FmtField *fld, const quint16 &prop);
    FmtTable *pTable;
};

#endif // FMTFILDSMODEL_H
