#include "jsfmttable.h"
#include <fmttable.h>
#include <fmtfield.h>
#include <fmtindex.h>
#include <fmtsegment.h>
#include <fmterrors.h>
#include <connectioninfo.h>
#include <QtScript>

typedef QSharedPointer<FmtTable> JsFmtTable;
Q_DECLARE_METATYPE(QSharedPointer<FmtTable>)

QScriptValue constructJsFmtTable(QScriptContext *context, QScriptEngine *engine)
{
    CJsFmtTable *jsTable = Q_NULLPTR;
    if (!context->isCalledAsConstructor())
    {
        FmtTable *table = qobject_cast<FmtTable*>(context->argument(0).toQObject());

        if (!table)
            return context->throwError(QScriptContext::SyntaxError, "please use the 'new' operator");

        jsTable = new CJsFmtTable(table);
    }

    ConnectionInfo *pInfo = qobject_cast<ConnectionInfo*>(context->argument(0).toQObject());
    if (!pInfo)
        return context->throwError(QScriptContext::TypeError, "please supply a ConnectionInfo as first argument");

    FmtTable *pTable = new FmtTable(pInfo, Q_NULLPTR);
    jsTable = new CJsFmtTable(pTable);

    return engine->newQObject((QObject*)/*pTable*/jsTable, QScriptEngine::ScriptOwnership);
}

QScriptValue constructJsFmtErrors(QScriptContext *context, QScriptEngine *engine)
{
    if (!context->isCalledAsConstructor())
        return context->throwError(QScriptContext::SyntaxError, "please use the 'new' operator");

    FmtErrors *pErrors = new FmtErrors(Q_NULLPTR);
    return engine->newQObject((QObject*)pErrors, QScriptEngine::ScriptOwnership);
}

QScriptValue constructJsFmtTablePtr(QScriptContext *context, QScriptEngine *engine)
{
    if (!context->isCalledAsConstructor())
        return context->throwError(QScriptContext::SyntaxError, "please use the 'new' operator");

    /*ConnectionInfo *pInfo = qobject_cast<ConnectionInfo*>(context->argument(0).toQObject());

    if (!pInfo)
        return context->throwError(QScriptContext::TypeError, "please supply a ConnectionInfo as first argument");*/

    /*FmtTable *pTable = new FmtTable(pInfo, Q_NULLPTR);*/
    JsFmtTable jsTable/*(pTable)*/;

    return engine->newVariant(context->thisObject(), QVariant::fromValue(jsTable));
}

QScriptValue FmtFieldToScriptValue(QScriptEngine *engine, FmtField* const &in)
{
    return engine->newQObject(in);
}

void FmtFieldFromScriptValue(const QScriptValue &object, FmtField* &out)
{
    out = qobject_cast<FmtField*>(object.toQObject());
}

QScriptValue FmtIndexToScriptValue(QScriptEngine *engine, FmtIndex* const &in)
{
    return engine->newQObject(in);
}

void FmtIndexFromScriptValue(const QScriptValue &object, FmtIndex* &out)
{
    out = qobject_cast<FmtIndex*>(object.toQObject());
}

QScriptValue FmtSegmentToScriptValue(QScriptEngine *engine, FmtSegment* const &in)
{
    return engine->newQObject(in);
}

void FmtErrorsFromScriptValue(const QScriptValue &object, FmtErrors* &out)
{
    out = qobject_cast<FmtErrors*>(object.toQObject());
}

QScriptValue FmtErrorsToScriptValue(QScriptEngine *engine, FmtErrors* const &in)
{
    return engine->newQObject(in);
}

void FmtSegmentFromScriptValue(const QScriptValue &object, FmtSegment* &out)
{
    out = qobject_cast<FmtSegment*>(object.toQObject());
}

