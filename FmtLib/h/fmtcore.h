#ifndef FMTCORE_H
#define FMTCORE_H

#include <QtCore>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSharedPointer>
#include <functional>
#include "fmtlib_global.h"

#define AutoIncType 15
#define BTNS_OFFSET 25
#define COLOR_GOLDEN_RATIO 0.618033988749895

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

class FmtTable;
class FmtField;
class ConnectionInfo;

FMTLIBSHARED_EXPORT void FmtInit();
bool hasTemporaryFlag(const FmtNumber10 &flag);
bool hasRecordFlag(const FmtNumber10 &flag);

int ExecuteQuery(QSqlQuery *query, QString *err = Q_NULLPTR);
int ExecuteQuery(const QString &query, QSqlDatabase db = QSqlDatabase(), QString *err = Q_NULLPTR);
int ExecuteQueryFile(const QString &queryFileName, QSqlDatabase db = QSqlDatabase());

QString ConfigOraFilePath();

QString PlusButtonCss();
QString MinusButtonCss();
QString AddTabButtonCss();
QString CheckSymbol();
QString NullString(const int &index);

QSettings *settings();

FMTLIBSHARED_EXPORT QStringList fmtTypes();
FMTLIBSHARED_EXPORT QString fmtTypeForId(const FmtFldType &id);
FMTLIBSHARED_EXPORT QString fmtOracleDecl(const FmtFldType &Type);
FMTLIBSHARED_EXPORT QString fmtCppStructTypeName(const FmtFldType &Type);
FMTLIBSHARED_EXPORT QString fmtCppStructDbTypeName(const FmtFldType &Type);
FMTLIBSHARED_EXPORT QString fmtCppStructDbBaseTypeName(const FmtFldType &Type);
FMTLIBSHARED_EXPORT QString fmtRslTypeName(const FmtFldType &Type);
FMTLIBSHARED_EXPORT QString fmtRslValueName(const FmtFldType &Type);
FMTLIBSHARED_EXPORT bool fmtTypeCanHaveCustomSize(const FmtFldType &Type);

quint32 fmtTypeIndexForId(const quint32 &id);
FmtFldType fmtTypeFromIndex(const FmtFldIndex &id);
FmtFldIndex fmtIndexForType(const FmtFldType &id);
quint16 fmtIndexFromFmtType(const quint32 &id);
QString fmtTypeNameForType(const FmtFldType &type);

quint16 fmtTypeSize(const FmtFldType &Type);
quint16 fmtTypeIndexSize(const FmtFldType &id);

int trn(QSqlDatabase &db, std::function<int(void)> func);

QString DatasourceFromService(const QString &service);

QString DbInitTextError(const qint16 &id);
int CoreStartProcess(QProcess *exe, const QString &program, const QStringList& arguments, bool waitForFinished = false, bool waitForStarted = false);
QString ProcessExitStatusText(qint16 State);
QString ProcessStateText(qint16 State);

QString BlobTypeToString(int type);
QString BoolToString(bool value);

QColor GenerateColor();
void StartUnloadDbf(ConnectionInfo *current, const QString &table, QWidget *parent);
void StartLoadDbf(ConnectionInfo *current, const QString &table, QWidget *parent);

void ExportFmtToXml(ConnectionInfo *connection, const QStringList &file, const QString &dir, bool ShowProgress, bool ShowReport, QWidget *parent = Q_NULLPTR);
void InitFmtTable(QSharedPointer<FmtTable> pTable, QWidget *parent);

void SaveFmtTableSql(QSharedPointer<FmtTable> pTable, QWidget *parent);
QString FmtTableSqlText(QSharedPointer<FmtTable> pTable);
void FmtHotFixCreate(QSharedPointer<FmtTable> pTable);
//QString GenerateTableXML(const FmtTable *pTable);

QString FmtTableStructName(const QString &table);
QStringList FmtTableStringList(const QString &table);
QString FmtGetTableExtension(const QString &table);
QString FmtGetTableFileName(const QString &table);

FMTLIBSHARED_EXPORT QString GetProcessErrorText(const QProcess::ProcessError &error);
QString GetVersionNumberString();

QString FmtGenUpdateDeleteColumnScript(QList<FmtField*> flds);
QString FmtGenUpdateAddColumnScript(QList<FmtField*> flds);
QString FmtGenModifyColumnScript(QList<FmtField*> flds);
QString FmtGenUpdateCreateTableScript(QSharedPointer<FmtTable> pTable);
void WrapSqlBlockObjectExists(QTextStream &stream, const QString &block, QList<FmtField *> flds = QList<FmtField*>(), const QSharedPointer<FmtTable> &pTable = QSharedPointer<FmtTable>());

FMTLIBSHARED_EXPORT QString ConstrType1RegExp();
FMTLIBSHARED_EXPORT bool ParseConnectionString(const QString &connString, QString &user, QString &pass, QString &service);

FMTLIBSHARED_EXPORT QString ReadTextFileContent(const QString &filename);
FMTLIBSHARED_EXPORT int CloneFmtFromConnection(QSqlDatabase &source, QSqlDatabase &dest, QWidget *parent);
FMTLIBSHARED_EXPORT bool CheckConnectionType(ConnectionInfo *pInfo, const int &Type, bool ShowMsg = false, QWidget *parent = Q_NULLPTR);

FMTLIBSHARED_EXPORT int ShowCodeDialog(QWidget *parent, const QString &title, const int &type, const QString &code);

#endif // FMTCORE_H
