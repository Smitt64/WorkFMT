#include "fmtindex.h"
#include "fmtcore.h"
#include "fmttable.h"
#include "fmtindex.h"
#include "fmtindecesmodel.h"
#include "fmtundoremovesegment.h"
#include "fmtundoindexaddsegment.h"
#include "fmtundoindexsegmentproperty.h"
#include "fmtsegment.h"
#include <QDataStream>
/*!
    \class FmtSegment
    \brief FmtSegment класс, обеспечивающий работу с сегментом индекса таблицы FMT

    \ingroup tools
*/
FmtSegment::FmtSegment(FmtIndecesModelItem *parent)
    :FmtIndecesModelItem(parent)
{
    m_Flags = 0;
    m_ExcludeNull = 0;
    pFld = nullptr;
    m_IsReal = false;
    m_fSetIgnoreUndoStack = false;
}

void FmtSegment::insertItem(int after)
{
    FmtIndex *index = dynamic_cast<FmtIndex*>(pParentItem);
    FmtSegment *segment = new FmtSegment(pParentItem);
    setField(index->pTable->m_pFields[0]);
    pParentItem->insertChild(after + 1, segment);
}

bool FmtSegment::isDup() const
{
    return (m_Flags & fmtkf_Duplicates) == fmtkf_Duplicates;
}

bool FmtSegment::isNotNull() const
{
    return m_ExcludeNull == EXCLUDENULL_TRUE ? true : false;
}

QVariant FmtSegment::data(int column, int role) const
{
    if (!pFld)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (column == FmtIndecesModelItem::fld_Name)
        {
            if (role == Qt::DisplayRole)
                return pFld->undecorateName();
            else if (role == Qt::EditRole)
                return pFld->index();
        }
        if (column == FmtIndecesModelItem::fld_Comment)
            return m_Comment;
        if (column == FmtIndecesModelItem::fld_Type)
            return fmtTypeForId(pFld->type());
        if(column == FmtIndecesModelItem::fld_Dup)
            return (isDup()) ? CheckSymbol() : "";
        if(column == FmtIndecesModelItem::fld_Desc)
        {
            if (role == Qt::DisplayRole)
                return descOrder() ? CheckSymbol() : "";
            else if (role == Qt::EditRole)
                return descOrder() ? true : false;
        }
        if (column == FmtIndecesModelItem::fld_NotNull)
        {
            // исключить из null
            FmtIndex *pIndex = dynamic_cast<FmtIndex*>(pParentItem);

            if (pIndex->nullValue() == keynullval_Any)
            {
                if (!isNotNull())
                {
                    if (role == Qt::DisplayRole)
                        return "";
                    else if (role == Qt::EditRole)
                        return false;
                }
                else
                {
                    if (role == Qt::DisplayRole)
                        return CheckSymbol();
                    else if (role == Qt::EditRole)
                        return true;
                }
            }
            else {
                if (role == Qt::DisplayRole)
                    return "";
            }
        }
        if(column == FmtIndecesModelItem::fld_Null)
        {
            FmtIndex *pIndex = dynamic_cast<FmtIndex*>(pParentItem);
            if (role == Qt::DisplayRole)
                return NullString(pIndex->nullValue());
            else if (role == Qt::EditRole)
                return pIndex->nullValue();
        }
        if (column == FmtIndecesModelItem::fld_AutoInc)
            return pParentItem->data(FmtIndecesModelItem::fld_AutoInc, Qt::DisplayRole);
        if (column == FmtIndecesModelItem::fld_Local)
            return pParentItem->data(FmtIndecesModelItem::fld_Local, Qt::DisplayRole);
        if (column == FmtIndecesModelItem::fld_ExcIndx)
        {
            if (role == Qt::DisplayRole)
                return m_IsReal ? CheckSymbol() : "";
            else if (role == Qt::EditRole)
                return m_IsReal;
        }
    }

    return QVariant();
}

bool FmtSegment::isAutoInc() const
{
    return pParentItem->data(FmtIndecesModelItem::fld_AutoInc, Qt::EditRole).toBool();
}

bool FmtSegment::isLocal() const
{
    return pParentItem->data(FmtIndecesModelItem::fld_Local, Qt::EditRole).toBool();
}

FmtField *FmtSegment::field()
{
    return pFld;
}

quint32 FmtSegment::segmentFlags() const
{
    return static_cast<quint32>(m_Flags);
}

qint16 FmtSegment::type() const
{
    return fmtIndexFromFmtType(static_cast<qint16>(pFld->type()));
}

qint32 FmtSegment::flags() const
{
    return m_Flags;
}

