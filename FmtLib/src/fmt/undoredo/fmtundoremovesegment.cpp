#include "fmtundoremovesegment.h"
#include "fmttable.h"
#include "fmtindex.h"
#include "fmtsegment.h"

FmtUndoRemoveSegment::FmtUndoRemoveSegment(FmtTable *table, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    pTable = table;
    m_IndexRow = 0;
    m_Segment = 0;
}

void FmtUndoRemoveSegment::setSegmentRow(const quint16 &row, FmtIndex *pIndex)
{
    m_Segment = row;
    m_IndexRow = pIndex->indexNumber();
    setText(QObject::tr("Удален сегмент индекса %1")
            .arg(pIndex->name()));
}

void FmtUndoRemoveSegment::undo()
{
    pTable->m_IgnoreUndoStack = true;
    FmtIndex *pIndex = pTable->tableIndex(m_IndexRow);
    FmtSegment *pSeg = pIndex->addSegmentPrivate(m_Segment);
    pSeg->restoreData(&m_storedData);
    /*FmtIndex *pIndex = pTable->addIndexPrivate(m_Row);
    pIndex->restoreData(&m_storedData);*/
    pTable->m_IgnoreUndoStack = false;
}

void FmtUndoRemoveSegment::redo()
{
    pTable->m_IgnoreUndoStack = true;
    FmtIndex *pIndex = pTable->tableIndex(m_IndexRow);
    FmtSegment *pSeg = pIndex->segment(m_Segment);
    pSeg->storeData(&m_storedData);
    pIndex->removeSegmentPrivate(m_Segment, true);
    pTable->m_IgnoreUndoStack = false;
}
