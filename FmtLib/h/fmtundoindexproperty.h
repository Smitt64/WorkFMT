#ifndef FMTUNDOINDEXPROPERTY_H
#define FMTUNDOINDEXPROPERTY_H

#include <QUndoCommand>
#include <QVariant>
#include "fmtcore.h"

class FmtTable;
class FmtIndex;
class FmtUndoIndexProperty : public QUndoCommand
{
public:
    FmtUndoIndexProperty(FmtTable *table, QUndoCommand *parent = Q_NULLPTR);
    virtual ~FmtUndoIndexProperty();

    void setValueToUndo(const QVariant &value);
    void setValueToRedo(const QVariant &value);
    void setProperty(const qint16 &index, const qint16 &property);

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;

    QString m_PropertyDisplayName;
    qint16 m_Property, m_Index;
    QVariant m_UndoValue, m_RedoValue;
};

#endif // FMTUNDOINDEXPROPERTY_H
