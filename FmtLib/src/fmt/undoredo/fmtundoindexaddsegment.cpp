#include "fmtundoindexaddsegment.h"
#include "fmtindex.h"
#include "fmttable.h"

FmtUndoIndexAddSegment::FmtUndoIndexAddSegment(FmtTable *table, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    pTable = table;
    pSegment = NULL;
}

FmtUndoIndexAddSegment::~FmtUndoIndexAddSegment()
{

}

void FmtUndoIndexAddSegment::setSegmentRow(const quint16 &row, FmtIndex *pIndex)
{
    m_Segment = row;
    m_IndexRow = pIndex->indexNumber();
    setText(QObject::tr("Добавлен сегмент в индекс %1")
            .arg(pIndex->name()));
}

FmtSegment *FmtUndoIndexAddSegment::segment()
{
    return pSegment;
}

void FmtUndoIndexAddSegment::undo()
{
    pTable->m_IgnoreUndoStack = true;
    FmtIndex *pIndex = pTable->tableIndex(m_IndexRow);
    pIndex->removeSegmentPrivate(m_Segment, true);
    pSegment = NULL;
    pTable->m_IgnoreUndoStack = false;
}

void FmtUndoIndexAddSegment::redo()
{
    pTable->m_IgnoreUndoStack = true;
    FmtIndex *pIndex = pTable->tableIndex(m_IndexRow);
    pSegment = pIndex->addSegmentPrivate(m_Segment);
    pTable->m_IgnoreUndoStack = false;
}
