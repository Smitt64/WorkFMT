#ifndef MASSDESTRIBPARAMMODEL_H
#define MASSDESTRIBPARAMMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QMap>
#include "fmttable.h"
#include "fmtfield.h"

typedef struct
{
}MassDestribParamModelFldElement;

typedef struct
{
    FmtSharedTablePtr table;
    QMap<FmtField, MassDestribParamModelFldElement> flds;
}MassDestribParamModelElement;

class ConnectionInfo;
class MassDestribCreate;
class MassDestribParamModel : public QAbstractItemModel
{
public:
    enum
    {
        fld_Name = 0,
        fld_Action,
        fld_UnloadFmt,
        fld_UnloadDat,

        fld_Count
    };

    MassDestribParamModel(MassDestribCreate *info, QObject *parent = Q_NULLPTR);
    virtual ~MassDestribParamModel() Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;


    void setTables(const QStringList &list);

private:
    QList<MassDestribParamModelElement*> m_Tables;
    MassDestribCreate *pInfo;
};

#endif // MASSDESTRIBPARAMMODEL_H
