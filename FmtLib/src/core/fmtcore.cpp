#include "fmtcore.h"
#include "commandsstorage.h"
#include "fmtapplication.h"
#include "fmtimpexpwrp.h"
#include <errorsmodel.h>
#include "fmttable.h"
#include "fmtfield.h"
#include "errordlg.h"
#include "dbinitdlg.h"
#include "oracletnslistmodel.h"
#include "loggingcategories.h"
#include "fmtdbftoolwrp.h"
#include "selectfolderdlg.h"
#include "selectfiltereddlg.h"
#include "selectfieldsmodel.h"
#include "fmtfildsmodel.h"
#include "toolsruntime.h"
#include "src/core/colormanager.h"
#include <Windows.h>
#include <QtCore>
#include <QSqlError>
#include <map>
#include <iostream>
#include <strstream>
#include <QProgressDialog>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>
#include <QFileDialog>
#include <QInputDialog>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QMenu>
#include <limits>
#include "DataStructure.hpp"

#undef max

static QStringList FmtTypesList/* = QStringList()
        << "INT"
        << "LONG"
        << "BIGINT"
        << "FLOAT"
        << "DOUBLE"
        << "MONEY"
        << "STRING"
        << "SNR"
        << "DATE"
        << "TIME"
        << "CHR"
        << "UCHR"
        << "NUMERIC"*/;

/*typedef struct tagFmtTypeInfoT : public
        DataStructure
        <
        qint16, // type;
        quint16,    // size;
        quint16,    // indexType;
        QString,    // oraType;
        QString,    // cppType;
        QString,    // cppDbType;
        QString,    // cppDbBaseType;
        QString,    // rslType;
        QString     // rslValueName;
        >
{

}FmtTypeInfoT;*/

typedef struct tagFmtTypeInfo
{
    qint16 _type;
    quint16 _size;
    quint16 _indexType;
    QString _oraType;
    QString _pgType;
    QString _cppType;
    QString _cppDbType;
    QString _cppDbBaseType;
    QString _rslType;
    QString _rslValueName;
    QString _xmlTypeName;
    QString _rsdType;
    QString _rsdConst;
    QString _zeroConstant;

    tagFmtTypeInfo
    (
            const quint16 &type = 0,
            const quint16 &size = 0,
            const QString &oraType = QString(),
            const quint16 &indexType = 0,
            const QString &cppType = QString(),
            const QString &cppDbType = QString(),
            const QString &cppDbBaseType = QString(),
            const QString &rslType = QString(),
            const QString &rslValueName = QString(),
            const QString &xmlTypeName = QString()
            )
    {
       _type = type;
       _size = size;
       _oraType = oraType;
       _indexType = indexType;
       _cppType = cppType;
       _cppDbType = cppDbType;
       _cppDbBaseType = cppDbBaseType;
       _rslType = rslType;
       _rslValueName = rslValueName;
       _xmlTypeName = xmlTypeName;
    }
}FmtTypeInfo;

typedef struct tagFmtTtypesItem
{
    QString name;
    int id;
}FmtIndexTypesItem;
typedef QMap<QString, FmtTypeInfo> FmtTypesMapType;
typedef QMap<QString, int> fmtTypesMap;

//Q_GLOBAL_STATIC(FmtTypesMapType, FmtTypesMap);
static FmtTypesMapType FmtTypesMap;
static QStringList FmtTypesNames;
static fmtTypesMap fmtTypesStrMap, indexTypesStrMap;

#define AddFmtTypesMap(constant) fmtTypesStrMap.insert(#constant, constant)
#define AddIndexTypesMap(constant) indexTypesStrMap.insert(#constant, constant)

void FmtInit()
{
    if (!indexTypesStrMap.isEmpty())
        return;

    AddFmtTypesMap(fmtt_DATETIME);
    AddFmtTypesMap(fmtt_INT);
    AddFmtTypesMap(fmtt_LONG);
    AddFmtTypesMap(fmtt_BIGINT);
    AddFmtTypesMap(fmtt_FLOAT);
    AddFmtTypesMap(fmtt_DOUBLE);
    AddFmtTypesMap(fmtt_MONEY);
    AddFmtTypesMap(fmtt_STRING);
    AddFmtTypesMap(fmtt_SNR);
    AddFmtTypesMap(fmtt_DATE);
    AddFmtTypesMap(fmtt_TIME);
    AddFmtTypesMap(fmtt_CHR);
    AddFmtTypesMap(fmtt_UCHR);
    AddFmtTypesMap(fmtt_NUMERIC);

    AddIndexTypesMap(fmtk_Estring);
    AddIndexTypesMap(fmtk_Einteger);
    AddIndexTypesMap(fmtk_Efloat);
    AddIndexTypesMap(fmtk_Edate);
    AddIndexTypesMap(fmtk_Etime);
    AddIndexTypesMap(fmtk_Edecimal);
    AddIndexTypesMap(fmtk_Emoney);
    AddIndexTypesMap(fmtk_Elogical);
    AddIndexTypesMap(fmtk_Enumeric);
    AddIndexTypesMap(fmtk_Ebfloat);
    AddIndexTypesMap(fmtk_Elstring);
    AddIndexTypesMap(fmtk_Ezstring);
    AddIndexTypesMap(fmtk_Eunbin);
    AddIndexTypesMap(fmtk_Eautoinc);
    AddIndexTypesMap(fmtk_Ecurrency);

    /*FmtTypesMap =
    {
        //           type      size  oraType          indexType      cppType      cppDbType     cppDbBaseType     rslType     rslValueName
        {"INT"    , {fmtt_INT,    2, "NUMBER(5)",     fmtk_Einteger, "int16",     "db_int16",   "db_baseint16",   "V_INTEGER",    "intval"  , "FT_INT16"} },
        {"LONG"   , {fmtt_LONG,   4, "NUMBER(10)",    fmtk_Einteger, "int32",     "db_int32",   "db_baseint32",   "V_INTEGER",    "intval"  , "FT_INT32"} },
        {"BIGINT" , {fmtt_BIGINT, 8, "NUMBER(19)",    fmtk_Einteger, "int64_t",   "db_int64",   "db_baseint64",   "V_BIGINT",     "bigint"  , "FT_INT64"} },
        {"FLOAT"  , {fmtt_FLOAT,  4, "FLOAT(24)",     fmtk_Efloat,   "float",     "db_float",   "db_basefloat",   "V_DOUBLE",     "doubvalL", "FT_FLOAT"} },
        {"DOUBLE" , {fmtt_DOUBLE, 8, "FLOAT(53)",     fmtk_Ebfloat,  "double",    "db_double",  "db_basedouble",  "V_DOUBLE",     "doubvalL", "FT_DOUBLE"} },
        {"MONEY"  , {fmtt_MONEY,  8, "NUMBER(19,4)",  fmtk_Emoney,   "lmoney",    "db_lmoney",  "db_lbasemoney",  "V_MONEY_FDEC", "numVal"  , "FT_LDMON"} },
        {"STRING" , {fmtt_STRING, 0, "VARCHAR2",      fmtk_Estring,  "char",      "char",       "char",           "V_STRING",     "string"  , "FT_STRING"} },
        {"SNR"    , {fmtt_SNR,    0, "VARCHAR2",      fmtk_Estring,  "char",      "char",       "char",           "V_STRING",     "string"  , "FT_SNR"} },
        {"DATE"   , {fmtt_DATE,   4, "DATE",          fmtk_Edate,    "bdate",     "bdate",      "bdate",          "V_DATE",       "date"    , "FT_DATE"} },
        {"TIME"   , {fmtt_TIME,   4, "DATE",          fmtk_Etime,    "btime",     "btime",      "btime",          "V_TIME",       "time"    , "FT_TIME"} },
        {"CHR"    , {fmtt_CHR,    1, "CHAR",          fmtk_Estring,  "char",      "char",       "char",           "V_BOOL",       "boolval" , "FT_CHR"} },
        {"UCHR"   , {fmtt_UCHR,   1, "RAW",           fmtk_Estring,  "char",      "char",       "char",           "V_STRING",     "string"  , "FT_UCHR"} },
        {"NUMERIC", {fmtt_NUMERIC,16,"NUMBER(32,12)", fmtk_Enumeric, "DBNumeric", "db_lmoney",  "db_lbasemoney",  "V_NUMERIC",    "decimal" , "FT_NUMERIC"} }
    };*/
    QFile jsonFile(":/FmtTypesMap.json");
    if (!jsonFile.open(QIODevice::ReadOnly))
        qCCritical(logCore()) << QString("Can't open fmt params file [%1]").arg(jsonFile.fileName());
    else
    {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(jsonFile.readAll(), &err);

        if (err.error != QJsonParseError::NoError)
        {
            qCCritical(logCore()) << QString("Error parse fmt params file [%1]: %2")
                                     .arg(jsonFile.fileName())
                                     .arg(err.errorString());
        }
        else
        {
            QJsonObject obj = doc.object();
            QJsonArray arr = obj["types"].toArray();

            for (auto item : arr)
            {
                QJsonObject element = item.toObject();
                QString display = element["display"].toString();

                FmtTypesList.append(display);

                FmtTypeInfo itemInfo;
                itemInfo._type = fmtTypesStrMap[element["type"].toString()];
                itemInfo._size = element["size"].toInt();
                itemInfo._oraType = element["oraType"].toString();
                itemInfo._pgType = element["pgType"].toString();
                itemInfo._indexType = indexTypesStrMap[element["indexType"].toString()];
                itemInfo._cppType = element["cppType"].toString();
                itemInfo._cppDbType = element["cppDbType"].toString();
                itemInfo._cppDbBaseType = element["cppDbBaseType"].toString();
                itemInfo._rslType = element["rslType"].toString();
                itemInfo._rslValueName = element["rslValueName"].toString();
                itemInfo._xmlTypeName = element["xmlTypeName"].toString();
                itemInfo._rsdType = element["rsdType"].toString();
                itemInfo._rsdConst = element["rsdConst"].toString();
                itemInfo._zeroConstant = element["zeroConstant"].toString();

                FmtTypesNames.push_back(display);
                FmtTypesMap.insert(display, itemInfo);
            }
        }
    }
}

