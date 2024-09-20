#ifndef FMTUNDOFIELDPROPERTY_H
#define FMTUNDOFIELDPROPERTY_H

#include <QObject>
#include <QUndoCommand>
#include <QVariant>
#include "fmtlib_global.h"

class FmtTable;
class FmtField;
class FmtUndoFieldProperty : public QUndoCommand
{
public:
    FmtUndoFieldProperty(FmtTable *table, const qint16 &row, QUndoCommand *parent = Q_NULLPTR);
    ~FmtUndoFieldProperty();

    void setValueToUndo(const QVariant &value);
    void setValueToRedo(const QVariant &value);
    void setProperty(const qint16 &property);

    virtual void undo();
    virtual void redo();

private:
    void createTypePropMacro();
    qint16 m_FldRow;
    qint16 m_Property;
    QVariant m_UndoValue, m_RedoValue;

    FmtTable *pTable;
};

#endif // FMTUNDOFIELDPROPERTY_H
