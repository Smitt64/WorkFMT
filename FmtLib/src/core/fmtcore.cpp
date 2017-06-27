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

typedef struct tagFmtTypeInfo
{
    quint16 _type;
    quint16 _size;
    quint16 _indexType;
    QString _oraType;
    QString _cppType;

    tagFmtTypeInfo(quint16 type = 0, quint16 size = 0, QString oraType = QString(), quint16 indexType = 0)
    {
       _type = type;
       _size = size;
       _oraType = oraType;
       _indexType = indexType;
    }
}FmtTypeInfo;

typedef struct tagFmtTtypesItem
{
    QString name;
    int id;
}FmtIndexTypesItem;

static QMap<QString, FmtTypeInfo> FmtTypesMap;

void FmtInit()
{
    FmtTypesMap["INT"]     = tagFmtTypeInfo(fmtt_INT,    2, "NUMBER(5)",     fmtk_Einteger);
    FmtTypesMap["LONG"]    = tagFmtTypeInfo(fmtt_LONG,   4, "NUMBER(10)",    fmtk_Einteger);
    FmtTypesMap["BIGINT"]  = tagFmtTypeInfo(fmtt_BIGINT, 8, "NUMBER(19)",    fmtk_Einteger);
    FmtTypesMap["FLOAT"]   = tagFmtTypeInfo(fmtt_FLOAT,  4, "FLOAT(24)",     fmtk_Efloat);
    FmtTypesMap["DOUBLE"]  = tagFmtTypeInfo(fmtt_DOUBLE, 8, "FLOAT(53)",     fmtk_Ebfloat);
    FmtTypesMap["MONEY"]   = tagFmtTypeInfo(fmtt_MONEY,  8, "NUMBER(19,4)",  fmtk_Emoney);
    FmtTypesMap["STRING"]  = tagFmtTypeInfo(fmtt_STRING, 0, "VARCHAR2",      fmtk_Estring);
    FmtTypesMap["SNR"]     = tagFmtTypeInfo(fmtt_SNR,    0, "VARCHAR2",      fmtk_Estring);
    FmtTypesMap["DATE"]    = tagFmtTypeInfo(fmtt_DATE,   4, "DATE",          fmtk_Edate);
    FmtTypesMap["TIME"]    = tagFmtTypeInfo(fmtt_TIME,   4, "DATE",          fmtk_Etime);
    FmtTypesMap["CHR"]     = tagFmtTypeInfo(fmtt_CHR,    1, "CHAR",          fmtk_Estring);
    FmtTypesMap["UCHR"]    = tagFmtTypeInfo(fmtt_UCHR,   1, "RAW",           fmtk_Estring);
    FmtTypesMap["NUMERIC"] = tagFmtTypeInfo(fmtt_NUMERIC,16,"NUMBER(32,12)", fmtk_Enumeric);
}

bool fmtTypeCanHaveCustomSize(const quint32 &Type)
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

bool hasTemporaryFlag(const quint32 &flag)
{
    return (flag & fmtnf_Temp) == fmtnf_Temp;
}

bool hasRecordFlag(const quint32 &flag)
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

QString fmtTypeForId(const quint32 &id)
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

quint16 fmtTypeSize(const quint32 &Type)
{
    QMapIterator<QString, FmtTypeInfo> iterator(FmtTypesMap);
    while(iterator.hasNext())
    {
        iterator.next();
        if (iterator.value()._type == Type)
            return iterator.value()._size;
    }
    return 0;
}

QString fmtOracleDecl(const quint32 &Type)
{
    return FmtTypesMap[fmtTypeForId(Type)]._oraType;
}

