#ifndef TOOLBARACTIONEXECUTOR_H
#define TOOLBARACTIONEXECUTOR_H

#include <rslexecutor.h>

class MainWindow;
class ToolbarActionExecutor : public RslExecutor
{
    Q_OBJECT
public:
    ToolbarActionExecutor(MainWindow *parent = nullptr);
    virtual ~ToolbarActionExecutor();

    void playRep(const QString &filename, const QString &output = QString(), RslExecutorProc proc = RslExecutorProc());

protected:
    virtual void PlayRepProc() Q_DECL_OVERRIDE;
    virtual void onInspectModuleSymbol(Qt::HANDLE sym) Q_DECL_OVERRIDE;

private:
    MainWindow *pMainWindow;
};

#endif // TOOLBARACTIONEXECUTOR_H
