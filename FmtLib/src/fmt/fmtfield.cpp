#include "fmtfield.h"
#include "fmttable.h"
#include "fmtcore.h"
#include "fmtundofieldproperty.h"
#include "fmtfildsmodel.h"
#include <QDataStream>
#include <QUndoStack>

QString FmtFieldPropertyTextById(const quint16 &fld)
{
    QString name;

    switch(fld)
    {
    case FmtField::fld_Id:
        name = QObject::tr("Идентификатор");
        break;
    case FmtField::fld_FmtId:
        name = QObject::tr("Таблица");
        break;
    case FmtField::fld_Name:
        name = QObject::tr("Наименование");
        break;
    case FmtField::fld_Type:
        name = QObject::tr("Тип");
        break;
    case FmtField::fld_Size:
        name = QObject::tr("Размер");
        break;
    case FmtField::fld_Offset:
        name = QObject::tr("Смещение");
        break;
    case FmtField::fld_Outlen:
        name = QObject::tr("Длина");
        break;
    case FmtField::fld_DecPoint:
        name = QObject::tr("Знаков после запятой");
        break;
    case FmtField::fld_Hidden:
        name = QObject::tr("Скрытое поле");
        break;
    case FmtField::fld_Comment:
        name = QObject::tr("Коментарий");
        break;
    }

    return name;
}

FmtField::FmtField(QObject *parent) : QObject(parent)
{
    m_Id = 0;
    m_Size = 0;
    m_Type = -1;
    m_Offset = 0;
    m_Outlen = 0;
    m_Decpoint = 0;
    m_isHidden = false;
    m_Name = "";
    m_Comment = "";

    pTable = (FmtTable*)parent;
    pUndoStack = pTable->undoStack();
    pLastCommand = NULL;
    pFieldsModel = pTable->fieldsModel();
    m_IgnoreUndoStack = false;
}

QString FmtField::comment() const
{
    return m_Comment;
}

void FmtField::setComment(const QString &v)
{
    QString value = v.mid(0, fmtm_FieldCommentMaxSize);
    if (m_Comment == value)
        return;

    if (!m_IgnoreUndoStack)
    {
        bool hasParentCommand = pLastCommand ? true : false;
        FmtUndoFieldProperty *cmd = new FmtUndoFieldProperty(pTable, pTable->fieldNum(this), pLastCommand);
        cmd->setValueToUndo(m_Comment);
        cmd->setValueToRedo(value);
        cmd->setProperty(fld_Comment);

        if (!pLastCommand)
            pLastCommand = cmd;

        if (!hasParentCommand)
            pLastCommand = NULL;

        pUndoStack->push(cmd);
    }
    else
        m_Comment = value;
}

QString FmtField::name() const
{
    return m_Name;
}

quint32 FmtField::tableId() const
{
    return pTable->id();
}

qint32 FmtField::index() const
{
    return pTable->m_pFields.indexOf((FmtField *const)this);
}

void FmtField::setName(const QString &v)
{
    QString value = v.mid(0, fmtm_FieldNameMaxSize);
    if (m_Name == value)
        return;

    if (!m_IgnoreUndoStack)
    {
        bool hasParentCommand = pLastCommand ? true : false;
        FmtUndoFieldProperty *cmd = new FmtUndoFieldProperty(pTable, pTable->fieldNum(this), pLastCommand);
        cmd->setValueToUndo(m_Name);
        cmd->setValueToRedo(value);
        cmd->setProperty(fld_Name);

        if (!pLastCommand)
            pLastCommand = cmd;

        if (!hasParentCommand)
            pLastCommand = NULL;

        pUndoStack->push(cmd);
    }
    else
        m_Name = value;
}

