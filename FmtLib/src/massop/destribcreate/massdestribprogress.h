#ifndef MASSDESTRIBPROGRESS_H
#define MASSDESTRIBPROGRESS_H

#include <QWizardPage>
#include <QRunnable>
#include <QDir>

namespace Ui {
class MassDestribProgress;
}

class ConnectionInfo;
class MassDestribCreate;
class ErrorDlg;
class ErrorsModel;
class QSettings;
class MassDestribProgressRun Q_DECL_FINAL: public QObject, public QRunnable
{
    Q_OBJECT
public:
    MassDestribProgressRun(MassDestribCreate *Interface, QObject *parent = Q_NULLPTR);
    virtual ~MassDestribProgressRun() Q_DECL_OVERRIDE;

    void setDsn(const QString &dsn);

    void run() Q_DECL_OVERRIDE;

Q_SIGNALS:
    void finished();
    void progress(int);
    void error(QString);
    void message(QString);

private:
    QDir getDir(const QDir &top, const QString &dirpath);
    void writefile(const QDir &dir, const QString &filename, const QString &data);
    MassDestribCreate *pInterface;
    QString m_dsn;
    QSettings *m_pPrm;
};

class MassDestribProgress : public QWizardPage
{
    Q_OBJECT

public:
    explicit MassDestribProgress(QWidget *parent = nullptr);
    virtual ~MassDestribProgress();

    void initializePage() Q_DECL_OVERRIDE;
    bool isComplete() const Q_DECL_OVERRIDE;

private:
    Ui::MassDestribProgress *ui;
    ErrorsModel *pErrors;
    ErrorDlg *pErrDlg;
    bool fIsComplete;
};

#endif // MASSDESTRIBPROGRESS_H
