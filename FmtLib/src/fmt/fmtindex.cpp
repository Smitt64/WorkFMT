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
            << "Исключить\nиз индекса" << "LOCAL" << "Флаг" << "Комментарий" ;

    if (headers.size() <= section)
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
/*!
    \class FmtIndex
    \brief FmtIndex класс, обеспечивающий работу с индексом таблицы FMT

    \ingroup tools
*/
FmtIndex::FmtIndex(FmtTable *table, QObject *parent):
    FmtIndecesModelItem(Q_NULLPTR, parent)
{
    m_fDup = false;
    m_NullValue = 0;
    m_fIgnoreStack = false;

    pTable = table;
    pLastCommand = Q_NULLPTR;
    pUndoStack = pTable->undoStack();
}

FmtIndex::~FmtIndex()
{

}

void FmtIndex::setName(const QString &n)
{
    m_Name = n;
}

void FmtIndex::setType(const qint16 &n)
{
    m_Type = n;
}

QVariant FmtIndex::data(int column, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
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
                return NullString(m_NullValue);
        }

        if (column == FmtIndecesModelItem::fld_AutoInc)
        {
            if (role == Qt::EditRole)
                return isAutoInc();
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
    }
    else if (role == Qt::DecorationRole)
    {
        if (column == FmtIndecesModelItem::fld_Name)
        {
            if (isPrimary())
                return QPixmap(":/img/PrimaryKeyHS.png");
            else
                return QVariant();
        }
    }

    return QVariant();
}

void FmtIndex::PushUndoSegmentProperty(FmtSegment *segment, const qint32 &oldFlags, QUndoCommand *parentCommand)
{
    FmtUndoIndexSegmentProperty *segCmd = new FmtUndoIndexSegmentProperty(pTable, parentCommand);
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

            qint32 oldFlags = segment->m_Flags;
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

            qint32 oldFlags = segment->m_Flags;
            if (!m_NullValue)
                segment->m_Flags &= ~fmtkf_NullVal;
            else
            {
                if (m_NullValue == keynullval_All)
                    segment->m_Flags |= fmtkf_NullVal;
                else
                {
                    segment->m_Flags &= ~fmtkf_NullVal;
                    segment->m_Flags |= fmtkf_Manual;
                }
            }

            PushUndoSegmentProperty(segment, oldFlags);
        }

        NormalizeFlags();

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

            qint32 oldFlags = segment->m_Flags;
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
        setNullValue(value.value<quint16>());
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

void FmtIndex::setFlags(const qint32 &flags)
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
        FmtSegment *segment = dynamic_cast<FmtSegment*>(childItems[i]);

        if (segment->field() == pFld)
            rows.append(i);
    }

    foreach (const int &row, rows) {
        removeSegment(static_cast<quint16>(row));
    }
}

void FmtIndex::insertItem(int after)
{
    addSegment(static_cast<quint16>(after));
}

FmtSegment *FmtIndex::addSegment(const quint16 &row)
{
    FmtUndoIndexAddSegment *cmd = new FmtUndoIndexAddSegment(pTable);
    cmd->setSegmentRow(row, this);
    pUndoStack->push(cmd);

    return cmd->segment();
}

FmtSegment *FmtIndex::addSegmentPrivate(const qint16 &row)
{
    QModelIndex modelIndex = pTable->pIndecesModel->indexForItem(this);
    int parentCount = pTable->pIndecesModel->rowCount(modelIndex);

    if (parentCount)
        pTable->pIndecesModel->beginInsertRows(modelIndex, row, row);
    else
        pTable->pIndecesModel->beginInsertRows(modelIndex, 0, 0);
    FmtSegment *segment = new FmtSegment(this);
    segment->m_fSetIgnoreUndoStack = true;
    segment->setField(pTable->m_pFields.first());
    insertChild(row, segment);
    segment->m_fSetIgnoreUndoStack = false;
    pTable->pIndecesModel->endInsertRows();

    return segment;
}

qint16 FmtIndex::indexNumber() const
{
    return static_cast<qint16>(pTable->m_pIndeces.indexOf(const_cast<FmtIndex*>(this)));
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
    return dynamic_cast<FmtSegment*>(child(index));
}

