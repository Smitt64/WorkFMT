#include "fmtfield.h"
#include "fmttable.h"
#include "fmtcore.h"
#include "fmtundofieldproperty.h"
#include "fmtfildsmodel.h"
#include "fmtindex.h"
#include "connectioninfo.h"
#include <QDataStream>
#include <QUndoStack>

QString FmtFieldPropertyTextById(const FmtFldIndex &fld)
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

/*!
    \class FmtField
    \brief FmtField класс, обеспечивающий работу с полем таблицы FMT

    \ingroup tools
*/

FmtField::FmtField(QObject *parent) : QObject(parent)
{
    m_Id = 0;
    m_Type = fmtt_INT;
    m_Size = fmtTypeSize(m_Type);
    m_Offset = 0;
    m_Outlen = 0;
    m_Decpoint = 0;
    m_isHidden = false;
    m_Name = "";
    m_Comment = "";

    pTable = dynamic_cast<FmtTable*>(parent);
    pUndoStack = pTable->undoStack();
    pLastCommand = Q_NULLPTR;
    pFieldsModel = pTable->fieldsModel();
    m_IgnoreUndoStack = false;
}

/*!
      \property FmtField::Comment

      Хранит информацию поля FmtField.T_COMMENT. Содержит описание поля
*/
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
            pLastCommand = Q_NULLPTR;

        pUndoStack->push(cmd);
    }
    else
        m_Comment = value;
}

QString FmtField::name() const
{
    return m_Name;
}

FmtRecId FmtField::tableId() const
{
    return pTable->id();
}

FmtFldIndex FmtField::index() const
{
    return static_cast<FmtFldIndex>(pTable->m_pFields.indexOf(const_cast<FmtField *const>(this)));
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
            pLastCommand = Q_NULLPTR;

        pUndoStack->push(cmd);
    }
    else
        m_Name = value;
}

void FmtField::setSize(const FmtNumber10 &v)
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
            setDecpoint(static_cast<FmtNumber5>(v));
        else
            setDecpoint(0);

        if (!hasParentCommand)
            pLastCommand = Q_NULLPTR;

        if (!hasParentCommand)
            pUndoStack->endMacro();
    }
    else
    {
        m_Size = v;
        bool old = pTable->m_IgnoreUndoStack;
        pTable->m_IgnoreUndoStack = true;
        pTable->rebuildOffsets();
        pTable->m_IgnoreUndoStack = old;
    }
}

void FmtField::setType(const FmtFldType &v)
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
        pLastCommand = Q_NULLPTR;

        pUndoStack->endMacro();
    }
    else
    {
        m_Type = v;

        if (v != fmtt_STRING && v != fmtt_CHR && v != fmtt_UCHR && v != fmtt_SNR)
            setSize(fmtTypeSize(v));

        bool old = pTable->m_IgnoreUndoStack;
        pTable->m_IgnoreUndoStack = true;
        pTable->rebuildOffsets();
        pTable->m_IgnoreUndoStack = old;
    }
}

void FmtField::load(const QSqlRecord &rec)
{
    m_Id = static_cast<quint32>(rec.value(FmtField::fld_Id).toInt());
    m_Size = rec.value(FmtField::fld_Size).toInt();
    m_Type = static_cast<FmtFldType>(rec.value(FmtField::fld_Type).toInt());
    m_Offset = rec.value(FmtField::fld_Offset).toInt();
    m_Outlen = static_cast<FmtNumber5>(rec.value(FmtField::fld_Outlen).toInt());
    m_Decpoint = static_cast<FmtNumber5>(rec.value(FmtField::fld_DecPoint).toInt());
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

FmtNumber10 FmtField::offset() const
{
    return m_Offset;
}

void FmtField::setOffset(const FmtNumber10 &v)
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
            pLastCommand = Q_NULLPTR;

        pUndoStack->push(cmd);
    }
    else
        m_Offset = v;
}

FmtNumber5 FmtField::outlen() const
{
    return m_Outlen;
}

void FmtField::setOutlen(const FmtNumber5 &v)
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
            pLastCommand = Q_NULLPTR;

        pUndoStack->push(cmd);
    }
    else
        m_Outlen = v;
}

FmtNumber5 FmtField::decpoint() const
{
    return m_Decpoint;
}

void FmtField::setDecpoint(const FmtNumber5 &v)
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
            pLastCommand = Q_NULLPTR;

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
            pLastCommand = Q_NULLPTR;

        pUndoStack->push(cmd);
    }
    else
        m_isHidden = v;
}

