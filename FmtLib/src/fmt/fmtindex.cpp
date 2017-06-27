#include "fmtindex.h"
#include "fmtcore.h"
#include "fmttable.h"
#include "fmtindex.h"
#include "fmtindecesmodel.h"
#include "fmtundoremovesegment.h"
#include "fmtundoindexaddsegment.h"
#include "fmtundoindexsegmentproperty.h"
#include "fmtsegment.h"
#include "fmtundoindexproperty.h"
#include <QDataStream>

QString FmtIndexPropertyTextByFieldId(const quint16 &section)
{
    static QStringList headers = QStringList()
            << "Имя" << "Тип" << "Dup" << "NULL" << "Исключить\nиз NUL" << "Auto\nInc" << "Desc\norder"
            << "Исключить\nиз индекса" << "LOCAL" << "Комментарий";

    if (headers.size() <= section || section < 0)
        return QString();

    return headers.at(section);
}

QString FmtUndoMacroTitle(const QString &name, const quint16 &prop)
{
    return QObject::tr("Изменено свойство '%1' индекса %2")
            .arg(FmtIndexPropertyTextByFieldId(prop).simplified())
            .arg(name);
}

// ================================================================

FmtIndex::FmtIndex(FmtTable *table, QObject *parent):
    FmtIndecesModelItem(0, parent)
{
    m_fDup = false;
    m_NullValue = 0;
    m_fIgnoreStack = false;

    pTable = table;
    pLastCommand = NULL;
    pUndoStack = pTable->undoStack();
}

FmtIndex::~FmtIndex()
{

}

void FmtIndex::addField(FmtField *fld)
{
    /*FmtSegment segment;
    segment.pFld = fld;
    m_Fields.append(segment);*/
}

void FmtIndex::setName(const QString &n)
{
    m_Name = n;
}

void FmtIndex::setType(const quint16 &n)
{
    m_Type = n;
}

QVariant FmtIndex::data(int column, int role) const
{
    if (column == FmtIndecesModelItem::fld_Name)
        return m_Name;

    if (column == FmtIndecesModelItem::fld_Dup)
    {
        if (role == Qt::DisplayRole)
            return isDup() ? CheckSymbol() : "";
        else if (role == Qt::EditRole)
            return isDup();
    }

    if (column == FmtIndecesModelItem::fld_Null)
    {
        if (role == Qt::EditRole)
            return m_NullValue;
        else
        {
            return NullString(m_NullValue);
        }
    }

    if (column == FmtIndecesModelItem::fld_AutoInc)
    {
        if (role == Qt::EditRole)
        {
            return isAutoInc();
        }
        else
        {
            if (isAutoInc())
                return CheckSymbol();
        }
    }

    if (column == FmtIndecesModelItem::fld_Local)
    {
        if (role == Qt::EditRole)
            return isLocal();
        else
        {
            if (isLocal())
                return CheckSymbol();
        }
    }

    return QVariant();
}

void FmtIndex::PushUndoSegmentProperty(FmtSegment *segment, const quint32 &oldFlags)
{
    FmtUndoIndexSegmentProperty *segCmd = new FmtUndoIndexSegmentProperty(pTable);
    segCmd->setValueToUndo(oldFlags);
    segCmd->setValueToRedo(segment->m_Flags);
    segCmd->setProperty(indexNumber(), segment->segmentNumber(), FmtIndecesModelItem::fld_MAXCOUNT);

    pUndoStack->push(segCmd);
}

void FmtIndex::setDup(bool use)
{
    if (isDup() == use)
        return;

    if (!m_fIgnoreStack)
    {
        pUndoStack->beginMacro(FmtUndoMacroTitle(name(), FmtIndecesModelItem::fld_Dup));

        FmtUndoIndexProperty *cmd = new FmtUndoIndexProperty(pTable);
        cmd->setValueToUndo(isDup());
        cmd->setValueToRedo(use);
        cmd->setProperty(indexNumber(), FmtIndecesModelItem::fld_Dup);

        pUndoStack->push(cmd);
        foreach (FmtIndecesModelItem *item, childItems) {
            FmtSegment *segment = static_cast<FmtSegment*>(item);

            quint32 oldFlags = segment->m_Flags;
            if (!use)
                segment->m_Flags &= ~fmtkf_Duplicates;
            else
                segment->m_Flags |= fmtkf_Duplicates;

            PushUndoSegmentProperty(segment, oldFlags);
        }
        pUndoStack->endMacro();
    }
    else
    {
        if (!use)
            m_Flags &= ~fmtkf_Duplicates;
        else
            m_Flags |= fmtkf_Duplicates;

        foreach (FmtIndecesModelItem *item, childItems) {
            FmtSegment *segment = static_cast<FmtSegment*>(item);
            if (!use)
                segment->m_Flags &= ~fmtkf_Duplicates;
            else
                segment->m_Flags |= fmtkf_Duplicates;
        }
    }

    EmitIndexPropertyChange(FmtIndecesModelItem::fld_Dup);
}

