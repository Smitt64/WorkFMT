#ifndef IMPORTOPERATION_H
#define IMPORTOPERATION_H

#include "baseoperation.h"
#include <QMutex>

class QThread;
class QProcess;
class ImportOperation : public BaseOperation
{
    Q_OBJECT
public:
    ImportOperation(DumpToolWizard *wizzard);
    virtual ~ImportOperation();

    virtual void run();

private slots:
    void readyReadStandardOutput();

private:
    void StandardRead(QString &data);
    void RecreateUserProc(const QString &Service,
                          const QString &SystemName,
                          const QString &SystemPass,
                          const QString &UserName);

    void CopyDumpFile(const QString &PathToDump,
                      const QString &DumpFile,
                      const QString &ImportPath);

    void ImportFile();

    qint64 m_Offset;
    QMutex m_LogMutex;
    QScopedPointer<QProcess> m_Proc;
};

#endif // IMPORTOPERATION_H
