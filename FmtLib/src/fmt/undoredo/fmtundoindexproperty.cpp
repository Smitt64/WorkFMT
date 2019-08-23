#include "fmtundoindexproperty.h"
#include "fmtindex.h"
#include "fmttable.h"

FmtUndoIndexProperty::FmtUndoIndexProperty(FmtTable *table, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    pTable = table;
    m_Index = -1;
}

FmtUndoIndexProperty::~FmtUndoIndexProperty()
{

}

void FmtUndoIndexProperty::setValueToUndo(const QVariant &value)
{
    m_UndoValue = value;
}

void FmtUndoIndexProperty::setValueToRedo(const QVariant &value)
{
    m_RedoValue = value;
}

void FmtUndoIndexProperty::setProperty(const FmtFldIndex &index, const FmtFldIndex &property)
{
    m_Index = index;
    FmtIndex *pIndex = pTable->tableIndex(m_Index);
    m_PropertyDisplayName = QObject::tr("Изменено свойство '%1' индекса %3: %2")
            .arg(FmtIndexPropertyTextByFieldId(property).simplified())
            .arg(m_RedoValue.toString())
            .arg(pIndex->name());
    setText(m_PropertyDisplayName);
    m_Property = property;
}

void FmtUndoIndexProperty::undo()
{
    pTable->setIgnoreUndoStack(true);
    FmtIndex *pIndex = pTable->tableIndex(m_Index);
    pIndex->setDataPrivate(m_Property, m_UndoValue);
    pTable->setIgnoreUndoStack(false);
}

void FmtUndoIndexProperty::redo()
{
    pTable->setIgnoreUndoStack(true);
    FmtIndex *pIndex = pTable->tableIndex(m_Index);
    pIndex->setDataPrivate(m_Property, m_RedoValue);
    pTable->setIgnoreUndoStack(false);
}