QScriptValue FmtTablePtr_toFmtTable(QScriptContext *context, QScriptEngine *engine)
{
    JsFmtTable obj = qscriptvalue_cast<JsFmtTable>(context->thisObject());
    if (!obj)
        return context->throwError(QScriptContext::TypeError, "This object is not an FmtTablePtr");

    CJsFmtTable *jsTable = new CJsFmtTable((JsFmtTable)obj, Q_NULLPTR);
    return engine->newQObject((QObject*)jsTable, QScriptEngine::ScriptOwnership);
}

// ====================================================================================================

void initializeJsFmtTable(QScriptEngine *engine)
{
    QScriptValue ctor = engine->newFunction(constructJsFmtTable);
    QScriptValue metaObject = engine->newQMetaObject(&/*FmtTable*/CJsFmtTable::staticMetaObject, ctor);
    engine->globalObject().setProperty("CJsFmtTable", metaObject);

    QScriptValue errorsctor = engine->newFunction(constructJsFmtTable);
    QScriptValue errorsmetaObject = engine->newQMetaObject(&FmtErrors::staticMetaObject, errorsctor);
    engine->globalObject().setProperty("FmtErrors", errorsmetaObject);

    QScriptValue jsTableProto = engine->newObject();
    jsTableProto.setProperty("toFmtTable", engine->newFunction(FmtTablePtr_toFmtTable));

    QScriptValue jsTablePtrCtor = engine->newFunction(constructJsFmtTablePtr, jsTableProto);
    engine->globalObject().setProperty("FmtTablePtr", jsTablePtrCtor);

    qScriptRegisterMetaType(engine, FmtFieldToScriptValue, FmtFieldFromScriptValue);
    qScriptRegisterMetaType(engine, FmtIndexToScriptValue, FmtIndexFromScriptValue);
    qScriptRegisterMetaType(engine, FmtSegmentToScriptValue, FmtSegmentFromScriptValue);
    qScriptRegisterMetaType(engine, FmtErrorsToScriptValue, FmtErrorsFromScriptValue);
}

/*!
    \class CJsFmtTable
    \brief FmtTable класс, обеспечивающий работу с записью Fmt словаря

    \ingroup tools

    \code
var pConnection = CreateConnection("user", "password", "DSN");
var table = new CJsFmtTable(pConnection);
if (table.load("dparty_dbt"))
{
    for (var i = 0; i < table.FieldsCount; i++)
    {
        var fld = table.field(i);
        print(fld.Name, ": ", fld.Comment);
    }
}
    \endcode
*/

/*!
      \enum CJsFmtTable::FmtFieldTypes

      This enum type specifies a corner in a rectangle:

      \value ftINT
        db_int16
      \value ftLONG
        db_int32
      \value ftBIGINT
        int64_t
      \value ftFLOAT
        db_float
      \value ftDOUBLE
        db_double
      \value ftMONEY
      \value ftSTRING
        char[]
      \value ftSNR
        char[]
      \value ftDATE
        bdate
      \value ftTIME
        btime
      \value ftCHR
        char
      \value ftUCHR
        char
      \value ftNUMERIC
        db_lmoney, db_decimal,DBNumeric
*/

/*!
      \property CJsFmtTable::Id

      Идентификатор таблицы в БД
      Является чисто информативным и недоступен для редактирования.
*/

CJsFmtTable::CJsFmtTable(FmtTable *Table, QObject *parent) :
    QObject(parent)
{
    pTable.reset(Table);
}

CJsFmtTable::CJsFmtTable(QSharedPointer<FmtTable> Table, QObject *parent) :
    QObject(parent)
{
    pTable = Table;
}

/*!
      \property CJsFmtTable::isTemporary

      Возвращает \c true если таблица является временной.
*/
bool CJsFmtTable::isTemporary() const
{
    return pTable->isTemporary();
}

/*!
      \property CJsFmtTable::isRecord

      Возвращает \c true если запись является структурой.
*/
bool CJsFmtTable::isRecord() const
{
    return pTable->isRecord();
}

void CJsFmtTable::setIsTemporary(const bool &v)
{
    pTable->setIsTemporary(v);
}

void CJsFmtTable::setIsRecord(const bool &v)
{
    pTable->setIsRecord(v);
}

