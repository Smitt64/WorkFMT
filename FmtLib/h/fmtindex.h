#ifndef FMTINDEX_H
#define FMTINDEX_H

#include <QObject>
#include "fmtfield.h"
#include "fmtindecesmodelitem.h"
#include "fmtlib_global.h"

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
public:
    explicit FmtIndex(FmtTable *table, QObject *parent = 0);
    ~FmtIndex();
    void addField(FmtField *fld);

    QString name() const { return m_Name; }
    int nullValue() const { return m_NullValue; }
    bool isAutoInc() const;
    bool isUnique() const;
    bool isDup() const;
    bool isLocal() const;
    qint32 indexNumber() const;
    qint32 segmentsCount() const;
    FmtTable *table();

    void setDup(bool use);
    void setNullValue(const quint16 &val);
    void setAutoInc(bool use);
    void setLocal(bool use);

    virtual QVariant data(int column, int role = Qt::DisplayRole) const;
    virtual bool setData(int column, const QVariant &value);
    virtual void insertItem(int after);
signals:
    void indexChanged();

public slots:
    void NormalizeFlags();
    void setFlags(const quint32 &flags);
    void setName(const QString &n);
    void setType(const quint16 &n);
    void removeField(FmtField *pFld);

    void removeSegment(const quint16 &segmentIndex);

    void copyTo(FmtIndex *other);

    FmtSegment *segment(const qint32 &index);
    FmtSegment *addSegment(const quint32 &row);

private slots:
    void UpdateIndexName(const QString &value);

protected:
    virtual void storeData(QByteArray *data);
    virtual void restoreData(QByteArray *data);

private:
    void EmitIndexPropertyChange(const quint16 &prop);
    void PushUndoSegmentProperty(FmtSegment *segment, const quint32 &oldFlags);
    void setDataPrivate(const quint16 &fld, const QVariant &value);
    FmtSegment *addSegmentPrivate(const quint32 &row);
    void removeSegmentPrivate(const quint32 &index, bool AutoDelete);
    int save();
    quint32 m_Flags;
    quint16 m_Type;
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