void FmtIndex::setNullValue(const quint16 &val)
{
    if (m_NullValue == val)
        return;

    if (!m_fIgnoreStack)
    {
        pUndoStack->beginMacro(FmtUndoMacroTitle(name(), FmtIndecesModelItem::fld_Null));

        FmtUndoIndexProperty *cmd = new FmtUndoIndexProperty(pTable);
        cmd->setValueToUndo(m_NullValue);
        cmd->setValueToRedo(val);
        cmd->setProperty(indexNumber(), FmtIndecesModelItem::fld_Null);

        pUndoStack->push(cmd);
        foreach (FmtIndecesModelItem *item, childItems) {
            FmtSegment *segment = static_cast<FmtSegment*>(item);

            quint32 oldFlags = segment->m_Flags;
            if (!m_NullValue)
                segment->m_Flags &= ~fmtkf_NullVal;
            else
                segment->m_Flags |= fmtkf_NullVal;

            PushUndoSegmentProperty(segment, oldFlags);
        }

        pUndoStack->endMacro();
    }
    else
    {
        m_NullValue = val;

        foreach (FmtIndecesModelItem *item, childItems) {
            FmtSegment *segment = static_cast<FmtSegment*>(item);
            if (!m_NullValue)
                segment->m_Flags &= ~fmtkf_NullVal;
            else
                segment->m_Flags |= fmtkf_NullVal;
        }
    }

    EmitIndexPropertyChange(FmtIndecesModelItem::fld_Null);
    EmitIndexPropertyChange(FmtIndecesModelItem::fld_NotNull);
}

void FmtIndex::EmitIndexPropertyChange(const quint16 &prop)
{
    emit pTable->pIndecesModel->setPropertyChanged(this, prop);

    foreach (FmtIndecesModelItem *item, childItems) {
        FmtSegment *segment = static_cast<FmtSegment*>(item);
        pTable->pIndecesModel->setPropertyChanged(segment, prop);
    }
}

void FmtIndex::setAutoInc(bool use)
{
    if (isAutoInc() == use)
        return;

    if (!m_fIgnoreStack)
    {
        FmtUndoIndexProperty *cmd = new FmtUndoIndexProperty(pTable);
        cmd->setValueToUndo(isAutoInc());
        cmd->setValueToRedo(use);
        cmd->setProperty(indexNumber(), FmtIndecesModelItem::fld_AutoInc);

        pUndoStack->push(cmd);
    }
    else
    {
        if (use)
            m_Type = AutoIncType;
        else
            m_Type = 0;
    }

    EmitIndexPropertyChange(FmtIndecesModelItem::fld_AutoInc);
}

void FmtIndex::setLocal(bool use)
{
    if (isLocal() == use)
        return;

    if (!m_fIgnoreStack)
    {
        pUndoStack->beginMacro(FmtUndoMacroTitle(name(), FmtIndecesModelItem::fld_Local));
        FmtUndoIndexProperty *cmd = new FmtUndoIndexProperty(pTable);
        cmd->setValueToUndo(isLocal());
        cmd->setValueToRedo(use);
        cmd->setProperty(indexNumber(), FmtIndecesModelItem::fld_Local);

        pUndoStack->push(cmd);

        foreach (FmtIndecesModelItem *item, childItems) {
            FmtSegment *segment = static_cast<FmtSegment*>(item);

            quint32 oldFlags = segment->m_Flags;
            if (!use)
                segment->m_Flags &= ~fmtkf_Local;
            else
                segment->m_Flags |= fmtkf_Local;

            PushUndoSegmentProperty(segment, oldFlags);
        }
        pUndoStack->endMacro();
    }
    else
    {
        if (use)
            m_Flags |= fmtkf_Local;
        else
            m_Flags &= ~fmtkf_Local;

        foreach (FmtIndecesModelItem *item, childItems)
        {
            FmtSegment *segment = static_cast<FmtSegment*>(item);
            if (!use)
                segment->m_Flags &= ~fmtkf_Local;
            else
                segment->m_Flags |= fmtkf_Local;
        }
    }

    EmitIndexPropertyChange(FmtIndecesModelItem::fld_Local);
}

