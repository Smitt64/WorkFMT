#ifndef FMTCORE_H
#define FMTCORE_H

#include <QtCore>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSharedPointer>
#include <functional>
#include "fmtlib_global.h"
#include <sstream>

#define QRSD_DRIVER "qrsd"
#define RSD_UNICODE "RSD_UNICODE"
#define RSD_BLOB_MAX_LEN 32768

#define AutoIncType 15
#define BTNS_OFFSET 25
#define COLOR_GOLDEN_RATIO 0.618033988749895
#define EXCLUDENULL_TRUE 255

#define RsExpUnlDirContext "RsExpUnlDir"
#define RsFmtUnlDirContext "RsFmtUnlDir"
#define RsCreateSqlContext "RsCreateSqlDir"
#define RsDiffToScriptContext "RsDiffToScript"

#define RsExportDatContext "RsExportDat"

#define HEX_COLOR_REGEX QString("#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})")

enum FmtNamesColumn
{
    fnc_Id,
    fnc_Name,
    fnc_CacheSize,
    fnc_Owner,
    fnc_Comment,
    fnc_PkIndx,
    fnc_BlobType,
    fcn_BlobLen,
    fnc_Flags
};

enum FmtKeysColumn
{
    fkc_FmtId,
    fkc_KeyNum,
    fkc_SegNum,
    fkc_FmtFld,
    fkc_Flags,
    fkc_Type,
    fkc_NullVal,
    fkc_IsReal,
    fkc_Comment
};

enum FmtNamesFlags
{
    fmtnf_NoFlags = 0,
    fmtnf_Temp = 1 << 4,
    fmtnf_Rec  = 1 << 5,
};

enum FmtKeyFlags
{
    fmtkf_Duplicates = 0x0001,
    fmtkf_Modifiable = 0x0002,
    fmtkf_Binary     = 0x0004,
    fmtkf_NullVal    = 0x0008,
    fmtkf_Segment    = 0x0010,
    fmtkf_Alternate  = 0x0020,
    fmtkf_Descending = 0x0040,
    fmtkf_Supplement = 0x0080,
    fmtkf_Extended   = 0x0100,
    fmtkf_Manual     = 0x0200,
    fmtkf_Local      = 0x4000,
};

enum FmtKeyNullVal
{
    keynullval_None = 0,
    keynullval_All,
    keynullval_Any,
};

enum fmtTypes
{
    fmtt_DATETIME = -1,

    fmtt_INT = 0,
    fmtt_LONG = 1,
    fmtt_BIGINT = 26,
    fmtt_FLOAT = 2,
    fmtt_DOUBLE = 4,
    fmtt_MONEY = 14,
    fmtt_STRING = 7,
    fmtt_SNR = 8,
    fmtt_DATE = 9,
    fmtt_TIME = 10,
    fmtt_CHR = 12,
    fmtt_UCHR = 13,
    fmtt_NUMERIC = 25,
};

enum FmtKeyTypes
{
    fmtk_Estring   = 0x0,
    fmtk_Einteger  = 0x1,
    fmtk_Efloat    = 0x2,
    fmtk_Edate     = 0x3,
    fmtk_Etime     = 0x4,
    fmtk_Edecimal  = 0x5,
    fmtk_Emoney    = 0x6,
    fmtk_Elogical  = 0x7,
    fmtk_Enumeric  = 0x8,
    fmtk_Ebfloat   = 0x9,
    fmtk_Elstring  = 0xA,
    fmtk_Ezstring  = 0xB,
    fmtk_Eunbin    = 0xE,
    fmtk_Eautoinc  = 0xF,
    fmtk_Ecurrency = 0x13
};

enum FmtMetrics
{
    fmtm_TableNameMaxSize         = 30,  // максимальная длина имени таблицы
    fmtm_TableOwnerMaxSize        = 9,   // максимальная длина имени владельца таблицы
    fmtm_TableCommentMaxSize      = 40,  // максимальная длина коментария таблицы
    fmtm_FieldNameMaxSize         = 128, // максимальная длина имени поля таблицы
    fmtm_FieldCommentMaxSize      = 128, // максимальная длина коментария поля таблицы
    fmtm_IndexCommentMaxSize      = 128, // максимальная длина коментария индекса таблицы
};

enum FmtBlobType
{
    BT_BLOB_NO = 0,
    BT_BLOB_VAR,
    BT_BLOB_STREAM,
    BT_CLOB
};

enum DataBaseType
{
    Oracle = 0,
    PostgreSQL
};

class QMenu;
class FmtTable;
class FmtField;
class ConnectionInfo;

FMTLIBSHARED_EXPORT void FmtInit();
bool hasTemporaryFlag(const qint32 &flag);
bool hasRecordFlag(const qint32 &flag);

