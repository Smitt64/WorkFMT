#include "fmtundotableaddfield.h"
#include "fmttable.h"
#include "fmtcore.h"

FmtUndoTableAddField::FmtUndoTableAddField(FmtTable *table, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    pTable = table;
    pField = Q_NULLPTR;
    m_Befor = -1;
    m_Row = 0;
}

void FmtUndoTableAddField::setNameAndType(const QString &name, const FmtFldType &type)
{
    m_Name = name;
    m_Type = type;

    if (m_Name.toLower() == name || m_Name.toUpper() == name)
        m_Name = FmtCapitalizeField(m_Name);

    QString mtypename = fmtTypeNameForType(type);
    setText(QObject::tr("Добавлено поле типа %2: %1")
            .arg(m_Name)
            .arg(mtypename));
}

void FmtUndoTableAddField::setInsertBefor(const FmtFldIndex &befor)
{
    m_Befor = befor;
}

void FmtUndoTableAddField::undo()
{
    pTable->setIgnoreUndoStack(true);
    pTable->removeInsertedFieldPrivate(m_Row);
    pTable->setIgnoreUndoStack(false);
    pField = Q_NULLPTR;
}

void FmtUndoTableAddField::redo()
{
    pTable->setIgnoreUndoStack(true);

    if (m_Befor == -1)
        pField = pTable->addFieldPrivate(m_Name, m_Type);
    else
        pField = pTable->insertFieldPrivate(m_Befor, m_Name, m_Type);

    m_Row = pTable->fieldNum(pField);

    pTable->setIgnoreUndoStack(false);
}

FmtField *FmtUndoTableAddField::getField() const
{
    return pField;
}
