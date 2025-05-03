#ifndef DIFFTABLEINFO_H
#define DIFFTABLEINFO_H

#include <QObject>
#include <difffield.h>
#include <fmttable.h>
#include <diffcore.h>
#include "toolsruntime.h"
#include "tablelinks.h"

class DiffTableInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QStringList realFields READ getRealFields CONSTANT)
    Q_PROPERTY(DiffFields *fields READ getFields CONSTANT)
    Q_PROPERTY(DiffFields *uniqFields READ getUniqFields CONSTANT)
    Q_PROPERTY(DatIndexes *indexes READ getIndexes CONSTANT)
public:
    DiffTableInfo();
    virtual ~DiffTableInfo();

    QString name;
    DiffFields fields;
    DatIndexes indexes;
    DiffFields uniqFields; //Ключевые поля записи таблицы
    QStringList realFields;

    void loadFromFmt(FmtTable* fmtTable, const QString &datfilename = QString()); //TODO replace FmtTable to QString
    void InitUniqFields(TableLinks* tableLink);

    DatIndex *firstUniq(bool skipAutoInc) const;

    Q_INVOKABLE DiffField *field(const QString &name) const;
    Q_INVOKABLE DiffFields *missingFldInDat() const;

    const QString &getName() const;
    const QStringList &getRealFields() const;
    DiffFields *getFields();
    DiffFields *getUniqFields();
    DatIndexes *getIndexes();

private:
    DiffFields *pMissingFldInDat;
    QStringList readColumnsFromFile(const QString& filePath);
};

class DiffTable: public DiffTableInfo
{
    Q_OBJECT
    Q_PROPERTY(DatRecords *records READ getRecords CONSTANT)
public:
    DiffTable();
    virtual ~DiffTable() = default;

    DatRecords records;
    DatRecords *getRecords();
};

#endif // DIFFTABLEINFO_H
