#ifndef PGEXPORTOPERATION_H
#define PGEXPORTOPERATION_H

#include "baseoperation.h"

class QProcess;
class PgExportOperation : public BaseOperation
{
    Q_OBJECT
public:
    PgExportOperation(DumpToolWizard *wizzard);
    virtual ~PgExportOperation();

    virtual void run();
    virtual void highlighter(BaseLogHighlighter **obj, QTextDocument *parent);

private slots:
    void readyReadStandardOutput();

private:
    void StandardRead(QByteArray &data);
    QScopedPointer<QProcess> m_Proc;
};

#endif // PGEXPORTOPERATION_H