FMTLIBSHARED_EXPORT int ExecuteQuery(QSqlQuery *query, QString *err = Q_NULLPTR);
FMTLIBSHARED_EXPORT int ExecuteQuery(const QString &query, QSqlDatabase db = QSqlDatabase(), QString *err = Q_NULLPTR);
FMTLIBSHARED_EXPORT int ExecuteQueryFile(const QString &queryFileName, QSqlDatabase db = QSqlDatabase());

QString ConfigOraFilePath();

QString PlusButtonCss();
QString MinusButtonCss();
QString AddTabButtonCss();
QString CheckSymbol();
QString CheckSymbolFromVariant(const bool &value);
FMTLIBSHARED_EXPORT QString NullString(const int &index);

QSettings *settings();

FMTLIBSHARED_EXPORT QStringList fmtTypes();
FMTLIBSHARED_EXPORT QString fmtTypeForId(const qint16 &id);
FMTLIBSHARED_EXPORT QString fmtOracleDecl(const qint16 &Type);
FMTLIBSHARED_EXPORT QString fmtPostgresDecl(const qint16 &Type);
FMTLIBSHARED_EXPORT QString fmtCppStructTypeName(const qint16 &Type);
FMTLIBSHARED_EXPORT QString fmtCppStructDbTypeName(const qint16 &Type);
FMTLIBSHARED_EXPORT QString fmtCppStructDbBaseTypeName(const qint16 &Type);
FMTLIBSHARED_EXPORT QString fmtRslTypeName(const qint16 &Type);
FMTLIBSHARED_EXPORT QString fmtRslValueName(const qint16 &Type);
FMTLIBSHARED_EXPORT bool fmtTypeCanHaveCustomSize(const qint16 &Type);
FMTLIBSHARED_EXPORT QString fmtGetOraDefaultVal(const qint16 &Type, const int &size);
FMTLIBSHARED_EXPORT QString fmtGetPgDefaultVal(const qint16 &Type, const int &size);

FMTLIBSHARED_EXPORT quint32 fmtTypeIndexForId(const quint32 &id);
FMTLIBSHARED_EXPORT qint16 fmtTypeFromIndex(const qint16 &id);
FMTLIBSHARED_EXPORT qint16 fmtIndexForType(const qint16 &id);
FMTLIBSHARED_EXPORT qint16 fmtIndexFromFmtType(const qint16 &id);
FMTLIBSHARED_EXPORT QString fmtTypeNameForType(const qint16 &type);
FMTLIBSHARED_EXPORT qint16 fmtTypeFromXmlType(const QString &type);

FMTLIBSHARED_EXPORT quint16 fmtTypeSize(const qint16 &Type);
FMTLIBSHARED_EXPORT quint16 fmtTypeIndexSize(const qint16 &id);
FMTLIBSHARED_EXPORT QString fmtRsdType(const qint16 &Type);
FMTLIBSHARED_EXPORT QString fmtRsdConstant(const qint16 &Type);
FMTLIBSHARED_EXPORT QString fmtZeroConstant(const qint16 &Type);

FMTLIBSHARED_EXPORT int trn(QSqlDatabase &db, std::function<int(void)> func);

QString DatasourceFromService(const QString &service);

QString FMTLIBSHARED_EXPORT DbInitTextError(const qint16 &id);
int FMTLIBSHARED_EXPORT CoreStartProcess(QProcess *exe, const QString &program,
                     const QStringList& arguments,
                     bool waitForFinished = false,
                     bool waitForStarted = false,
                     int timeout = 30000,
                     bool waitForReadyRead = false);
QString FMTLIBSHARED_EXPORT ProcessExitStatusText(qint16 State);
QString FMTLIBSHARED_EXPORT ProcessStateText(qint16 State);

FMTLIBSHARED_EXPORT QString BlobTypeToString(int type);
FMTLIBSHARED_EXPORT QString BlobFieldString(int type);
FMTLIBSHARED_EXPORT bool isBlobType(const QString &typeName);
FMTLIBSHARED_EXPORT QString BoolToString(bool value);
FMTLIBSHARED_EXPORT QString BlobFieldTypeOraString(int type);
FMTLIBSHARED_EXPORT QString BlobFieldTypePgString(int type);

QColor GenerateColor();
void StartUnloadDbf(ConnectionInfo *current, const QString &table, QWidget *parent, QSettings *settings = nullptr);
void StartLoadDbf(ConnectionInfo *current, const QString &table, QWidget *parent);
void StartLoadDbfSelectFile(ConnectionInfo *current, const QString &table, QWidget *parent);