void FmtField::setSize(const qint32 &v)
{
    if (m_Size == v)
        return;

    if (!m_IgnoreUndoStack)
    {
        bool hasParentCommand = pLastCommand ? true : false;
        FmtUndoFieldProperty *cmd = new FmtUndoFieldProperty(pTable, pTable->fieldNum(this));
        cmd->setValueToUndo(m_Size);
        cmd->setValueToRedo(v);
        cmd->setProperty(fld_Size);

        if (!pLastCommand)
            pLastCommand = cmd;

        if (!hasParentCommand)
            pUndoStack->beginMacro(tr("Изменен размер поля '%1'").arg(m_Name));

        pUndoStack->push(cmd);
        pTable->rebuildOffsets(pLastCommand);

        if (m_Type == fmtt_STRING || m_Type == fmtt_CHR || m_Type == fmtt_UCHR || m_Type == fmtt_SNR)
            setDecpoint(v);
        else
            setDecpoint(0);

        if (!hasParentCommand)
            pLastCommand = NULL;

        if (!hasParentCommand)
            pUndoStack->endMacro();
    }
    else
    {
        m_Size = v;
        pTable->rebuildOffsets();
    }
}

void FmtField::setType(const qint32 &v)
{
    if (m_Type == v)
        return;

    if (!m_IgnoreUndoStack)
    {
        FmtUndoFieldProperty *cmd = new FmtUndoFieldProperty(pTable, pTable->fieldNum(this));
        cmd->setValueToUndo(m_Type);
        cmd->setValueToRedo(v);
        cmd->setProperty(fld_Type);

        QString macroName = tr("Изменен тип поля на '%1', перерасчитаны смещения")
                .arg(fmtTypeForId(v));

        pUndoStack->beginMacro(macroName);
        pLastCommand = cmd;
        pUndoStack->push(cmd);

        if (v != fmtt_STRING && v != fmtt_CHR && v != fmtt_UCHR && v != fmtt_SNR)
            setSize(fmtTypeSize(v));
        pLastCommand = NULL;

        pUndoStack->endMacro();
    }
    else
    {
        m_Type = v;

        if (v != fmtt_STRING && v != fmtt_CHR && v != fmtt_UCHR && v != fmtt_SNR)
            setSize(fmtTypeSize(v));
        pTable->rebuildOffsets();
    }
}

void FmtField::load(const QSqlRecord &rec)
{
    m_Id = rec.value(FmtField::fld_Id).toInt();
    m_Size = rec.value(FmtField::fld_Size).toInt();
    m_Type = rec.value(FmtField::fld_Type).toInt();
    m_Offset = rec.value(FmtField::fld_Offset).toInt();
    m_Outlen = rec.value(FmtField::fld_Outlen).toInt();
    m_Decpoint = rec.value(FmtField::fld_DecPoint).toInt();
    m_isHidden = rec.value(FmtField::fld_Hidden).toBool();

    m_Name = rec.value(FmtField::fld_Name).toString();
    m_Comment = rec.value(FmtField::fld_Comment).toString();
}

QString FmtField::undecorateName() const
{
    QString str = name();

    if (str.mid(0, 2).toLower() == "t_")
        str = str.mid(2);
    return str;
}

qint32 FmtField::offset() const
{
    return m_Offset;
}

void FmtField::setOffset(const qint32 &v)
{
    if (m_Offset == v)
        return;

    if (!m_IgnoreUndoStack)
    {
        bool hasParentCommand = pLastCommand ? true : false;
        FmtUndoFieldProperty *cmd = new FmtUndoFieldProperty(pTable, pTable->fieldNum(this));
        cmd->setValueToUndo(m_Offset);
        cmd->setValueToRedo(v);
        cmd->setProperty(fld_Offset);

        if (!pLastCommand)
            pLastCommand = cmd;

        if (!hasParentCommand)
            pLastCommand = NULL;

        pUndoStack->push(cmd);
    }
    else
        m_Offset = v;
}

quint32 FmtField::outlen() const
{
    return m_Outlen;
}

