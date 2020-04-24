#include "fmttable.h"
#include "fmtcore.h"
#include "fmtfildsmodel.h"
#include "fmtindecesmodel.h"
#include "fmtfield.h"
#include "fmtindex.h"
#include "fmterrors.h"
#include "connectioninfo.h"
#include "fmtundotableproperty.h"
#include "fmtundotableaddfield.h"
#include "fmtundotableremovefield.h"
#include "fmtundotableaddindex.h"
#include "fmtundoremoveindex.h"
#include "fmtundotablepastefield.h"
#include "fmtsegment.h"
#include "loggingcategories.h"
#include <QProcess>
#include <QEventLoop>
#include <QtXml>
#include <QRegExpValidator>
#include <QUndoStack>

#define NOT_EXISTS 2

QString FmtTableMakeIndexName(FmtTable *pTable, const FmtFldIndex &indexNum)
{
    QString TableName = pTable->name().toUpper().trimmed();

    if (!TableName.length())
    {
        TableName = "NONAME";
    }

    return QString("%1_IDX%2")
            .arg(TableName)
            .arg(QString::number(indexNum, 16).toUpper());
}

QString FmtTablePropertyByFieldId(const FmtFldIndex &fld)
{
    QString name;
    switch(fld)
    {
    case FmtTable::fld_Id:
        name = "Id";
        break;
    case FmtTable::fld_Name:
        name = "Name";
        break;
    case FmtTable::fld_CacheSize:
        name = "CacheSize";
        break;
    case FmtTable::fld_Owner:
        name = "Owner";
        break;
    case FmtTable::fld_Comment:
        name = "Comment";
        break;
    case FmtTable::fld_PkIndx:
        name = "PkIDx";
        break;
    case FmtTable::fld_BlobType:
        name = "BlobType";
        break;
    case FmtTable::fld_BlobLen:
        name = "BlobLen";
        break;
    case FmtTable::fld_Flags:
        name = "";
        break;
    case FmtTable::fld_isTemporary:
        name = "isTemporary";
        break;
    case FmtTable::fld_isRecord:
        name = "isRecord";
        break;
    }

    return name;
}

QString FmtTablePropertyTextByFieldId(const FmtFldIndex &fld)
{
    QString name;
    switch(fld)
    {
    case FmtTable::fld_Id:
        name = QObject::tr("ID");
        break;
    case FmtTable::fld_Name:
        name = QObject::tr("Имя таблицы");
        break;
    case FmtTable::fld_CacheSize:
        name = QObject::tr("Длина кеша");
        break;
    case FmtTable::fld_Owner:
        name = QObject::tr("Владелец");
        break;
    case FmtTable::fld_Comment:
        name = QObject::tr("Комментарий");
        break;
    case FmtTable::fld_PkIndx:
        name = QObject::tr("Первичный ключ");
        break;
    case FmtTable::fld_BlobType:
        name = QObject::tr("Тип переменной части");
        break;
    case FmtTable::fld_BlobLen:
        name = QObject::tr("Длина переменной части");
        break;
    case FmtTable::fld_Flags:
        name = QObject::tr("Флаги");
        break;
    case FmtTable::fld_isTemporary:
        name = QObject::tr("Временная");
        break;
    case FmtTable::fld_isRecord:
        name = QObject::tr("Структура");
        break;
    }

    return name;
}

FmtTable::FmtTable(QObject *parent) : QAbstractItemModel(parent)
{
    pFieldsModel = Q_NULLPTR;
    pIndecesModel = Q_NULLPTR;
    m_pErrors = Q_NULLPTR;
    pUndoStack = Q_NULLPTR;
    pUndoAction = Q_NULLPTR;
    pRedoAction = Q_NULLPTR;
    init();
    qInfo(logFmt()) << "Construct " << this;
}

FmtTable::FmtTable(ConnectionInfo *connection, QObject *parent)
    : QAbstractItemModel(parent)
{
    pFieldsModel = Q_NULLPTR;
    pIndecesModel = Q_NULLPTR;
    m_pErrors = Q_NULLPTR;
    pUndoAction = Q_NULLPTR;
    pRedoAction = Q_NULLPTR;
    pUndoStack = Q_NULLPTR;
    pConnection = connection;
    db = connection->db();
    init();
    qInfo(logFmt()) << "Construct " << this;
}

FmtTable::~FmtTable()
{
    qInfo(logFmt()) << "Destruct " << this;
    clear();
    delete pIndecesModel;
    delete pFieldsModel;
    delete m_pErrors;
}

void FmtTable::init()
{
    m_Id = 0; m_CacheSize = 0; m_BlobLen = 0; m_Flags = 0;
    m_PkIDx = -1; m_BlobType = 0;
    m_IgnoreUndoStack = false;

    if (!pFieldsModel) pFieldsModel = new FmtFildsModel(const_cast<FmtTable*>(this));
    if (!pIndecesModel) pIndecesModel = new FmtIndecesModel(const_cast<FmtTable*>(this));
    if (!m_pErrors) m_pErrors = new FmtErrors(this);
    if (!pUndoStack)
    {
        pUndoStack = new QUndoStack(this);
        pUndoAction = pUndoStack->createUndoAction(this);
        pRedoAction = pUndoStack->createRedoAction(this);
    }

    //connect(pIndecesModel, SIGNAL(indexAdded()), SIGNAL(indexAdded()));
}

QAction *FmtTable::undoAction()
{
    return pUndoAction;
}

QAction *FmtTable::redoAction()
{
    return pRedoAction;
}

ConnectionInfo *FmtTable::connection()
{
    return pConnection;
}

void FmtTable::setCacheSize(const FmtNumber10 &v)
{
    if (m_CacheSize == v)
        return;

    if (!m_IgnoreUndoStack)
    {
        FmtUndoTableProperty *cmd = new FmtUndoTableProperty(this);
        cmd->setValueToUndo(m_CacheSize);
        cmd->setValueToRedo(v);
        cmd->setProperty(fld_CacheSize);
        pUndoStack->push(cmd);
    }
    else
    {
        m_CacheSize = v;
    }

    emit dataChanged(createIndex(0, 0), createIndex(0, fld_CacheSize));
}

