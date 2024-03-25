#ifndef SQLSCRIPT_H
#define SQLSCRIPT_H

#include <QObject>
#include <QTextStream>

#include "dattable.h"
#include "dbspelling.h"
#include "autoincprocess.h"
#include "join.h"
#include "wherescript.h"

#include <memory>


class SqlScript : public QObject
{
    Q_OBJECT
public:
    bool excludeAutoInc;
    explicit SqlScript(DatTable *datTable, DbSpelling* dbSpelling, WhereScript* whereScript, QObject *parent = nullptr);

    DatRecords::iterator buildInsert(QTextStream &os, const IndexFields& fields, DatRecords::iterator first, DatRecords::iterator last);
    DatRecords::iterator buildDelete(QTextStream &os, const IndexFields& fields, DatRecords::iterator first, DatRecords::iterator last);
    DatRecords::iterator buildUpdate(QTextStream &os, const IndexFields& fields, DatRecords::iterator first, DatRecords::iterator last);
    void buildScript(QTextStream& os);
    void setAutoIncProcess(AutoIncProcess* autoIncProcess);
signals:
private:
    DatTable *_datTable;
    DbSpelling *_dbSpelling;
    WhereScript* _whereScript;
    std::unique_ptr<AutoIncProcess> _autoIncProcess;
    FieldList _index;
    IndexFields getIndexFieldColumns(const DiffFields& fields) const;
    QStringList getValues(const DatRecord& record) const;
    bool checkRecord(const DiffFields& fields, const DatRecord& record, QTextStream& os) const;

};

#endif // SQLSCRIPT_H
