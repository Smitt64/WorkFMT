#ifndef FMTINDECESMODELITEM_H
#define FMTINDECESMODELITEM_H

#include <QtCore>
#include "fmtlib_global.h"

class FMTLIBSHARED_EXPORT FmtIndecesModelItem : public QObject
{
    Q_OBJECT

    friend class FmtTable;
    friend class FmtIndecesModel;
    friend class FmtIndex;
public:
    enum
    {
        fld_Name,
        fld_Type,
        fld_Dup,
        fld_Null,
        fld_NotNull,
        fld_AutoInc,
        fld_Desc,
        fld_ExcIndx,
        fld_Local,
        fld_Flag,
        fld_Comment,

        fld_MAXCOUNT
    };
    FmtIndecesModelItem(FmtIndecesModelItem *parent = Q_NULLPTR, QObject *parentObject = Q_NULLPTR);
    virtual ~FmtIndecesModelItem();

    FmtIndecesModelItem *child(int number);
    FmtIndecesModelItem *parent();

    virtual QVariant data(int column, int role = Qt::DisplayRole) const = 0;
    virtual bool setData(int column, const QVariant &value) = 0;

    FmtIndecesModelItem *take(int i);
    virtual void insertItem(int after);

    quint32 indexOfChild(FmtIndecesModelItem *item) const;

    int childCount() const;
    int childNumber() const;

    void appendChild(FmtIndecesModelItem *intem);
    void insertChild(const int &after, FmtIndecesModelItem *intem);

    void setFlag(int f);
    int flag() { return m_Flag; }

protected:
    virtual void storeData(QByteArray *data) = 0;
    virtual void restoreData(QByteArray *data) = 0;
    //void insertChild(const int &after, FmtIndecesModelItem *item);
    QList<FmtIndecesModelItem*> childItems;
    FmtIndecesModelItem *pParentItem;
    int m_Flag;
};

#endif // FMTINDECESMODELITEM_H