void FmtTable::setPkIDx(const FmtNumber5 &v)
{
    if (m_PkIDx == v)
        return;

    if (!m_IgnoreUndoStack)
    {
        FmtUndoTableProperty *cmd = new FmtUndoTableProperty(this);
        cmd->setValueToUndo(m_PkIDx);
        cmd->setValueToRedo(v);
        cmd->setProperty(fld_PkIndx);
        pUndoStack->push(cmd);
    }
    else
    {
        m_PkIDx = v;
    }

    emit dataChanged(createIndex(0, 0), createIndex(0, fld_PkIndx));
}

void FmtTable::setBlobType(const FmtNumber5 &v)
{
    if (m_BlobType == v)
        return;

    if (!m_IgnoreUndoStack)
    {
        FmtUndoTableProperty *cmd = new FmtUndoTableProperty(this);
        cmd->setValueToUndo(m_BlobType);
        cmd->setValueToRedo(v);
        cmd->setProperty(fld_BlobType);
        pUndoStack->push(cmd);
    }
    else
    {
        m_BlobType = v;
    }

    emit dataChanged(createIndex(0, 0), createIndex(0, fld_BlobType));
}

void FmtTable::setTableFlags(const FmtNumber10 &v)
{
    m_Flags = v;
    emit dataChanged(createIndex(0, 0), createIndex(0, fld_Flags));
}

void FmtTable::setBlobLen(const FmtNumber10 &v)
{
    if (m_BlobLen == v)
        return;

    if (!m_IgnoreUndoStack)
    {
        pUndoStack->beginMacro(tr("Изменено свойство таблицы '%1'")
                               .arg(FmtTablePropertyTextByFieldId(fld_BlobLen)));
        FmtUndoTableProperty *cmd = new FmtUndoTableProperty(this);
        cmd->setValueToUndo(m_BlobLen);
        cmd->setValueToRedo(v);
        cmd->setProperty(fld_BlobLen);
        pUndoStack->push(cmd);

        if (!m_BlobType)
            setBlobType(BT_CLOB);

        pUndoStack->endMacro();
    }
    else
    {
        m_BlobLen = v;
        if (!m_BlobType)
            m_BlobType = BT_CLOB;
    }

    emit dataChanged(createIndex(0, 0), createIndex(0, fld_BlobLen));
}

QString FmtTable::name() const
{
    return m_Name;
}

QString FmtTable::dbtName() const
{
    if (m_Name.isEmpty())
        return QString();

    QString str;

    if (m_Name.at(0) == 'd')
        str =  m_Name.mid(1).toLower();

    int pos = -1;
    if ((pos = str.lastIndexOf(isTemporary() ? "_tmp" : "_dbt")) != -1)
    {
        str = str.remove(pos, 4);
        str += isTemporary() ? ".tmp" : ".dbt";
    }

    return str;
}

void FmtTable::setName(const QString &v)
{
    QString value = v.mid(0, fmtm_TableNameMaxSize).toLower().trimmed();

    if (m_Name == value)
        return;

    if (!m_IgnoreUndoStack)
    {
        FmtUndoTableProperty *cmd = new FmtUndoTableProperty(this);
        cmd->setValueToUndo(m_Name);
        cmd->setValueToRedo(value);
        cmd->setProperty(fld_Name);
        pUndoStack->push(cmd);
    }
    else
    {
        m_Name = value;
    }

    emit dataChanged(createIndex(0, 0), createIndex(0, fld_Name));
}

QString FmtTable::comment() const
{
    return m_Comment;
}

QString FmtTable::owner() const
{
    return m_Owner;
}

void FmtTable::setComment(const QString &v)
{
    QString value = v.mid(0, fmtm_TableCommentMaxSize);
    if (m_Comment == value)
        return;

    if (!m_IgnoreUndoStack)
    {
        FmtUndoTableProperty *cmd = new FmtUndoTableProperty(this);
        cmd->setValueToUndo(m_Comment);
        cmd->setValueToRedo(value);
        cmd->setProperty(fld_Comment);
        pUndoStack->push(cmd);
    }
    else
    {
        m_Comment = value;
    }

    emit dataChanged(createIndex(0, 0), createIndex(0, fld_Comment));
}

void FmtTable::setOwner(const QString &v)
{
    QString value = v.mid(0, fmtm_TableOwnerMaxSize);
    if (m_Owner == value)
        return;

    if (!m_IgnoreUndoStack)
    {
        FmtUndoTableProperty *cmd = new FmtUndoTableProperty(this);
        cmd->setValueToUndo(m_Owner);
        cmd->setValueToRedo(value);
        cmd->setProperty(fld_Owner);
        pUndoStack->push(cmd);
    }
    else
    {
        m_Owner = value;
    }

    emit dataChanged(createIndex(0, 0), createIndex(0, fld_Owner));
}

bool FmtTable::isTemporary() const
{
    return hasTemporaryFlag(m_Flags);
}

bool FmtTable::isRecord() const
{
    return hasRecordFlag(m_Flags);
}

void FmtTable::setIsTemporary(const bool &v)
{
    if (isTemporary() == v)
        return;

    FmtNumber10 value = m_Flags;
    if (v)
        value = m_Flags | fmtnf_Temp;
    else
        value &= ~fmtnf_Temp;

    if (!m_IgnoreUndoStack)
    {
        FmtUndoTableProperty *cmd = new FmtUndoTableProperty(this);
        cmd->setValueToUndo(isTemporary());
        cmd->setValueToRedo(value);
        cmd->setProperty(fld_isTemporary);
        pUndoStack->push(cmd);
    }
    else
        m_Flags = value;

    emit isTemporaryChanged(v);
    emit dataChanged(createIndex(0, 0), createIndex(0, fld_isTemporary));
}

