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
    enum FindSubText
    {
        FindSubTextRole = Qt::UserRole + 1
    };

    FmtFildsModel(FmtTable *parent);
    virtual ~FmtFildsModel();
    bool isInsertRow(const QModelIndex &index) const;

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

    FmtTable *fmtTable();
    QVariant ProcessHighlightFields(const QModelIndex &index, int role, const QString &HighlightText = QString(), const QString &color = QString("9BFF9B")) const;
    QVariant ProcessHighlightFieldsEx(const QString &text, const QString &HighlightText = QString(), const QString &color = QString("9BFF9B")) const;

signals:
    void AddedToLast(FmtField *fld, const QModelIndex &index);

private:
    void setFieldPropertyChanged(const FmtField *fld, const FmtFldIndex &prop);
    FmtTable *pTable;
};

#endif // FMTFILDSMODEL_H
