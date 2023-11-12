#ifndef EXPORTOPERATION_H
#define EXPORTOPERATION_H

#include "baseoperation.h"

class QProcess;
class ExportOperation : public BaseOperation
{
    Q_OBJECT
public:
    ExportOperation(DumpToolWizard *wizzard);
    virtual ~ExportOperation();

    virtual void run();

private slots:
    void readyReadStandardOutput();

private:
    void StandardRead(QString &data);

    QScopedPointer<QProcess> m_Proc;
};

#endif // EXPORTOPERATION_H
