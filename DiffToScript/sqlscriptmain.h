#ifndef SQLSCRIPTMAIN_H
#define SQLSCRIPTMAIN_H

#include <QTextStream>
#include <QMapIterator>
#include "join.h"
#include "dbspelling.h"
#include "diffconnection.h"
#include <QSharedPointer>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

typedef struct
{
    QString name, returnType;
    QString fullname;
}InsertFunction;

enum SqlScriptBuildErrors
{
    // Различается состав полей в dat и fmt
    SqlBuildError_FieldComposition = 1000,
};

QString Padding(int depth = 1);

class SqlScriptMain
{
public:
    SqlScriptMain(QSharedPointer<DbSpelling>& dbSpelling, QSharedPointer<DiffConnection> connection);

    QString undecorateTableName(const QString &name) const;
    QStringList makeVariables(JoinTable* joinTable);
    QStringList makeInsertFunctions(JoinTable* joinTable);

    QString buildVariableName(const ScriptTable* datTable);
    int getAutoincIndex(const ScriptTable* datTable);

    QString buildVariable(const ScriptTable* datTable);
    QStringList buildInsertFunctions(const ScriptTable* datTable);

    QVector<int> indexesOfKeyFields(const JoinTable *joinTable);
    QVector<int> indexesOfUniqueIndex(const JoinTable *joinTable);

    void replaceForeignAutoinc(const JoinTable* joinTable, DatRecord& rec);
    int indexForeignAutoinc(const JoinTable* joinTable, const DatRecord& rec);
    int getForeignReplaceField(const JoinTable* joinTable, DatRecord& rec);

    QStringList buildWhere(const JoinTable* joinTable, const DatRecord& rec);
    int buildInsertStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex, const QString &childPadding = QString());
    int buildDeleteStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex);
    int buildUpdateStatement(QTextStream &os, const JoinTable *joinTable, QStringList &sql, int recIndex, int newIndex);
    int buildStatement(QTextStream& os, JoinTable* joinTable,
                       QStringList& sql,
                       int recIndex,
                       Join *childJoin = nullptr,
                       const QStringList &ParentValuesByIndex = QStringList(),
                       const QString &childPadding = QString());
    int buildChildStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex, const QString &childPadding = QString());
    int build(QTextStream& os, JoinTable* joinTable);

    void dateSpelling(const JoinTable* joinTable, DatRecord& rec);
    void stringSpelling(const JoinTable* joinTable, DatRecord& rec);

    void checkDatFldsCount(QStringList& sql, JoinTable *joinTable);

    void toScript(const JoinTable* joinTable, DatRecord& rec);
private:
    QSharedPointer<DiffConnection> _connection;
    QSharedPointer<DbSpelling> _dbSpelling;

    QMap<QString, InsertFunction> m_InsertFunctions;
};

#endif // SQLSCRIPTMAIN_H