bool fmtTypeCanHaveCustomSize(const qint16 &Type)
{
    bool hr = false;
    switch(Type)
    {
    case fmtt_STRING:
    case fmtt_SNR:
    case fmtt_CHR:
    case fmtt_UCHR:
        hr = true;
        break;
    }

    return hr;
}

bool hasTemporaryFlag(const qint32 &flag)
{
    return (flag & fmtnf_Temp) == fmtnf_Temp;
}

bool hasRecordFlag(const qint32 &flag)
{
    return (flag & fmtnf_Rec) == fmtnf_Rec;
}

QSettings *settings()
{
    FmtApplication *FmtApp = qobject_cast<FmtApplication*>(qApp);

    if (!FmtApp)
        return nullptr;

    return FmtApp->settings();
}

QStringList fmtTypes()
{
    return QStringList(FmtTypesList);
}

QString fmtTypeForId(const qint16 &id)
{
    QMapIterator<QString, FmtTypeInfo> iterator(FmtTypesMap);
    while(iterator.hasNext())
    {
        iterator.next();
        if (iterator.value()._type == id)
            return iterator.key();
    }

    return QString::number(id);
}

quint16 fmtTypeSize(const qint16 &Type)
{
    qint16 type = Type;
    if (type == fmtt_DATETIME)
        type = fmtt_DATE;

    QMapIterator<QString, FmtTypeInfo> iterator(FmtTypesMap);
    while(iterator.hasNext())
    {
        iterator.next();
        if (iterator.value()._type == Type)
            return iterator.value()._size;
    }
    return 0;
}

QString fmtOracleDecl(const qint16 &Type)
{
    return FmtTypesMap[fmtTypeForId(Type)]._oraType;
}

QString fmtPostgresDecl(const qint16 &Type)
{
    return FmtTypesMap[fmtTypeForId(Type)]._pgType;
}

QString fmtCppStructTypeName(const qint16 &Type)
{
    QString fldType = fmtTypeForId(Type);
    return FmtTypesMap[fldType]._cppType;
}

QString fmtCppStructDbTypeName(const qint16 &Type)
{
    QString fldType = fmtTypeForId(Type);
    return FmtTypesMap[fldType]._cppDbType;
}

QString fmtRslTypeName(const qint16 &Type)
{
    QString fldType = fmtTypeForId(Type);
    return FmtTypesMap[fldType]._rslType;
}

QString fmtRslValueName(const qint16 &Type)
{
    QString fldType = fmtTypeForId(Type);
    return FmtTypesMap[fldType]._rslValueName;
}

QString fmtCppStructDbBaseTypeName(const qint16 &Type)
{
    QString fldType = fmtTypeForId(Type);
    return FmtTypesMap[fldType]._cppDbBaseType;
}

quint32 fmtTypeIndexForId(const quint32 &id)
{
    QMapIterator<QString, FmtTypeInfo> iterator(FmtTypesMap);
    while(iterator.hasNext())
    {
        iterator.next();
        if (iterator.value()._type == id)
            return static_cast<quint32>(FmtTypesList.indexOf(iterator.key()));
    }

    return 0;
}

qint16 fmtIndexForType(const qint16 &id)
{
   /*QMapIterator<QString, FmtTypeInfo> iterator(FmtTypesMap);

    int indx = -1;
    while(iterator.hasNext())
    {
        iterator.next();
        FmtTypeInfo info = iterator.value();
        if (info._type == id)
        {
            indx = FmtTypesNames.indexOf(iterator.key());
            break;
        }
    }

    return static_cast<qint16>(indx);*/
    //return id;

    const QVector<int> ids =
    {
        fmtt_INT,
        fmtt_LONG,
        fmtt_BIGINT,
        fmtt_FLOAT,
        fmtt_DOUBLE,
        fmtt_MONEY,
        fmtt_STRING,
        fmtt_SNR,
        fmtt_DATE,
        fmtt_TIME,
        fmtt_CHR,
        fmtt_UCHR,
        fmtt_NUMERIC
    };

    return ids.indexOf(id);
}

QString fmtTypeNameForType(const qint16 &type)
{
    int index = static_cast<int>(fmtIndexForType(type));
    if (index < 0 || index >= FmtTypesList.size())
        return "Unknown";
    return FmtTypesList[fmtIndexForType(type)];
}

qint16 fmtTypeFromXmlType(const QString &type)
{
    QMapIterator<QString, FmtTypeInfo> iterator(FmtTypesMap);
    while(iterator.hasNext())
    {
        iterator.next();
        if (iterator.value()._xmlTypeName == type)
            return iterator.value()._type;
    }

    return fmtt_INT;
}

qint16 fmtTypeFromIndex(const qint16 &id)
{
    /*int index = static_cast<int>(fmtIndexForType(id));
    if (index < 0 || index >= FmtTypesList.size())
        return -1;*/
    return FmtTypesMap[FmtTypesList[id]]._type;
}

quint16 fmtTypeIndexSize(const qint16 &id)
{
    /*int index = fmtIndexForType(id);
    if (index < 0 || index >= FmtTypesList.size())
        return -1;*/
    return FmtTypesMap[FmtTypesList[id]]._size;
}

QString fmtRsdType(const qint16 &Type)
{
    int index = fmtIndexForType(Type);
    if (index < 0 || index >= FmtTypesList.size())
        return QString();

    return FmtTypesMap[FmtTypesList[index]]._rsdType;
}

QString fmtRsdConstant(const qint16 &Type)
{
    int index = fmtIndexForType(Type);
    if (index < 0 || index >= FmtTypesList.size())
        return QString();

    return FmtTypesMap[FmtTypesList[index]]._rsdConst;
}

QString fmtZeroConstant(const qint16 &Type)
{
    int index = fmtIndexForType(Type);
    if (index < 0 || index >= FmtTypesList.size())
        return QString();

    return FmtTypesMap[FmtTypesList[index]]._zeroConstant;
}

qint16 fmtIndexFromFmtType(const qint16 &id)
{
    QMapIterator<QString, FmtTypeInfo> iterator(FmtTypesMap);
    while(iterator.hasNext())
    {
        iterator.next();
        if (iterator.value()._type == id)
            return iterator.value()._indexType;
    }

    return 0;
}

bool fmtIsStringType(const qint16 &Type, const int &size)
{
    bool hr = false;
    if (Type == fmtt_STRING || Type == fmtt_SNR)
        hr = true;
    else if (Type == fmtt_CHR || Type == fmtt_UCHR)
    {
        if (size > 1)
            hr = true;
    }
    return hr;
}