void FmtIndex::NormalizeFlags()
{
    bool use = (m_Flags & fmtkf_Local) == fmtkf_Local;

    bool first = true;
    foreach (FmtIndecesModelItem *item, childItems) {
        FmtSegment *segment = static_cast<FmtSegment*>(item);
        qint32 oldFlags = segment->m_Flags;

        if (!use)
            segment->m_Flags &= ~fmtkf_Local;
        else
            segment->m_Flags |= fmtkf_Local;

        if (m_NullValue != keynullval_All)
            segment->m_Flags &= ~fmtkf_NullVal;
        else
        {
            if (m_NullValue == keynullval_All)
            {
                segment->m_Flags |= fmtkf_NullVal;
                segment->m_Flags &= ~fmtkf_Manual;
                segment->setNotNull(false);
            }
        }

        if ((m_Flags & fmtkf_Duplicates) == fmtkf_Duplicates)
            segment->m_Flags |= fmtkf_Duplicates;
        else
            segment->m_Flags &= ~fmtkf_Duplicates;
        segment->m_Flags &= ~fmtkf_Segment;

        if (!first)
            segment->m_Flags |= fmtkf_Segment;
        else
            first = false;

        PushUndoSegmentProperty(segment, oldFlags);
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

bool FmtIndex::isPrimary() const
{
    return pTable->pkIDx() == pTable->tableIndexNum(this);
}

void FmtIndex::UpdateIndexName(const QString &value)
{
    Q_UNUSED(value);
    m_Name = FmtTableMakeIndexName(pTable, indexNumber());
}

void FmtIndex::removeSegment(const quint16 &segmentIndex)
{
    FmtUndoRemoveSegment *cmd = new FmtUndoRemoveSegment(pTable, pLastCommand);
    cmd->setSegmentRow(segmentIndex, this);

    pUndoStack->push(cmd);
}

void FmtIndex::removeSegmentPrivate(const qint16 &index, bool AutoDelete)
{
    if (index > childCount())
        return;

    QModelIndex itemIndex = pTable->pIndecesModel->indexForItem(this);
    pTable->pIndecesModel->beginRemoveRows(itemIndex, index, index);
    FmtSegment *item = dynamic_cast<FmtSegment*>(childItems.takeAt(index));
    pTable->pIndecesModel->endRemoveRows();

    if (AutoDelete)
        delete item;
}

bool FmtIndex::hasField(FmtField *pFld)
{
    bool hr = false;
    for (int i = 0; i < childItems.size(); i++)
    {
        FmtSegment *segment = dynamic_cast<FmtSegment*>(childItems[i]);

        if (segment->field() == pFld)
            hr = true;
    }
    return hr;
}

void FmtIndex::storeData(QByteArray *data)
{
    QMap<quint16,QVariant> storedData;
    for (quint16 i = 0; i < FmtIndecesModelItem::fld_MAXCOUNT; i++)
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

void FmtIndex::setDataPrivate(const qint16 &fld, const QVariant &value)
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
        setNullValue(value.value<quint16>());
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

        FmtSegment *segment = dynamic_cast<FmtSegment*>(childItems[i]);

        int prm = 0;
        QSqlQuery qfld(pTable->db);
        qfld.prepare("insert into FMT_KEYS values(?,?,?,?,?,?,?,?,?)");

        qfld.bindValue(prm++, pTable->id());
        qfld.bindValue(prm++, indexNumber());
        qfld.bindValue(prm++, i);
        qfld.bindValue(prm++, segment->field()->id());
        qfld.bindValue(prm++, segment->m_Flags);
        qfld.bindValue(prm++, isAutoInc() ? AutoIncType : fmtIndexFromFmtType(segment->field()->type()));
        qfld.bindValue(prm++, segment->m_ExcludeNull);
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
        qint16 index = static_cast<qint16>(childItems.size() - 1);
        FmtSegment *segment = dynamic_cast<FmtSegment*>(childItems[i]);
        FmtSegment *seg = other->addSegmentPrivate(index);
        segment->copyTo(seg);
    }
}

FmtKeyNullVal FmtIndex::nullType() const
{
    FmtKeyNullVal value = keynullval_None;
    if ((m_Flags & fmtkf_NullVal) == fmtkf_NullVal)
        value = keynullval_All;
    else if ((m_Flags & fmtkf_NullVal) != fmtkf_NullVal && (m_Flags & fmtkf_Manual))
        value = keynullval_Any;

    return value;
}
