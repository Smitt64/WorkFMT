#include "fmtcore.h"
#include "fmtapplication.h"
#include "fmtimpexpwrp.h"
#include "fmterrors.h"
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
#include "DataStructure.hpp"

static QStringList FmtTypesList = QStringList()
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
        << "NUMERIC";

/*typedef struct tagFmtTypeInfoT : public
        DataStructure
        <
        FmtFldType, // type;
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
    FmtFldType _type;
    quint16 _size;
    quint16 _indexType;
    QString _oraType;
    QString _cppType;
    QString _cppDbType;
    QString _cppDbBaseType;
    QString _rslType;
    QString _rslValueName;
    QString _xmlTypeName;

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

//Q_GLOBAL_STATIC(FmtTypesMapType, FmtTypesMap);
static FmtTypesMapType FmtTypesMap;

void FmtInit()
{
    //tagFmtTypeInfoT::
    //                                       type     size  oraType          indexType      cppType      cppDbType     cppDbBaseType     rslType
    /*FmtTypesMap["INT"]     = tagFmtTypeInfo(fmtt_INT,    2, "NUMBER(5)",     fmtk_Einteger, "int16",     "db_int16",   "db_baseint16",   "V_INTEGER",    "intval");
    FmtTypesMap["LONG"]    = tagFmtTypeInfo(fmtt_LONG,   4, "NUMBER(10)",    fmtk_Einteger, "int32",     "db_int32",   "db_baseint32",   "V_INTEGER",    "intval");
    FmtTypesMap["BIGINT"]  = tagFmtTypeInfo(fmtt_BIGINT, 8, "NUMBER(19)",    fmtk_Einteger, "int64_t",   "db_int64",   "db_baseint64",   "V_BIGINT",     "bigint");
    FmtTypesMap["FLOAT"]   = tagFmtTypeInfo(fmtt_FLOAT,  4, "FLOAT(24)",     fmtk_Efloat,   "float",     "db_float",   "db_basefloat",   "V_DOUBLE",     "doubvalL");
    FmtTypesMap["DOUBLE"]  = tagFmtTypeInfo(fmtt_DOUBLE, 8, "FLOAT(53)",     fmtk_Ebfloat,  "double",    "db_double",  "db_basedouble",  "V_DOUBLE",     "doubvalL");
    FmtTypesMap["MONEY"]   = tagFmtTypeInfo(fmtt_MONEY,  8, "NUMBER(19,4)",  fmtk_Emoney,   "lmoney",    "db_lmoney",  "db_lbasemoney",  "V_MONEY_FDEC", "numVal");
    FmtTypesMap["STRING"]  = tagFmtTypeInfo(fmtt_STRING, 0, "VARCHAR2",      fmtk_Estring,  "char",      "char",       "char",           "V_STRING",     "string");
    FmtTypesMap["SNR"]     = tagFmtTypeInfo(fmtt_SNR,    0, "VARCHAR2",      fmtk_Estring,  "char",      "char",       "char",           "V_STRING",     "string");
    FmtTypesMap["DATE"]    = tagFmtTypeInfo(fmtt_DATE,   4, "DATE",          fmtk_Edate,    "bdate",     "bdate",      "bdate",          "V_DATE",       "date");
    FmtTypesMap["TIME"]    = tagFmtTypeInfo(fmtt_TIME,   4, "DATE",          fmtk_Etime,    "btime",     "btime",      "btime",          "V_TIME",       "time");
    FmtTypesMap["CHR"]     = tagFmtTypeInfo(fmtt_CHR,    1, "CHAR",          fmtk_Estring,  "char",      "char",       "char",           "V_BOOL",       "boolval");
    FmtTypesMap["UCHR"]    = tagFmtTypeInfo(fmtt_UCHR,   1, "RAW",           fmtk_Estring,  "char",      "char",       "char",           "V_STRING",     "string");
    FmtTypesMap["NUMERIC"] = tagFmtTypeInfo(fmtt_NUMERIC,16,"NUMBER(32,12)", fmtk_Enumeric, "DBNumeric", "db_lmoney",  "db_lbasemoney",  "V_NUMERIC",    "decimal");*/

    FmtTypesMap =
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
    };
}

bool fmtTypeCanHaveCustomSize(const FmtFldType &Type)
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