QString fmtGetOraDefaultVal(const qint16 &Type, const int &size, bool escaped)
{
    QString t;
    if (fmtIsStringType(Type, size) && Type != fmtt_UCHR)
        t = "CHR(1)";
    else
    {
        switch(Type)
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
            if (escaped)
                t = "TO_DATE(''01/01/0001'', ''MM/DD/YYYY'')";
            else
                t = "TO_DATE('01/01/0001', 'MM/DD/YYYY')";
            break;
        case fmtt_TIME:
            if (escaped)
                t = "TO_DATE(''01/01/0001 00:00:00'',''MM/DD/YYYY HH24:MI:SS'')";
            else
                t = "TO_DATE('01/01/0001 00:00:00','MM/DD/YYYY HH24:MI:SS')";
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

QString fmtGetPgDefaultVal(const qint16 &Type, const int &size)
{
    QString t;
    if (fmtIsStringType(Type, size) && Type != fmtt_UCHR)
        t = "GLOB_FUNC.CHR(1)";
    else
    {
       switch(Type)
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
           t = "GLOB_FUNC.TO_TIMESTAMP_IMMUTABLE(''01/01/0001'', ''MM/DD/YYYY'')";
           break;
       case fmtt_TIME:
           t = "GLOB_FUNC.TO_TIMESTAMP_IMMUTABLE(''01/01/0001 00:00:00'',''MM/DD/YYYY HH24:MI:SS'')";
           break;
       case fmtt_CHR:
           t = "GLOB_FUNC.CHR(0)";
           break;
       case fmtt_UCHR:
           t = "DECODE(''00'', ''hex'')";
           break;
       }
    }

    return t;
}

qint16 fmtTypeFromCppType(const QString& typeName, quint16 size)
{
    // Сначала проверяем специальные случаи
    if (typeName == "bdate") return fmtt_DATE;
    if (typeName == "btime") return fmtt_TIME;
    if (typeName == "lmoney" || typeName == "db_lmoney") return fmtt_NUMERIC;
    if (typeName == "DBNumeric") return fmtt_NUMERIC;
    if (typeName == "QDateTime") return fmtt_DATETIME;

    // Для строковых типов учитываем размер
    if (typeName == "char") {
        if (size > 1) return fmtt_STRING;
        return fmtt_CHR; // или fmtt_UCHR, если нужно различать
    }

    // Проходим по всем типам из JSON
    for (const auto& typeInfo : FmtTypesMap)
    {
        if (typeInfo._cppType == typeName ||
                typeInfo._cppDbType == typeName ||
                typeInfo._cppDbBaseType == typeName)
        {
            return typeInfo._type;
        }
    }

    // Для числовых типов можно добавить дополнительные проверки
    if (typeName == "int16") return fmtt_INT;
    if (typeName == "int32") return fmtt_LONG;
    if (typeName == "int64_t") return fmtt_BIGINT;
    if (typeName == "float") return fmtt_FLOAT;
    if (typeName == "double") return fmtt_DOUBLE;

    // Если тип не найден, возвращаем STRING по умолчанию
    return fmtt_INT;
}

QString AddTabButtonCss()
{
    return "QToolButton         {image: url(':/img/addtab-icon.png'); border: 0; background: transparent; }"
           "QToolButton:hover   {image: url(':/img/addtab-icon-hover.png');}"
           "QToolButton:pressed {image: url(':/img/addtab-icon-pressed.png');} ";
}


QString PlusButtonCss()
{
    return "*         {image: url(':/img/plus-icon.png'); border: 0;}"
           "*:hover   {image: url(':/img/plus-icon-hover.png');}"
           "*:pressed {image: url(':/img/plus-icon-pressed.png');} ";
}

QString MinusButtonCss()
{
    return "*         {image: url(':/img/minus-icon.png'); border: 0;}"
           "*:hover   {image: url(':/img/minus-icon-hover.png');}"
           "*:pressed {image: url(':/img/minus-icon-pressed.png');} ";
}

QString CheckSymbol()
{
    return "✓";
}

QString CheckSymbolFromVariant(const bool &value)
{
    if (value)
        return CheckSymbol();
    return QString();
}

QString NullString(const int &index)
{
    QString v = "";
    if (index == keynullval_All)
        v = "All";
    else if (index == keynullval_Any)
        v = "Any";
    return v;
}

int ExecuteQuery(QSqlQuery *query, QString *err)
{
    return toolExecuteQuery(query, err);
}

int ExecuteQuery(const QString &query, QSqlDatabase db, QString *err)
{
    QSqlQuery q(db);
    q.prepare(query);
    return ExecuteQuery(&q, err);
}

int ExecuteQueryFile(const QString &queryFileName, QSqlDatabase db)
{
    QString content = ReadTextFileContent(queryFileName);

    if (content.isEmpty())
        return -1;

    return ExecuteQuery(content, db);
}

int trn(QSqlDatabase &db, std::function<int(void)> func)
{
    int stat = 0;

    if (!db.transaction())
    {
        qCWarning(logSql()) << QString("Begin transaction error: %1").arg(db.lastError().text());
        stat = 1;
    }

    if (!stat)
    {
        qCInfo(logSql()) << "Begin transaction";
        stat = func();
    }
    if (stat)
    {
        qCInfo(logSql()) << "Transaction rollback";
        db.rollback();
    }
    else
    {
        qCInfo(logSql()) << "Transaction commit";
        db.commit();
    }

    return stat;
}

QString DatasourceFromService(const QString &service)
{
    OracleTnsListModel *model = ((FmtApplication*)qApp)->getOracleTnsModel();
    QString str;

    if (model)
    {
        QModelIndexList lst = model->match(model->index(0, OracleTnsListModel::mtns_ServiceName), Qt::DisplayRole, service.simplified(), 10);
        QStringList values;
        QListIterator<QModelIndex> iter(lst);

        while(iter.hasNext())
        {
            QModelIndex element = iter.next();
            QModelIndex index = model->index(element.row(), OracleTnsListModel::mtns_Name);
            values.append(index.data().toString());
        }

        if (values.isEmpty())
            return service;

        if (values.size() == 1)
            return values.first();
        else
        {
            QWidget *parent = Q_NULLPTR;
            QWidgetList windows = qApp->topLevelWidgets();
            QListIterator<QWidget*> witer(windows);
            while(witer.hasNext())
            {
                QMainWindow *w = qobject_cast<QMainWindow*>(witer.next());

                if (w)
                {
                    parent = w;
                    break;
                }
            }

            str = QInputDialog::getItem(parent, QObject::tr("Выбор DSN"), QObject::tr("Сервису '%1' соответствуют DSN:").arg(service),
                                        values, 0, false, nullptr, Qt::Dialog);
        }
    }

    return OracleTnsListModel::clearString(str);
}
/*
 * Result codes:
 0 - OK
 1 - DataSource is not specified      2 - Invalid DBMS type.
 3 - Invalid connection string        4 - Btrieve DEF file is not specified
 5 - LOG file is not specified        6 - Invalid operation type
 7 - Invalid text coding              8 - Tables list are not specified
 9 - Invalid option              10 - Couldn't open response file

11 - Dictionary conversion warning   21 - Dictionary conversion error
12 - Data conversion warning         22 - Data conversion error
13 - Index creation warning          23 - Index creation error
14 - Btrieve warning                 24 - Btrieve error
 */
QString DbInitTextError(const qint16 &id)
{
    if (id < 0)
        return "Unknown status";

    QString error;
    QStringList lst;
    lst << "OK"
        << "DataSource is not specified"
        << "Invalid DBMS type"
        << "Invalid connection string"
        << "Btrieve DEF file is not specified"
        << "LOG file is not specified"
        << "Invalid operation type"
        << "Invalid text coding"
        << "Tables list are not specified"
        << "Invalid option"
        << "Couldn't open response file"
        << "Dictionary conversion warning"
        << "Data conversion warning"
        << "Index creation warning"
        << "Btrieve warning";

    QStringList lst_;
    lst_ << "Dictionary conversion error"
         << "Data conversion error"
         << "Index creation error"
         << "Btrieve error";

    if (id < lst.size())
        error = lst.at(id);
    else
    {
        error = lst_.at(id - 21);
    }

    return error;
}

QString BlobTypeToString(int type)
{
    switch (type)
    {
    case 0:
        return "BT_BLOB_NO";
    case 1:
        return "BT_BLOB_VAR";
    case 2:
        return "BT_BLOB_STREAM";
    case 3:
        return "BT_CLOB";
    }

    throw std::runtime_error(QObject::tr("Неизвестное значение типа блоба: %1")
                             .arg(type).toLocal8Bit().data());
}

QString BlobFieldString(int type)
{
    switch (type)
    {
    case 0:
        return "";
    case 1:
        return "T_FMTBLOBDATA_XXXX";
    case 2:
        return "T_FMTBLOBDATA_XXXX";
    case 3:
        return "T_FMTCLOBDATA_XXXX";
    }

    throw std::runtime_error(QObject::tr("Неизвестное значение типа блоба: %1")
                             .arg(type).toLocal8Bit().data());
}

QString BlobFieldTypeOraString(int type)
{
    switch (type)
    {
    case 0:
        return "";
    case 1:
        return "BLOB";
    case 2:
        return "BLOB";
    case 3:
        return "CLOB";
    }

    throw std::runtime_error(QObject::tr("Неизвестное значение типа блоба: %1")
                             .arg(type).toLocal8Bit().data());
}

QString BlobFieldTypePgString(int type)
{
    switch (type)
    {
    case 0:
        return "";
    case 1:
        return "BYTEA";
    case 2:
        return "BYTEA";
    case 3:
        return "TEXT";
    }

    throw std::runtime_error(QObject::tr("Неизвестное значение типа блоба: %1")
                             .arg(type).toLocal8Bit().data());
}

bool isBlobType(const QString &typeName)
{
    static QStringList lst = {"BT_BLOB_VAR", "BT_BLOB_STREAM", "BT_CLOB"};
    return lst.contains(typeName);
}

QString BoolToString(bool value)
{
    return value ? "true" : "false";
}

/*QColor GenerateColor()
{
    static double h = 0.0;
    static QColor accentColor("#217346");

    // Инициализация начального значения
    if (h == 0.0) {
        h = qrand() % 255 / 255.0;
    }

    // Константы для генерации гармоничных цветов
    static const double GOLDEN_RATIO = 0.618033988749895;
    static const double SATURATION = 0.6;
    static const double VALUE = 0.8;

    // Генерация нового оттенка
    h += GOLDEN_RATIO;
    double hue = fmod(h, 1.0);

    // Получение цвета из HSV
    QColor generatedColor = QColor::fromHsvF(hue, SATURATION, VALUE, 1.0);

    // Проверка контрастности с акцентным цветом
    QColor accent("#217346");

    // Вычисление относительной яркости (WCAG 2.1)
    auto luminance = [](const QColor& color) {
        double r = color.redF();
        double g = color.greenF();
        double b = color.blueF();

        r = (r <= 0.03928) ? r / 12.92 : pow((r + 0.055) / 1.055, 2.4);
        g = (g <= 0.03928) ? g / 12.92 : pow((g + 0.055) / 1.055, 2.4);
        b = (b <= 0.03928) ? b / 12.92 : pow((b + 0.055) / 1.055, 2.4);

        return 0.2126 * r + 0.7152 * g + 0.0722 * b;
    };

    double l1 = luminance(generatedColor);
    double l2 = luminance(accent);

    // Контрастное отношение
    double contrast = (qMax(l1, l2) + 0.05) / (qMin(l1, l2) + 0.05);

    // Если контраст недостаточен, корректируем яркость
    if (contrast < 3.0) {
        // Делаем цвет светлее или темнее для лучшего контраста
        if (l1 > l2) {
            // Сгенерированный цвет светлее акцентного
            QColor adjusted = generatedColor.lighter(120);
            if (luminance(adjusted) - l2 > 0.3) {
                return adjusted;
            }
        } else {
            // Сгенерированный цвет темнее акцентного
            QColor adjusted = generatedColor.darker(120);
            if (l2 - luminance(adjusted) > 0.3) {
                return adjusted;
            }
        }

        // Если корректировка не помогла, выбираем комплементарный цвет
        double complementaryHue = fmod(hue + 0.5, 1.0);
        return QColor::fromHsvF(complementaryHue, 0.7, 0.85, 1.0);
    }

    return generatedColor;
}*/
QColor GenerateColor(QObject *obj)
{
    return ConnectionColors->getColorForObject(obj);
    /*static int colorIndex = 0;

    // Базовый акцентный цвет (#217346)
    QColor accentBase("#217346");

    // Предопределённая палитра, гармонирующая с акцентным цветом
    static const QVector<QColor> colorPalette = {
        // Тёплые оттенки
        QColor("#2E8B57"), // Морская зелень (близкий к акцентному)
        QColor("#4682B4"), // Стальной синий
        QColor("#D2691E"), // Шоколадный
        QColor("#CD5C5C"), // Индийский красный
        QColor("#9370DB"), // Средний фиолетовый
        QColor("#20B2AA"), // Светлый морской волны
        QColor("#DAA520"), // Золотистый
        QColor("#6495ED"), // Васильковый
        QColor("#DC143C"), // Малиновый
        QColor("#008B8B"), // Тёмный бирюзовый

        // Дополнительные оттенки
        QColor("#4169E1"), // Королевский синий
        QColor("#8B4513"), // Коричневый седла
        QColor("#2F4F4F"), // Тёмный аспидно-серый
        QColor("#6A5ACD"), // Аспидно-синий
        QColor("#B22222")  // Кирпичный
    };

    // Увеличиваем индекс для следующего вызова
    colorIndex = (colorIndex + 1) % colorPalette.size();

    return colorPalette[colorIndex];*/


}

/*QColor GenerateColor()
{
    bool useAccentHarmony = true;
    static QList<QColor> history;
    static int callCount = 0;
    static const int HISTORY_SIZE = 12;

    // Используем константу золотого сечения для равномерного распределения
    const double goldenRatio = 0.618033988749895;
    static double currentHue = (qrand() % 360) / 360.0;

    // Основной акцентный цвет Office 2013
    static QColor accentColor("#217346");
    QColor accentHsv = accentColor.toHsv();
    double accentHue = accentHsv.hueF();
    double accentSaturation = accentHsv.saturationF();
    double accentValue = accentHsv.valueF();

    callCount++;

    // Генерируем новый оттенок с использованием золотого сечения
    currentHue = fmod(currentHue + goldenRatio, 1.0);

    // Если используем гармонию с акцентным цветом, корректируем оттенок
    if (useAccentHarmony) {
        // Избегаем слишком близких оттенков к акцентному
        double hueDiff = qAbs(currentHue - accentHue);
        hueDiff = qMin(hueDiff, 1.0 - hueDiff);

        if (hueDiff < 0.15) { // Минимум 15% расстояния в цветовом круге
            // Сдвигаем на гармоничное расстояние (триадная схема)
            currentHue = fmod(accentHue + 0.33, 1.0);
        }
    }

    // Определяем насыщенность и яркость на основе позиции в последовательности
    // Создаём мягкие, но выразительные цвета
    double saturation, value;

    // Используем разные схемы для разнообразия
    int pattern = callCount % 6;
    switch (pattern) {
    case 0: // Тёплые мягкие тона
        saturation = 0.55 + 0.15 * sin(callCount * 0.3);
        value = 0.82 + 0.08 * cos(callCount * 0.4);
        break;

    case 1: // Прохладные пастельные
        saturation = 0.50 + 0.10 * sin(callCount * 0.5);
        value = 0.78 + 0.10 * cos(callCount * 0.3);
        break;

    case 2: // Средняя насыщенность
        saturation = 0.60 + 0.10 * sin(callCount * 0.7);
        value = 0.85 + 0.05 * cos(callCount * 0.6);
        break;

    case 3: // Более глубокие тона
        saturation = 0.65 + 0.08 * sin(callCount * 0.9);
        value = 0.75 + 0.08 * cos(callCount * 0.5);
        break;

    case 4: // Светлые пастельные
        saturation = 0.45 + 0.12 * sin(callCount * 1.1);
        value = 0.88 + 0.06 * cos(callCount * 0.7);
        break;

    case 5: // Сбалансированные
    default:
        saturation = 0.58 + 0.12 * sin(callCount * 1.3);
        value = 0.80 + 0.07 * cos(callCount * 0.8);
        break;
    }

    // Ограничиваем значения для мягких, но не блеклых цветов
    saturation = qBound(0.45, saturation, 0.72);  // Мягкая, но заметная насыщенность
    value = qBound(0.72, value, 0.92);            // Яркие, но не ослепляющие

    // Создаём цвет
    QColor newColor = QColor::fromHsvF(currentHue, saturation, value, 1.0);

    // Для Office-стиля добавляем небольшую "приглушённость"
    // Преобразуем в RGB для коррекции яркости
    QColor rgbColor = newColor.toRgb();

    // Слегка снижаем контрастность для мягкого вида
    int r = rgbColor.red();
    int g = rgbColor.green();
    int b = rgbColor.blue();

    // Применяем мягкую коррекцию (делаем цвета более пастельными)
    const double softenFactor = 0.88; // Мягкое приглушение

    r = qBound(0, static_cast<int>(128 + (r - 128) * softenFactor), 255);
    g = qBound(0, static_cast<int>(128 + (g - 128) * softenFactor), 255);
    b = qBound(0, static_cast<int>(128 + (b - 128) * softenFactor), 255);

    newColor = QColor(r, g, b);

    // Проверяем на схожесть с предыдущими цветами
    bool isSimilar = false;
    for (const QColor& oldColor : history) {
        // Быстрая проверка по разности компонентов
        int diff = qAbs(newColor.red() - oldColor.red()) +
                   qAbs(newColor.green() - oldColor.green()) +
                   qAbs(newColor.blue() - oldColor.blue());

        // Более строгая проверка в HSV пространстве
        QColor newHsv = newColor.toHsv();
        QColor oldHsv = oldColor.toHsv();

        double hueDiff = qAbs(newHsv.hueF() - oldHsv.hueF());
        hueDiff = qMin(hueDiff, 1.0 - hueDiff);

        if (diff < 60 || // Быстрая проверка по RGB
            (hueDiff < 0.08 &&
             qAbs(newHsv.saturationF() - oldHsv.saturationF()) < 0.15 &&
             qAbs(newHsv.valueF() - oldHsv.valueF()) < 0.15)) {
            isSimilar = true;
            break;
        }
    }

    // Если цвет слишком похож на предыдущие, слегка корректируем
    if (isSimilar && history.size() > 0) {
        // Берём среднюю точку между текущим и наиболее отличающимся от него
        QColor mostDifferent;
        double maxHueDiff = 0;

        for (const QColor& oldColor : history) {
            QColor oldHsv = oldColor.toHsv();
            QColor newHsv = newColor.toHsv();

            double hueDiff = qAbs(newHsv.hueF() - oldHsv.hueF());
            hueDiff = qMin(hueDiff, 1.0 - hueDiff);

            if (hueDiff > maxHueDiff) {
                maxHueDiff = hueDiff;
                mostDifferent = oldColor;
            }
        }

        if (maxHueDiff > 0.1) {
            // Смешиваем с наиболее отличающимся цветом
            QColor mostDifferentHsv = mostDifferent.toHsv();
            double newHue = (currentHue + mostDifferentHsv.hueF()) / 2.0;
            newHue = fmod(newHue, 1.0);

            // Немного увеличиваем насыщенность для выразительности
            double newSat = qMin(0.7, (saturation + mostDifferentHsv.saturationF()) / 2.0 + 0.05);
            double newVal = qBound(0.75, (value + mostDifferentHsv.valueF()) / 2.0, 0.9);

            newColor = QColor::fromHsvF(newHue, newSat, newVal, 1.0);
        } else {
            // Просто сдвигаем оттенок
            currentHue = fmod(currentHue + 0.25, 1.0);
            newColor = QColor::fromHsvF(currentHue, saturation * 0.9, value * 0.95, 1.0);
        }
    }

    // Преобразуем для Office-стиля (немного приглушаем если нужно)
    if (useAccentHarmony) {
        // Делаем цвета гармоничными с акцентным
        QColor finalRgb = newColor.toRgb();

        // Создаём тёплую или холодную гамму в зависимости от акцентного цвета
        bool accentIsWarm = (accentHue < 0.33 || accentHue > 0.66); // Примерная граница

        QColor finalHsv = finalRgb.toHsv();
        double finalHue = finalHsv.hueF();

        // Корректируем цветовую температуру
        if (accentIsWarm && (finalHue > 0.5 && finalHue < 0.8)) {
            // Если акцент тёплый, а цвет холодный - немного согреваем
            finalHue = fmod(finalHue - 0.1, 1.0);
        } else if (!accentIsWarm && (finalHue < 0.33 || finalHue > 0.66)) {
            // Если акцент холодный, а цвет тёплый - немного охлаждаем
            finalHue = fmod(finalHue + 0.1, 1.0);
        }

        // Слегка снижаем насыщенность для гармонии
        double finalSat = qBound(0.4, finalHsv.saturationF() * 0.92, 0.68);
        double finalVal = qBound(0.76, finalHsv.valueF(), 0.88);

        newColor = QColor::fromHsvF(finalHue, finalSat, finalVal, 1.0);
    }

    // Добавляем в историю
    history.append(newColor);
    if (history.size() > HISTORY_SIZE) {
        history.removeFirst();
    }

    // Дополнительная коррекция для очень ярких цветов
    if (newColor.lightness() > 220) {
        newColor = newColor.darker(110); // Слегка затемняем
    }

    return newColor;
}*/

QString FmtTableSqlText(FmtTable *pTable)
{
    QString str = QString("-- Таблица %1\n").arg(pTable->name().toUpper());
    str += pTable->generateCreateTableSql() + ";\n\n";

    QString Comment1 = pTable->getCommentSql();
    if (!Comment1.isEmpty())
        str += Comment1 + ";\n";

    for(int i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *f = pTable->field(i);
        QString Comment2 = f->getCommentSql();

        if (!Comment2.isEmpty())
            str += Comment2 + ";\n";
    }

    return str;
}

void SaveFmtTableSql(QSharedPointer<FmtTable> pTable, QWidget *parent, const QString &icon)
{
    SelectFolderDlg folder(RsCreateSqlContext, parent);

    if (!icon.isEmpty())
        folder.setWindowIcon(QIcon::fromTheme(icon));

    if (folder.exec() == QDialog::Accepted)
    {
        QString fileName = QString("%1.sql").arg(pTable->name().toUpper());
        QDir dir = folder.selectedPath();

        if (dir.exists())
        {
            QFile file(dir.absoluteFilePath(fileName));
            if (file.open(QIODevice::WriteOnly))
            {
                QFileInfo info(fileName);
                QTextStream stream(&file);
                stream.setCodec("IBM 866");
                stream << FmtTableSqlText(pTable.data());
                file.close();
            }
        }
    }
}

void ExportFmtToXml(ConnectionInfo *connection, const QStringList &files, const QString &dir, bool ShowProgress, bool ShowReport, QWidget *parent)
{
    ConnectionInfo *current = connection;

    if (!current)
        return;

    FmtImpExpWrp imp(current, parent);

    QProgressDialog dlg(QObject::tr("Выгрузка в xml файл"), QObject::tr("Прервать"), 0, 0, parent);
    QEventLoop loop;

    if (ShowProgress)
    {
        dlg.open(&imp, SIGNAL(finished()));
        QObject::connect(&dlg, SIGNAL(canceled()), &imp, SLOT(cancel()));
        QObject::connect(&dlg, SIGNAL(canceled()), &loop, SLOT(quit()));
    }

    QObject::connect(&imp, SIGNAL(finished(int)), &loop, SLOT(quit()));

    imp.addTable(files);
    imp.exportTable(dir);

    if (imp.isRunning())
    {
        loop.exec();

        if (ShowReport)
        {
            ErrorsModel log;
            imp.parseProtocol(&log);
            ErrorDlg edlg(ErrorDlg::ModeInformation, parent);
            edlg.setErrors(&log);
            edlg.setMessage(QObject::tr("Протокол выгрузки в xml файл: "));
            edlg.exec();
        }
    }
}

qint16 InitFmtTableExec(FmtTable *pTable, QString *err)
{
    qint16 stat = 0;
    QTemporaryFile tmp;

    if (!tmp.open()) {
        if (err) *err = "Не удалось создать временный файл";
        return -1;
    }

    QString errorMessage;
    stat = pTable->dbInit(tmp.fileName(), &errorMessage);

    // Пытаемся прочитать файл
    tmp.seek(0);
    QByteArray logcontent = tmp.readAll();
    tmp.close();

    QString decodedLog = toolDecodeRussianText(logcontent);

    // Объединяем сообщение об ошибке из dbInit и содержимое лога
    QString fullError;
    if (!errorMessage.isEmpty()) {
        fullError = errorMessage;
        if (!decodedLog.isEmpty() && decodedLog != errorMessage) {
            fullError += "\n" + decodedLog;
        }
    } else {
        fullError = decodedLog;
    }

    if (err) {
        *err = fullError;
    }

    return stat;
}

void InitFmtTable(FmtTable *pTable, QWidget *parent, const QString &icon)
{
    DbInitDlg dlg(pTable, parent);

    if (!icon.isEmpty())
        dlg.setWindowIcon(QIcon::fromTheme(icon));

    int stat = 0;
    if (dlg.exec() == QDialog::Accepted)
    {
        QProgressDialog progress(QObject::tr("Инициализация таблицы"), QObject::tr("Отмена"), 0, 0, parent);
        progress.open();
        if (dlg.getCreateTableFlag() /*&& !pTable->isTemporary()*/ && !pTable->isRecord())
        {
            QString err;
            stat = pTable->createDbTable(&err);

            if (stat)
            {
                QMessageBox msg(parent);
                msg.setWindowTitle(QObject::tr("Ошибка"));
                msg.setIcon(QMessageBox::Critical);
                msg.setText(QObject::tr("Не удалось выполнить команду <b>CREATE TABLE</b>"));
                msg.setDetailedText(err);
                msg.exec();
            }
            QApplication::processEvents();
        }

        if (!stat && dlg.getCreteIndexFlag())
        {
            QString err;
            stat = InitFmtTableExec(pTable, &err);
            QApplication::processEvents();

            QMessageBox msg(parent);

            if (stat)
            {
                msg.setWindowTitle(QObject::tr("Ошибка"));
                msg.setIcon(QMessageBox::Critical);
                msg.setText(QObject::tr("<b>DbInit вернул код ошибки [%1]: %2</b>").arg(stat).arg(DbInitTextError(stat)));
            }
            else
            {
                msg.setWindowTitle(QObject::tr("Результат выполнения"));
                msg.setIcon(QMessageBox::Information);
                msg.setText(QObject::tr("<b>DbInit завершился с кодом [%1]: %2</b>").arg(stat).arg(DbInitTextError(stat)));
            }

            if (err.size())
            {
                msg.setDetailedText(err);
                QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
                QGridLayout* layout = (QGridLayout*)msg.layout();
                layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
            }
            msg.exec();


        }
        progress.close();
    }
}

QString ConfigOraFilePath()
{
    QDir appDir = qApp->applicationDirPath();
    return appDir.absoluteFilePath("OracleAuthDlg.cfg");
}

QString FmtGetTableExtension(const QString &table)
{
    int pos = table.lastIndexOf('_');
    QString str = table.mid(pos + 1);
    if (str.length() != 3)
        str = "";

    return str.toLower();
}

QString FmtGetTableFileName(const QString &table)
{
    QString filename = QString("%1.%2")
            .arg(FmtTableStructName(table))
            .arg(FmtGetTableExtension(table));

    return filename.toLower();
}

QString FmtTableStructName(const QString &table)
{
    QString StructName = table.toUpper();
    QString ext = FmtGetTableExtension(table);

    if (StructName[0] == 'D')
        StructName = StructName.mid(1);

    if (!ext.isEmpty())
    {
        StructName = StructName.remove(StructName.length() - ext.length() - 1, ext.length() + 1).toUpper();
    }

    return StructName;
}

QStringList FmtTableStringList(const QString &table)
{
    if (table.isEmpty())
        return QStringList();

    QStringList lst;
    QString StructName = FmtTableStructName(table);

    lst << StructName;
    lst << table.toUpper();
    lst << table.toLower();

    lst << QString("File%1").arg(StructName);
    lst << QString("%1_KEYS").arg(StructName);
    lst << QString("Find%1").arg(StructName);
    lst << QString("iOpen%1").arg(StructName);
    lst << QString("SKF_%1").arg(StructName);

    return lst;
}

QString GetProcessErrorText(const QProcess::ProcessError &error)
{
    QString errText;
    switch(error)
    {
    case QProcess::FailedToStart:
        errText = "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program";
        break;
    case QProcess::Crashed:
        errText = "The process crashed some time after starting successfully";
        break;
    case QProcess::Timedout:
        errText = "The process timedout";
        break;
    case QProcess::WriteError:
        errText = "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel";
        break;
    case QProcess::ReadError:
        errText = "An error occurred when attempting to read from the process. For example, the process may not be running";
        break;
    case QProcess::UnknownError:
        errText = "An unknown error occurred";
        break;
    }

    return errText;
}

QString ProcessStateText(qint16 State)
{
    QString result;
    switch(State)
    {
    case QProcess::Starting:
        result = "The process is starting, but the program has not yet been invoked.";
        break;
    case QProcess::Running:
        result = "The process is running and is ready for reading and writing.";
        break;
    default:
        result = "The process is not running.";
    }

    return result;
}

QString ProcessExitStatusText(qint16 State)
{
    QString result;
    switch(State)
    {
    case QProcess::CrashExit:
        result = "The process crashed.";
        break;
    default:
        result = "The process exited normally.";
    }

    return result;
}

int CoreStartProcess(QProcess *exe, const QString &program, const QStringList& arguments, bool waitForFinished, bool waitForStarted, int timeout, bool waitForReadyRead)
{
    int stat = 0;
    qCInfo(logCore()) << "Process: " << exe;
    qCInfo(logCore()) << "Executable path:" << program;
    qCInfo(logCore()) << "Working directory:" << exe->workingDirectory();
    qCInfo(logCore()) << "Arguments" << arguments;

    QObject::connect(exe, &QProcess::stateChanged, [&exe](QProcess::ProcessState newState)
    {
        qCInfo(logCore()) << QString("Process state changed to: %1 (%2)")
                             .arg(newState).arg(ProcessStateText(newState));
    });
    QObject::connect(exe, &QProcess::errorOccurred, [&exe,&stat](QProcess::ProcessError error)
    {
        qCInfo(logCore()) << QString("Process error occurred: %1 (%2)")
                             .arg(error).arg(GetProcessErrorText(error));
        stat = -1;
    });
    exe->start(program, arguments);

    if (waitForStarted)
        exe->waitForStarted();

    if (waitForReadyRead)
        exe->waitForReadyRead();

    if (waitForFinished)
    {
        exe->waitForFinished(timeout);

        if (!stat)
        {
            stat = exe->exitCode();
            qCInfo(logCore()) << "Process exit code: " << stat;
        }
        int status= exe->exitStatus();
        qCInfo(logCore()) << QString("Process exit status: %1 (%2)").arg(status).arg(ProcessExitStatusText(status));
    }

    return stat;
}


QString GetVersionNumberString()
{
    QString versionNumberString;
    char moduleName[MAX_PATH + 1];
    GetModuleFileNameA(NULL, moduleName, MAX_PATH);

    DWORD dummyZero;
    DWORD versionSize = GetFileVersionInfoSizeA(moduleName, &dummyZero);

    if(!versionSize)
        return QString();

    void* pVersion = malloc(versionSize);
    if(!pVersion)
        return QString();

    if(!GetFileVersionInfoA(moduleName, NULL, versionSize, pVersion))
    {
        free(pVersion);
        return QString();
    }

    UINT length;
    VS_FIXEDFILEINFO* pFixInfo;
    if (VerQueryValueA(pVersion, "\\", (LPVOID*)&pFixInfo, &length))
    {
        versionNumberString = QString("%1.%2.%3.%4")
                .arg((pFixInfo->dwFileVersionMS >> 16) & 0xffff)
                .arg((pFixInfo->dwFileVersionMS >>  0) & 0xffff)
                .arg((pFixInfo->dwFileVersionLS >> 16) & 0xffff)
                .arg((pFixInfo->dwFileVersionLS >>  0) & 0xffff);
    }
    free(pVersion);

    return versionNumberString;
}

QString ConstrType1RegExp()
{
    return "(CONNSTRING\\s*\\=\\s*)*dsn\\s*\\=\\s*(\\w+)\\s*\\;\\s*user id\\s*\\=\\s*(\\w+)\\s*\\;\\s*password\\s*\\=\\s*(\\w+)";
}

bool ParseConnectionString(const QString &connString, QString &user, QString &pass, QString &service)
{
    bool hr = false;
    QString constrtype1regexp = ConstrType1RegExp();

    QRegularExpression rexpr(constrtype1regexp);
    QRegularExpressionMatch match = rexpr.match(connString);

    hr = match.hasMatch();
    if (hr)
    {
        enum
        {
            match_dsn = 2,
            match_user,
            match_pswd,
        };

        service = match.captured(match_dsn);
        user = match.captured(match_user);
        pass = match.captured(match_pswd);
    }

    return hr;
}

QString ReadTextFileContent(const QString &filename)
{
    QString content;
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        content = f.readAll();
        f.close();
    }
    return content;
}

