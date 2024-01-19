#ifndef SQLBUILDER_H
#define SQLBUILDER_H

#include <QObject>
#include <QTextStream>

class SqlBuilder : public QObject
{
    Q_OBJECT
public:
    explicit SqlBuilder(QObject *parent = nullptr);
    void buildInsert(QTextStream& os, QStringList insList, QString tableName);

signals:

};

#endif // SQLBUILDER_H
