#ifndef SQLSCRIPTMAIN_H
#define SQLSCRIPTMAIN_H

#include <QTextStream>
#include <QMapIterator>
#include "join.h"
#include "dbspelling.h"
#include "diffconnection.h"
#include "taskoptions.h"
#include <QSharedPointer>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

enum SqlScriptBuildErrors
{
    // Различается состав полей в dat и fmt
    SqlBuildError_FieldComposition = 1000,
};

extern QString Padding(int depth = 1);

class FunctionInfo;
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
    Q_INVOKABLE QStringList dropInserFunctions(const QString &scheme = QString());
    Q_INVOKABLE QStringList dropInserFunctions(QObject *spelling, const QString &scheme = QString());
    Q_INVOKABLE QStringList getInserFunctionsNames();
    //Q_INVOKABLE void getInserFunction(const QString &name, QString &returnType, QString &fullname);

    QVector<int> indexesOfKeyFields_(const JoinTable *joinTable);
    QVector<int> indexesOfUniqueIndex_(const JoinTable *joinTable);

    Q_INVOKABLE QVariantList indexesOfUniqueIndex(const JoinTable *joinTable);
    Q_INVOKABLE QVariantList indexesOfKeyFields(const JoinTable *joinTable);

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
    int build(QStringList& sql, JoinTable* joinTable, const TaskOptions &opts);

    Q_INVOKABLE int stdBuildChildStatement(const JoinTable* joinTable, QStringList& sql, int recIndex, const int depth = 0);

    Q_INVOKABLE void dateSpelling(const JoinTable* joinTable, DatRecord *rec);
    Q_INVOKABLE void stringSpelling(const JoinTable* joinTable, DatRecord *rec);

    Q_INVOKABLE void checkDatFldsCount(QStringList& sql, JoinTable *joinTable);

    Q_INVOKABLE void toScript(const JoinTable* joinTable, DatRecord *rec);
    Q_INVOKABLE QStringList disableEnableAutoIncTrigger(ScriptTable* datTable, bool enable = false, int depth = 1);

private:
    QSharedPointer<DiffConnection> _connection;
    QSharedPointer<DbSpelling> _dbSpelling;

    QMap<QString, FunctionInfo*> m_InsertFunctions;
};

#endif // SQLSCRIPTMAIN_H