void FmtTable::setIsRecord(const bool &v)
{
    if (isRecord() == v)
        return;

    FmtNumber10 value = m_Flags;
    if (v)
        value = m_Flags | fmtnf_Rec;
    else
        value &= ~fmtnf_Rec;

    if (!m_IgnoreUndoStack)
    {
        FmtUndoTableProperty *cmd = new FmtUndoTableProperty(this);
        cmd->setValueToUndo(isRecord());
        cmd->setValueToRedo(value);
        cmd->setProperty(fld_isRecord);
        pUndoStack->push(cmd);
    }
    else
        m_Flags = value;

    emit dataChanged(createIndex(0, 0), createIndex(0, fld_isRecord));
}

bool FmtTable::load(const QString &table)
{
    bool hr = false;

    FmtRecId id = 0;
    QSqlQuery q(db);
    q.prepare("select T_ID from FMT_NAMES where T_NAME = ?");
    q.bindValue(0, table);

    if (!ExecuteQuery(&q) && q.next())
        id = static_cast<FmtRecId>(q.value(0).toInt());

    if (id)
        hr = load(id);

    return hr;
}

bool FmtTable::load(const FmtRecId &id)
{
    bool hr = true;
    QSqlQuery q(db);
    q.prepare("select * from FMT_NAMES WHERE T_ID = ?");
    q.bindValue(0, id);

    qInfo(logFmt()) << "Loading table" << this << "by id:" << id;
    if (!ExecuteQuery(&q) && q.next())
    {
        m_Id = static_cast<FmtRecId>(q.value(fnc_Id).toInt());
        m_Name = q.value(fnc_Name).toString();
        m_Comment = q.value(fnc_Comment).toString();
        m_Flags = static_cast<FmtNumber10>(q.value(fnc_Flags).toInt());
        m_BlobLen = static_cast<FmtNumber10>(q.value(fcn_BlobLen).toInt());
        m_PkIDx = static_cast<FmtNumber5>(q.value(fnc_PkIndx).toInt());
        m_BlobType = static_cast<FmtNumber5>(q.value(fnc_BlobType).toInt());
        m_Record = q.record();

        QSqlQuery qf(db);
        qf.prepare("select * from FMT_FIELDS WHERE T_FMTID = ? ORDER BY T_OFFSET");
        qf.bindValue(0, m_Id);

        qInfo(logFmt()) << "Loading table fields:" << this;
        if (!ExecuteQuery(&qf))
        {
            while(qf.next())
            {
                FmtField *fld = new FmtField(this);
                fld->load(qf.record());
                m_pFields.append(fld);
            }
        }
        qInfo(logFmt()) << "Loading table indices: " << this;
        FillIndex();
    }
    else
    {
        qInfo(logFmt()) << "Failed load table" << this;
    }

    return hr;
}

void FmtTable::copyTo(FmtSharedTablePtr cTable)
{
    qInfo(logFmt()) << "Start copy table from" << this << "(" << m_Name << ") to" << cTable.data();
    cTable->clear();

    cTable->m_Id = 0;
    cTable->m_Name = m_Name;
    cTable->m_Comment = m_Comment;
    cTable->m_Flags = m_Flags;
    cTable->m_BlobLen = m_BlobLen;
    cTable->m_PkIDx = m_PkIDx;
    cTable->m_BlobType = m_BlobType;

    for (FmtField *pFld: m_pFields) {
        FmtField *fld = cTable->addFieldPrivate(pFld->undecorateName(), pFld->type());
        fld->m_Id = 0;
        fld->m_Size = pFld->m_Size;
        fld->m_Type = pFld->m_Type;
        fld->m_Offset = pFld->m_Offset;
        fld->m_Outlen = pFld->m_Outlen;
        fld->m_Decpoint = pFld->m_Decpoint;
        fld->m_Name = pFld->m_Name;
        fld->m_Comment = pFld->m_Comment;
        fld->m_isHidden = pFld->m_isHidden;
    }

    for (FmtIndex *pIndex: m_pIndeces) {
        FmtIndex *nIndex = cTable->addIndexPrivate();
        pIndex->copyTo(nIndex);
    }
}

void FmtTable::copyToAsTmp(QSharedPointer<FmtTable> cTable)
{
    copyTo(cTable);
    cTable->setIsTemporary(true);

    QString name = m_Name;
    int pos = name.lastIndexOf('_');
    name = name.remove(pos, name.length() - pos);
    name += QString("_tmp");

    cTable->setName(name);
}

FmtField *FmtTable::FindField(const FmtRecId &Id)
{
    foreach (FmtField *fld, m_pFields) {
        if (fld->id() == Id)
            return fld;
    }

    return Q_NULLPTR;
}

void FmtTable::rebuildOffsets(QUndoCommand *pLastCommand)
{
    int value = 0;

    bool hasParentCommand = (pLastCommand && pLastCommand != Q_NULLPTR) ? true : false;
    if (!m_IgnoreUndoStack)
    {
        if (!hasParentCommand)
            pUndoStack->beginMacro(tr("Пересчет смещений"));

        for (int i = 0; i < m_pFields.size(); i++)
        {
            if (m_IgnoreUndoStack)
                m_pFields[i]->m_IgnoreUndoStack = true;

            m_pFields[i]->setOffset(value);
            value += m_pFields[i]->size();

            if (m_IgnoreUndoStack)
                m_pFields[i]->m_IgnoreUndoStack = false;
        }

        if (!hasParentCommand)
            pUndoStack->endMacro();
    }
    else
    {
        for (int i = 0; i < m_pFields.size(); i++)
        {
            if (m_IgnoreUndoStack)
                m_pFields[i]->m_IgnoreUndoStack = true;

            m_pFields[i]->setOffset(value);
            value += m_pFields[i]->size();

            if (m_IgnoreUndoStack)
                m_pFields[i]->m_IgnoreUndoStack = false;
        }
    }
}

