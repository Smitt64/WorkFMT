#include "fmtundofieldproperty.h"
#include "fmtfield.h"
#include "fmttable.h"

FmtUndoFieldProperty::FmtUndoFieldProperty(FmtTable *table, const quint32 &row, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    m_FldRow = row;
    pTable = table;
}

FmtUndoFieldProperty::~FmtUndoFieldProperty()
{

}

void FmtUndoFieldProperty::setValueToUndo(const QVariant &value)
{
    m_UndoValue = value;
}

void FmtUndoFieldProperty::setValueToRedo(const QVariant &value)
{
    m_RedoValue = value;
}

void FmtUndoFieldProperty::setProperty(const quint16 &property)
{
    setText(QObject::tr("Изменено свойство '%1'")
            .arg(FmtFieldPropertyTextById(property)));
    m_Property = property;
}

void FmtUndoFieldProperty::undo()
{
    FmtField *fld = pTable->field(m_FldRow);
    fld->setDataPrivate(m_Property, m_UndoValue);
}

void FmtUndoFieldProperty::redo()
{
    FmtField *fld = pTable->field(m_FldRow);
    fld->setDataPrivate(m_Property, m_RedoValue);
}