void FmtField::setOutlen(const qint32 &v)
{
    if (m_Outlen == v)
        return;

    if (!m_IgnoreUndoStack)
    {
        bool hasParentCommand = pLastCommand ? true : false;
        FmtUndoFieldProperty *cmd = new FmtUndoFieldProperty(pTable, pTable->fieldNum(this), pLastCommand);
        cmd->setValueToUndo(m_Outlen);
        cmd->setValueToRedo(v);
        cmd->setProperty(fld_Outlen);

        if (!pLastCommand)
            pLastCommand = cmd;

        if (!hasParentCommand)
            pLastCommand = NULL;

        pUndoStack->push(cmd);
    }
    else
        m_Outlen = v;
}

quint32 FmtField::decpoint() const
{
    return m_Decpoint;
}

void FmtField::setDecpoint(const qint32 &v)
{
    if (m_Decpoint == v)
        return;

    if (!m_IgnoreUndoStack)
    {
        bool hasParentCommand = pLastCommand ? true : false;
        FmtUndoFieldProperty *cmd = new FmtUndoFieldProperty(pTable, pTable->fieldNum(this));
        cmd->setValueToUndo(m_Decpoint);
        cmd->setValueToRedo(v);
        cmd->setProperty(fld_DecPoint);

        if (!pLastCommand)
            pLastCommand = cmd;

        if (!hasParentCommand)
            pLastCommand = NULL;

        pUndoStack->push(cmd);
    }
    else
        m_Decpoint = v;
}

bool FmtField::isHidden() const
{
    return m_isHidden;
}

void FmtField::setHidden(const bool &v)
{
    if (m_isHidden == v)
        return;

    if (!m_IgnoreUndoStack)
    {
        bool hasParentCommand = pLastCommand ? true : false;
        FmtUndoFieldProperty *cmd = new FmtUndoFieldProperty(pTable, pTable->fieldNum(this), pLastCommand);
        cmd->setValueToUndo(m_isHidden);
        cmd->setValueToRedo(v);
        cmd->setProperty(fld_Hidden);

        if (!pLastCommand)
            pLastCommand = cmd;

        if (!hasParentCommand)
            pLastCommand = NULL;

        pUndoStack->push(cmd);
    }
    else
        m_isHidden = v;
}

int FmtField::FindFirstEmptyID()
{
    int id = 1;
    QSqlQuery q(pTable->db);
    q.prepare("SELECT min(t.T_ID) + 1 as free FROM FMT_FIELDS t WHERE (SELECT T_ID FROM FMT_FIELDS "
              "WHERE T_ID = t.T_ID + 1 ) IS NULL");
    if (q.exec() && q.next())
        id = q.value(0).toInt();

    return id;
}

int FmtField::save()
{
    int stat = 0;

    QSqlQuery qfld(pTable->db);
    qfld.prepare("insert into FMT_FIELDS values(?,?,?,?,?,?,?,?,CHR(0),?)");

    m_Id = FindFirstEmptyID();
    int prm = 0;
    qfld.bindValue(prm++, m_Id);
    qfld.bindValue(prm++, pTable->id());
    qfld.bindValue(prm++, m_Name);
    qfld.bindValue(prm++, m_Type);
    qfld.bindValue(prm++, m_Size);
    qfld.bindValue(prm++, m_Offset);
    qfld.bindValue(prm++, m_Outlen);
    qfld.bindValue(prm++, m_Decpoint);
    qfld.bindValue(prm++, m_Comment);

    stat = ExecuteQuery(&qfld);

    return stat;
}

QString FmtField::getOraName() const
{
    return QString();
}

QString FmtField::getOraDecl() const
{
    QString _oraDecl;
    if (m_Type == fmtt_STRING || m_Type == fmtt_SNR)
    {
        _oraDecl = QString("%1(%2)")
                .arg(getOraTypeName())
                .arg(m_Size - 1);
    }
    else if (m_Type == fmtt_CHR || m_Type == fmtt_UCHR)
    {
        _oraDecl = QString("%1(%2)")
                .arg(getOraTypeName())
                .arg(m_Size);
    }
    else
    {
        _oraDecl = getOraTypeName();
    }

    return _oraDecl;
}