void FmtTable::FillIndex()
{
    QSqlQuery q(db);
    q.prepare("select * from fmt_keys where t_fmtid = ? ORDER BY t_keynum, t_segnum");
    q.bindValue(0, m_Id);

    int lastkeynum = -1;
    int i = 0;
    FmtIndex *ptr = Q_NULLPTR;
    if (!ExecuteQuery(&q))
    {
        while(q.next())
        {
            if (lastkeynum != q.value(fkc_KeyNum).toInt())
            {
                FmtIndex *indx = new FmtIndex(this);
                indx->pParentItem = pIndecesModel->rootItem;
                indx->setFlags(q.value(fkc_Flags).toInt());

                indx->setFlag(0);
                indx->setName(QString("%1_IDX%2")
                              .arg(m_Name.toUpper())
                              .arg(QString::number(i++, 16).toUpper()));
                indx->setType(static_cast<FmtNumber5>(q.value(fkc_Type).toInt()));
                ptr = indx;
                lastkeynum = q.value(fkc_KeyNum).toInt();
                m_pIndeces.append(ptr);
                pIndecesModel->rootItem->appendChild(indx);

                connect(indx, SIGNAL(indexChanged()), SLOT(onIndexChanged()));
            }
            FmtField *fld = FindField(static_cast<FmtRecId>(q.value(fkc_FmtFld).toInt()));

            if (fld)
            {
                FmtSegment *segment = new FmtSegment(ptr);
                segment->pFld = fld;
                segment->m_Comment = q.value(fkc_Comment).toString();
                segment->m_Flags = static_cast<FmtNumber10>(q.value(fkc_Flags).toInt());
                segment->m_IsReal = q.value(fkc_IsReal).toInt() ? true : false;
                segment->m_ExcludeNull = q.value(fkc_NullVal).toInt();

                if (q.value(fkc_SegNum).toInt() == 0)
                {
                    if (segment->m_Flags & fmtkf_NullVal)
                        ptr->m_NullValue = keynullval_All;
                    else if ((segment->m_Flags & fmtkf_NullVal) != fmtkf_NullVal && (segment->m_Flags & fmtkf_Manual))
                        ptr->m_NullValue = keynullval_Any;
                }

                segment->setFlag(1);
                ptr->appendChild(segment);
            }
        }
    }
}

int FmtTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 9;
}

QVariant FmtTable::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    QVariant value;
    switch(index.column())
    {
    case fld_Id:
        value = m_Id;
        break;
    case fld_Name:
        value = m_Name;
        break;
    case fld_Comment:
        value = m_Comment;
        break;
    case fld_BlobLen:
        value = m_BlobLen;
        break;
    case fld_PkIndx:
        value = m_PkIDx;
        break;
    case fld_isTemporary:
        value = isTemporary();
        break;
    case fld_isRecord:
        value = isRecord();
        break;
    }
    return value;
}

bool FmtTable::setIgnoreUndoStack(const bool &v)
{
    bool old = m_IgnoreUndoStack;
    m_IgnoreUndoStack = v;

    return old;
}

bool FmtTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || data(index, role) == value)
        return QAbstractItemModel::setData(index, value, role);

    setIgnoreUndoStack(false);
    setDataPrivate(static_cast<FmtFldType>(index.column()), value);

    return true;
}

bool FmtTable::setDataPrivate(const FmtFldType &fld, const QVariant &value)
{
    switch(fld)
    {
    case fld_Id:
        m_Id = static_cast<FmtRecId>(value.toInt());
        break;
    case fld_Name:
        setName(value.toString());
        emit nameChanged(m_Name);
        break;
    case fld_Comment:
        setComment(value.toString());
        break;
    case fld_BlobLen:
        setBlobLen(value.toInt());
        break;
    case fld_isTemporary:
        setIsTemporary(value.toBool());
        break;
    case fld_isRecord:
        setIsRecord(value.toBool());
        break;
    case fld_PkIndx:
        setPkIDx(value.value<FmtNumber5>());
        break;
    case fld_CacheSize:
        setCacheSize(value.toInt());
        break;
    case fld_Owner:
        setOwner(value.toString());
        break;
    case fld_BlobType:
        setBlobType(value.value<FmtNumber5>());
        break;
    }

    return true;
}

FmtField *FmtTable::addField(const QString &name, const FmtFldType &type)
{
    FmtUndoTableAddField *cmd = new FmtUndoTableAddField(this);
    cmd->setNameAndType(name, type);
    pUndoStack->push(cmd);

    return cmd->getField();
}

FmtField *FmtTable::addField(const QMap<quint16,QVariant> &data)
{
    FmtUndoTablePasteField *cmd = new FmtUndoTablePasteField(this);
    cmd->setData(data);
    pUndoStack->push(cmd);

    return cmd->getField();
}

FmtField *FmtTable::addFieldPrivate(const QString &name, const FmtFldType &type)
{
    pFieldsModel->beginInsertRows(QModelIndex(), m_pFields.size(), m_pFields.size());
    FmtField *fld = new FmtField(this);
    m_pFields.append(fld);

    fld->m_IgnoreUndoStack = true;
    fld->setName("t_" + name);
    fld->setType(type);

    int value = 0;
    int idx = m_pFields.indexOf(const_cast<FmtField*>(fld));
    FmtFldIndex fieldsSize = static_cast<FmtFldIndex>(m_pFields.size());
    FmtFldIndex i = 0;
    while (i < fieldsSize && i < idx)
    {
        value += m_pFields[i]->size();
        ++i;
    }

    fld->setOffset(value);
    fld->m_IgnoreUndoStack = false;
    emit fieldAdded(fld);
    pFieldsModel->endInsertRows();

    return fld;
}