bool FmtIndex::setData(int column, const QVariant &value)
{
    bool changed = false;
    if (column == FmtIndecesModelItem::fld_Dup)
    {
        setDup(value.toBool());
        changed = true;
    }

    if (column == FmtIndecesModelItem::fld_Null)
    {
        setNullValue(value.toInt());
        changed = true;
    }

    if (column == FmtIndecesModelItem::fld_AutoInc)
    {
        setAutoInc(value.toBool());
        changed = true;
    }

    if (column == FmtIndecesModelItem::fld_Local)
    {
        setLocal(value.toBool());
        changed = true;
    }

    //NormalizeFlags();

    if (changed)
        emit indexChanged();

    return false;
}

void FmtIndex::setFlags(const quint32 &flags)
{
    m_Flags = flags;
}

bool FmtIndex::isAutoInc() const
{
    if (m_Type == AutoIncType)
        return true;
    return false;
}

void FmtIndex::removeField(FmtField *pFld)
{
    QList<int> rows;
    for (int i = 0; i < childItems.size(); i++)
    {
        FmtSegment *segment = (FmtSegment*)childItems[i];

        if (segment->field() == pFld)
            rows.append(i);
    }

    foreach (const quint16 &row, rows) {
        removeSegment(row);
    }
}

void FmtIndex::insertItem(int after)
{
    addSegment(after);
}

FmtSegment *FmtIndex::addSegment(const quint32 &row)
{
    FmtUndoIndexAddSegment *cmd = new FmtUndoIndexAddSegment(pTable);
    cmd->setSegmentRow(row, this);
    pUndoStack->push(cmd);

    return cmd->segment();
}

FmtSegment *FmtIndex::addSegmentPrivate(const quint32 &row)
{
    QModelIndex modelIndex = pTable->pIndecesModel->indexForItem(this);

    pTable->pIndecesModel->beginInsertRows(modelIndex, row, row);
    FmtSegment *segment = new FmtSegment(this);
    segment->m_fSetIgnoreUndoStack = true;
    segment->setField(pTable->m_pFields.first());
    insertChild(row, segment);
    segment->m_fSetIgnoreUndoStack = false;
    pTable->pIndecesModel->endInsertRows();

    return segment;
}

qint32 FmtIndex::indexNumber() const
{
    return pTable->m_pIndeces.indexOf((FmtIndex *const)this);
}

qint32 FmtIndex::segmentsCount() const
{
    return childCount();
}

FmtTable *FmtIndex::table()
{
    return pTable;
}

FmtSegment *FmtIndex::segment(const qint32 &index)
{
    return (FmtSegment*)child(index);
}

void FmtIndex::NormalizeFlags()
{
    bool use = (m_Flags & fmtkf_Local) == fmtkf_Local;

    bool first = true;
    foreach (FmtIndecesModelItem *item, childItems) {
        FmtSegment *segment = static_cast<FmtSegment*>(item);
        if (!use)
            segment->m_Flags &= ~fmtkf_Local;
        else
            segment->m_Flags |= fmtkf_Local;

        if (!m_NullValue)
            segment->m_Flags &= ~fmtkf_NullVal;
        else
            segment->m_Flags |= fmtkf_NullVal;

        if ((m_Flags & fmtkf_Duplicates) == fmtkf_Duplicates)
            segment->m_Flags |= fmtkf_Duplicates;
        else
            segment->m_Flags &= ~fmtkf_Duplicates;
        segment->m_Flags &= ~fmtkf_Segment;

        if (!first)
            segment->m_Flags |= fmtkf_Segment;
        else
            first = true;
    }
}

bool FmtIndex::isUnique() const
{
    return (m_Flags & fmtkf_Duplicates) != fmtkf_Duplicates;
}

bool FmtIndex::isDup() const
{
    return (m_Flags & fmtkf_Duplicates) == fmtkf_Duplicates;
}

bool FmtIndex::isLocal() const
{
    return (m_Flags & fmtkf_Local) == fmtkf_Local;
}

void FmtIndex::UpdateIndexName(const QString &value)
{
    m_Name = FmtTableMakeIndexName(pTable, indexNumber());
}

void FmtIndex::removeSegment(const quint16 &segmentIndex)
{
    FmtUndoRemoveSegment *cmd = new FmtUndoRemoveSegment(pTable, pLastCommand);
    cmd->setSegmentRow(segmentIndex, this);

    pUndoStack->push(cmd);
}

