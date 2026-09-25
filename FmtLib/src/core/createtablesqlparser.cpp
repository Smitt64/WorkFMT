#include "createtablesqlparser.h"
#include "fmtcore.h"
#include "fmttable.h"
#include "fmtfield.h"
#include "fmtindex.h"
#include "fmtsegment.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStringList>

namespace
{
    // Длина blob-поля по умолчанию для таблиц, создаваемых из SQL-скрипта
    const qint32 DEFAULT_BLOB_LEN = 16384;

    // Убрать завершающие запятую/точку с запятой и лишние пробелы
    QString trimComma(const QString &s)
    {
        QString str = s;
        while (!str.isEmpty() && (str.endsWith(QLatin1Char(',')) || str.endsWith(QLatin1Char(';'))))
            str.chop(1);
        return str.trimmed();
    }

    // Удалить однострочные SQL-комментарии, не трогая содержимое строк в кавычках
    QString stripLineComment(const QString &line)
    {
        bool inString = false;
        for (int i = 0; i < line.length(); i++)
        {
            const QChar ch = line.at(i);
            if (ch == QLatin1Char('\''))
                inString = !inString;
            else if (!inString && ch == QLatin1Char('-') &&
                     i + 1 < line.length() && line.at(i + 1) == QLatin1Char('-'))
                return line.left(i);
        }
        return line;
    }

    bool matchDecl(const QString &upper, const char *pattern, qint16 type, bool typeHasSize,
                   qint16 &outType, qint32 &outSize)
    {
        // matchDecl вызывается на каждое поле таблицы — кэшируем
        // скомпилированные регэкспы, чтобы не компилировать их заново
        static QHash<QString, QRegularExpression> rxCache;
        const QString key = QLatin1String(pattern);

        auto it = rxCache.find(key);
        if (it == rxCache.end())
            it = rxCache.insert(key, QRegularExpression(key, QRegularExpression::CaseInsensitiveOption));

        QRegularExpressionMatch match = it->match(upper);

        if (!match.hasMatch())
            return false;

        outType = type;
        outSize = fmtTypeSize(type);

        if (typeHasSize && match.lastCapturedIndex() > 0)
            outSize = match.captured(match.lastCapturedIndex()).toInt();

        return true;
    }

