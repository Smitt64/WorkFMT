#ifndef ACTIONLOGPAGE_H
#define ACTIONLOGPAGE_H

#include <QWizardPage>
#include <QPlainTextEdit>

namespace Ui {
class ActionLogPage;
}

class TextBrowser : public QPlainTextEdit
{
    Q_OBJECT
public:
    TextBrowser(QWidget *parent = nullptr);
    virtual ~TextBrowser();

public slots:
    void appendHtml(const QString &msg);
    void message(const QString &msg);
    void errormsg(const QString &msg);
    void succeeded(const QString &msg);
};

class QThread;
class BaseOperation;
class QProgressDialog;
class ActionLogPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ActionLogPage(QWidget *parent = nullptr);
    ~ActionLogPage();

    virtual void initializePage();
    virtual bool isComplete() const;
    virtual int nextId() const;

private slots:
    void finished();
    void message(const QString &msg);
    void errormsg(const QString &msg);
    void succeeded(const QString &msg);
    void progressDialog(const QString &msg, qint64 max);
    void progressDialogEnd();
    void stageChanged(const QString &msg);

private:
    Ui::ActionLogPage *ui;

    bool m_IsFinished;
    QScopedPointer<BaseOperation> m_Operation;
    QScopedPointer<QThread> m_Thread;
    QScopedPointer<TextBrowser> m_TextBrowser;
    //QScopedPointer<QProgressDialog> m_ProgressDialog;
};

#endif // ACTIONLOGPAGE_H