FmtField *FmtTable::addFieldPrivate(const QMap<quint16,QVariant> &data)
{
    pFieldsModel->beginInsertRows(QModelIndex(), m_pFields.size(), m_pFields.size());
    FmtField *fld = new FmtField(this);
    m_pFields.append(fld);

    fld->m_IgnoreUndoStack = true;
    fld->setName(data[FmtField::fld_Name].toString());
    fld->setType(static_cast<FmtFldType>(data[FmtField::fld_Type].toInt()));
    fld->setSize(static_cast<FmtNumber10>(data[FmtField::fld_Size].toInt()));
    fld->setComment(data[FmtField::fld_Comment].toString());
    fld->setOutlen(static_cast<FmtNumber5>(data[FmtField::fld_Outlen].toInt()));
    fld->setDecpoint(static_cast<FmtNumber5>(data[FmtField::fld_DecPoint].toInt()));
    fld->setHidden(data[FmtField::fld_Hidden].toBool());

    int value = 0;
    int idx = m_pFields.indexOf(const_cast<FmtField*>(fld));
    FmtFldIndex fieldsSize = static_cast<FmtFldIndex>(m_pFields.size());
    FmtFldIndex i = 0;
    while (i < fieldsSize && i < idx)
    {
        value += m_pFields[i]->size();
        ++i;
    }

    fld->setOffset(value);
    fld->m_IgnoreUndoStack = false;
    emit fieldAdded(fld);
    pFieldsModel->endInsertRows();

    return fld;
}

void FmtTable::removeField(const FmtFldIndex &row)
{
    if (row >= m_pFields.size())
        return;

    if (!m_IgnoreUndoStack)
    {
        FmtField *fld = field(row);
        pUndoStack->beginMacro(tr("Удалено поле '%1'").arg(fld->name()));
        FmtUndoTableRemoveField *cmd = new FmtUndoTableRemoveField(this, Q_NULLPTR);
        cmd->setFieldRow(row);

        foreach (FmtIndex *index, m_pIndeces) {
            index->removeField(fld);
        }

        pUndoStack->push(cmd);
        rebuildOffsets(cmd);
        pUndoStack->endMacro();
    }
    else
    {
        removeFieldPrivate(row);
        rebuildOffsets();
    }
    emit fieldRemoved(row);
}

void FmtTable::removeFieldPrivate(const FmtFldIndex &row)
{
    pFieldsModel->beginRemoveField(row);
    FmtField *fld = m_pFields.takeAt(row);

    foreach (FmtIndex *index, m_pIndeces) {
        index->removeField(fld);
    }
    pFieldsModel->endRemoveRows();
}

void FmtTable::removeInsertedFieldPrivate(const FmtFldIndex &row)
{
    if (row >= m_pFields.size())
        return;

    pFieldsModel->beginRemoveField(row);
    FmtField *fld = m_pFields.takeAt(row);
    delete fld;
    pFieldsModel->endRemoveRows();
    emit fieldRemoved(row);
}

FmtField *FmtTable::insertField(const FmtFldIndex &befor, const QString &name, const FmtFldType &type)
{
    FmtField *fld = Q_NULLPTR;
    pUndoStack->beginMacro(tr("Вставлено поле '%1'").arg(name));
    FmtUndoTableAddField *cmd = new FmtUndoTableAddField(this);
    cmd->setNameAndType(name, type);
    cmd->setInsertBefor(befor);
    rebuildOffsets(cmd);
    pUndoStack->push(cmd);
    pUndoStack->endMacro();

    fld = cmd->getField();

    return fld;
}

FmtField *FmtTable::insertFieldPrivate(const FmtFldIndex &befor, const QString &name, const FmtFldType &type)
{
    pFieldsModel->beginInsertRows(QModelIndex(), befor, befor);
    FmtField *fld = new FmtField(this);
    m_pFields.insert(befor, fld);

    fld->m_IgnoreUndoStack = true;
    fld->setName("t_" + name);
    fld->setType(type);
    rebuildOffsets(Q_NULLPTR);
    pFieldsModel->endInsertRows();
    fld->m_IgnoreUndoStack = false;
    emit fieldAdded(fld);

    return fld;
}

FmtField *FmtTable::insertFieldPrivate(const FmtFldIndex &befor, const QMap<quint16,QVariant> &data)
{
    pFieldsModel->beginInsertRows(QModelIndex(), befor, befor);
    FmtField *fld = new FmtField(this);
    m_pFields.insert(befor, fld);

    fld->m_IgnoreUndoStack = true;
    fld->setName(data[FmtField::fld_Name].toString());
    fld->setType(static_cast<FmtFldType>(data[FmtField::fld_Type].toInt()));
    fld->setSize(static_cast<FmtNumber10>(data[FmtField::fld_Type].toInt()));
    fld->setComment(data[FmtField::fld_Comment].toString());
    fld->setOutlen(static_cast<FmtNumber5>(data[FmtField::fld_Outlen].toInt()));
    fld->setDecpoint(static_cast<FmtNumber5>(data[FmtField::fld_DecPoint].toInt()));
    fld->setHidden(data[FmtField::fld_Hidden].toBool());

    rebuildOffsets(Q_NULLPTR);
    pFieldsModel->endInsertRows();
    fld->m_IgnoreUndoStack = false;
    emit fieldAdded(fld);

    return fld;
}

FmtField *FmtTable::field(const FmtNumber5 &index)
{
    return m_pFields.at(index);
}

FmtFldIndex FmtTable::fieldNum(FmtField *fld)
{
    return static_cast<FmtFldIndex>(m_pFields.indexOf(fld));
}

FmtFldIndex FmtTable::fieldsCount() const
{
    return static_cast<FmtFldIndex>(m_pFields.size());
}

bool FmtTable::isFieldAutoInc(FmtField *fld) const
{
    return fld->isAutoInc();
}

bool FmtTable::isFieldAutoInc(const FmtFldIndex &index) const
{
    return m_pFields[index]->isAutoInc();
}

FmtIndex *FmtTable::tableIndex(const FmtNumber5 &index)
{
    return m_pIndeces.at(static_cast<int>(index));
}

QModelIndex FmtTable::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex FmtTable::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}

int FmtTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

bool FmtTable::isExists() const
{
    //return m_Id ? true : false;
    if (!m_Id)
    {
        QSqlQuery q(db);
        q.prepare("SELECT T_ID, T_NAME, T_COMMENT FROM FMT_NAMES WHERE UPPER(T_NAME) = UPPER(?)");
        q.bindValue(0, m_Name);

        if (!ExecuteQuery(&q))
        {
            return q.next();
        }
    }

    return m_Id ? true : false;
}

