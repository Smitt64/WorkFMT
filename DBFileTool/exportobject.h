#ifndef EXPORTOBJECT_H
#define EXPORTOBJECT_H

#include <QDir>
#include <QObject>

class ConnectionInfo;
class ExportObject : public QObject
{
    Q_OBJECT
public:
    enum ClobMode
    {
        ClobMode_Simplified = 0,
        ClobMode_SimplifiedTrimmed,
        ClobMode_SplitFile,
    };
    explicit ExportObject(QObject *parent = nullptr);
    virtual ~ExportObject();

    void setConnectionInfo(const QString &user, const QString &pswd, const QString &dsn, bool unicode);
    void exportTable(const QString &table, const QDir &outdir);
    void exportTable(const QStringList &table, const QDir &outdir);

    void setClobMode(const ClobMode &mode);

signals:

private:
    bool HasClobFields(const QString &table, ConnectionInfo *connection);
    QString _user, _pswd, _dsn;
    bool _unicode;
    ClobMode m_ClobMode;
};

#endif // EXPORTOBJECT_H