static bool isSqliteObjectExists(QSqlDatabase &source, const QString &type, const QString &table)
{
    bool hr = false;
    QSqlQuery q(source);
    q.prepare("SELECT COUNT(*) FROM sqlite_master WHERE type = ? AND name = ?");
    q.bindValue(0, type);
    q.bindValue(1, table);

    if (!ExecuteQuery(&q))
    {
        if (q.next() && q.value(0).toInt())
            hr = true;
    }

    return hr;
}

static int CountRecordsInTable(const QString &table, QSqlDatabase &source)
{
    int count = 0;
    QSqlQuery q(source);
    q.prepare(QString("select count(*) from %1").arg(table));
    if (!ExecuteQuery(&q))
    {
        if (q.next())
            count = q.value(0).toInt();
    }
    return count;
}

static int CloneFmtCopyTable(QSqlDatabase &source, QSqlDatabase &dest, QProgressDialog &dlg, const QString &table)
{
    int stat = 0;
    int i = 0;
    int fields = 0;
    int namsecount = CountRecordsInTable(table, source);
    dlg.setMaximum(namsecount);
    dlg.setMinimum(0);

    QString mask;
    QSqlQuery namesquery(source);
    namesquery.prepare(QString("select * from %1").arg(table));
    stat = ExecuteQuery(&namesquery);

    while (namesquery.next() && !stat)
    {
        if (mask.isEmpty())
        {
            fields = namesquery.record().count();
            for (int k = 0; k < fields; k++)
            {
                if (k == 0) mask = "?";
                else mask += ",?";
            }
            if (mask.isEmpty()) stat = 1;
        }

        if (!stat)
        {
            dlg.setValue(++ i);
            stat = dlg.wasCanceled();
            dlg.setLabelText(QObject::tr("Копирование таблицы %3: %1 из %2...").arg(i).arg(namsecount).arg(table));
            qApp->processEvents();

            if (!stat)
            {
                QSqlQuery insert(dest);
                insert.prepare(QString("insert into %1 values(%2)").arg(table).arg(mask));
                for (int j = 0; j < fields; j++)
                    insert.bindValue(j, namesquery.value(j));
                stat = ExecuteQuery(&insert);
            }
        }
    }
    return stat;
}