bool FmtTable::isExistsInDb() const
{
    QSqlQuery q(db);
    q.prepare("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = UPPER(?);");
    q.bindValue(0, m_Name);

    if (!ExecuteQuery(&q))
    {
        if (q.next())
        {
            if (q.value(0).toInt() > 0)
                return true;
        }
    }

    return false;
}

Qt::ItemFlags FmtTable::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

int FmtTable::RemoveTrn()
{
    int stat = 0;
    QSqlQuery qkeys(this->db);
    qkeys.prepare("delete from FMT_KEYS where T_FMTID = ?");
    qkeys.bindValue(0, this->m_Id);

    stat = ExecuteQuery(&qkeys);
    if (!stat)
    {
        QSqlQuery qflds(this->db);
        qflds.prepare("delete from FMT_FIELDS where T_FMTID = ?");
        qflds.bindValue(0, this->m_Id);
        stat = ExecuteQuery(&qflds);
    }

    if (!stat)
    {
        QSqlQuery qtbl(this->db);
        qtbl.prepare("delete from FMT_NAMES where T_ID = ?");
        qtbl.bindValue(0, this->m_Id);
        stat = ExecuteQuery(&qtbl);
    }

    return stat;
}

int FmtTable::removeFmtTable()
{
    int stat = 0;
    stat = trn(this->db, std::bind(&FmtTable::RemoveTrn, this));
    return stat;
}

int FmtTable::deleteDbTable()
{
    int stat = isExists() ? 0 : NOT_EXISTS;

    if (!stat)
    {
        stat = RemoveTrn();
    }
    return stat;
}

QString FmtTableFindFirstEmptyIDSql(const QString &table, const QString &fld)
{
    return QString("SELECT t1.%2 + 1 "
        "    FROM %1 t1 LEFT OUTER JOIN %1 t2 ON (t1.%2 + 1 = t2.%2) "
        "   WHERE t2.%2 IS NULL AND ROWNUM = 1 "
        "ORDER BY t1.%2").arg(table, fld);
}

FmtRecId FmtTable::FindFirstEmptyID()
{
    FmtRecId id = 1;
    QSqlQuery q(db);
    q.prepare(FmtTableFindFirstEmptyIDSql("FMT_NAMES", "T_ID"));
    if (q.exec() && q.next())
        id = static_cast<FmtRecId>(q.value(0).toInt());

    return id;
}

void FmtTable::onIndexChanged()
{
    FmtIndex *indx = dynamic_cast<FmtIndex*>(sender());
    emit indexChanged(indx);
}

FmtIndex *FmtTable::addIndex()
{
    pUndoStack->beginMacro(tr("Добавлен индекс '%1'")
                           .arg(FmtTableMakeIndexName(this, static_cast<FmtFldIndex>(m_pIndeces.size() + 1))));
    FmtUndoTableAddIndex *cmd = new FmtUndoTableAddIndex(this);
    pUndoStack->push(cmd);
    pUndoStack->endMacro();
    emit pIndecesModel->indexAdded();

    return cmd->getIndex();
}

FmtIndex *FmtTable::addIndexPrivate(const FmtFldIndex &row)
{
    FmtFldIndex iRow = row == -1 ? static_cast<FmtFldIndex>(m_pIndeces.size()) : row;

    if (iRow != 0)
        pIndecesModel->beginInsertRows(QModelIndex(), iRow, iRow);
    else
        pIndecesModel->beginResetModel();

    FmtIndex *indx = new FmtIndex(this);
    indx->pParentItem = pIndecesModel->rootItem;
    indx->setFlags(0);
    indx->setName(FmtTableMakeIndexName(this, iRow));
    indx->setType(0);
    //m_pIndeces.append(indx);
    m_pIndeces.insert(iRow, indx);

    if (iRow == 0)
        pIndecesModel->rootItem->appendChild(indx);
    else
        pIndecesModel->rootItem->insertChild(iRow, indx);

    if (iRow != 0)
        pIndecesModel->endInsertRows();
    else
        pIndecesModel->endResetModel();

    connect(indx, SIGNAL(indexChanged()), SLOT(onIndexChanged()));
    connect(this, SIGNAL(nameChanged(QString)), indx, SLOT(UpdateIndexName(QString)));

    emit indexAdded(indx);

    return indx;
}

void FmtTable::removeIndex(const FmtFldIndex &index)
{
    pUndoStack->beginMacro(tr("Удален индекс '%1'")
                           .arg(FmtTableMakeIndexName(this, index)));

    FmtUndoRemoveIndex *cmd = new FmtUndoRemoveIndex(this, Q_NULLPTR);
    cmd->setIndexRow(index);

    if (m_PkIDx == index)
        setPkIDx(-1);

    FmtIndex *pIndex = m_pIndeces[index];

    int count = pIndex->childCount();
    for (int i = 0; i < count; i++)
        pIndex->removeSegment(0);

    pUndoStack->push(cmd);
    pUndoStack->endMacro();
}

void FmtTable::removeIndexPrivate(const FmtFldIndex &index, bool AutoDelete)
{
    if (index > static_cast<FmtFldIndex>(m_pIndeces.size()) || index < 0)
        return;

    pIndecesModel->beginRemoveRows(QModelIndex(), index, index);
    FmtIndex *item = m_pIndeces.takeAt(index);
    pIndecesModel->RemoveFmtIndex(item);
    pIndecesModel->endRemoveRows();

    if (AutoDelete)
        delete item;

    emit indexRemoved(index);
}

FmtFldIndex FmtTable::tableIndexNum(const FmtIndex *pIndex)
{
    return static_cast<FmtFldIndex>(m_pIndeces.indexOf(const_cast<FmtIndex*>(pIndex)));
}

