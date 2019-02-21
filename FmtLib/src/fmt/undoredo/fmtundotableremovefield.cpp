#include "fmtundotableremovefield.h"
#include "fmtfield.h"

FmtUndoTableRemoveField::FmtUndoTableRemoveField(FmtTable *table, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    pTable = table;
    m_Row = 0;
}

FmtUndoTableRemoveField::~FmtUndoTableRemoveField()
{
    m_StoredData.clear();
}

QByteArray *FmtUndoTableRemoveField::storedDataPtr()
{
    return &m_StoredData;
}

void FmtUndoTableRemoveField::setFieldRow(const FmtFldIndex &row)
{
    m_Row = row;
}

void FmtUndoTableRemoveField::undo()
{
    pTable->setIgnoreUndoStack(true);
    FmtField *fld = pTable->insertFieldPrivate(m_Row, "", 0);
    fld->restoreData(&m_StoredData);
    pTable->setIgnoreUndoStack(false);
}

void FmtUndoTableRemoveField::redo()
{
    FmtField *fld = pTable->field(m_Row);
    pTable->setIgnoreUndoStack(true);
    m_StoredData.clear();
    fld->stroreData(&m_StoredData);
    pTable->removeFieldPrivate(m_Row);
    pTable->setIgnoreUndoStack(false);
}
