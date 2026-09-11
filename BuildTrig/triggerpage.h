#ifndef TRIGGERPAGE_H
#define TRIGGERPAGE_H

#include <QWizardPage>
#include <QRunnable>
#include <QScopedPointer>

class QVBoxLayout;
class QProgressBar;
class ErrorDlg;
class ErrorsModel;

class TriggerOperation : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit TriggerOperation(QObject *parent = nullptr);
    virtual ~TriggerOperation();

    virtual void run() Q_DECL_OVERRIDE;

    QString repoPath;
    QStringList files;

Q_SIGNALS:
    void appendError(const QString &text, int type);
    void progressValue(int value);
    void operationFinished(bool hasErrors);
};

class TriggerPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit TriggerPage(QWidget *parent = nullptr);
    virtual ~TriggerPage();

    virtual bool isComplete() const Q_DECL_OVERRIDE;
    virtual void initializePage() Q_DECL_OVERRIDE;

private slots:
    void onOperationFinished(bool hasErrors);

private:
    QScopedPointer<QVBoxLayout> m_Layout;
    QScopedPointer<QProgressBar> m_Progress;
    QScopedPointer<ErrorDlg> m_LogWidget;
    QScopedPointer<ErrorsModel> m_Log;

    bool m_Finished;
};

#endif // TRIGGERPAGE_H