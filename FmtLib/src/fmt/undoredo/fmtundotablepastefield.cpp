#include "fmtundotablepastefield.h"
#include "fmtfield.h"

FmtUndoTablePasteField::FmtUndoTablePasteField(FmtTable *table, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    pTable = table;
    pField = Q_NULLPTR;
    m_Befor = -1;
    m_Row = 0;
}

FmtUndoTablePasteField::~FmtUndoTablePasteField()
{

}

FmtField *FmtUndoTablePasteField::getField() const
{
    return pField;
}

void FmtUndoTablePasteField::setData(const QMap<quint16,QVariant> &data)
{
    fldDataMap = data;
    setText(QObject::tr("Вставка поля из буффера %1").arg(data[FmtField::fld_Name].toString()));
}

void FmtUndoTablePasteField::setInsertBefor(const FmtFldIndex &befor)
{
    m_Befor = befor;
}

void FmtUndoTablePasteField::undo()
{
    pTable->setIgnoreUndoStack(true);
    pTable->removeInsertedFieldPrivate(m_Row);
    pTable->setIgnoreUndoStack(false);
    pField = Q_NULLPTR;
}

void FmtUndoTablePasteField::redo()
{
    pTable->setIgnoreUndoStack(true);

    if (m_Befor == -1)
        pField = pTable->addFieldPrivate(fldDataMap);
    else
        pField = pTable->insertFieldPrivate(m_Befor, fldDataMap);

    m_Row = pTable->fieldNum(pField);

    pTable->setIgnoreUndoStack(false);
}
