#include "fmtundotableproperty.h"
#include "fmttable.h"
#include <QDebug>

FmtUndoTableProperty::FmtUndoTableProperty(FmtTable *table, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    pTable = table;
}

FmtUndoTableProperty::~FmtUndoTableProperty()
{
    qDebug() << "FmtUndoTableProperty::~FmtUndoTableProperty()";
}

void FmtUndoTableProperty::setValueToUndo(const QVariant &value)
{
    m_UndoValue = value;
}

void FmtUndoTableProperty::setValueToRedo(const QVariant &value)
{
    m_RedoValue = value;
}

void FmtUndoTableProperty::setProperty(const quint16 &property)
{
    m_PropertyDisplayName = QObject::tr("Изменено свойство таблицы '%1': %2")
            .arg(FmtTablePropertyTextByFieldId(property))
            .arg(m_RedoValue.toString());
    setText(m_PropertyDisplayName);
    m_Property = property;
}

void FmtUndoTableProperty::undo()
{
    //qDebug() << FmtTablePropertyByFieldId(m_Property).toLocal8Bit().data() << m_UndoValue;
    pTable->setIgnoreUndoStack(true);
    pTable->setDataPrivate(m_Property, m_UndoValue);
    pTable->setIgnoreUndoStack(false);
}

void FmtUndoTableProperty::redo()
{
    //qDebug() << FmtTablePropertyByFieldId(m_Property).toLocal8Bit().data() << m_RedoValue;
    pTable->setIgnoreUndoStack(true);
    pTable->setDataPrivate(m_Property, m_RedoValue);
    pTable->setIgnoreUndoStack(false);
}