int CloneFmtFromConnection(QSqlDatabase &source, QSqlDatabase &dest, QWidget *parent)
{
    QProgressDialog dlg(parent);
    dlg.setLabelText(QObject::tr("Подготовка к выгрузке словаря в sqlite..."));
    dlg.open();

    return trn(dest, [&source,&dest,&dlg]()
    {
        int stat = 0;
        if (!isSqliteObjectExists(dest, "table", "FMT_NAMES"))
        {
            dlg.setLabelText(QObject::tr("Создание таблицы FMT_NAMES..."));
            qApp->processEvents();
            stat = ExecuteQueryFile(":/fmt_names", dest);
            if (!stat) stat = ExecuteQueryFile(":/fmt_names_index", dest);
        }
        else
            stat = ExecuteQuery("DELETE FROM FMT_NAMES", dest);

        if(!stat)
        {
            if (!isSqliteObjectExists(dest, "table", "FMT_FIELDS"))
            {
                dlg.setLabelText(QObject::tr("Создание таблицы FMT_FIELDS..."));
                qApp->processEvents();
                stat = ExecuteQueryFile(":/fmt_fields", dest);
                if (!stat) stat = ExecuteQueryFile(":/fmt_field_index", dest);
            }
            else
                stat = ExecuteQuery("DELETE FROM FMT_FIELDS", dest);
        }

        if (!stat)
        {
            if (!isSqliteObjectExists(dest, "table", "FMT_KEYS"))
            {
                dlg.setLabelText(QObject::tr("Создание таблицы FMT_KEYS..."));
                qApp->processEvents();
                stat = ExecuteQueryFile(":/fmt_keys", dest);
                if (!stat) stat = ExecuteQueryFile(":/fmt_keys_index", dest);
            }
            else
                stat = ExecuteQuery("DELETE FROM FMT_KEYS", dest);
        }

        if (!stat)
            stat = CloneFmtCopyTable(source, dest, dlg, "FMT_NAMES");

        if (!stat)
            stat = CloneFmtCopyTable(source, dest, dlg, "FMT_FIELDS");

        if (!stat)
            stat = CloneFmtCopyTable(source, dest, dlg, "FMT_KEYS");

        return stat;
    });
}