bool hasTemporaryFlag(const FmtNumber10 &flag)
{
    return (flag & fmtnf_Temp) == fmtnf_Temp;
}

bool hasRecordFlag(const FmtNumber10 &flag)
{
    return (flag & fmtnf_Rec) == fmtnf_Rec;
}

QSettings *settings()
{
    return ((FmtApplication*)qApp)->settings();
}

QStringList fmtTypes()
{
    return QStringList(FmtTypesList);
}

QString fmtTypeForId(const FmtFldType &id)
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

quint16 fmtTypeSize(const FmtFldType &Type)
{
    FmtFldType type = Type;
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

QString fmtOracleDecl(const FmtFldType &Type)
{
    return FmtTypesMap[fmtTypeForId(Type)]._oraType;
}

QString fmtCppStructTypeName(const FmtFldType &Type)
{
    QString fldType = fmtTypeForId(Type);
    return FmtTypesMap[fldType]._cppType;
}

QString fmtCppStructDbTypeName(const FmtFldType &Type)
{
    QString fldType = fmtTypeForId(Type);
    return FmtTypesMap[fldType]._cppDbType;
}

QString fmtRslTypeName(const FmtFldType &Type)
{
    QString fldType = fmtTypeForId(Type);
    return FmtTypesMap[fldType]._rslType;
}

QString fmtRslValueName(const FmtFldType &Type)
{
    QString fldType = fmtTypeForId(Type);
    return FmtTypesMap[fldType]._rslValueName;
}

QString fmtCppStructDbBaseTypeName(const FmtFldType &Type)
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

FmtFldIndex fmtIndexForType(const FmtFldType &id)
{
    QMapIterator<QString, FmtTypeInfo> iterator(FmtTypesMap);

    int indx = -1;
    while(iterator.hasNext())
    {
        indx ++;
        iterator.next();
        FmtTypeInfo info = iterator.value();
        if (info._type == id)
        {
            indx = FmtTypesList.indexOf(iterator.key());
            break;
        }
    }

    return static_cast<FmtFldIndex>(indx);
}

QString fmtTypeNameForType(const FmtFldType &type)
{
    int index = static_cast<int>(fmtIndexForType(type));
    if (index < 0 || index >= FmtTypesList.size())
        return "Unknown";
    return FmtTypesList[fmtIndexForType(type)];
}

FmtFldType fmtTypeFromXmlType(const QString &type)
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

FmtFldType fmtTypeFromIndex(const FmtFldIndex &id)
{
    int index = static_cast<int>(fmtIndexForType(id));
    if (index < 0 || index >= FmtTypesList.size())
        return -1;
    return FmtTypesMap[FmtTypesList[id]]._type;
}

quint16 fmtTypeIndexSize(const FmtFldType &id)
{
    int index = fmtIndexForType(id);
    if (index < 0 || index >= FmtTypesList.size())
        return -1;
    return FmtTypesMap[FmtTypesList[id]]._size;
}

FmtFldType fmtIndexFromFmtType(const FmtFldType &id)
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

QString AddTabButtonCss()
{
    return "*         {image: url(':/img/addtab-icon.png'); border: 0;}"
           "*:hover   {image: url(':/img/addtab-icon-hover.png');}"
           "*:pressed {image: url(':/img/addtab-icon-pressed.png');} ";
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
    int stat = 0;

    QMap<QString, QVariant> values = query->boundValues();
    QMapIterator<QString, QVariant> i(values);
    while(i.hasNext())
    {
        i.next();
        qCInfo(logSql()) << i.key() << ": " << i.value();
    }

    bool result = query->exec();
    if (!result)
    {
        stat = 1;
        qCCritical(logSql()) << query->lastError().text();

        if (err != Q_NULLPTR)
            *err = query->lastError().text();
    }
    qCInfo(logSql()) << query->executedQuery();
    qCInfo(logSql()) << "Result:" << result;

    return stat;
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
        qCWarning(logSql()) << db.lastError().text();
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

QString BoolToString(bool value)
{
    return value ? "true" : "false";
}

QColor GenerateColor()
{
    static double h = 0.0;
    if (h == 0.0)
        h = qrand() % 255 / 255;

    h += COLOR_GOLDEN_RATIO;
    double hc = (h - (int)h);
    return QColor::fromHsvF(hc, 0.5, 0.95, 1);
}

QString FmtTableSqlText(QSharedPointer<FmtTable> pTable)
{
    QString str = QString("-- Таблица %1\n").arg(pTable->name().toUpper());
    str += pTable->generateCreateTableSql() + ";\n\n";
    str += pTable->getCommentSql() + ";\n";

    for(int i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *f = pTable->field(i);
        str += f->getCommentSql() + ";\n";
    }

    return str;
}

void SaveFmtTableSql(QSharedPointer<FmtTable> pTable, QWidget *parent)
{
    SelectFolderDlg folder(RsCreateSqlContext, parent);

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
                stream << FmtTableSqlText(pTable);
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
            FmtErrors log;
            imp.parseProtocol(&log);
            ErrorDlg edlg(ErrorDlg::mode_Information, parent);
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
    tmp.open();

    stat = pTable->dbInit(tmp.fileName());

    QByteArray logcontent = tmp.readAll();

    if (err)
       *err = logcontent;

    tmp.close();

    return stat;
}

void InitFmtTable(QSharedPointer<FmtTable> pTable, QWidget *parent)
{
    DbInitDlg dlg(pTable, parent);

    int stat = 0;
    if (dlg.exec() == QDialog::Accepted)
    {
        QProgressDialog progress(QObject::tr("Инициализация таблицы"), QObject::tr("Отмена"), 0, 0, parent);
        progress.open();
        if (dlg.getCreateTableFlag())
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
            stat = InitFmtTableExec(pTable.data(), &err);
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
    return QDir::current().absoluteFilePath("OracleAuthDlg.cfg");
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

int CoreStartProcess(QProcess *exe, const QString &program, const QStringList& arguments, bool waitForFinished, bool waitForStarted)
{
    int stat = 0;
    qCInfo(logCore()) << "Process: " << exe;
    qCInfo(logCore()) << "Executable path:" << program;
    qCInfo(logCore()) << "Working directory:" << exe->workingDirectory();
    qCInfo(logCore()) << "Аrguments" << arguments;

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

    if (waitForFinished)
    {
        exe->waitForFinished();

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
    QFile f(filename, qApp);
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

void StartUnloadDbf(ConnectionInfo *current, const QString &table, QWidget *parent)
{
    ErrorDlg dlg(ErrorDlg::mode_Widget, parent);
    dlg.setMessage(QObject::tr("Экспорт содержимого таблицы %1").arg(table));
    dlg.setWindowModality(Qt::WindowModal);
    FmtDbfToolWrp wrp(current, parent);
    dlg.setErrors(wrp.fmterrors());
    QObject::connect(&dlg, SIGNAL(canceled()), &wrp, SLOT(stop()));
    QObject::connect(&wrp, SIGNAL(started()), &dlg, SLOT(exec()));
    QObject::connect(&wrp, SIGNAL(startError()), &dlg, SLOT(exec()));

    SelectFolderDlg folder(RsExpUnlDirContext, parent);
    if (folder.exec() == QDialog::Accepted)
        wrp.unload(folder.selectedPath(), table);
}

void StartLoadDbf(ConnectionInfo *current, const QString &table, QWidget *parent)
{
    ErrorDlg dlg(ErrorDlg::mode_Widget, parent);
    dlg.setMessage(QObject::tr("Экспорт содержимого таблицы %1").arg(table));
    dlg.setWindowModality(Qt::WindowModal);
    FmtDbfToolWrp wrp(current, parent);
    dlg.setErrors(wrp.fmterrors());
    QObject::connect(&dlg, SIGNAL(canceled()), &wrp, SLOT(stop()));
    QObject::connect(&wrp, SIGNAL(started()), &dlg, SLOT(exec()));
    QObject::connect(&wrp, SIGNAL(startError()), &dlg, SLOT(exec()));
    wrp.load(table);
}

void StartLoadDbfSelectFile(ConnectionInfo *current, const QString &table, QWidget *parent)
{
    SelectFolderDlg folder(RsExpUnlDirContext, parent);
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

int SelectTableFieldsDlg(QSharedPointer<FmtTable> pTable, const QString &title, QList<FmtField*> *pFldList, QWidget *parent)
{
    int stat = 0;

    SelectFieldsModel selFldModel(pTable.data(), parent);
    SelectFilteredDlg dlg(parent);
    dlg.setWindowTitle(title);
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
