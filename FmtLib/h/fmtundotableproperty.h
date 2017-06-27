#ifndef HMTUNDOTABLEPROPERTY_H
#define HMTUNDOTABLEPROPERTY_H

#include <QObject>
#include <QUndoCommand>
#include <QVariant>

class FmtTable;
class FmtUndoTableProperty : public QUndoCommand
{
public:
    FmtUndoTableProperty(FmtTable *table, QUndoCommand *parent = NULL);
    ~FmtUndoTableProperty();

    void setValueToUndo(const QVariant &value);
    void setValueToRedo(const QVariant &value);
    void setProperty(const quint16 &property);

    virtual void undo();
    virtual void redo();

private:
    FmtTable *pTable;
    QString m_PropertyDisplayName;
    quint16 m_Property;
    QVariant m_UndoValue, m_RedoValue;
};

#endif // HMTUNDOTABLEPROPERTY_H
