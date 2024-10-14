#ifndef FMTDBFTOOLWRP_H
#define FMTDBFTOOLWRP_H

#include <QObject>
#include <QProcess>

class ConnectionInfo;
class ErrorsModel;
class FmtDbfToolWrp : public QObject
{
    Q_OBJECT
public:
    explicit FmtDbfToolWrp(ConnectionInfo *info, QObject *parent = nullptr);
    virtual ~FmtDbfToolWrp();

    ErrorsModel *errorsModel();
    void setDsn(const QString &dsn);

signals:
    void started();
    void startError();
    void finished(int exitCode = 0);

public slots:
    void unload(const QString &ExportDir, const QString &dbt,
                const int &DefaultAction = 0,
                const bool &UseOld = false);
    void load(const QString &dbt);
    void stop();

private slots:
    void readyReadStandardError();
    void readyReadStandardOutput();

private:
    QTextCodec *codec;
    ErrorsModel *err;
    QScopedPointer<QProcess> proc;
    ConnectionInfo *pInfo;
    QString m_dsn;
};

#endif // FMTDBFTOOLWRP_H
