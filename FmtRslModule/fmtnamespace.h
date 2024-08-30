#ifndef FMTNAMESPACE_H
#define FMTNAMESPACE_H

#include <QObject>
#include "fmtcore.h"

#define BegiFmtEnumNamespace(Name) class Name##Namespace : public QObject \
{\
    Q_OBJECT \
public:\
    Name##Namespace() : QObject(){}   \
    enum Name\
    { \

#define EndFmtEnumNamespace(Name) }; Q_ENUM(Name) };

BegiFmtEnumNamespace(FmtKeyFlags)
Duplicates = fmtkf_Duplicates,
Modifiable = fmtkf_Modifiable,
Binary     = fmtkf_Binary,
NullVal    = fmtkf_NullVal,
Segment    = fmtkf_Segment,
Alternate  = fmtkf_Alternate,
Descending = fmtkf_Descending,
Supplement = fmtkf_Supplement,
Extended   = fmtkf_Extended,
Manual     = fmtkf_Manual,
Local      = fmtkf_Local,
EndFmtEnumNamespace(FmtKeyFlags)

BegiFmtEnumNamespace(FmtTypes)
INT = fmtt_INT,
LONG = fmtt_LONG,
BIGINT = fmtt_BIGINT,
FLOAT = fmtt_FLOAT,
DOUBLE = fmtt_DOUBLE,
MONEY = fmtt_MONEY,
STRING = fmtt_STRING,
SNR = fmtt_SNR,
DATE = fmtt_DATE,
TIME = fmtt_TIME,
CHR = fmtt_CHR,
UCHR = fmtt_UCHR,
NUMERIC = fmtt_NUMERIC,
EndFmtEnumNamespace(FmtTypes)

BegiFmtEnumNamespace(FmtKeyNullVal)
Nullval_None = keynullval_None,
Nullval_All = keynullval_All,
Nullval_Any = keynullval_Any,
EndFmtEnumNamespace(FmtKeyNullVal)

class FmtNamespace : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fmtVersion READ fmtVersion)
    Q_PROPERTY(QStringList fmtTypes READ fmtTypes)
public:
    enum FmtBlobType
    {
        BLOBT_BLOB_NO = 0,
        BLOBT_BLOB_VAR,
        BLOBT_BLOB_STREAM,
        BLOBT_CLOB
    };

    Q_ENUM(FmtBlobType)

    FmtNamespace(QObject *parent = nullptr);
    virtual ~FmtNamespace();

    QString fmtVersion() const;

    /**
     * @brief Возвращает имя типа для типа
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtTypeNameForType(const qint16 &type) const;

    /**
     * @brief Возвращает тип из индекса
     * @param индекс
     * @return
     */
    Q_INVOKABLE qint16 fmtTypeFromIndex(const qint16 &id) const;

    /**
     * @brief Возвращает объявление для Oracle
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtOracleDecl(const qint16 &Type) const;

    /**
     * @brief Возвращает объявление для Postgres
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtPostgresDecl(const qint16 &Type) const;

    /**
     * @brief Возвращает имя структуры для типа
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtCppStructTypeName(const qint16 &Type) const;

    /**
     * @brief Возвращает имя структуры базы данных для типа
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtCppStructDbTypeName(const qint16 &Type) const;

    /**
     * @brief Возвращает имя базовой структуры базы данных для типа
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtCppStructDbBaseTypeName(const qint16 &Type) const;

    /**
     * @brief Возвращает имя типа для RSL
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtRslTypeName(const qint16 &Type) const;

    /**
     * @brief Возвращает имя значения для RSL
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtRslValueName(const qint16 &Type) const;

    /**
     * @brief Проверяет, может ли тип иметь пользовательский размер
     * @param тип
     * @return
     */
    Q_INVOKABLE bool fmtTypeCanHaveCustomSize(const qint16 &Type) const;

    /**
     * @brief Возвращает стандартное значение для Oracle
     * @param тип
     * @param размер
     * @return
     */
    Q_INVOKABLE QString fmtGetOraDefaultVal(const qint16 &Type, const int &size) const;

    /**
     * @brief Возвращает стандартное значение для Postgres
     * @param тип
     * @param размер
     * @return
     */
    Q_INVOKABLE QString fmtGetPgDefaultVal(const qint16 &Type, const int &size) const;


    /**
     * @brief Возвращает размер типа
     * @param тип
     * @return
     */
    Q_INVOKABLE quint16 fmtTypeSize(const qint16 &Type) const;

    /**
     * @brief Возвращает размер типа в индексе
     * @param тип
     * @return
     */
    Q_INVOKABLE quint16 fmtTypeIndexSize(const qint16 &Type) const;

    /**
     * @brief Возвращает тип для RSD
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtRsdType(const qint16 &Type) const;

    /**
     * @brief Возвращает константу для RSD
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtRsdConstant(const qint16 &Type) const;

    /**
     * @brief Возвращает константу пустого значения
     * @param тип
     * @return
     */
    Q_INVOKABLE QString fmtZeroConstant(const qint16 &Type) const;

    /**
     * @brief Возвращает имя структуры таблицы
     * @param таблица
     * @return
     */
    Q_INVOKABLE QString fmtTableStructName(const QString &table);

    /**
     * @brief Возвращает список наименований таблицы
     * @param таблица
     * @return
     */
    Q_INVOKABLE QStringList fmtTableStringList(const QString &table);

    /**
     * @brief Возвращает расширение таблицы
     * @param таблица
     * @return
     */
    Q_INVOKABLE QString fmtGetTableExtension(const QString &table);

    /**
     * @brief Возвращает имя файла таблицы
     * @param таблица
     * @return
     */
    Q_INVOKABLE QString fmtGetTableFileName(const QString &table);

    /**
     * @brief Возвращает список триггеров для таблицы
     * @param соединение
     * @param таблица
     * @return
     */
    Q_INVOKABLE QStringList fmtGenGetTriggers(ConnectionInfo *connection, const QString &table);

    /**
    * @brief Возвращает строковое представление типа BLOB
    * @param тип
    * @return
    */
    Q_INVOKABLE QString BlobTypeToString(const qint16 &Type) const;

    /**
     * @brief Возвращает строковое представление поля BLOB
     * @param тип
     * @return
     */
    Q_INVOKABLE QString BlobFieldString(const qint16 &Type) const;

    /**
     * @brief Возвращает строковое представление типа BLOB для Oracle
     * @param тип
     * @return
     */
    Q_INVOKABLE QString BlobFieldTypeOraString(const qint16 &Type) const;

    /**
     * @brief Возвращает строковое представление типа BLOB для Postgres
     * @param тип
     * @return
     */
    Q_INVOKABLE QString BlobFieldTypePgString(const qint16 &Type) const;


    /**
     * @brief Возвращает SQL-текст для таблицы
     * @param таблица
     * @return
     */
    Q_INVOKABLE QString fmtTableSqlText(FmtTable *pTable) const;


    /**
     * @brief Возвращает поле приведенное в CamelCase
     * @param поле
     * @param принудительно
     * @return
     */
    Q_INVOKABLE QString fmtCapitalizeField(const QString &undecoratedfield, bool force = false);

    Q_INVOKABLE QStringList fmtCapitalizeField(const QStringList &undecoratedfield, bool force = false);

    /**
     * @brief Возвращает скрипт для удаления столбца
     * @param поля
     * @return
     */
    Q_INVOKABLE QString fmtGenDeleteColumnScript(const QVariantList &flds);

    /**
     * @brief Возвращает скрипт для добавления столбца
     * @param поля
     * @return
     */
    Q_INVOKABLE QString fmtGenAddColumnScript(const QVariantList &flds);

    /**
     * @brief Возвращает скрипт для изменения столбца
     * @param поля
     * @return
     */
    Q_INVOKABLE QString fmtGenModifyColumnScript(const QVariantList &flds);


    /**
     * @brief Инициализирует таблицу
     * @param таблица
     * @param ошибка
     * @return
     */
    Q_INVOKABLE int fmtInitTable(FmtTable *pTable, QString &err);

        /**
     * @brief Инициализирует таблицу для GUI
     * @param таблица
     */
    Q_INVOKABLE void fmtInitTableGui(FmtTable *pTable);


    Q_INVOKABLE QString NullString(const int &index) const;

    /**
     * @brief Возвращает список типов
     * @return
     */
    QStringList fmtTypes() const;

signals:

};

#endif // FMTNAMESPACE_H
