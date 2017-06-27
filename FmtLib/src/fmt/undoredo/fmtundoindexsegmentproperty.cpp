#include "fmtundoindexsegmentproperty.h"
#include "fmttable.h"
#include "fmtindex.h"
#include "fmtsegment.h"

FmtUndoIndexSegmentProperty::FmtUndoIndexSegmentProperty(FmtTable *table, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    pTable = table;
}

FmtUndoIndexSegmentProperty::~FmtUndoIndexSegmentProperty()
{

}

void FmtUndoIndexSegmentProperty::setValueToUndo(const QVariant &value)
{
    m_UndoValue = value;
}

void FmtUndoIndexSegmentProperty::setValueToRedo(const QVariant &value)
{
    m_RedoValue = value;
}

void FmtUndoIndexSegmentProperty::setProperty(const quint16 &index, const quint16 &segment, const quint16 &property)
{
    m_Index = index;
    m_Segment = segment;

    m_PropertyDisplayName = QObject::tr("Изменено свойство '%1' сегмента #%2")
            .arg(FmtIndexPropertyTextByFieldId(property).simplified())
            .arg(segment);

    setText(m_PropertyDisplayName);
    m_Property = property;
}

void FmtUndoIndexSegmentProperty::undo()
{
    pTable->setIgnoreUndoStack(true);
    FmtIndex *pIndex = pTable->tableIndex(m_Index);
    FmtSegment *pSegment = pIndex->segment(m_Segment);
    pSegment->setDataPrivate(m_UndoValue, m_Property);
    pTable->setIgnoreUndoStack(false);
}

void FmtUndoIndexSegmentProperty::redo()
{
    pTable->setIgnoreUndoStack(true);
    FmtIndex *pIndex = pTable->tableIndex(m_Index);
    FmtSegment *pSegment = pIndex->segment(m_Segment);
    pSegment->setDataPrivate(m_RedoValue, m_Property);
    pTable->setIgnoreUndoStack(false);
}
