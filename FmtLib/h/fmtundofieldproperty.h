#ifndef FMTUNDOFIELDPROPERTY_H
#define FMTUNDOFIELDPROPERTY_H

#include <QObject>
#include <QUndoCommand>
#include <QVariant>

class FmtTable;
class FmtField;
class FmtUndoFieldProperty : public QUndoCommand
{
public:
    FmtUndoFieldProperty(FmtTable *table, const quint32 &row, QUndoCommand *parent = NULL);
    ~FmtUndoFieldProperty();

    void setValueToUndo(const QVariant &value);
    void setValueToRedo(const QVariant &value);
    void setProperty(const quint16 &property);

    virtual void undo();
    virtual void redo();

private:
    void createTypePropMacro();
    quint32 m_FldRow;
    quint16 m_Property;
    QVariant m_UndoValue, m_RedoValue;

    FmtTable *pTable;
};

#endif // FMTUNDOFIELDPROPERTY_H
