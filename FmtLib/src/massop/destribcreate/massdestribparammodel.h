#ifndef MASSDESTRIBPARAMMODEL_H
#define MASSDESTRIBPARAMMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QMap>
#include "fmttable.h"
#include "fmtfield.h"

typedef struct tagMassDestribParamModelFldElement
{
    tagMassDestribParamModelFldElement()
    {
        action = 0;
    }
    qint16 action;
}MassDestribParamModelFldElement;

typedef struct tagMassDestribParamModelElement
{
    tagMassDestribParamModelElement()
    {
        UnloadFmt = true;
        UnloadCreateTables = true;
        UnloadDat = false;
        action = 0;
    }
    FmtSharedTablePtr table;
    qint16 action;
    bool UnloadFmt, UnloadCreateTables, UnloadDat;
    QMap<FmtField*, MassDestribParamModelFldElement> flds;
}MassDestribParamModelElement;

class ConnectionInfo;
class MassDestribCreate;
class MassDestribParamModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum
    {
        fld_Name = 0,
        fld_Action,
        fld_UnloadFmt,
        fld_UnloadCreateTables,
        fld_UnloadDat,

        fld_Count
    };

    enum
    {
        ActionCreateTable = 0,
        ActionModifyFld,
    };

    enum
    {
        ActionFldSkip = 0,
        ActionFldAdd,
        ActionFldEdit,
        ActionFldRemove
    };

    MassDestribParamModel(MassDestribCreate *info, QObject *parent = Q_NULLPTR);
    virtual ~MassDestribParamModel() Q_DECL_OVERRIDE;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    const QStringList &getTablesActions() const;
    const QStringList &getFieldsActions() const;

    void setTables(const QStringList &list);

    const MassDestribParamModelElement *getTableParam(const int &index);

private:
    QList<MassDestribParamModelElement*> m_Tables;
    MassDestribCreate *pInfo;

    static QStringList m_TableActions, m_FldsActions;
};

#endif // MASSDESTRIBPARAMMODEL_H
