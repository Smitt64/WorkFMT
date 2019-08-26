#ifndef FMTSEGMENT_H
#define FMTSEGMENT_H

#include <QObject>
#include "fmtfield.h"
#include "fmtindecesmodelitem.h"
#include "fmtlib_global.h"

class FmtTable;
class FmtIndex;
class FMTLIBSHARED_EXPORT FmtSegment : public FmtIndecesModelItem
{
    Q_OBJECT

    Q_PROPERTY(bool isReal READ isReal WRITE setIsReal NOTIFY isRealChanged)
    Q_PROPERTY(bool isNotNull READ isNotNull WRITE setNotNull NOTIFY isNotNullChanged)
    Q_PROPERTY(bool descOrder READ descOrder WRITE setDescOrder NOTIFY descOrderChanged)
    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)

    Q_PROPERTY(bool isDup READ isDup)
    Q_PROPERTY(bool isAutoInc READ isAutoInc)
    Q_PROPERTY(bool isLocal READ isLocal)
    Q_PROPERTY(FmtFldType type READ type)
    Q_PROPERTY(quint32 segmentFlags READ segmentFlags)

    friend class FmtIndex;
    friend class FmtTable;
    friend class FmtUndoRemoveSegment;
    friend class FmtUndoIndexSegmentProperty;
public:
    FmtSegment(FmtIndecesModelItem *parent);

    virtual QVariant data(int column, int role = Qt::DisplayRole) const;
    virtual bool setData(int column, const QVariant &value);
    virtual void insertItem(int after);

    bool isReal() const;
    bool isNotNull() const;
    bool descOrder() const;
    QString comment() const;

    // read only
    bool isAutoInc() const;
    bool isLocal() const;
    bool isDup() const;
    FmtFldType type() const;
    FmtNumber10 flags() const;
    quint32 segmentFlags() const;

    void setNotNull(bool use);
    void setComment(const QString &val);
    void setDescOrder(bool use);
    void setIsReal(bool use);
    void setFlags(const FmtNumber10 &value);

    void copyTo(FmtSegment *other);

public slots:
    FmtField *field();
    void setField(FmtField *fld);
    void setField(const FmtFldIndex &fld);
    FmtFldIndex fieldIndex();

    FmtIndex *index();
    FmtTable *table();
    FmtFldIndex segmentNumber();

signals:
    void isRealChanged(bool);
    void isNotNullChanged(bool);
    void commentChanged(QString);
    void descOrderChanged(bool);

protected:
    virtual void storeData(QByteArray *data);
    virtual void restoreData(QByteArray *data);

private:
    void setDataPrivate(const QVariant &value, const quint16 &column);
    FmtField *pFld;
    bool m_fSetIgnoreUndoStack;
    FmtNumber10 m_Flags;
    bool m_IsReal;
    int m_ExcludeNull;
    QString m_Comment;
};

#endif // FMTSEGMENT_H
