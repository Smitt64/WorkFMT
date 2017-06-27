#ifndef FMTUNDOINDEXPROPERTY_H
#define FMTUNDOINDEXPROPERTY_H

#include <QUndoCommand>
#include <QVariant>

class FmtTable;
class FmtIndex;
class FmtUndoIndexProperty : public QUndoCommand
{
public:
    FmtUndoIndexProperty(FmtTable *table, QUndoCommand *parent = NULL);
    virtual ~FmtUndoIndexProperty();

    void setValueToUndo(const QVariant &value);
    void setValueToRedo(const QVariant &value);
    void setProperty(const quint16 &index, const quint16 &property);

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;

    QString m_PropertyDisplayName;
    quint16 m_Property, m_Index;
    QVariant m_UndoValue, m_RedoValue;
};

#endif // FMTUNDOINDEXPROPERTY_H
