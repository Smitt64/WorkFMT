#ifndef EXPORTPAGE_H
#define EXPORTPAGE_H

#include <QWizardPage>
#include <QRunnable>

namespace Ui {
class ExportPage;
}

class DBFileObject;
class LogHighlighter;
class ExportPage : public QWizardPage
{
    Q_OBJECT

    friend class RsExpOperationOld;
    friend class RsExpOperation;
public:
    explicit ExportPage(QWidget *parent = nullptr);
    ~ExportPage();

    virtual void initializePage();
    virtual bool isComplete() const;

public slots:
    void setCompleteOn();

private:
    Ui::ExportPage *ui;
    LogHighlighter *logColor;

    bool m_Complete;
};

class DbtToolWizard;
class RsExpOperationOld : public QObject,  public QRunnable
{
    Q_OBJECT

    ExportPage *pParent;
    DbtToolWizard *pWizard;
public:
    RsExpOperationOld(DbtToolWizard *Wizard, ExportPage *parent);

    virtual void run() Q_DECL_OVERRIDE;

signals:
    void procMessage(const QString &str);
    void procInfo(const QString &str);
    void procError(const QString &str);
};

class RsExpOperation : public QObject,  public QRunnable
{
    Q_OBJECT

    ExportPage *pParent;
    DbtToolWizard *pWizard;
public:
    RsExpOperation(DbtToolWizard *Wizard, ExportPage *parent);
    virtual void run() Q_DECL_OVERRIDE;

signals:
    void procMessage(const QString &str);
    void procInfo(const QString &str);
    void procError(const QString &str);
};

#endif // EXPORTPAGE_H
