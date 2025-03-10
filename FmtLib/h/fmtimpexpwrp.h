#ifndef FMTIMPEXPWRP_H
#define FMTIMPEXPWRP_H

#include <QObject>
#include <QWidget>
#include "connectioninfo.h"
#include "fmtlib_global.h"

class QProcess;
class ErrorsModel;
class FMTLIBSHARED_EXPORT FmtImpExpWrp : public QObject
{
    Q_OBJECT
public:
    explicit FmtImpExpWrp(ConnectionInfo *connection, QObject *parent = Q_NULLPTR);
    virtual ~FmtImpExpWrp();

signals:
    void finished(int exitCode = 0);
    void started();
    void error();

public slots:
    bool isRunning();
    bool isNoXsd();
    void cancel();
    void addTable(const QString &table);
    void addTable(const QStringList &tables);
    void removeTable(const QString &table);
    void clearTables();

    int tablesCount();

    bool isExists(const QString &table);
    QString connectionString() const;
    QString codePage() const;
    QString programName() const;
    QString lastImportDir() const;

    QString protocol() const;
    void parseProtocol(ErrorsModel *ptr);

    void setDsn(const QString &dsn);

    int importDir(const QString &impdir);
    void importFile(const QString &file);
    void exportTable(const QString &dir, bool waitForFinished = false, bool waitForStarted = false);
    //void exportTableEventLoop(const QString &dir);

private slots:
    void processError(QProcess::ProcessError error);
    void processReadyReadStandardOutput();

private:
    void getArgs(const QString &dir, QStringList &arg);
    QSettings *m_pPrm;
    QString FileContent(const QString &filename) const;
    QDir m_WorkDir;
    QProcess *pFmtXml;
    QString m_Protocol;
    ConnectionInfo *pConnection;
    QStringList m_Tables;
    QString m_dsn;
};

#endif // FMTIMPEXPWRP_H
