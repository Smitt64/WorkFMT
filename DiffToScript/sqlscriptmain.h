#ifndef SQLSCRIPTMAIN_H
#define SQLSCRIPTMAIN_H

#include <QTextStream>

#include "join.h"
#include "dbspelling.h"
#include "diffconnection.h"
#include <QSharedPointer>

class SqlScriptMain
{
public:
    SqlScriptMain(QSharedPointer<DbSpelling>& dbSpelling, QSharedPointer<DiffConnection> connection);

    QStringList makeVariables(JoinTable* joinTable);
    QString buildVariableName(const DatTable* datTable);
    int getAutoincIndex(const DatTable* datTable);
    QString buildVariable(const DatTable* datTable);

    QVector<int> indexesOfKeyFields(const JoinTable *joinTable);
    QVector<int> indexesOfUniqueIndex(const JoinTable *joinTable);

    void replaceForeignAutoinc(const JoinTable* joinTable, DatRecord& rec);
    int indexForeignAutoinc(const JoinTable* joinTable, const DatRecord& rec);
    int getForeignReplaceField(const JoinTable* joinTable, DatRecord& rec);

    QStringList buildWhere(const JoinTable* joinTable, const DatRecord& rec);
    int buildInsertStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex);
    int buildDeleteStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex);
    int buildUpdateStatement(QTextStream &os, const JoinTable *joinTable, QStringList &sql, int recIndex, int newIndex);
    int buildStatement(QTextStream& os, const JoinTable* joinTable,
                       QStringList& sql,
                       int recIndex,
                       Join *childJoin = nullptr,
                       const QStringList &ParentValuesByIndex = QStringList());
    int buildChildStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex);
    void build(QTextStream& os, JoinTable* joinTable);

    void dateSpelling(const JoinTable* joinTable, DatRecord& rec);
    void stringSpelling(const JoinTable* joinTable, DatRecord& rec);

private:
    QSharedPointer<DiffConnection> _connection;
    QSharedPointer<DbSpelling> _dbSpelling;
};

#endif // SQLSCRIPTMAIN_H
