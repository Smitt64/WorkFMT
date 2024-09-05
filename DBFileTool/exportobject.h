#ifndef EXPORTOBJECT_H
#define EXPORTOBJECT_H

#include <QDir>
#include <QObject>

class ExportObject : public QObject
{
    Q_OBJECT
public:
    explicit ExportObject(QObject *parent = nullptr);
    virtual ~ExportObject();

    void setConnectionInfo(const QString &user, const QString &pswd, const QString &dsn, bool unicode);
    void exportTable(const QString &table, const QDir &outdir);

signals:

private:
    QString _user, _pswd, _dsn;
    bool _unicode;
};

#endif // EXPORTOBJECT_H