bool FmtSegment::isReal() const
{
    return m_IsReal;
}

QString FmtSegment::comment() const
{
    return m_Comment;
}

void FmtSegment::setFlags(const qint32 &value)
{
    if (flags() == value)
        return;

    FmtTable *pTable = table();
    if (!m_fSetIgnoreUndoStack)
    {
        qint32 oldFlags = flags();
        FmtUndoIndexSegmentProperty *segCmd = new FmtUndoIndexSegmentProperty(pTable);
        segCmd->setValueToUndo(oldFlags);
        segCmd->setValueToRedo(value);
        segCmd->setProperty(index()->indexNumber(), segmentNumber(), FmtIndecesModelItem::fld_Flag);

        pTable->undoStack()->push(segCmd);
    }
    else
    {
        m_Flags = value;
    }
}

void FmtSegment::setNotNull(bool use)
{
    if (isNotNull() == use)
        return;

    if (!m_fSetIgnoreUndoStack)
    {
        FmtUndoIndexSegmentProperty *cmd = new FmtUndoIndexSegmentProperty(table(), Q_NULLPTR);
        cmd->setValueToUndo(isNotNull());
        cmd->setValueToRedo(use);
        cmd->setProperty(index()->indexNumber(), segmentNumber(), FmtIndecesModelItem::fld_NotNull);

        table()->pUndoStack->push(cmd);
    }
    else
    {
        if (use)
            m_ExcludeNull = EXCLUDENULL_TRUE;
        else
            m_ExcludeNull = 0;
    }

    FmtTable *pTable = table();
    emit pTable->pIndecesModel->setPropertyChanged(this, FmtIndecesModelItem::fld_NotNull);
}

qint16 FmtSegment::segmentNumber()
{
    return static_cast<qint16>(parent()->indexOfChild(this));
}

void FmtSegment::setComment(const QString &val)
{
    if (m_Comment == val)
        return;

    if (!m_fSetIgnoreUndoStack)
    {
        FmtUndoIndexSegmentProperty *cmd = new FmtUndoIndexSegmentProperty(table(), Q_NULLPTR);
        cmd->setValueToUndo(m_Comment);
        cmd->setValueToRedo(val);
        cmd->setProperty(index()->indexNumber(), segmentNumber(), FmtIndecesModelItem::fld_Comment);

        table()->pUndoStack->push(cmd);
    }
    else
    {
        m_Comment = val;
    }

    FmtTable *pTable = table();
    emit pTable->pIndecesModel->setPropertyChanged(this, FmtIndecesModelItem::fld_Comment);
}

void FmtSegment::setDescOrder(bool use)
{
    if (descOrder() == use)
        return;

    if (!m_fSetIgnoreUndoStack)
    {
        FmtUndoIndexSegmentProperty *cmd = new FmtUndoIndexSegmentProperty(table(), Q_NULLPTR);
        cmd->setValueToUndo(descOrder());
        cmd->setValueToRedo(use);
        cmd->setProperty(index()->indexNumber(), segmentNumber(), FmtIndecesModelItem::fld_Desc);

        table()->pUndoStack->push(cmd);
    }
    else
    {
        if (use)
            m_Flags |= fmtkf_Descending;
        else
            m_Flags &= ~fmtkf_Descending;
    }

    FmtTable *pTable = table();
    emit pTable->pIndecesModel->setPropertyChanged(this, FmtIndecesModelItem::fld_Desc);
}

void FmtSegment::setIsReal(bool use)
{
    if (m_IsReal == use)
        return;

    if (!m_fSetIgnoreUndoStack)
    {
        FmtUndoIndexSegmentProperty *cmd = new FmtUndoIndexSegmentProperty(table(), Q_NULLPTR);
        cmd->setValueToUndo(m_IsReal);
        cmd->setValueToRedo(use);
        cmd->setProperty(index()->indexNumber(), segmentNumber(), FmtIndecesModelItem::fld_ExcIndx);

        table()->pUndoStack->push(cmd);
    }
    else
    {
        m_IsReal = use;
    }

    FmtTable *pTable = table();
    emit pTable->pIndecesModel->setPropertyChanged(this, FmtIndecesModelItem::fld_ExcIndx);
}

bool FmtSegment::descOrder() const
{
    return (m_Flags & fmtkf_Descending) == fmtkf_Descending;
}

qint16 FmtSegment::fieldIndex()
{
    return static_cast<qint16>(table()->m_pFields.indexOf(pFld));
}

void FmtSegment::setField(FmtField *fld)
{
    FmtIndex *parentIndex = dynamic_cast<FmtIndex*>(parent());
    FmtField *const cpFld = const_cast<FmtField*>(fld);
    qint16 iFld = static_cast<qint16>(parentIndex->pTable->m_pFields.indexOf(cpFld));
    setField(iFld);
}