/*!
   \fn bool CJsFmtTable::load(const QString &table)
    Загружает таблицу из FMT_NAMES в память.
    Возвращает true в случаее успеха, и fasle если произошла ошибка
*/
bool CJsFmtTable::load(const QString &table)
{
    return pTable->load(table);
}

/*!
   \fn bool CJsFmtTable::load(const quint32 &id)
    Загружает таблицу из FMT_NAMES в память.
    Возвращает true в случаее успеха, и fasle если произошла ошибка
*/
bool CJsFmtTable::load(const quint32 &id)
{
    return pTable->load(id);
}

/*!
      \property CJsFmtTable::CacheSize

      Хранит информацию поля FMT_NAMES.T_CACHESIZE
*/
void CJsFmtTable::setCacheSize(const quint32 &v)
{
    pTable->setCacheSize(v);
}

/*!
      \property CJsFmtTable::PkIDx

      Хранит информацию поля FMT_NAMES.T_PKIDX. Содержит информацию о первичном ключе таблицы
*/
void CJsFmtTable::setPkIDx(const quint32 &v)
{
    pTable->setPkIDx(v);
}

/*!
      \property CJsFmtTable::BlobType

      Хранит информацию поля FMT_NAMES.T_BLOBTYPE. Содержит информацию о типе блоба
*/
void CJsFmtTable::setBlobType(const quint16 &v)
{
    pTable->setBlobType(v);
}

/*!
      \property CJsFmtTable::BlobLen

      Хранит информацию поля FMT_NAMES.T_BLOBLEN
*/
void CJsFmtTable::setBlobLen(const quint32 &v)
{
    pTable->setBlobLen(v);
}

/*!
      \property CJsFmtTable::Name

      Хранит информацию поля FMT_NAMES.T_NAME. Содержит информацию о наименовании таблицы
*/
QString CJsFmtTable::name() const
{
    return pTable->name();
}

/*!
      \property CJsFmtTable::DbtName

      Содержит информацию о наименовании таблицы вида party.dbt
*/
QString CJsFmtTable::dbtName() const
{
    return pTable->dbtName();
}

void CJsFmtTable::setName(const QString &v)
{
    pTable->setName(v);
}

/*!
      \property CJsFmtTable::Comment

      Хранит информацию поля FMT_NAMES.T_COMMENT. Содержит описание таблицы
*/
QString CJsFmtTable::comment() const
{
    return pTable->comment();
}

/*!
      \property CJsFmtTable::Owner

      Хранит информацию поля FMT_NAMES.T_OWNER.
*/
QString CJsFmtTable::owner() const
{
    return pTable->owner();
}

void CJsFmtTable::setComment(const QString &v)
{
    pTable->setComment(v);
}

void CJsFmtTable::setOwner(const QString &v)
{
    pTable->setOwner(v);
}

/*!
   \fn FmtField *CJsFmtTable::addField(const QString &name, const qint16 &type)
    Добавляет поле в таблицу с именем \tt name типа \tt type (\tt {FmtFieldTypes}).
    Возвращает объект, описывающий свойства поля

    \code
var fld1 = table.addField("ID", FmtTable.ftLONG);
fld1.Comment = "Системный ID записи для ссылок.";
    \endcode
*/
FmtField *CJsFmtTable::addField(const QString &name, const qint16 &type)
{
    return pTable->addField(name, type);
}

/*!
   \fn FmtField *CJsFmtTable::insertField(const int &befor, const QString &name, const qint16 &type)
    Добавляет поле в таблицу с именем \tt name, в позицию \tt befor и типа \tt type (\tt {FmtFieldTypes}).
    Возвращает объект, описывающий свойства поля
*/
FmtField *CJsFmtTable::insertField(const int &befor, const QString &name, const qint16 &type)
{
    return pTable->insertField(befor, name, type);
}

