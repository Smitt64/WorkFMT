#ifndef FMTDBFTOOLWRP_H
#define FMTDBFTOOLWRP_H

#include <QObject>
#include <QProcess>

class ConnectionInfo;
class FmtErrors;
class FmtDbfToolWrp : public QObject
{
    Q_OBJECT
public:
    explicit FmtDbfToolWrp(ConnectionInfo *info, QObject *parent = nullptr);
    virtual ~FmtDbfToolWrp();

    FmtErrors *fmterrors();
    void setDsn(const QString &dsn);

signals:
    void started();
    void startError();
    void finished(int exitCode = 0);

public slots:
    void unload(const QString &ExportDir, const QString &dbt);
    void load(const QString &dbt);
    void stop();

private slots:
    void readyReadStandardError();
    void readyReadStandardOutput();

private:
    QTextCodec *codec;
    FmtErrors *err;
    QScopedPointer<QProcess> proc;
    ConnectionInfo *pInfo;
    QString m_dsn;
};

#endif // FMTDBFTOOLWRP_H
