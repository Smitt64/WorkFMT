#ifndef FMTINDEX_H
#define FMTINDEX_H

#include <QObject>
#include "fmtfield.h"
#include "fmtindecesmodelitem.h"
#include "fmtlib_global.h"
#include "fmtcore.h"

class FmtTable;
class FmtSegment;
class FMTLIBSHARED_EXPORT FmtIndex : public FmtIndecesModelItem
{
    Q_OBJECT

    friend class FmtSegment;
    friend class FmtTable;
    friend class FmtUndoRemoveIndex;
    friend class FmtUndoRemoveSegment;
    friend class FmtUndoIndexAddSegment;
    friend class FmtUndoIndexProperty;

    Q_PROPERTY(QString Name READ name WRITE setName)
    Q_PROPERTY(int NullValue READ nullValue WRITE setNullValue)
    Q_PROPERTY(qint32 IndexNumber READ indexNumber)
    Q_PROPERTY(qint32 SegmentsCount READ segmentsCount)
    Q_PROPERTY(bool isDup READ isDup WRITE setDup)
    Q_PROPERTY(bool isAutoInc READ isAutoInc WRITE setAutoInc)
    Q_PROPERTY(bool isUnique READ isUnique)
    Q_PROPERTY(bool isLocal READ isLocal WRITE setLocal)
public:
    explicit FmtIndex(FmtTable *table, QObject *parent = Q_NULLPTR);
    ~FmtIndex();

    QString name() const { return m_Name; }
    int nullValue() const { return m_NullValue; }
    FmtTable *table();

    void setDup(bool use);
    void setNullValue(const quint16 &val);
    void setAutoInc(bool use);
    void setLocal(bool use);
    void setFlags(const qint32 &flags);
    void setName(const QString &n);
    void setType(const qint16 &n);
    void setKeyNum(const qint16 &n);

    bool isAutoInc() const;
    bool isUnique() const;
    bool isDup() const;
    bool isLocal() const;
    bool isPrimary() const;
    FmtKeyNullVal nullType() const;
    qint16 keyNum() const;

    virtual QVariant data(int column, int role = Qt::DisplayRole) const;
    virtual bool setData(int column, const QVariant &value);
    virtual void insertItem(int after) Q_DECL_OVERRIDE;
signals:
    void indexChanged();

public slots:
    void NormalizeFlags();
    void removeField(FmtField *pFld);
    void removeSegment(const quint16 &segmentIndex);
    qint16 indexNumber() const;
    qint32 segmentsCount() const;
    void copyTo(FmtIndex *other);

    FmtSegment *segment(const qint32 &index);
    FmtSegment *addSegment(const quint16 &row);
    bool hasField(FmtField *pFld);

private slots:
    void UpdateIndexName(const QString &value);

protected:
    virtual void storeData(QByteArray *data) override;
    virtual void restoreData(QByteArray *data) override;

private:
    void EmitIndexPropertyChange(const quint16 &prop);
    void PushUndoSegmentProperty(FmtSegment *segment, const qint32 &oldFlags, QUndoCommand *parentCommand = Q_NULLPTR);
    void setDataPrivate(const qint16 &fld, const QVariant &value);
    FmtSegment *addSegmentPrivate(const qint16 &row);
    void removeSegmentPrivate(const qint16 &index, bool AutoDelete);
    int save();

    qint32 m_Flags;
    qint16 m_Type, m_KeyNum;
    bool m_fDup, m_fAutoInc;
    bool m_fIgnoreStack;
    int m_NullValue;
    QString m_Name;

    FmtTable *pTable;
    QUndoStack *pUndoStack;
    QUndoCommand *pLastCommand;
};

QString FmtIndexPropertyTextByFieldId(const quint16 &section);
QString FmtUndoMacroTitle(const QString &name, const quint16 &prop);
#endif // FMTINDEX_H
