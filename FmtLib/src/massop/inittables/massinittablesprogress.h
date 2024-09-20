#ifndef MASSINITTABLESPROGRESS_H
#define MASSINITTABLESPROGRESS_H

#include <QWizardPage>
#include <QRunnable>

namespace Ui {
class MassInitTablesProgress;
}

class ConnectionInfo;
class MassInitTableOperation;
class MassInitTablesProgressRun Q_DECL_FINAL: public QObject, public QRunnable
{
    Q_OBJECT

public:
    MassInitTablesProgressRun(MassInitTableOperation *Interface, QObject *parent = Q_NULLPTR);
    virtual ~MassInitTablesProgressRun() Q_DECL_OVERRIDE;

    void run() Q_DECL_OVERRIDE;

signals:
    void finished();
    void progress(int);
    void error(QString);
    void message(QString);

private:
    MassInitTableOperation *pInterface;
};

class ErrorDlg;
class ErrorsModel;
class MassInitTablesProgress : public QWizardPage
{
    Q_OBJECT

public:
    explicit MassInitTablesProgress(QWidget *parent = nullptr);
    virtual ~MassInitTablesProgress();

    void initializePage() Q_DECL_OVERRIDE;
    bool isComplete() const Q_DECL_OVERRIDE;


private:
    Ui::MassInitTablesProgress *ui;
    ErrorsModel *pErrors;
    ErrorDlg *pErrDlg;
    bool fIsComplete;
};

#endif // MASSINITTABLESPROGRESS_H
