#ifndef CHECKSAVEEXECUTOR_H
#define CHECKSAVEEXECUTOR_H

#include <rslexecutor.h>

class FmtTable;
class ErrorsModel;
class CheckSaveExecutor : private RslExecutor
{
public:
    CheckSaveExecutor(FmtTable *pTable, ErrorsModel *errors);
    QVariant check();

protected:
    virtual void PlayRepProc() Q_DECL_OVERRIDE;

private:
    FmtTable *m_pTable;
    ErrorsModel *m_pErrors;
    QVariant m_Result;
};

#endif // CHECKSAVEEXECUTOR_H
