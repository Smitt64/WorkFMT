#include "fmtundotableaddindex.h"
#include "fmtindex.h"
#include "fmttable.h"

FmtUndoTableAddIndex::FmtUndoTableAddIndex(FmtTable *table, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    pTable = table;
    pIndex = Q_NULLPTR;
    m_Row = -1;
}

FmtUndoTableAddIndex::~FmtUndoTableAddIndex()
{
    pIndex = Q_NULLPTR;
}

FmtIndex *FmtUndoTableAddIndex::getIndex()
{
    return pIndex;
}

void FmtUndoTableAddIndex::undo()
{
    pTable->setIgnoreUndoStack(true);
    pTable->removeIndexPrivate(m_Row);
    pTable->setIgnoreUndoStack(false);
}

void FmtUndoTableAddIndex::redo()
{
    pTable->setIgnoreUndoStack(true);
    pIndex = pTable->addIndexPrivate();

    m_Row = pTable->tableIndexNum(pIndex);
    pTable->setIgnoreUndoStack(false);
}
