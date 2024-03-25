#ifndef SQLSCRIPTMAIN_H
#define SQLSCRIPTMAIN_H

#include <QTextStream>

#include "join.h"
#include "dbspelling.h"

class SqlScriptMain
{
public:
    SqlScriptMain(QScopedPointer<DbSpelling>& dbSpelling);

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
    int buildStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex);
    int buildChildStatement(QTextStream& os, const JoinTable* joinTable, QStringList& sql, int recIndex);
    void build(QTextStream& os, JoinTable* joinTable);

    void dateSpelling(const JoinTable* joinTable, DatRecord& rec);
private:
    QScopedPointer<DbSpelling> _dbSpelling;

};

#endif // SQLSCRIPTMAIN_H
