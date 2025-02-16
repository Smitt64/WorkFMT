#ifndef GENERATORPAGE_H
#define GENERATORPAGE_H

#include <QWizardPage>
#include <QRunnable>
#include <QMutex>

class ErrorDlg;
class QVBoxLayout;
class QProgressBar;
class ErrorsModel;
class HotfixContentModel;

class GeneratorOp : public QObject, public QRunnable
{
public:
    GeneratorOp(QObject *parent = nullptr);
    virtual ~GeneratorOp();

    virtual void run() Q_DECL_OVERRIDE;

    ErrorsModel *m_pLog;
    HotfixContentModel *m_pModel;

private:
    QMutex m_Mutex;
};

class GeneratorPage : public QWizardPage
{
    Q_OBJECT
public:
    GeneratorPage(QWidget *parent = nullptr);
    virtual ~GeneratorPage();

    virtual void initializePage() Q_DECL_OVERRIDE;

private:
    QScopedPointer<QVBoxLayout> m_Layout;
    QScopedPointer<QProgressBar> m_Progress;
    QScopedPointer<ErrorDlg> m_LogWidget;

    QScopedPointer<ErrorsModel> m_Log;
};

#endif // GENERATORPAGE_H
