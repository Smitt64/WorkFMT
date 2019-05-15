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
    explicit FmtDbfToolWrp(ConnectionInfo *info, QObject *parent = 0);
    virtual ~FmtDbfToolWrp();

    FmtErrors *fmterrors();

signals:
    void started();
    void startError();

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
};

#endif // FMTDBFTOOLWRP_H