bool CheckConnectionType(ConnectionInfo *pInfo, const int &Type, bool ShowMsg, QWidget *parent)
{
    bool hr = true;
    if (pInfo->type() != Type)
    {
        hr = false;

        if (ShowMsg)
            QMessageBox::information(parent, QObject::tr("Информация"), QObject::tr("Это действие не доступно для данного вида подключения"));
    }
    return hr;
}

bool CheckConnectionType(ConnectionInfo *pInfo, const QList<int> &Type, bool ShowMsg, QWidget *parent)
{
    bool hr = true;
    if (!Type.contains(pInfo->type()))
    {
        hr = false;

        if (ShowMsg)
            QMessageBox::information(parent, QObject::tr("Информация"), QObject::tr("Это действие не доступно для данного вида подключения"));
    }
    return hr;
}

QString FmtCapitalizeField(const QString &undecoratedfield, bool force)
{
    if (!settings())
        return undecoratedfield;

    if (!settings()->value("AutoCamelCase", true).toBool() && !force)
        return undecoratedfield;

    QString result = undecoratedfield;
    QScopedPointer<QProcess> proc(new QProcess());
    proc->setProgram(toolFullFileNameFromDir("CapitalizeField.exe"));

    QStringList args;
    args << "--field" << undecoratedfield;
    proc->setArguments(args);

    CoreStartProcess(proc.data(), proc->program(), proc->arguments());
    proc->waitForFinished();
    result = proc->readAllStandardOutput().simplified();

    if (result.isEmpty())
        result = undecoratedfield;

    return result;
}

