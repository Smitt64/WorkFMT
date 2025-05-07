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

class SqlScriptMain : public QObject
{
    Q_OBJECT
public:
    SqlScriptMain(QSharedPointer<DbSpelling>& dbSpelling, QSharedPointer<DiffConnection> connection);

    Q_INVOKABLE QString undecorateTableName(const QString &name) const;
    Q_INVOKABLE QStringList makeVariables(JoinTable* joinTable);
    Q_INVOKABLE QStringList makeInsertFunctions(JoinTable* joinTable);

    Q_INVOKABLE QString buildVariableName(const ScriptTable* datTable);
    Q_INVOKABLE int getAutoincIndex(const ScriptTable* datTable);

    Q_INVOKABLE QString buildVariable(const ScriptTable* datTable);
    Q_INVOKABLE QStringList buildInsertFunctions(const ScriptTable* datTable);

    QVector<int> indexesOfKeyFields(const JoinTable *joinTable);
    QVector<int> indexesOfUniqueIndex(const JoinTable *joinTable);

    Q_INVOKABLE void replaceForeignAutoinc(const JoinTable* joinTable, DatRecord *rec);
    Q_INVOKABLE int indexForeignAutoinc(const JoinTable* joinTable, const DatRecord *rec);
    Q_INVOKABLE int getForeignReplaceField(const JoinTable* joinTable, DatRecord& rec);

    Q_INVOKABLE QStringList buildWhere(const JoinTable* joinTable, const DatRecord *rec);

    int buildInsertStatement(const JoinTable* joinTable, QStringList& sql, int recIndex, const QString &childPadding = QString());
    int buildDeleteStatement(const JoinTable* joinTable, QStringList& sql, int recIndex);
    int buildUpdateStatement(const JoinTable *joinTable, QStringList &sql, int recIndex, int newIndex);
    int buildStatement(JoinTable* joinTable,
                       QStringList& sql,
                       int recIndex,
                       Join *childJoin = nullptr,
                       const QStringList &ParentValuesByIndex = QStringList(),
                       const QString &childPadding = QString());
    int buildChildStatement(const JoinTable* joinTable, QStringList& sql, int recIndex, const QString &childPadding = QString());
    int build(QStringList& sql, JoinTable* joinTable);

    Q_INVOKABLE void dateSpelling(const JoinTable* joinTable, DatRecord *rec);
    Q_INVOKABLE void stringSpelling(const JoinTable* joinTable, DatRecord *rec);

    Q_INVOKABLE void checkDatFldsCount(QStringList& sql, JoinTable *joinTable);

    Q_INVOKABLE void toScript(const JoinTable* joinTable, DatRecord *rec);

private:
    QSharedPointer<DiffConnection> _connection;
    QSharedPointer<DbSpelling> _dbSpelling;

    QMap<QString, InsertFunction> m_InsertFunctions;
};

#endif // SQLSCRIPTMAIN_H