void FmtSegment::setField(const qint16 &fld)
{
    if (fieldIndex() == fld)
        return;

    if (!m_fSetIgnoreUndoStack)
    {
        FmtUndoIndexSegmentProperty *cmd = new FmtUndoIndexSegmentProperty(table(), Q_NULLPTR);
        cmd->setValueToUndo(fieldIndex());
        cmd->setValueToRedo(fld);
        cmd->setProperty(index()->indexNumber(), segmentNumber(), FmtIndecesModelItem::fld_Name);

        table()->pUndoStack->push(cmd);
    }
    else
    {
        pFld = table()->m_pFields[fld];
    }

    FmtTable *pTable = table();
    emit pTable->pIndecesModel->setPropertyChanged(this, FmtIndecesModelItem::fld_Name);
}

bool FmtSegment::setData(int column, const QVariant &value)
{
    bool changed = false;
    if(column == FmtIndecesModelItem::fld_NotNull)
    {
        setNotNull(value.toBool());
        changed = true;
    }
    if (column == FmtIndecesModelItem::fld_Comment)
    {
        setComment(value.toString());
        changed = true;
    }
    if (column == FmtIndecesModelItem::fld_Desc)
    {
        setDescOrder(value.toBool());
        changed = true;
    }
    if (column == FmtIndecesModelItem::fld_ExcIndx)
    {
        setIsReal(value.toBool());
        changed = true;
    }
    if (column == FmtIndecesModelItem::fld_Name)
    {
        setField(value.value<qint16>());
        changed = true;
    }

    return false;
}

void FmtSegment::setDataPrivate(const QVariant &value, const quint16 &column)
{
    m_fSetIgnoreUndoStack = true;
    if(column == FmtIndecesModelItem::fld_NotNull)
        setNotNull(value.toBool());
    if (column == FmtIndecesModelItem::fld_Comment)
        setComment(value.toString());
    if (column == FmtIndecesModelItem::fld_Desc)
        setDescOrder(value.toBool());
    if (column == FmtIndecesModelItem::fld_ExcIndx)
        setIsReal(value.toBool());
    if (column == FmtIndecesModelItem::fld_Name)
        setField(value.value<qint16>());
    if (column == FmtIndecesModelItem::fld_Flag)
        setFlags(value.value<qint32>());

    if (column == FmtIndecesModelItem::fld_MAXCOUNT)
        m_Flags = value.toInt();
    m_fSetIgnoreUndoStack = false;
}

void FmtSegment::storeData(QByteArray *data)
{
    QMap<quint16,QVariant> storedData;
    for (quint16 i = 0; i < FmtIndecesModelItem::fld_MAXCOUNT; i++)
    {
        if (i == FmtIndecesModelItem::fld_Comment)
            storedData[i] = comment();

        if (i == FmtIndecesModelItem::fld_Name)
            storedData[i] = fieldIndex();

        if (i == FmtIndecesModelItem::fld_NotNull)
            storedData[i] = isNotNull();

        if (i == FmtIndecesModelItem::fld_Desc)
            storedData[i] = descOrder();

        if (i == FmtIndecesModelItem::fld_ExcIndx)
            storedData[i] = isReal();

        if (i == FmtIndecesModelItem::fld_Flag)
            storedData[i] = flags();
    }

    storedData[FmtIndecesModelItem::fld_MAXCOUNT] = m_Flags;

    QDataStream stream(data, QIODevice::WriteOnly);
    stream << storedData;
}

FmtIndex *FmtSegment::index()
{
    return dynamic_cast<FmtIndex*>(parent());
}

FmtTable *FmtSegment::table()
{
    FmtIndex *pIndex = index();
    return pIndex->table();
}

void FmtSegment::restoreData(QByteArray *data)
{
    QMap<quint16,QVariant> storedData;
    QDataStream stream(data, QIODevice::ReadOnly);
    stream >> storedData;

    QMapIterator<quint16,QVariant> iterator(storedData);
    while(iterator.hasNext())
    {
        iterator.next();
        setDataPrivate(iterator.value(), iterator.key());
    }

    m_Flags = storedData[FmtIndecesModelItem::fld_MAXCOUNT].toInt();
}

void FmtSegment::copyTo(FmtSegment *other)
{
    other->pFld = other->table()->field(fieldIndex());
    other->m_Flags = m_Flags;
    other->m_IsReal = m_IsReal;
    other->m_Comment = m_Comment;
    other->m_ExcludeNull = m_ExcludeNull;
}