QStringList FmtCapitalizeField(const QStringList &undecoratedfield, bool force)
{
    QStringList result = undecoratedfield;
    if (!settings()->value("AutoCamelCase", true).toBool() && !force)
        return result;

    QDir d = QDir::current();
    QScopedPointer<QProcess> proc(new QProcess());
    proc->setProgram(d.absoluteFilePath("CapitalizeField.exe"));

    QStringList args;
    args << "--fieldlist" << result.join(";");
    proc->setArguments(args);

    CoreStartProcess(proc.data(), proc->program(), proc->arguments());
    proc->waitForFinished();

    QString str = proc->readAllStandardOutput().simplified();
    result = str.split(";");

    return result;
}

void StartUnloadDbf(ConnectionInfo *current, const QString &table, QWidget *parent, QSettings *settings)
{
    ErrorDlg dlg(ErrorDlg::ModeWidget, parent);
    dlg.setMessage(QObject::tr("Экспорт содержимого таблицы %1").arg(table));
    dlg.setWindowModality(Qt::WindowModal);

    bool UseOld = false;
    int DefaultAction = 0;
    if (settings)
    {
        settings->beginGroup(RsExportDatContext);
        UseOld = settings->value("UseRSexp", false).toBool();
        DefaultAction = settings->value("DefaultAction", 0).toInt();
        settings->endGroup();

        int size = settings->beginReadArray(RsExportDatContext);
        for (int i = 0; i < size; i++)
        {
            settings->setArrayIndex(i);
            int action = settings->value("action").toInt();
            QString settings_table = settings->value("table").toString();

            if (!settings_table.trimmed().compare(table, Qt::CaseInsensitive))
            {
                DefaultAction = action;
                break;
            }
        }
        settings->endArray();
    }

    FmtDbfToolWrp wrp(current, parent);
    dlg.setErrors(wrp.errorsModel());
    wrp.setDsn(current->dsn());

    QObject::connect(&dlg, SIGNAL(canceled()), &wrp, SLOT(stop()));
    QObject::connect(&wrp, SIGNAL(started()), &dlg, SLOT(exec()));
    QObject::connect(&wrp, SIGNAL(startError()), &dlg, SLOT(exec()));

    SelectFolderDlg folder(RsExpUnlDirContext, parent);
    folder.setWindowIcon(QIcon::fromTheme("UnloadDbf"));
    if (folder.exec() == QDialog::Accepted)
        wrp.unload(folder.selectedPath(), table, DefaultAction, UseOld);
}

void StartLoadDbf(ConnectionInfo *current, const QString &table, QWidget *parent)
{
    ErrorDlg dlg(ErrorDlg::ModeWidget, parent);
    dlg.setMessage(QObject::tr("Экспорт содержимого таблицы %1").arg(table));
    dlg.setWindowModality(Qt::WindowModal);

    FmtDbfToolWrp wrp(current, parent);
    dlg.setErrors(wrp.errorsModel());
    wrp.setDsn(current->dsn());

    QObject::connect(&dlg, SIGNAL(canceled()), &wrp, SLOT(stop()));
    QObject::connect(&wrp, SIGNAL(started()), &dlg, SLOT(exec()));
    QObject::connect(&wrp, SIGNAL(startError()), &dlg, SLOT(exec()));
    wrp.load(table);
}

void StartLoadDbfSelectFile(ConnectionInfo *current, const QString &table, QWidget *parent)
{
    SelectFolderDlg folder(RsExpUnlDirContext, parent);
    folder.setWindowIcon(QIcon::fromTheme("LoadDbf"));
    if (folder.exec() == QDialog::Accepted)
    {
        QString file = QString("%1.dat").arg(table.toUpper());
        QDir selectedFolder = folder.selectedPath();

        if (selectedFolder.exists(file))
            StartLoadDbf(current, selectedFolder.absoluteFilePath(file), parent);
        else
            QMessageBox::critical(parent, QObject::tr("Ошибка"), QObject::tr("Файл <b>%1</b> не найден в выбранном каталоге.").arg(file));
    }
}

int SelectTableFieldsDlg(FmtTable *pTable, const QString &title, QList<FmtField*> *pFldList, QWidget *parent, QWidget *userwidget, const QString icon)
{
    int stat = 0;

    SelectFieldsModel selFldModel(pTable, parent);
    SelectFilteredDlg dlg(parent);
    dlg.setWindowTitle(title);

    if (!icon.isEmpty())
        dlg.setWindowIcon(QIcon::fromTheme(icon));

    dlg.setFilteredModel(&selFldModel);
    dlg.setHidenColumns(QList<int>()
                        << FmtFildsModel::fld_Size
                        << FmtFildsModel::fld_DbName
                        << FmtFildsModel::fld_Custom
                        << FmtFildsModel::fld_Offset
                        << FmtFildsModel::fld_Outlen
                        << FmtFildsModel::fld_Decpoint
                        << FmtFildsModel::fld_Hidden
                        << FmtFildsModel::fld_Id
                        << FmtFildsModel::fld_FmtId
                        << FmtFildsModel::fld_TypeIndex);
    dlg.setColumnWidth(FmtFildsModel::fld_Name, 150);

    if (userwidget)
        ((QVBoxLayout*)dlg.layout())->insertWidget(0, userwidget);

    QPushButton *allSelect = dlg.dialogButtonBox()->addButton(QObject::tr("Выбрать всё"), QDialogButtonBox::ActionRole);
    QPushButton *allDeSelect = dlg.dialogButtonBox()->addButton(QObject::tr("Снять всё"), QDialogButtonBox::ActionRole);

    selFldModel.checkFields(*pFldList);
    QObject::connect(&dlg, SIGNAL(textChanged(QString)), &selFldModel, SLOT(setFilterFieldName(QString)));
    QObject::connect(allSelect, SIGNAL(clicked(bool)), &selFldModel, SLOT(selectAll()));
    QObject::connect(allDeSelect, SIGNAL(clicked(bool)), &selFldModel, SLOT(deselectAll()));

    stat = dlg.exec();

    if (pFldList && stat == QDialog::Accepted)
        *pFldList = selFldModel.checkedFields();

    return stat;
}