FmtRecId FmtField::FindFirstEmptyID()
{
    int id = 1;
    QSqlQuery q(pTable->db);
    q.prepare("SELECT min(t.T_ID) + 1 as free FROM FMT_FIELDS t WHERE (SELECT T_ID FROM FMT_FIELDS "
              "WHERE T_ID = t.T_ID + 1 ) IS NULL");
    if (q.exec() && q.next())
        id = q.value(0).toInt();

    return static_cast<FmtRecId>(id);
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

bool FmtField::isAutoInc() const
{
    bool hr = false;
    FmtField *pThis = const_cast<FmtField*>(this);
    for (FmtNumber5 i = 0; i < pThis->table()->indecesCount(); i++)
    {
        FmtIndex *indx = pThis->table()->tableIndex(i);
        if (indx->isAutoInc() && indx->hasField(pThis))
            hr = true;
    }
    return hr;
}

/*!
   \fn QString FmtField::getOraDecl() const
    Возвращает описание поля в БД Oracle.
    \code
var fld = table.addField("Simple", CJsFmtTable.ftLONG);
print(fld.getOraDecl());
    \endcode
    Вывод:
    \code
NUMBER(10)
    \endcode
*/
QString FmtField::getOraDecl() const
{
    QString _oraDecl;
    if (m_Type == fmtt_STRING || m_Type == fmtt_SNR || (m_Type == fmtt_UCHR && m_Size > 1))
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

/*!
   \fn QString FmtField::getOraTypeName() const
    Возвращает наименование типа поля в БД Oracle.
    \code
var fld = table.addField("Simple", CJsFmtTable.ftDATE);
print(fld.getOraTypeName());
    \endcode
    Вывод:
    \code
DATE
    \endcode
*/
QString FmtField::getOraTypeName() const
{
    return fmtOracleDecl(m_Type);
}

/*!
   \fn QString FmtField::getCppTypeName(bool Short) const
    Возвращает тип поля в с++.
    \code
var pConnection = CreateConnection("user", "password", "DSN");
var table = new CJsFmtTable(pConnection);

var fld = table.addField("SimpleField", CJsFmtTable.ftLONG);
print(fld.getCppTypeName());
print(fld.getCppTypeName(true));
    \endcode
    Вывод:
    \code
db_int32
int32
    \endcode
*/
QString FmtField::getCppTypeName(bool Short) const
{
    QString _name = fmtCppStructDbTypeName(m_Type);
    if (Short)
        _name = fmtCppStructTypeName(m_Type);
    return _name;
}

/*!
   \fn QString FmtField::getCppDecl(bool funcPrm = false) const
    Возвращает описание поля в с++. \tt funcPrm установлен в true, функция вернет описание поля, если бы оно использовалось как параметр функции:
    \code
var pConnection = CreateConnection("user", "password", "DSN");
var table = new CJsFmtTable(pConnection);

var fld = table.addField("SimpleString", CJsFmtTable.ftSTRING);
fld.Size = 50;
print(fld.getCppDecl());
print(fld.getCppDecl(true));
    \endcode
    Вывод:
    \code
SimpleString[50]
*SimpleString
    \endcode
*/
QString FmtField::getCppDecl(bool funcPrm) const
{
    QString _cppDecl;
    QString name = m_Name.mid(0,2).toLower() == "t_" ? m_Name.mid(2) : m_Name;
    if (m_Type == fmtt_STRING || m_Type == fmtt_SNR || (m_Type == fmtt_UCHR && m_Size > 1))
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
        _cppDecl = name;

    return _cppDecl;
}

/*!
   \fn QString FmtField::getOraDefaultVal() const
    Возвращает значение по умолчанию для поля в БД Oracle.
    \code
var fld = table.addField("Simple", CJsFmtTable.ftDATE);
print(fld.getOraDefaultVal());
    \endcode
    Вывод:
    \code
TO_DATE(''01/01/0001'', ''MM/DD/YYYY'')
    \endcode
*/
QString FmtField::getOraDefaultVal() const
{
    QString t;
    if (isStringType() && m_Type != fmtt_UCHR)
        t = "CHR(1)";
    else
    {
       switch(m_Type)
       {
       case fmtt_INT:
       case fmtt_LONG:
       case fmtt_BIGINT:
           t = "0";
           break;
       case fmtt_FLOAT:
       case fmtt_DOUBLE:
       case fmtt_NUMERIC:
       case fmtt_MONEY:
           t = "0.0";
           break;
       case fmtt_DATE:
           t = "TO_DATE(''01/01/0001'', ''MM/DD/YYYY'')";
           break;
       case fmtt_TIME:
           t = "TO_DATE(''01/01/0001 00:00:00'',''MM/DD/YYYY HH24:MI:SS'')";
           break;
       case fmtt_CHR:
           t = "CHR(0)";
           break;
       case fmtt_UCHR:
           t = "UTL_RAW.CAST_TO_RAW(CHR(0))";
           break;
       }
    }

    return t;
}

QString FmtField::getCommentSql() const
{
    QSqlDriver *driver = pTable->connection()->driver();
    QSqlField fld;
    fld.setType(QVariant::String);
    fld.setValue(m_Comment);

    QString sql = QString("COMMENT ON COLUMN %1.%2 IS %3")
            .arg(pTable->name().toUpper())
            .arg(m_Name.toUpper())
            .arg(driver->formatValue(fld, true));

    return sql;
}

qint32 FmtField::typeIndex() const
{
    return fmtIndexForType(m_Type);
}

void FmtField::setTypeIndex(const FmtFldIndex &v)
{
    setType(fmtTypeFromIndex(v));
    emit typeChanged(m_Type);
}

bool FmtField::isStringType() const
{
    bool hr = false;
    if (m_Type == fmtt_STRING || m_Type == fmtt_SNR)
        hr = true;
    else if (m_Type == fmtt_CHR || m_Type == fmtt_UCHR)
    {
        if (m_Size > 1)
            hr = true;
    }
    return hr;
}

bool FmtField::setDataPrivate(const FmtFldIndex &fld, const QVariant &value)
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
        m_Type = static_cast<FmtFldType>(value.toInt());
        break;
    case fld_Size:
        m_Size = value.toInt();
        break;
    case fld_Offset:
        m_Offset = value.toInt();
        break;
    case fld_Outlen:
        m_Outlen = static_cast<FmtNumber5>(value.toInt());
        break;
    case fld_DecPoint:
        m_Decpoint = static_cast<FmtNumber5>(value.toInt());
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

void FmtField::stroreData(QDataStream &stream)
{
    QMap<quint16,QVariant> fldDataMap;

    for (quint16 fld = 0; fld < fld_MAXCOUNT; fld++)
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
    stream << fldDataMap;
}

void FmtField::stroreData(QByteArray *data)
{
    QDataStream stream(data, QIODevice::WriteOnly);
    stroreData(stream);
}

void FmtField::restoreData(QByteArray *data)
{
    QMap<FmtFldIndex,QVariant> fldDataMap;
    QDataStream stream(data, QIODevice::ReadOnly);
    stream >> fldDataMap;

    QMapIterator<FmtFldIndex,QVariant> iterator(fldDataMap);
    while(iterator.hasNext())
    {
        iterator.next();
        setDataPrivate(iterator.key(), iterator.value());
    }
}

FmtTable *FmtField::table()
{
    return pTable;
}

bool FmtField::isNumber() const
{
    bool hr = false;
    switch(m_Type)
    {
    case fmtt_INT:
    case fmtt_LONG:
    case fmtt_BIGINT:
    case fmtt_FLOAT:
    case fmtt_DOUBLE:
    case fmtt_MONEY:
    case fmtt_NUMERIC:
        hr = true;
        break;
    }

    return hr;
}

bool FmtField::isRealNumber() const
{
    bool hr = false;

    if (isNumber())
    {
        switch(m_Type)
        {
        case fmtt_FLOAT:
        case fmtt_DOUBLE:
        case fmtt_MONEY:
        case fmtt_NUMERIC:
            hr = true;
            break;
        }
    }

    return hr;
}

bool FmtField::isString() const
{
    bool hr = false;
    switch(m_Type)
    {
    case fmtt_STRING:
    case fmtt_SNR:
        hr = true;
        break;
    }

    if (m_Type == fmtt_UCHR && m_Size > 1)
        hr = true;

    return hr;
}

QString FmtField::getMimeType()
{
    return QString("application/FmtFieldList");
}

QDataStream &operator <<(QDataStream &stream, FmtField *fld)
{
    fld->stroreData(stream);
    return stream;
}