int FmtTable::SaveTrn()
{
    int stat = deleteDbTable();

    if (!stat || stat == NOT_EXISTS)
    {
        int prm = 0;
        QSqlQuery qtbl(db);
        qtbl.prepare("insert into FMT_NAMES values(?,?,?,?,?,?,?,?,?)");

        m_Id = FindFirstEmptyID();
        qtbl.bindValue(prm++, m_Id);
        qtbl.bindValue(prm++, m_Name);
        qtbl.bindValue(prm++, m_CacheSize);
        qtbl.bindValue(prm++, m_Owner);
        qtbl.bindValue(prm++, m_Comment);
        qtbl.bindValue(prm++, m_PkIDx);
        qtbl.bindValue(prm++, m_BlobType);
        qtbl.bindValue(prm++, m_BlobLen);
        qtbl.bindValue(prm++, m_Flags);

        stat = ExecuteQuery(&qtbl);

        if (!stat)
        {
            foreach (FmtField *fld, m_pFields)
            {
                if (!stat)
                    stat = fld->save();
            }

            foreach (FmtIndex *idx, m_pIndeces)
            {
                if (!stat)
                    stat = idx->save();
            }
        }
    }

    return stat;
}

QStringList FmtTable::uniqueIndeces() const
{
    QStringList lst;
    for (int i = 0; i < m_pIndeces.size(); i++)
    {
        if (m_pIndeces[i]->isUnique())
            lst.append(QString::number(i));
    }
    return lst;
}

int FmtTable::safeSave(const bool &IgnoreWarnings)
{
    int stat = 1;
    if (!checkErrors(m_pErrors))
    {
        if (m_pErrors->hasErrors() || (IgnoreWarnings && m_pErrors->hasWarnings()))
            stat = 1;
    }
    if (!stat)
        stat = save();
    return stat;
}

int FmtTable::save()
{
    int stat = 1;
    beginResetModel();
    stat = trn(db, std::bind(&FmtTable::SaveTrn, this));
    endResetModel();
    return stat;
}

qint16 FmtTable::calcMaxOraLenght(qint16 *maxfieldname)
{
    qint16 len = 0, fieldname = 0;

    foreach (FmtField *f, m_pFields) {
        len = qMax(len, static_cast<qint16>(f->getOraDecl().length()));
        fieldname = qMax(fieldname, static_cast<qint16>(f->getOraName().length()));
    }

    if (maxfieldname)
        *maxfieldname = fieldname;

    return len;
}

QString FmtTable::generateCreateTableSql()
{
    QString sql;
    QTextStream stream(&sql);
    stream << "CREATE " << (isTemporary() ? "GLOBAL TEMPORARY " : QString("")) << "TABLE " << m_Name.toUpper() << endl;
    stream << "(" << endl;

    qint16 fldname = 0;
    //qint16 maxlen = calcMaxOraLenght(&fldname);

    bool hasFields = false;
    foreach (FmtField *f, m_pFields) {
        stream << "\t" << f->name().toUpper().leftJustified(fldname) << " ";
        stream << f->getOraDecl();
        hasFields = true;

        if (f != m_pFields.last())
        {
            stream << ",";
            stream << endl;
        }
    }

    if (m_BlobType || m_BlobLen)
    {
        if (hasFields) stream << "," << endl;

        QString blobtypestyring = "BLOB",
                blobfldname = "T_FMTBLOBDATA_XXXX";
        switch(m_BlobType)
        {
        case BT_BLOB_VAR:
            blobtypestyring = "BLOB";
            blobfldname = "T_FMTBLOBDATA_XXXX";
            break;
        case BT_BLOB_STREAM:
            // ?
            break;
        case BT_CLOB:
            blobtypestyring = "CLOB";
            blobfldname = "T_FMTBLOBDATA_XXXX";
            break;
        }

        stream << "\t" << blobfldname << " " << blobtypestyring;
    }
    stream << endl << ")" << (isTemporary() ? " ON COMMIT PRESERVE ROWS" : "");

    return sql;
}

QString FmtTable::getCommentSql()
{
    QSqlDriver *driver = connection()->driver();
    QSqlField fld;
    fld.setType(QVariant::String);
    fld.setValue(m_Comment);
    QString sql = QString("COMMENT ON TABLE %1 IS %2")
            .arg(m_Name.toUpper())
            .arg(driver->formatValue(fld, true));

    return sql;
}

qint16 FmtTable::createDbTable(QString *err)
{
    qint16 stat = 0;

    if (isExists())
    {
        QSqlQuery q(QString("DROP TABLE %1 CASCADE CONSTRAINTS").arg(m_Name), db);
        ExecuteQuery(&q);
    }

    if (!stat)
    {
        QString sql = generateCreateTableSql();
        QSqlQuery q(sql, db);
        stat = ExecuteQuery(&q, err);

        if (!stat)
        {
            foreach (FmtField *f, m_pFields) {
                QSqlQuery qf(db);
                QString sql = f->getCommentSql();
                qf.prepare(sql);
                ExecuteQuery(&qf);
            }
        }
    }

    return stat;
}

FmtNumber5 FmtTable::indecesCount() const
{
    return static_cast<FmtNumber5>(m_pIndeces.count());
}

int FmtTable::dbInit(const QString &log)
{
    int stat = 0;

    QDir dbinitexe = QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir();
    dbinitexe.cd("DLL_ms");

    QProcess exe;
    QStringList arguments;

    QString service = db.connectionName().split("@").at(1);
    service = service.mid(0, service.indexOf("#"));

    QString tablelist = dbtName();

    arguments << QString("%1:%2@%3")
                 .arg(db.userName().simplified())
                 .arg(db.password().simplified())
                 .arg(DatasourceFromService(service))
              << "-DT:ORA" << "-OP:IDX"
              << "-O" <<  QString("-TBLI:%1").arg(tablelist);

    if (!log.isEmpty())
        arguments.append(QString("-LOG:%1").arg(log));
    exe.setWorkingDirectory(dbinitexe.absolutePath());
    stat = CoreStartProcess(&exe, dbinitexe.absoluteFilePath("DbInit.exe"), arguments, true);

    return stat;
}