int SelectTableFieldsDlg(QSharedPointer<FmtTable> pTable, const QString &title, QList<FmtField*> *pFldList, QWidget *parent, QWidget *userwidget, const QString icon)
{
    return SelectTableFieldsDlg(pTable.data(), title, pFldList, parent, userwidget, icon);
}

void readCSVRow(const QString &row, QVector<QString> &fields, const QChar &quote)
{
    enum class CSVState : quint8
    {
        UnquotedField,
        QuotedField,
        QuotedQuote
    };

    CSVState state = CSVState::UnquotedField;
    qsizetype i = 0;
    fields.push_back("");

    for (QChar c : row)
    {
        switch (state)
        {
        case CSVState::UnquotedField:
            if (c == ',')
            {
                fields.append("");
                ++i;
            }
            else if (c == quote)
                state = CSVState::QuotedField;
            else
                fields[i].push_back(c);
            break;
        case CSVState::QuotedField:
            if (c == quote)
                state = CSVState::QuotedQuote;
            else
                fields[i].push_back(c);
            break;
        case CSVState::QuotedQuote:
            if (c == ',')
            {
                fields.push_back("");
                i++;
                state = CSVState::UnquotedField;
            }
            else if (c == quote)
            {
                fields[i].push_back('"');
                state = CSVState::QuotedField;
            }
            else
                state = CSVState::UnquotedField;
            break;
        }
    }
}

QString getFullFileNameFromDir(const QString &file)
{
    QDir dir = QDir::current();
    QString fullfilename = dir.absoluteFilePath(file);

    if (QFile::exists(fullfilename))
        return fullfilename;

    dir = QDir(QApplication::applicationDirPath());
    fullfilename = dir.absoluteFilePath(file);

    if (QFile::exists(fullfilename))
        return fullfilename;

    return QString();
}

QString FmtGenDiffToScript(const QString &filename,
                           const QString &connectionString,
                           bool IsUnicodeDb,
                           QString *err)
{
    QString result;

    QStringList args{"diff", filename};
    QProcess process;
    CoreStartProcess(&process, "svn", args, true, true);

    auto GetResult = [=](const QString &filename) -> QString
    {
        QString result;
        QFile f(filename);

        if (f.open(QIODevice::ReadOnly))
        {
            QTextStream ostream(&f);
            ostream.setCodec("IBM 866");
            ostream.seek(0);

            result = ostream.readAll();
            f.close();
        }

        return result;
    };

    QByteArray errdata = process.readAllStandardError();
    if (!errdata.isEmpty())
    {
        *err = QString::fromLocal8Bit(errdata);
        return QString();
    }
    else
    {
        QTemporaryFile tmp;
        QTemporaryFile resultfile;
        tmp.open();
        resultfile.open();

        QByteArray diff = process.readAllStandardOutput();
        tmp.write(diff);
        tmp.close();
        resultfile.close();

        args = QStringList
        {
            "--ora",
            "--input",
            tmp.fileName(),
            "--output",
            resultfile.fileName(),
            "--delete",
            "--insert",
            "--update",
            "--cs",
            connectionString,
            "--dat",
            QDir::toNativeSeparators(filename)
        };

        if (IsUnicodeDb)
            args.append("--unicodedb");

        CoreStartProcess(&process, "DiffToScript.exe", args, true, true);

        result = GetResult(resultfile.fileName());
    }

    return result;
}

void CreateUserCommandsMenu(QMenu **menu, const QString &title,
                            QObject *receiver, const char *slot)
{
    FmtApplication *app = qobject_cast<FmtApplication*>(qApp);

    if (!app)
        return;

    QSettings *settings = app->settings();

    QScopedPointer<ToolBarsStorage> pStorage(new ToolBarsStorage());
    settings->beginGroup("UserCommands");
    QByteArray data = QByteArray::fromBase64(settings->value("UserActions").toString().toLocal8Bit());
    settings->endGroup();

    if (!data.isEmpty())
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject root = doc.object();

        QJsonArray actions = root["UserActions"].toArray();
        pStorage->load(actions);

        if (pStorage->count())
        {
            *menu = new QMenu(title);
            for (int i = 0; i < pStorage->count(); i++)
            {
                ToolBarsAction *info = pStorage->action(i);

                QAction *action = (*menu)->addAction(QIcon(info->icon), info->title);
                action->setData(info->macrofile);
                action->setToolTip(info->title);

                QObject::connect(action, SIGNAL(triggered()), receiver, slot);
            }
        }
    }
}

// ------------------------------------------------------------
class StartGuiconverterRestoreProperty : public QRunnable
{
public:
    StartGuiconverterRestoreProperty(const QByteArray &data, const QString &filename)
    {
        _data = data;
        _filename = filename;

        setAutoDelete(true);
    }

    void run() Q_DECL_OVERRIDE
    {
        QThread::sleep(5);

        QFile Property(_filename);
        if (Property.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            Property.write(_data);
            Property.close();
        }
    }

private:
    QByteArray _data;
    QString _filename;
};

int StartGuiconverter(const QString &userScheme,
                      const QString &userPwd,
                      const QString &dbName,
                      const QString &dsn,
                      const QString &userNs,
                      const QString &indxNs,
                      const QString &sysScheme,
                      const QString &sysPwd,
                      const QString &ipAddr,
                      QString *err)
{
    FmtApplication *app = qobject_cast<FmtApplication*>(qApp);
    QSettings *settings = app->settings();

    settings->beginGroup("GuiConverter");
    QString path = settings->value("path").toString();
    QString jdkBinPath = settings->value("jdkBinPath").toString();
    settings->endGroup();

    if (path.isEmpty())
    {
        if (err)
            *err = QObject::tr("Не задан путь к GuiConverter.exe");

        return -1;
    }

    QByteArray backup;
    QDir dir(path);

    QFile PropertyBackup(dir.absoluteFilePath("Property.json"));
    if (PropertyBackup.open(QIODevice::ReadOnly))
    {
        backup = PropertyBackup.readAll();
        PropertyBackup.close();
    }
    else
    {
        if (err)
            *err = QObject::tr("Не найден <b>Property.json</b>");

        return -1;
    }

    QJsonObject prop;
    QJsonObject root;
    prop.insert("userNs", userNs);
    prop.insert("indxNs", indxNs);
    prop.insert("ipAddr", ipAddr);
    prop.insert("sysScheme", sysScheme);
    prop.insert("sysPwd", sysPwd);
    prop.insert("funcArgOutType", "function");
    prop.insert("isEnableSessionGttCreation", "ON");
    prop.insert("userScheme", userScheme);
    prop.insert("userPwd", userPwd);
    prop.insert("dsn", dsn);
    prop.insert("dbName", dbName);
    prop.insert("rootUpgDir", "..");
    //prop.insert("dstrScheme", "");
    //prop.insert("myExternalIpAddr", "");
    //prop.insert("listenPort", "");
    //prop.insert("oraTns", "");

    QDir jdkbin(jdkBinPath);
    if (jdkbin.cd("bin"))
        prop.insert("jdkBinPath", QDir::toNativeSeparators(jdkbin.absolutePath()));

    root.insert("prop", prop);

    QJsonDocument doc;
    doc.setObject(root);

    QFile Property(dir.absoluteFilePath("Property.json"));
    if (Property.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QByteArray json = doc.toJson();
        Property.write(json);
        Property.close();
    }

    QProcess::startDetached(dir.absoluteFilePath("GuiConverter.exe"),
                            {}, dir.absolutePath());

    StartGuiconverterRestoreProperty *restore = new StartGuiconverterRestoreProperty(backup,                                                                      dir.absoluteFilePath("Property.json"));
    QThreadPool::globalInstance()->start(restore);

    return 0;
}

/*
{
    "prop": {
        "userNs": "users",--
        "indxNs": "indx",--
        "ipAddr": "10.97.24.207:5432",
        "dbName": "rsc_at_90_00_pg",
        "dsn": "rsc_at_90_00_pg",--
        "oraTns": "",
        "sysScheme": "postgres",
        "sysPwd": "system",
        "userScheme": "rsc_at_90_00_pg",
        "userPwd": "rsc_at_90_00_pg",
        "dstrScheme": "",
        "rootUpgDir": "..",
        "jdkBinPath": "C:/Program Files/Java/jdk-10.0.2/bin",
        "distrVer": "6.20.031.087",
        "myExternalIpAddr": "",
        "listenPort": "",
        "funcArgOutType": "function",
        "isEnableSessionGttCreation": "ON"--
    }
}
*/
