#ifndef DIFFPARSER_H
#define DIFFPARSER_H

#include <QObject>
#include <QTextStream>

class DiffParser : public QObject
{
    Q_OBJECT
public:
    explicit DiffParser(QObject *parent = nullptr);
    void parseDoc(QTextStream& is);
    const QStringList& getInsList() const {return insList;};
    QString getTableName() const {return newTable;};

signals:
    void finished();

private:
    QStringList updList;
    QStringList delList;
    QStringList insList;
    QString newTable;
    QString oldTable;
    QString getToken(QTextStream& is);
    bool parseNewLine(QTextStream& is);
    void parseNewLines(QTextStream& is);
    bool parseOldLine(QTextStream& is);
    void parseOldLines(QTextStream& is);
};

#endif // DIFFPARSER_H
