#ifndef RECORDPARSER_H
#define RECORDPARSER_H

#include <QObject>
#include <QTextStream>
#include "DiffToScriptl_global.h"
#include "difffield.h"

//string
//  ' char '
// SOH

//char
//  !'
//  char ''

//substring
//  '' string ''

class QSqlDatabase;
class DIFFTOSCRIPTL_EXPORT RecordParser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList values READ getValues CONSTANT)
    Q_PROPERTY(QStringList errors READ getErrors CONSTANT)
public:
    Q_INVOKABLE RecordParser(DiffFields* datTable, const QStringList &realFields, QObject *parent = nullptr);

    Q_INVOKABLE bool parseRecord(QString line);
    QStringList getValues() const {return _values; }
    QStringList getErrors() const {return _errors;}

private:
    DiffField *field(const QString &name);
    QString getToken(QTextStream& is);
    bool parseString(QTextStream& is, QString& value);
    bool parseValue(QTextStream& is, QString& value);

    QStringList _values;
    DiffFields* _diffFields;
    QStringList _realFields;
    QStringList _errors;
};

class DiffTable;
class SqlDatabase;

DIFFTOSCRIPTL_EXPORT QString diffCreateTableForSqlite(DiffTable *table);
DIFFTOSCRIPTL_EXPORT QString diffCreateChangesTableForSqlite(DiffTable *table);
DIFFTOSCRIPTL_EXPORT bool diffLoadDatToSqlite(const QString &filename, SqlDatabase *Connection, DiffTable *table, bool changes = false);

DIFFTOSCRIPTL_EXPORT bool diffLoadDatToSqlite(const QString &filename, QSqlDatabase &Connection, DiffTable *table, bool changes);

#endif // RECORDPARSER_H
