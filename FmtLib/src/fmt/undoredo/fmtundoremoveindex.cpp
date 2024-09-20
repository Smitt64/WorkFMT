#include "fmtundoremoveindex.h"
#include "fmtindex.h"

FmtUndoRemoveIndex::FmtUndoRemoveIndex(FmtTable *table, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    pTable = table;
    m_PkIndex = -1;
}

FmtUndoRemoveIndex::~FmtUndoRemoveIndex()
{

}

void FmtUndoRemoveIndex::setIndexRow(const qint16 &row)
{
    m_Row = row;
    setText(QObject::tr("Удален индекс _IDX%1")
            .arg(QString::number(row, 16).toUpper()));
}

void FmtUndoRemoveIndex::undo()
{
    pTable->m_IgnoreUndoStack = true;
    FmtIndex *pIndex = pTable->addIndexPrivate(m_Row);
    pIndex->restoreData(&m_storedData);
    //pTable->setPkIDx(m_PkIndex);
    pTable->m_IgnoreUndoStack = false;
}

void FmtUndoRemoveIndex::redo()
{
    pTable->m_IgnoreUndoStack = true;
    FmtIndex *pIndex = pTable->tableIndex(m_Row);
    pIndex->storeData(&m_storedData);
    //m_PkIndex = pTable->pkIDx();
    pTable->removeIndexPrivate(m_Row);
    pTable->m_IgnoreUndoStack = false;
}