QString FmtField::getOraTypeName() const
{
    return fmtOracleDecl(m_Type);
}

QString FmtField::getCppTypeName(bool Short) const
{
    QString _name = fmtCppStructTypeName(m_Type);
    if (Short)
        _name = _name.remove("db_");
    return _name;
}

QString FmtField::getCppDecl(bool funcPrm) const
{
    QString _cppDecl;
    QString name = m_Name.mid(0,2).toLower() == "t_" ? m_Name.mid(2) : m_Name;
    if (m_Type == fmtt_STRING || m_Type == fmtt_SNR)
    {
        if (!funcPrm)
        {
            _cppDecl = QString("%1[%2]")
                    .arg(name)
                    .arg(m_Size);
        }
        else
        {
            _cppDecl = QString("*%1")
                    .arg(name);
        }
    }
    else
    {
        _cppDecl = name;
    }
    return _cppDecl;
}

QString FmtField::getCommentSql() const
{
    QString sql = QString("COMMENT ON COLUMN %1.%2 IS '%3'")
            .arg(pTable->name().toUpper())
            .arg(m_Name.toUpper())
            .arg(m_Comment);

    return sql;
}

qint32 FmtField::typeIndex() const
{
    return fmtIndexForType(m_Type);
}

void FmtField::setTypeIndex(const qint32 &v)
{
    setType(fmtTypeFromIndex(v));
    emit typeChanged(m_Type);
}

bool FmtField::setDataPrivate(const quint16 &fld, const QVariant &value)
{
    switch(fld)
    {
    case fld_Id:
        break;
    case fld_FmtId:
        break;
    case fld_Name:
        m_Name = value.toString();
        break;
    case fld_Type:
        m_Type = value.toInt();
        break;
    case fld_Size:
        m_Size = value.toInt();
        break;
    case fld_Offset:
        m_Offset = value.toInt();
        break;
    case fld_Outlen:
        m_Outlen = value.toInt();
        break;
    case fld_DecPoint:
        m_Decpoint = value.toInt();
        break;
    case fld_Hidden:
        m_isHidden = value.toBool();
        break;
    case fld_Comment:
        m_Comment = value.toString();
        break;
    }
    pFieldsModel->setFieldPropertyChanged(this, fld);
    return true;
}

void FmtField::stroreData(QByteArray *data)
{
    QMap<quint16,QVariant> fldDataMap;

    for (int fld = 0; fld < fld_MAXCOUNT; fld++)
    {
        switch(fld)
        {
        case fld_Id:
            fldDataMap[fld] = m_Id;
            break;
        case fld_FmtId:
            fldDataMap[fld] = pTable->id();
            break;
        case fld_Name:
            fldDataMap[fld] = m_Name;
            break;
        case fld_Type:
            fldDataMap[fld] = m_Type;
            break;
        case fld_Size:
            fldDataMap[fld] = m_Size;
            break;
        case fld_Offset:
            fldDataMap[fld] = m_Offset;
            break;
        case fld_Outlen:
            fldDataMap[fld] = m_Outlen;
            break;
        case fld_DecPoint:
            fldDataMap[fld] = m_Decpoint;
            break;
        case fld_Hidden:
            fldDataMap[fld] = m_isHidden;
            break;
        case fld_Comment:
            fldDataMap[fld] = m_Comment;
            break;
        }
    }

    QDataStream stream(data, QIODevice::WriteOnly);
    stream << fldDataMap;
}

void FmtField::restoreData(QByteArray *data)
{
    QMap<quint16,QVariant> fldDataMap;
    QDataStream stream(data, QIODevice::ReadOnly);
    stream >> fldDataMap;

    QMapIterator<quint16,QVariant> iterator(fldDataMap);
    while(iterator.hasNext())
    {
        iterator.next();
        setDataPrivate(iterator.key(), iterator.value());
    }
}
