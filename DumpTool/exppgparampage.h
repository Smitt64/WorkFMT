#ifndef EXPPGPARAMPAGE_H
#define EXPPGPARAMPAGE_H

#include <QWizardPage>
#include <QDir>

namespace Ui {
class ExpPgParamPage;
}

class ExpPgParamPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExpPgParamPage(QWidget *parent = nullptr);
    ~ExpPgParamPage();

    virtual bool validatePage();
    virtual bool isComplete() const;
    virtual int nextId() const;

private slots:
    void onSelectServer();

    void on_selfileButton_clicked();

    void on_addParamsBtn_clicked();

private:
    Ui::ExpPgParamPage *ui;
};

QDir GetPostgreSQLInstallLocation();

#endif // EXPPGPARAMPAGE_H