void ExportFmtToXml(ConnectionInfo *connection, const QStringList &file, const QString &dir, bool ShowProgress, bool ShowReport, QWidget *parent = Q_NULLPTR);
FMTLIBSHARED_EXPORT void InitFmtTable(FmtTable *pTable, QWidget *parent);
FMTLIBSHARED_EXPORT qint16 InitFmtTableExec(FmtTable *pTable, QString *err);

void SaveFmtTableSql(QSharedPointer<FmtTable> pTable, QWidget *parent);
FMTLIBSHARED_EXPORT QString FmtTableSqlText(FmtTable *pTable);
void FmtHotFixCreate(QSharedPointer<FmtTable> pTable);
//QString GenerateTableXML(const FmtTable *pTable);

FMTLIBSHARED_EXPORT QString FmtTableStructName(const QString &table);
FMTLIBSHARED_EXPORT QStringList FmtTableStringList(const QString &table);
FMTLIBSHARED_EXPORT QString FmtGetTableExtension(const QString &table);
FMTLIBSHARED_EXPORT QString FmtGetTableFileName(const QString &table);

FMTLIBSHARED_EXPORT QString GetProcessErrorText(const QProcess::ProcessError &error);
FMTLIBSHARED_EXPORT QString GetVersionNumberString();

FMTLIBSHARED_EXPORT QStringList FmtGenGetTriggers(ConnectionInfo *connection, const QString &table);
FMTLIBSHARED_EXPORT QString FmtGenTriggersScrip(QList<FmtField*> flds, bool disable);
FMTLIBSHARED_EXPORT QStringList FmtGenGetTriggers(ConnectionInfo *connection, const QString &table);

FMTLIBSHARED_EXPORT QString FmtGenUpdateDeleteColumnScript(QList<FmtField*> flds);
FMTLIBSHARED_EXPORT QString FmtGenUpdateAddColumnScript(QList<FmtField*> flds);
FMTLIBSHARED_EXPORT QString FmtGenModifyColumnScript(QList<FmtField*> flds);
QString FmtGenUpdateCreateTableScript(QSharedPointer<FmtTable> pTable);
QString FmtGenInsertTemplateSqlScript(QList<FmtField*> flds);
void WrapSqlBlockObjectExists(QTextStream &stream, const QString &block, QList<FmtField *> flds = QList<FmtField*>(), const QSharedPointer<FmtTable> &pTable = QSharedPointer<FmtTable>());

FMTLIBSHARED_EXPORT QString ConstrType1RegExp();
FMTLIBSHARED_EXPORT bool ParseConnectionString(const QString &connString, QString &user, QString &pass, QString &service);

FMTLIBSHARED_EXPORT QString ReadTextFileContent(const QString &filename);
FMTLIBSHARED_EXPORT int CloneFmtFromConnection(QSqlDatabase &source, QSqlDatabase &dest, QWidget *parent);
FMTLIBSHARED_EXPORT bool CheckConnectionType(ConnectionInfo *pInfo, const int &Type, bool ShowMsg = false, QWidget *parent = Q_NULLPTR);

FMTLIBSHARED_EXPORT int SelectTableFieldsDlg(FmtTable *pTable, const QString &title, QList<FmtField*> *pFldList, QWidget *parent = Q_NULLPTR);
FMTLIBSHARED_EXPORT int SelectTableFieldsDlg(QSharedPointer<FmtTable> pTable, const QString &title, QList<FmtField*> *pFldList, QWidget *parent = Q_NULLPTR);

FMTLIBSHARED_EXPORT void readCSVRow(const QString &row, QVector<QString> &fields, const QChar &quote = '"');

FMTLIBSHARED_EXPORT QString getFullFileNameFromDir(const QString &file);

FMTLIBSHARED_EXPORT QString FmtCapitalizeField(const QString &undecoratedfield, bool force = false);
FMTLIBSHARED_EXPORT QStringList FmtCapitalizeField(const QStringList &undecoratedfield, bool force = false);

FMTLIBSHARED_EXPORT QString FmtGenDiffToScript(const QString &filename, const QString &connectionString, bool IsUnicodeDb, QString *err);
FMTLIBSHARED_EXPORT void CreateUserCommandsMenu(QMenu **menu, const QString &title,
                            QObject *receiver, const char *slot);

FMTLIBSHARED_EXPORT int StartGuiconverter(
        const QString &userScheme,
        const QString &userPwd,
        const QString &dbName,
        const QString &dsn,
        const QString &userNs,
        const QString &indxNs,
        const QString &sysScheme,
        const QString &sysPwd,
        const QString &ipAddr,
        QString *err = nullptr);

template<class T> std::string ContainerToString(const T &container)
{
    std::ostringstream oss;

    if (!container.empty())
    {
        std::copy(container.cbegin(), container.cend() - 1, std::ostream_iterator<typename T::value_type>(oss, ","));
        oss << container.back();
    }

    return oss.str();
}

#endif // FMTCORE_H
