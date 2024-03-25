#ifndef RECORDPARSER_H
#define RECORDPARSER_H

#include <QObject>
#include <QTextStream>

#include "difffield.h"

//string
//  ' char '
// SOH

//char
//  !'
//  char ''

//substring
//  '' string ''


class RecordParser : public QObject
{
    Q_OBJECT
public:
    explicit RecordParser(DiffFields* datTable, QObject *parent = nullptr);
    bool parseRecord(QString line);
    QStringList getValues() const {return _values; }
    QStringList getErrors() const {return _errors;}
signals:
private:
    QString getToken(QTextStream& is);
    bool parseString(QTextStream& is, QString& value);
    bool parseValue(QTextStream& is, QString& value);
    QStringList _values;
    DiffFields* _diffFields;
    QStringList _errors;
};

#endif // RECORDPARSER_H