/*!
   \fn FmtField *CJsFmtTable::field(const qint32 &index)
    Возвращает объект, описывающий свойства поля, стоящий на позиции \tt index
    \code
var fld = table.field(i);
    \endcode
*/
FmtField *CJsFmtTable::field(const qint32 &index)
{
    return pTable->field(index);
}

/*!
   \fn qint32 CJsFmtTable::fieldNum(FmtField *fld)
    Возвращает позицию объекта \tt fld, описывающего свойства поля
*/
qint32 CJsFmtTable::fieldNum(FmtField *fld)
{
    return pTable->fieldNum(fld);
}

/*!
      \property CJsFmtTable::FieldsCount

      Содержит информацию о количестве полей в таблице
      Является чисто информативным и недоступен для редактирования.
*/
qint32 CJsFmtTable::fieldsCount() const
{
    return pTable->fieldsCount();
}

/*!
      \property CJsFmtTable::IndexesCount

      Содержит информацию о количестве индексов в таблице
      Является чисто информативным и недоступен для редактирования.
*/
quint32 CJsFmtTable::indexesCount() const
{
    return pTable->indecesCount();
}

bool CJsFmtTable::isExists() const
{
    return pTable->isExists();
}

bool CJsFmtTable::isExistsInDb() const
{
    return pTable->isExistsInDb();
}

/*!
   \fn QStringList CJsFmtTable::uniqueIndeces() const

    Возвращает \c список индексов, являющихся иникальными для данной таблицы

    \code
var pConnection = CreateConnection("user", "password", "DSN");
var table = new CJsFmtTable(pConnection);
if (table.load("dparty_dbt"))
{
    print("uniqueIndeces:");
    var uniqueIndeces = table.uniqueIndeces();

    for (var i = 0; i < uniqueIndeces.length; i++)
        print("Index: ", uniqueIndeces[i]);
}

    \endcode
*/
QStringList CJsFmtTable::uniqueIndeces() const
{
    return pTable->uniqueIndeces();
}

FmtIndex *CJsFmtTable::addIndex()
{
    return pTable->addIndex();
}

FmtIndex *CJsFmtTable::tableIndex(const qint32 &index)
{
    return pTable->tableIndex(index);
}

quint32 CJsFmtTable::tableIndexNum(FmtIndex *pIndex)
{
    return pTable->tableIndexNum(pIndex);
}

void CJsFmtTable::removeIndex(const quint32 &index)
{
    pTable->removeIndex(index);
}

/*!
   \fn FmtErrors *CJsFmtTable::checkErrors()

    Возвращает результат проверки таблицы на ошибки (список ошибок и предупреждений).

    \code
    function ErrorType(e) {
        var str = "";
        if (e.type == FmtErrors.fmtet_Error)
            str = "Error";
        else if (e.type == FmtErrors.fmtet_Warning)
            str = "Warning";
        return str;
    }

    var pConnection = CreateConnection("user", "password", "DSN");
    var table = new CJsFmtTable(pConnection);
    var err = table.checkErrors();

    while(err.next())
        print(ErrorType(err), ": ", err.text());
    \endcode

    Вывод:
    \code
    Error: Имя таблицы не соответствует шаблону: (d)name_(dbt|tmp|rec)
    Warning: Таблица не содержит комментария
    Error: Таблица не содержит полей
    Warning: Таблица не содержит индексов
    \endcode
*/
FmtErrors *CJsFmtTable::checkErrors()
{
    FmtErrors *e = new FmtErrors(this);
    pTable->checkErrors(e);
    return e;
}

QSharedPointer<FmtTable> CJsFmtTable::fmtTable()
{
    return pTable;
}

Q_SCRIPT_DECLARE_QMETAOBJECT(FmtSegment, FmtIndex*)
Q_SCRIPT_DECLARE_QMETAOBJECT(FmtIndex, FmtTable*)
Q_SCRIPT_DECLARE_QMETAOBJECT(FmtField, QObject*)
Q_SCRIPT_DECLARE_QMETAOBJECT(FmtTable, QObject*)
Q_SCRIPT_DECLARE_QMETAOBJECT(FmtErrors, QObject*)