void FmtTable::clear()
{
    qDeleteAll(m_pIndeces);
    m_pIndeces.clear();

    pFieldsModel->beginResetModel();
    qDeleteAll(m_pFields);
    m_pFields.clear();
    pFieldsModel->endResetModel();
    pUndoStack->clear();

    m_Id = 0;
    m_CacheSize = 0;
    m_BlobLen = 0;
    m_Flags = 0;
    m_PkIDx = -1;
    m_BlobType = 0;
    m_Name = "";
    m_Owner = "";
    m_Comment = "";
}

FmtErrors *FmtTable::lastErrors()
{
    return m_pErrors;
}

bool FmtTable::hasUniqueIndexes() const
{
    bool hr = false;

    foreach (FmtIndex *index, m_pIndeces) {
        if (index->isUnique())
            hr = true;
    }

    return hr;
}

bool FmtTable::hasNonUniqueIndexes() const
{
    bool hr = false;

    foreach (FmtIndex *index, m_pIndeces) {
        if (!index->isUnique())
            hr = true;
    }

    return hr;
}

QUndoStack *FmtTable::undoStack()
{
    return pUndoStack;
}

bool FmtTable::checkErrors(FmtErrors *e)
{
    QRegExp rx("\\bd(\\w+)\\_(dbt|tmp|rec)\\b");
    QRegExpValidator validator(QRegExp("([a-zA-Z_][a-zA-Z0-9]*)*"));

    int pos = 0;
    if ((pos = rx.indexIn(m_Name, pos)) == -1)
    {
        e->appendError(tr("Имя таблицы не соответствует шаблону: (d)<b>name</b>_(dbt|tmp|rec)"));
    }

    pos = 0;
    if (validator.validate(m_Name, pos) == QValidator::Invalid)
    {
        e->appendError(tr("Имя таблицы имеет недопустимое имя"));
    }

    if (m_Comment.isEmpty())
        e->appendError(tr("Таблица не содержит комментария"), FmtErrors::fmtet_Warning);

    if (m_pFields.isEmpty())
    {
        e->appendError(tr("Таблица не содержит полей"));
    }

    if (m_pIndeces.isEmpty())
    {
        e->appendError(tr("Таблица не содержит индексов"), FmtErrors::fmtet_Warning);
    }

    if (!m_Id)
    {
        QSqlQuery q(db);
        q.prepare("SELECT T_ID, T_NAME, T_COMMENT FROM FMT_NAMES WHERE UPPER(T_NAME) = UPPER(?)");
        q.bindValue(0, m_Name);

        int stat = ExecuteQuery(&q);
        if (!stat && q.next())
        {
            e->appendError(tr("Уже существует таблица <b>%1</b> с идентификатором %2: %3")
                           .arg(q.value(1).toString())
                           .arg(q.value(0).toString())
                           .arg(q.value(2).toString()), FmtErrors::fmtet_Warning);
        }
    }

    for (int i = 0; i < m_pFields.size(); i++)
    {
        FmtField *fld = m_pFields[i];
        int pos = 0;
        QString tmpName = fld->name();
        if (validator.validate(tmpName, pos) == QValidator::Invalid)
        {
            e->appendError(tr("Поле №%1 <b>%2</b> (%3) имеет недопустимое имя")
                           .arg(i + 1)
                           .arg(fld->name())
                           .arg(fld->comment()), FmtErrors::fmtet_Warning);
        }

        if (!fld->size())
        {
            e->appendError(tr("Поле №%1 <b>%2</b> (%3) типа '%4' имеет нулевую длину")
                           .arg(i + 1)
                           .arg(fld->name())
                           .arg(fld->comment())
                           .arg(fmtTypeForId(fld->type())), FmtErrors::fmtet_Warning);
        }
        else
        {
            if (!fmtTypeCanHaveCustomSize(fld->type()))
            {
                int fldsize = fmtTypeSize(fld->type());
                if (fldsize != fld->size())
                {
                    e->appendError(tr("Поле №%1 <b>%2</b> c типом '%3' имеет размер %4, когда ожидался %5")
                                   .arg(i + 1)
                                   .arg(fld->name())
                                   .arg(fmtTypeForId(fld->type()))
                                   .arg(fld->size())
                                   .arg(fldsize), FmtErrors::fmtet_Warning);
                }
            }
            else
            {
                qint32 type = fld->type();
                if ((type == fmtt_STRING || type == fmtt_SNR) && fld->size() == 1)
                {
                    e->appendError(tr("Поле №%1 <b>%2</b> (%3) типа '%4' не может иметь длину равную одному")
                                   .arg(i + 1)
                                   .arg(fld->name())
                                   .arg(fld->comment())
                                   .arg(fmtTypeForId(fld->type())), FmtErrors::fmtet_Warning);
                }
            }
        }
    }

    for (int i = 0; i < m_pIndeces.size(); i++)
    {
        FmtIndex *ind = m_pIndeces[i];
        if (!ind->childCount())
        {
            e->appendError(tr("У индекса <b>%1</b> не заданы сегменты").arg(ind->name()), FmtErrors::fmtet_Warning);
        }

        if (ind->isAutoInc())
        {
            if (!ind->isUnique())
            {
                e->appendError(tr("Индекс <b>%1</b> указан автоинкрементным, но не является уникальным")
                               .arg(ind->name()), FmtErrors::fmtet_Warning);
            }

            if (ind->childCount() > 1)
            {
                e->appendError(tr("Индекс <b>%1</b> указан автоинкрементным, но имеет несколько сегментов").arg(ind->name()), FmtErrors::fmtet_Warning);
            }
            /*else if (ind->childCount() == 1)
            {
                FmtField *fld = ((FmtSegment*)ind->child(0))->pFld;
                if (fld->type() != fmtt_LONG)
                {
                    e->appendError(tr("Индекс <b>%1</b> указан автоинкриментным, но поле <b>%2</b> имеет тип '%3', когда ожидался тип 'LONG'")
                                   .arg(ind->name())
                                   .arg(fld->name())
                                   .arg(fmtTypeForId(fld->type())));
                }
            }*/
        }
    }
    return e->isEmpty();
}

const QList<FmtField*> &FmtTable::getFieldsList() const
{
    return m_pFields;
}
