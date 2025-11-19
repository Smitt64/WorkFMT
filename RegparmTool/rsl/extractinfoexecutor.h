#ifndef EXTRACTINFOEXECUTOR_H
#define EXTRACTINFOEXECUTOR_H

#include <rslexecutor.h>

class QTextDocument;
class ExtractInfoExecutor : public RslExecutor
{
public:
    explicit ExtractInfoExecutor(QObject *parent = nullptr);
    virtual ~ExtractInfoExecutor();

    void setDocument(QTextDocument *pDocument);

protected:
    virtual void PlayRepProc() Q_DECL_OVERRIDE;
    virtual void onSetStModuleAdd() Q_DECL_OVERRIDE;

private:
    QTextDocument *m_pDocument;
};

#endif // EXTRACTINFOEXECUTOR_H
