#ifndef ORACLEBUILDER_H
#define ORACLEBUILDER_H

#include "dbspelling.h"

class OracleSpelling : public DbSpelling
{
public:
    //explicit OracleSpelling(QObject *parent = nullptr);
    QString toDate(QString value) override;
    void setSpelling(const DiffFields& fields, QStringList& values) override;
};

#endif // ORACLEBUILDER_H