void FmtIndex::removeSegmentPrivate(const quint32 &index, bool AutoDelete)
{
    if (index > childCount() || index < 0)
        return;

    QModelIndex itemIndex = pTable->pIndecesModel->indexForItem(this);
    pTable->pIndecesModel->beginRemoveRows(itemIndex, index, index);
    FmtSegment *item = (FmtSegment*)childItems.takeAt(index);
    //pTable->pIndecesModel->RemoveFmtIndex(item);
    pTable->pIndecesModel->endRemoveRows();

    if (AutoDelete)
        delete item;
}


void FmtIndex::storeData(QByteArray *data)
{
    QMap<quint16,QVariant> storedData;
    for (int i = 0; i < FmtIndecesModelItem::fld_MAXCOUNT; i++)
    {
        switch(i)
        {
        case FmtIndecesModelItem::fld_Name:
            storedData[i] = m_Name;
            break;
        case FmtIndecesModelItem::fld_Dup:
            storedData[i] = isDup();
            break;
        case FmtIndecesModelItem::fld_Null:
            storedData[i] = m_NullValue;
            break;
        case FmtIndecesModelItem::fld_AutoInc:
            storedData[i] = isAutoInc();
            break;
        case FmtIndecesModelItem::fld_Local:
            storedData[i] = isLocal();
            break;
        }
    }

    QDataStream stream(data, QIODevice::WriteOnly);
    stream << storedData;
}

void FmtIndex::setDataPrivate(const quint16 &fld, const QVariant &value)
{
    m_fIgnoreStack = true;
    switch(fld)
    {
    case FmtIndecesModelItem::fld_Name:
        setName(value.toString());
        break;
    case FmtIndecesModelItem::fld_Dup:
        setDup(value.toBool());
        break;
    case FmtIndecesModelItem::fld_Null:
        setNullValue(value.toInt());
        break;
    case FmtIndecesModelItem::fld_AutoInc:
        setAutoInc(value.toBool());
        break;
    case FmtIndecesModelItem::fld_Local:
        setLocal(value.toBool());
        break;
    }
    m_fIgnoreStack = false;
}

void FmtIndex::restoreData(QByteArray *data)
{
    QMap<quint16,QVariant> storedData;
    QDataStream stream(data, QIODevice::ReadOnly);
    stream >> storedData;

    QMapIterator<quint16,QVariant> iterator(storedData);

    m_fIgnoreStack = true;
    while(iterator.hasNext())
    {
        iterator.next();

        switch(iterator.key())
        {
        case FmtIndecesModelItem::fld_Name:
            m_Name = iterator.value().toString();
            break;
        case FmtIndecesModelItem::fld_Dup:
            setDup(iterator.value().toBool());
            break;
        case FmtIndecesModelItem::fld_Null:
            m_NullValue = iterator.value().toInt();
            break;
        case FmtIndecesModelItem::fld_AutoInc:
            setAutoInc(iterator.value().toBool());
            break;
        case FmtIndecesModelItem::fld_Local:
            setLocal(iterator.value().toBool());
            break;
        }
    }
    m_fIgnoreStack = false;
}

int FmtIndex::save()
{
    int stat = 0;

    for (int i = 0; i < childItems.size(); i++)
    {
        if (stat)
            break;

        FmtSegment *segment = (FmtSegment*)childItems[i];

        int prm = 0;
        QSqlQuery qfld(pTable->db);
        qfld.prepare("insert into FMT_KEYS values(?,?,?,?,?,?,?,?,?)");

        qfld.bindValue(prm++, pTable->id());
        qfld.bindValue(prm++, indexNumber());
        qfld.bindValue(prm++, i);
        qfld.bindValue(prm++, segment->field()->id());
        qfld.bindValue(prm++, segment->m_Flags);
        qfld.bindValue(prm++, isAutoInc() ? AutoIncType : fmtIndexFromFmtType(segment->field()->type()));
        qfld.bindValue(prm++, m_NullValue);
        qfld.bindValue(prm++, segment->m_IsReal ? 1 : 0);
        qfld.bindValue(prm++, segment->m_Comment);

        stat = ExecuteQuery(&qfld);
    }

    return stat;
}

void FmtIndex::copyTo(FmtIndex *other)
{
    other->m_Flags = m_Flags;
    other->m_Type = m_Type;
    other->m_fDup = m_fDup;
    other->m_fAutoInc = m_fAutoInc;
    other->m_fIgnoreStack = m_fIgnoreStack;
    other->m_NullValue = m_NullValue;
    other->m_Name = m_Name;

    for (int i = 0; i < childItems.size(); i++)
    {
        FmtSegment *segment = (FmtSegment*)childItems[i];
        FmtSegment *seg = other->addSegmentPrivate(childItems.size() - 1);
        segment->copyTo(seg);
    }
}
