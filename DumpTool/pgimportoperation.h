#ifndef PGIMPORTOPERATION_H
#define PGIMPORTOPERATION_H

#include <QDir>
#include "baseoperation.h"

class QProcess;
class PgImportOperation : public BaseOperation
{
    Q_OBJECT
public:
    PgImportOperation(DumpToolWizard *wizzard);
    virtual ~PgImportOperation();

    virtual void run();
    virtual void highlighter(BaseLogHighlighter **obj, QTextDocument *parent);

private slots:
    void readyReadStandardOutput();
    void readyReadStandardError();

private:
    QStringList QueryCommandParams(const QString &server,
                       const quint32 &port,
                       const QString &admin,
                       const QString &database);

    int CreateDatabase(const QString &server,
                       const quint32 &port,
                       const QString &admin,
                       const QString &database);

    int ExecQueryCommand(const QString &server,
                         const quint32 &port,
                         const QString &admin,
                         const QString &database,
                         const QString &command);

    int ExecQueryFile(const QString &server,
                         const quint32 &port,
                         const QString &admin,
                         const QString &database,
                         const QString &filename);

    int LoadJars(const QString &server,
                 const quint32 &port,
                 const QString &admin,
                 const QString &database,
                 const QStringList &PgJarFiles);

    int LoadJarFile(const QString &server,
                    const quint32 &port,
                    const QString &admin,
                    const QString &database,
                    const QString &filename);

    bool CanProcessJarFile();
    void ResetProcess();
    void StandardRead(QByteArray &data, const QString &encode = QString("IBM 866"));
    QScopedPointer<QProcess> m_Proc;

    QDir m_PostgresDir;
    QString lo_import;
};

#endif // PGIMPORTOPERATION_H