    // NUMBER(p, s) / NUMERIC(p, s) / DECIMAL(p, s): точность уходит в decpoint,
    // размер остаётся стандартным для fmtt_NUMERIC
    bool matchNumericDecl(const QString &upper, qint16 &outType, qint32 &outSize, qint16 &outDecpoint,
                          bool postgres = false)
    {
        static const QRegularExpression rx(
                    "^(NUMBER|NUMERIC|DECIMAL)\\s*\\(\\s*(\\d+)\\s*,\\s*(\\d+)\\s*\\)$",
                    QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = rx.match(upper);

        if (!match.hasMatch())
            return false;

        const int precision = match.captured(2).toInt();
        const int scale = match.captured(3).toInt();

        // NUMBER — Oracle, NUMERIC/DECIMAL в Postgres ведут себя так же;
        // канонические размеры MONEY/NUMERIC из карты типов FMT (см. FmtTypesMap.json)
        if (postgres || !match.captured(1).compare(QLatin1String("NUMBER"), Qt::CaseInsensitive))
        {
            // NUMBER(32,12) — каноническая запись дефолтного NUMERIC из карты
            // типов FMT. В fmt_dump такие поля лежат с decpoint=0, scale=12
            // в decpoint не переносим (иначе сломается round-trip)
            if (precision == 32 && scale == 12)
            {
                outType = fmtt_NUMERIC;
                outSize = fmtTypeSize(fmtt_NUMERIC);
                outDecpoint = 0;
                return true;
            }

            // NUMBER(19,4) — так FMT представляет MONEY (см. карту типов),
            // в fmt_dump MONEY-поля лежат с decpoint=2
            if (precision == 19 && scale == 4)
            {
                outType = fmtt_MONEY;
                outSize = fmtTypeSize(fmtt_MONEY);
                outDecpoint = 2;
                return true;
            }
        }

        // NUMBER(p, 0) — целое число, подбираем тип по точности
        if (scale == 0)
        {
            if (precision <= 5)
                outType = fmtt_INT;
            else if (precision <= 10)
                outType = fmtt_LONG;
            else if (precision <= 19)
                outType = fmtt_BIGINT;
            else
                outType = fmtt_NUMERIC;

            outSize = fmtTypeSize(outType);
            outDecpoint = 0;
            return true;
        }

        // Остальные NUMBER(p, s): scale сохраняем в decpoint
        // (так лежат, например, денежные суммы NUMBER(*,2) -> decpoint=2)
        outType = fmtt_NUMERIC;
        outSize = fmtTypeSize(fmtt_NUMERIC);
        outDecpoint = static_cast<qint16>(scale);

        return true;
    }

    // Определение типа и размера поля по фрагменту Oracle-декларации
    bool mapOracleType(const QString &decl, qint16 &type, qint32 &size, qint16 &decpoint)
    {
        const QString upper = decl.simplified().toUpper();
        decpoint = 0;

        if (matchNumericDecl(upper, type, size, decpoint))
            return true;

        if (matchDecl(upper, "^VARCHAR2\\s*\\((\\d+)\\)$", fmtt_STRING, true, type, size))
        {
            size++; // строка FMT хранит размер с запасом на завершающий ноль
            return true;
        }
        if (matchDecl(upper, "^VARCHAR\\s*\\((\\d+)\\)$", fmtt_STRING, true, type, size))
        {
            size++;
            return true;
        }
        if (matchDecl(upper, "^NVARCHAR2\\s*\\((\\d+)\\)$", fmtt_STRING, true, type, size))
        {
            size++;
            return true;
        }
        if (matchDecl(upper, "^SNR\\s*\\((\\d+)\\)$", fmtt_SNR, true, type, size))
        {
            size++;
            return true;
        }
        if (matchDecl(upper, "^CHAR\\s*\\((\\d+)\\)$", fmtt_CHR, true, type, size))
        {
            if (size > 1)
            {
                // CHAR(1) обычно флаговое поле, CHAR(n>1) — строка
                // с запасом на завершающий ноль, как и VARCHAR2
                type = fmtt_STRING;
                size++;
            }
            return true;
        }
        if (matchDecl(upper, "^RAW\\s*\\((\\d+)\\)$", fmtt_UCHR, true, type, size))
            return true;
        if (matchDecl(upper, "^NUMBER\\s*\\((\\d+)\\)$", fmtt_INT, true, type, size))
        {
            // По точности подбираем целочисленный тип с запасом:
            // INT хранит до 5 знаков, LONG до 10, BIGINT до 19
            if (size <= 5)
                type = fmtt_INT;
            else if (size <= 10)
                type = fmtt_LONG;
            else if (size <= 19)
                type = fmtt_BIGINT;
            else
                type = fmtt_NUMERIC;
            size = fmtTypeSize(type);
            return true;
        }
        // NUMBER без точности в Oracle — число с плавающей точностью
        // произвольной разрядности, целочисленным его считать нельзя
        if (matchDecl(upper, "^NUMBER$", fmtt_NUMERIC, false, type, size))
            return true;
        if (matchDecl(upper, "^INTEGER$", fmtt_LONG, false, type, size))
            return true;
        if (matchDecl(upper, "^BIGINT$", fmtt_BIGINT, false, type, size))
            return true;
        if (matchDecl(upper, "^SMALLINT$", fmtt_INT, false, type, size))
            return true;
        if (matchDecl(upper, "^FLOAT\\s*\\((\\d+)\\)$", fmtt_FLOAT, true, type, size))
        {
            if (size > 24)
                type = fmtt_DOUBLE;
            size = fmtTypeSize(type);
            return true;
        }
        if (matchDecl(upper, "^FLOAT$", fmtt_FLOAT, false, type, size))
            return true;
        if (matchDecl(upper, "^DOUBLE PRECISION$", fmtt_DOUBLE, false, type, size))
            return true;
        if (matchDecl(upper, "^REAL$", fmtt_FLOAT, false, type, size))
            return true;
        if (matchDecl(upper, "^DATE$", fmtt_DATE, false, type, size))
            return true;
        if (matchDecl(upper, "^TIMESTAMP(?:\\s*\\(\\d*\\))?(?:\\s+WITH\\s+\\w+\\s+TIME\\s+ZONE)?$", fmtt_DATE, false, type, size))
            return true;
        if (matchDecl(upper, "^MONEY$", fmtt_MONEY, false, type, size))
            return true;
        if (matchDecl(upper, "^NUMERIC$", fmtt_NUMERIC, false, type, size))
            return true;
        if (matchDecl(upper, "^DECIMAL$", fmtt_NUMERIC, false, type, size))
            return true;

        return false;
    }

    // Определение типа и размера поля по фрагменту PostgreSQL-декларации.
    // Соответствие типов — по карте FmtTypesMap.json (поле pgType):
    //   INT/SMALLINT -> INTEGER, LONG -> INTEGER, BIGINT -> BIGINT,
    //   FLOAT -> FLOAT(24), DOUBLE -> FLOAT(53), MONEY -> NUMERIC(19, 4),
    //   STRING/SNR -> VARCHAR, DATE/TIME -> TIMESTAMP, CHR -> CHAR,
    //   UCHR -> BYTEA, NUMERIC -> NUMERIC(32, 12)
    bool mapPostgresType(const QString &decl, qint16 &type, qint32 &size, qint16 &decpoint)
    {
        // Длина строки по умолчанию: TEXT в Postgres не ограничена,
        // для FMT берём разумный дефолт с запасом на завершающий ноль
        const qint32 DEFAULT_TEXT_LEN = 255;

        const QString upper = decl.simplified().toUpper();
        decpoint = 0;

        if (matchNumericDecl(upper, type, size, decpoint, true))
            return true;

        if (matchDecl(upper, "^VARCHAR\\s*\\((\\d+)\\)$", fmtt_STRING, true, type, size))
        {
            size++; // строка FMT хранит размер с запасом на завершающий ноль
            return true;
        }
        if (matchDecl(upper, "^CHARACTER\\s+VARYING\\s*\\((\\d+)\\)$", fmtt_STRING, true, type, size))
        {
            size++;
            return true;
        }
        // TEXT — строка без явного размера
        if (matchDecl(upper, "^TEXT$", fmtt_STRING, true, type, size))
        {
            size = DEFAULT_TEXT_LEN + 1;
            return true;
        }
        // CHAR(1) обычно флаговое поле, CHAR(n>1) — строка
        if (matchDecl(upper, "^(?:CHAR|CHARACTER|BPCHAR)\\s*\\((\\d+)\\)$", fmtt_CHR, true, type, size))
        {
            if (size > 1)
            {
                type = fmtt_STRING;
                size++;
            }
            return true;
        }
        // SERIAL — автоинкрементное целое: smallserial/int2 -> INT,
        // serial/int4 -> LONG, bigserial/int8 -> BIGINT
        if (matchDecl(upper, "^(?:SMALLINT|INT2|SMALLSERIAL|SERIAL2)$", fmtt_INT, false, type, size))
            return true;
        if (matchDecl(upper, "^(?:INTEGER|INT|INT4|SERIAL|SERIAL4)$", fmtt_LONG, false, type, size))
            return true;
        if (matchDecl(upper, "^(?:BIGINT|INT8|BIGSERIAL|SERIAL8)$", fmtt_BIGINT, false, type, size))
            return true;
        if (matchDecl(upper, "^(?:REAL|FLOAT4)$", fmtt_FLOAT, false, type, size))
            return true;
        if (matchDecl(upper, "^(?:DOUBLE\\s+PRECISION|FLOAT8)$", fmtt_DOUBLE, false, type, size))
            return true;
        if (matchDecl(upper, "^FLOAT\\s*\\((\\d+)\\)$", fmtt_FLOAT, true, type, size))
        {
            if (size > 24)
                type = fmtt_DOUBLE;
            size = fmtTypeSize(type);
            return true;
        }
        if (matchDecl(upper, "^FLOAT$", fmtt_FLOAT, false, type, size))
            return true;
        // NUMERIC/DECIMAL без точности — произвольная разрядность
        if (matchDecl(upper, "^(?:NUMERIC|DECIMAL)$", fmtt_NUMERIC, false, type, size))
            return true;
        // Денежный тип Postgres напрямую соответствует fmtt_MONEY
        if (matchDecl(upper, "^MONEY$", fmtt_MONEY, false, type, size))
            return true;
        // BOOLEAN -> флаговое CHAR(1) (fmtk_Elogical)
        if (matchDecl(upper, "^(?:BOOLEAN|BOOL)$", fmtt_CHR, false, type, size))
            return true;
        if (matchDecl(upper, "^DATE$", fmtt_DATE, false, type, size))
            return true;
        // TIMESTAMP [(p)] [WITH|WITHOUT TIME ZONE] — так DATE представлен в Postgres
        if (matchDecl(upper, "^TIMESTAMP(?:\\s*\\(\\d*\\))?(?:\\s+(?:WITH|WITHOUT)\\s+TIME\\s+ZONE)?$", fmtt_DATE, false, type, size))
            return true;
        if (matchDecl(upper, "^TIMESTAMPTZ$", fmtt_DATE, false, type, size))
            return true;
        // TIME [(p)] [WITH|WITHOUT TIME ZONE]
        if (matchDecl(upper, "^TIME(?:\\s*\\(\\d*\\))?(?:\\s+(?:WITH|WITHOUT)\\s+TIME\\s+ZONE)?$", fmtt_TIME, false, type, size))
            return true;
        if (matchDecl(upper, "^TIMETZ$", fmtt_TIME, false, type, size))
            return true;

        return false;
    }

    // Является ли фрагмент декларацией BLOB/CLOB (или их Postgres-аналогов)
    bool mapBlobType(const QString &decl, qint16 &blobType, bool postgres = false)
    {
        const QString upper = decl.simplified().toUpper();

        if (upper == QLatin1String("BLOB"))
        {
            blobType = BT_BLOB_VAR;
            return true;
        }
        if (upper == QLatin1String("CLOB"))
        {
            blobType = BT_CLOB;
            return true;
        }

        if (postgres)
        {
            // BYTEA в Postgres — аналог BLOB (см. PostgresExporter)
            if (upper == QLatin1String("BYTEA"))
            {
                blobType = BT_BLOB_VAR;
                return true;
            }
            // Документные типы Postgres храним как CLOB
            if (upper == QLatin1String("JSON") || upper == QLatin1String("JSONB") ||
                    upper == QLatin1String("XML"))
            {
                blobType = BT_CLOB;
                return true;
            }
        }

        return false;
    }

    // Разбить тело скобок на определения колонок с учётом вложенных скобок и кавычек
    QList<QString> splitColumns(const QString &body)
    {
        QList<QString> result;
        QString current;
        int depth = 0;
        bool inString = false;

        for (int i = 0; i < body.length(); i++)
        {
            const QChar ch = body.at(i);

            if (ch == QLatin1Char('\''))
            {
                inString = !inString;
                current.append(ch);
                continue;
            }

            if (!inString)
            {
                if (ch == QLatin1Char('('))
                    depth++;
                else if (ch == QLatin1Char(')'))
                    depth--;
                else if (ch == QLatin1Char(',') && depth == 0)
                {
                    result.append(current);
                    current.clear();
                    continue;
                }
            }

            current.append(ch);
        }

        if (!current.trimmed().isEmpty())
            result.append(current);

        return result;
    }

    // Разделить определение колонки на имя и декларацию типа
    bool splitColumnDecl(const QString &columnDecl, QString &name, QString &typeDecl)
    {
        const QString simplified = columnDecl.simplified();

        // Имя: либо quoted-идентификатор ("...", допустимы пробелы и не-ASCII),
        // либо обычный идентификатор Oracle
        static const QRegularExpression rxName(
                    "^(?:\"([^\"]+)\"|([A-Za-z][A-Za-z0-9_$#]*))\\s+(.+)$",
                    QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match = rxName.match(simplified);

        if (!match.hasMatch())
            return false;

        name = match.captured(1).isEmpty() ? match.captured(2) : match.captured(1);
        typeDecl = match.captured(3);

        return true;
    }

    // Первый непустой из двух захватов (quoted-либо-обычный идентификатор)
    QString pickIdent(const QRegularExpressionMatch &match, int quotedGroup, int plainGroup)
    {
        const QString quoted = match.captured(quotedGroup);
        return quoted.isEmpty() ? match.captured(plainGroup) : quoted;
    }

    // Автоопределение диалекта по маркерам текста запроса.
    // Postgres: serial-типы, bytea, text, boolean, character varying,
    // double precision, create temp/unlogged table, касты "::".
    // Oracle: varchar2, number, raw, clob/blob-колонки, global temporary,
    // storage/tablespace и т.п. Неоднозначный текст считаем Oracle
    // (поведение до появления поддержки Postgres)
    int detectDialect(const QString &text)
    {
        static const QRegularExpression rxPg(
                    "\\b(?:SMALLSERIAL|BIGSERIAL|SERIAL|BYTEA|BOOLEAN|BOOL|TIMESTAMPTZ|TIMETZ|"
                    "JSONB|JSON|XML|CHARACTER\\s+VARYING|DOUBLE\\s+PRECISION)\\b|"
                    "CREATE\\s+(?:UNLOGGED|TEMP)\\s+TABLE|::",
                    QRegularExpression::CaseInsensitiveOption);
        static const QRegularExpression rxOra(
                    "\\b(?:VARCHAR2|NVARCHAR2|NUMBER|RAW|CLOB|BLOB|NLOB|GLOBAL\\s+TEMPORARY|"
                    "TABLESPACE|STORAGE|PCTFREE|PCTUSED|INITRANS|MAXTRANS)\\b",
                    QRegularExpression::CaseInsensitiveOption);

        const bool hasPg = rxPg.match(text).hasMatch();
        const bool hasOra = rxOra.match(text).hasMatch();

        if (hasPg && !hasOra)
            return CreateTableSqlParser::DialectPostgres;

        return CreateTableSqlParser::DialectOracle;
    }

    // Паттерн необязательного префикса схемы: любой идентификатор с точкой.
    // Не привязываемся к схеме из CREATE TABLE — в COMMENT ON она может
    // быть указана, а в CREATE TABLE нет (и наоборот)
    const QString &schemaPrefixPattern()
    {
        static const QString pattern =
                "(?:\"[^\"]+\"|[A-Za-z][A-Za-z0-9_$#]*)\\s*\\.\\s*";
        return pattern;
    }

    // Извлечь комментарий из строк "COMMENT ON COLUMN ..."
    void parseColumnComments(const QString &sqlText, const QString &tableName,
                             QHash<QString, QString> &comments)
    {
        QRegularExpression rxComment(
                    QString("COMMENT\\s+ON\\s+COLUMN\\s+(?:%1)?\"?%2\"?\\s*\\.\\s*(?:\"([^\"]+)\"|([A-Za-z][A-Za-z0-9_$#]*))\\s+IS\\s+'((?:[^']|'')*)'")
                    .arg(schemaPrefixPattern(), QRegularExpression::escape(tableName)),
                    QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatchIterator it = rxComment.globalMatch(sqlText);

        while (it.hasNext())
        {
            QRegularExpressionMatch match = it.next();
            QString commentText = match.captured(3);
            commentText.replace("''", "'");
            comments.insert(pickIdent(match, 1, 2).toUpper(), commentText);
        }
    }

    // Позиция парной закрывающей скобки для скобки в openPos
    int matchParenBody(const QString &text, int openPos)
    {
        int depth = 0;
        bool inString = false;

        for (int i = openPos; i < text.length(); i++)
        {
            const QChar ch = text.at(i);

            if (ch == QLatin1Char('\''))
                inString = !inString;

            if (inString)
                continue;

            if (ch == QLatin1Char('('))
                depth++;
            else if (ch == QLatin1Char(')'))
            {
                depth--;
                if (depth == 0)
                    return i;
            }
        }

        return -1;
    }

    // Разобрать индексы из "CREATE [UNIQUE] INDEX [schema.]idx
    // ON [schema.]tbl (col [ASC|DESC], ...) <хвост STORAGE/TABLESPACE...>"
    // Индексы, созданные для другой таблицы, пропускаются.
    // Postgres-варианты: CREATE UNIQUE INDEX CONCURRENTLY ... ON ONLY tbl
    // USING btree (...)
    void parseIndeces(const QString &sqlText, const QString &tableName,
                      QList<CreateTableSqlIndex> &indeces, bool postgres)
    {
        // Номера захватов общие для обоих диалектов; Postgres-модификаторы
        // добавлены незахватывающими группами
        const QString indexNamePattern =
                postgres ? "(?:CONCURRENTLY\\s+)?(?:\"([^\"]+)\"|([A-Za-z][A-Za-z0-9_$#]*))"
                          : "(?:\"([^\"]+)\"|([A-Za-z][A-Za-z0-9_$#]*))";

        const QRegularExpression rxIndex(
                    QString("CREATE\\s+(UNIQUE\\s+)?INDEX\\s+%1"
                            "(?:\\s*\\.\\s*(?:\"([^\"]+)\"|([A-Za-z][A-Za-z0-9_$#]*)))?"
                            "\\s+ON\\s+%2"
                            "(?:\"([^\"]+)\"|([A-Za-z][A-Za-z0-9_$#]*))"
                            "(?:\\s*\\.\\s*(?:\"([^\"]+)\"|([A-Za-z][A-Za-z0-9_$#]*)))?"
                            "%3\\s*\\(")
                    .arg(indexNamePattern,
                         postgres ? "(?:ONLY\\s+)?" : QString(),
                         postgres ? "(?:\\s+USING\\s+\\w+)?" : QString()),
                    QRegularExpression::CaseInsensitiveOption);

        QRegularExpressionMatchIterator it = rxIndex.globalMatch(sqlText);

        while (it.hasNext())
        {
            QRegularExpressionMatch match = it.next();

            // Таблица из ON <schema.>table: сверяем с таблицей запроса
            QString onTable = pickIdent(match, 8, 9);
            if (onTable.isEmpty())
                onTable = pickIdent(match, 6, 7);

            if (onTable.compare(tableName, Qt::CaseInsensitive) != 0)
                continue;

            const int bodyStart = match.capturedEnd() - 1;
            const int bodyEnd = matchParenBody(sqlText, bodyStart);

            if (bodyEnd == -1)
                continue;

            CreateTableSqlIndex index;
            index.unique = !match.captured(1).isEmpty();

            index.name = pickIdent(match, 4, 5); // имя индекса было со схемой
            if (index.name.isEmpty())
                index.name = pickIdent(match, 2, 3);

            const QList<QString> cols = splitColumns(
                        sqlText.mid(bodyStart + 1, bodyEnd - bodyStart - 1));

            for (const QString &col : cols)
            {
                QString colName = stripLineComment(col).simplified();

                // Отбрасываем сортировку сегмента: T_ID ASC -> T_ID
                colName.remove(QRegularExpression("\\s+(ASC|DESC)\\s*$",
                                                  QRegularExpression::CaseInsensitiveOption));

                if (colName.startsWith(QLatin1Char('"')))
                {
                    // Quoted-идентификатор колонки
                    const int endQuote = colName.indexOf(QLatin1Char('"'), 1);
                    if (endQuote != -1)
                        colName = colName.mid(1, endQuote - 1);
                }
                else
                    colName = colName.section(QLatin1Char(' '), 0, 0);

                // Функциональные сегменты (напр. UPPER(...)) восстановить нельзя
                if (colName.startsWith(QLatin1Char('(')))
                    continue;

                if (!colName.isEmpty())
                    index.columns.append(colName);
            }

            if (!index.columns.isEmpty())
                indeces.append(index);
        }
    }

    // Поиск поля таблицы по имени колонки из CREATE INDEX:
    // имена полей в FMT хранятся с префиксом t_, сравниваем без него
    // (T_ID в запросе -> t_Id в FMT)
    FmtField *findFieldByName(FmtTable *table, const QString &name)
    {
        QString searchName = name;
        if (searchName.startsWith(QLatin1String("t_"), Qt::CaseInsensitive))
            searchName = searchName.mid(2);

        for (qint32 i = 0; i < table->fieldsCount(); i++)
        {
            FmtField *fld = table->field(static_cast<qint16>(i));

            if (!fld)
                continue;

            QString fldName = fld->name();
            if (fldName.startsWith(QLatin1String("t_"), Qt::CaseInsensitive))
                fldName = fldName.mid(2);

            if (fldName.compare(searchName, Qt::CaseInsensitive) == 0)
                return fld;
        }

        return Q_NULLPTR;
    }

    // Извлечь комментарий таблицы из "COMMENT ON TABLE ..."
    QString parseTableComment(const QString &sqlText, const QString &tableName)
    {
        QRegularExpression rxComment(
                    QString("COMMENT\\s+ON\\s+TABLE\\s+(?:%1)?\"?%2\"?\\s+IS\\s+'((?:[^']|'')*)'")
                    .arg(schemaPrefixPattern(), QRegularExpression::escape(tableName)),
                    QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = rxComment.match(sqlText);

        if (!match.hasMatch())
            return QString();

        QString commentText = match.captured(1);
        commentText.replace("''", "'");

        return commentText;
    }
}

namespace CreateTableSqlParser
{
    bool parse(const QString &sqlText, CreateTableSqlResult &result, int dialect)
    {
        result = CreateTableSqlResult();

        // Убираем блочные комментарии /* ... */
        QString text = sqlText;
        text.remove(QRegularExpression("/\\*.*\\*/", QRegularExpression::DotMatchesEverythingOption));

        if (dialect == DialectAuto)
            dialect = detectDialect(text);
        result.dialect = dialect;

        const bool isPostgres = (dialect == DialectPostgres);

        // Ищем CREATE TABLE. Временная: Oracle GLOBAL TEMPORARY TABLE,
        // Postgres TEMP/TEMPORARY TABLE. Postgres UNLOGGED TABLE
        // обрабатываем как обычную таблицу
        QRegularExpression rxCreate(
                    "CREATE\\s+(?:GLOBAL\\s+)?TEMP(?:ORARY)?\\s+TABLE\\s+",
                    QRegularExpression::CaseInsensitiveOption);
        QRegularExpression rxCreatePlain(
                    isPostgres ? "CREATE\\s+(?:UNLOGGED\\s+)?TABLE\\s+"
                               : "CREATE\\s+TABLE\\s+",
                    QRegularExpression::CaseInsensitiveOption);

        QRegularExpressionMatch match = rxCreate.match(text);
        if (match.hasMatch())
            result.isTemporary = true;
        else
            match = rxCreatePlain.match(text);

        if (!match.hasMatch())
        {
            result.error = QObject::tr("Не найден запрос CREATE TABLE");
            return false;
        }

        QString tail = text.mid(match.capturedEnd());

        // Имя таблицы (возможно со схемой; quoted-идентификаторы поддерживаются)
        static const QRegularExpression rxName(
                    "^\\s*(?:\"([^\"]+)\"|([A-Za-z][A-Za-z0-9_$#]*))"
                    "\\s*(?:\\.\\s*(?:\"([^\"]+)\"|([A-Za-z][A-Za-z0-9_$#]*)))?\\s*\\(",
                    QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch nameMatch = rxName.match(tail);

        if (!nameMatch.hasMatch())
        {
            result.error = QObject::tr("Не удалось определить имя таблицы");
            return false;
        }

        const QString secondName = pickIdent(nameMatch, 3, 4);
        if (!secondName.isEmpty())
            result.tableName = secondName; // было schema.table
        else
            result.tableName = pickIdent(nameMatch, 1, 2);

        // Тело скобок: от первой '(' до парной закрывающей
        int bodyStart = nameMatch.capturedEnd() - 1;
        int depth = 0;
        int bodyEnd = -1;
        bool inString = false;

        for (int i = bodyStart; i < tail.length(); i++)
        {
            const QChar ch = tail.at(i);

            if (ch == QLatin1Char('\''))
                inString = !inString;

            if (inString)
                continue;

            if (ch == QLatin1Char('('))
                depth++;
            else if (ch == QLatin1Char(')'))
            {
                depth--;
                if (depth == 0)
                {
                    bodyEnd = i;
                    break;
                }
            }
        }

        if (bodyEnd == -1)
        {
            result.error = QObject::tr("Не найдена закрывающая скобка в определении таблицы");
            return false;
        }

        const QString body = tail.mid(bodyStart + 1, bodyEnd - bodyStart - 1);

        // Комментарии на колонки
        QHash<QString, QString> columnComments;
        parseColumnComments(text, result.tableName, columnComments);
        result.comment = parseTableComment(text, result.tableName);

        QList<QString> columns = splitColumns(body);

        for (int i = 0; i < columns.count(); i++)
        {
            QString decl = stripLineComment(columns[i]);
            decl = trimComma(decl);

            if (decl.isEmpty())
                continue;

            // Пропускаем ограничения (PK, FK, UNIQUE, CHECK, CONSTRAINT).
            // EXCLUDE — Postgres-аналог CHECK для операторных классов
            if (decl.contains(QRegularExpression("^(CONSTRAINT|PRIMARY\\s+KEY|FOREIGN\\s+KEY|UNIQUE|CHECK|EXCLUDE|SUPPLEMENTAL\\s+LOG)\\b",
                                                 QRegularExpression::CaseInsensitiveOption)))
                continue;

            QString fldName, fldTypeDecl;
            if (!splitColumnDecl(decl, fldName, fldTypeDecl))
            {
                result.error = QObject::tr("Не удалось разобрать определение поля: %1").arg(decl);
                return false;
            }

            QString typeOnly = fldTypeDecl.simplified();

            if (isPostgres)
            {
                // Postgres-модификаторы колонки: отсекаем всё начиная с
                // первого из них, чтобы "integer NOT NULL DEFAULT 0 PRIMARY KEY"
                // превратилось в "integer"
                static const QRegularExpression rxModifiers(
                            "\\s+(?:NOT\\s+NULL|NULL|DEFAULT|PRIMARY\\s+KEY|UNIQUE|CHECK|"
                            "REFERENCES|COLLATE|GENERATED)\\b",
                            QRegularExpression::CaseInsensitiveOption);
                const int modifierPos = typeOnly.indexOf(rxModifiers);
                if (modifierPos != -1)
                    typeOnly = typeOnly.left(modifierPos);
            }
            else
            {
                // Отсекаем завершающие модификаторы декларации: ENABLE, NOT NULL/NULL
                typeOnly.remove(QRegularExpression("\\s+ENABLE\\s*$",
                                                   QRegularExpression::CaseInsensitiveOption));
                typeOnly.remove(QRegularExpression("\\s+NOT\\s+NULL\\s*$",
                                                   QRegularExpression::CaseInsensitiveOption));
                typeOnly.remove(QRegularExpression("\\s+NULL\\s*$",
                                                   QRegularExpression::CaseInsensitiveOption));

                // DEFAULT <выражение> — отсекаем всё начиная с DEFAULT
                int defaultPos = typeOnly.indexOf(QRegularExpression("\\s+DEFAULT\\s+",
                                                                     QRegularExpression::CaseInsensitiveOption));
                if (defaultPos != -1)
                    typeOnly = typeOnly.left(defaultPos);
            }

            typeOnly = typeOnly.simplified();

            // Убираем модификаторы длины CHAR/BYTE: VARCHAR2(35 BYTE), VARCHAR2(120 CHAR)
            typeOnly.replace(QRegularExpression("\\s+(CHAR|BYTE)\\s*\\)",
                                                QRegularExpression::CaseInsensitiveOption),
                             QLatin1String(")"));
            typeOnly.remove(QRegularExpression("\\s+(CHAR|BYTE)\\s*$",
                                               QRegularExpression::CaseInsensitiveOption));

            const QString commentForField = columnComments.value(fldName.toUpper());

            // BLOB/CLOB-колонка (t_fmtblobdata_xxx) — не поле, а параметр
            // самой FMT-таблицы; в список полей не добавляем
            qint16 blobType = 0;
            if (mapBlobType(typeOnly, blobType, isPostgres))
            {
                result.hasBlob = true;
                result.blobType = blobType;
                continue;
            }

            CreateTableSqlField field;
            field.name = fldName;

            const bool typeMapped = isPostgres
                    ? mapPostgresType(typeOnly, field.type, field.size, field.decpoint)
                    : mapOracleType(typeOnly, field.type, field.size, field.decpoint);

            if (!typeMapped)
            {
                result.error = QObject::tr("Неизвестный тип поля %1: %2").arg(fldName, typeOnly);
                return false;
            }

            field.comment = commentForField;
            result.fields.append(field);
        }

        if (result.fields.isEmpty())
        {
            result.error = QObject::tr("В запросе не найдено ни одного поля");
            return false;
        }

        // Снимаем префикс T_ индивидуально у каждого поля:
        // FmtTable::addFieldPrivate сама добавит префикс "t_", а
        // camelCase-преобразование в setName останется включённым
        // (t_IdStep и т.п.). Если префикс не снять, получится t_T_IdStep.
        for (CreateTableSqlField &fld : result.fields)
        {
            if (fld.name.startsWith(QLatin1String("T_"), Qt::CaseInsensitive))
                fld.name = fld.name.mid(2);
        }

        // Индексы из CREATE [UNIQUE] INDEX ... ON <таблица>
        parseIndeces(text, result.tableName, result.indeces, isPostgres);

        return true;
    }

    QSharedPointer<FmtTable> createTable(ConnectionInfo *current,
                                         const CreateTableSqlResult &result,
                                         const QString &tableName,
                                         const QString &tableComment)
    {
        QSharedPointer<FmtTable> table(new FmtTable(current));

        // Флаг временной таблицы (GLOBAL TEMPORARY) берём из запроса.
        // Безусловно выставлять его нельзя: fmtnf_Temp заставит создать
        // таблицу в БД как GLOBAL TEMPORARY TABLE
        table->setIsTemporary(result.isTemporary);
        table->setName(tableName);
        if (!tableComment.isEmpty())
            table->setComment(tableComment);

        // BLOB/CLOB в FMT является свойством таблицы, а не отдельным полем
        if (result.hasBlob)
        {
            table->setBlobType(result.blobType);
            table->setBlobLen(DEFAULT_BLOB_LEN);
        }

        // Наполняем полями нужных типов и размеров
        for (const CreateTableSqlField &fld : result.fields)
        {
            FmtField *field = table->addField(fld.name, fld.type);
            if (!field)
                continue;

            field->setSize(fld.size);

            if (fld.decpoint > 0)
                field->setDecpoint(fld.decpoint);

            if (!fld.comment.isEmpty())
                field->setComment(fld.comment);
        }

        // Восстанавливаем индексы из CREATE [UNIQUE] INDEX:
        // уникальность через setDup (дубликаты разрешены для не-unique),
        // сегменты привязываем к полям созданной таблицы
        for (const CreateTableSqlIndex &idx : result.indeces)
        {
            FmtIndex *index = table->addIndex();

            if (!index)
                break;

            if (!idx.name.isEmpty())
                index->setName(idx.name);

            index->setDup(!idx.unique);

            for (const QString &col : idx.columns)
            {
                FmtField *fld = findFieldByName(table.data(), col);

                if (!fld)
                    continue;

                FmtSegment *segment = index->addSegment(index->segmentsCount());

                if (segment)
                    segment->setField(fld);
            }
        }

        return table;
    }
}