QString fmtCppStructTypeName(const quint32 &Type)
{
    QString type;

    switch(Type)
    {
    case fmtt_INT:
        type = "db_int16";
        break;
    case fmtt_LONG:
        type = "db_int32";
        break;
    case fmtt_BIGINT:
        type = "int64_t";
        break;
    case fmtt_FLOAT:
        type = "db_float";
        break;
    case fmtt_DOUBLE:
        type = "db_double";
        break;
    case fmtt_MONEY:
        type = "?";
        break;
    case fmtt_STRING:
    case fmtt_SNR:
    case fmtt_CHR:
    case fmtt_UCHR:
        type = "char";
        break;
    case fmtt_DATE:
        type = "bdate";
        break;
    case fmtt_TIME:
        type = "btime";
        break;
    case fmtt_NUMERIC:
        type = "db_decimal";
        break;
    }

    return type;
}

quint32 fmtTypeIndexForId(const quint32 &id)
{
    QMapIterator<QString, FmtTypeInfo> iterator(FmtTypesMap);
    while(iterator.hasNext())
    {
        iterator.next();
        if (iterator.value()._type == id)
            return FmtTypesList.indexOf(iterator.key());
    }

    return 0;
}

quint32 fmtIndexForType(const quint32 &id)
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

    return indx;
}

QString fmtTypeNameForType(const quint32 &type)
{
    return FmtTypesList[fmtIndexForType(type)];
}

quint16 fmtTypeFromIndex(const quint32 &id)
{
    return FmtTypesMap[FmtTypesList[id]]._type;
}

quint16 fmtTypeIndexSize(const quint32 &id)
{
    return FmtTypesMap[FmtTypesList[id]]._size;
}

quint16 fmtIndexFromFmtType(const quint32 &id)
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

QString NullString(const int &index)
{
    QString v = "";
    if (index == 1)
        v = "All";
    else if (index == 2)
        v = "Any";
    return v;
}

int ExecuteQuery(QSqlQuery *query)
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
    }
    qCInfo(logSql()) << query->executedQuery();
    qCInfo(logSql()) << "Result:" << result;

    return stat;
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
    for (int i = 0; i < model->rowCount(); i++)
    {
        if (model->item(i, 1)->text() == service)
            str = model->item(i, 0)->text();
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
QString DbInitTextError(const quint16 &id)
{
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
    QSettings *s = settings();
    QString lastCreateTableSqlDir = s->value("LastCreateTableSqlDir", ".").toString();
    QString fileName = QFileDialog::getSaveFileName(parent, QObject::tr("CreateTableSql"), QString("%2/%1.sql")
                                                    .arg(pTable->name().toUpper())
                                                    .arg(lastCreateTableSqlDir), "SQL (*.sql)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QFileInfo info(fileName);
        s->setValue("LastCreateTableSqlDir", info.filePath());
        QTextStream stream(&file);
        stream.setCodec("IBM 866");
        stream << FmtTableSqlText(pTable);
        file.close();
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
            stat = pTable->createDbTable();
            QApplication::processEvents();
        }
        if (!stat && dlg.getCreteIndexFlag())
        {
            QTemporaryFile tmp;
            tmp.open();

            stat = pTable->dbInit(tmp.fileName());
            QApplication::processEvents();

            QMessageBox msg(parent);

            if (stat)
            {
                msg.setWindowTitle(QObject::tr("Ошибка"));
                msg.setIcon(QMessageBox::Critical);
                msg.setText(QObject::tr("<b>DbInit вернул код ошибки [%1]:</b>").arg(stat));
            }
            else
            {
                msg.setWindowTitle(QObject::tr("Результат выполнения"));
                msg.setIcon(QMessageBox::Information);
                msg.setText(QObject::tr("<b>DbInit завершился с кодом [%1]: %2</b>").arg(stat).arg(DbInitTextError(stat)));
            }

            QByteArray logcontent = tmp.readAll();

            if (logcontent.size())
            {
                msg.setDetailedText(logcontent);
                QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
                QGridLayout* layout = (QGridLayout*)msg.layout();
                layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
            }
            msg.exec();

            tmp.close();
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
