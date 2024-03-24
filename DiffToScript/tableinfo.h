#ifndef TABLEINFO_H
#define TABLEINFO_H

#include <QObject>

class TableInfo : public QObject
{
    Q_OBJECT
public:
    explicit TableInfo(QObject *parent = nullptr);
    void load(QString tableName);

signals:

};

#endif // TABLEINFO_H
