#ifndef PGIMPPARAMPAGE_H
#define PGIMPPARAMPAGE_H

#include <QWizardPage>

namespace Ui {
class PgImpParamPage;
}

class PgImpParamPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit PgImpParamPage(QWidget *parent = nullptr);
    ~PgImpParamPage();

    bool isComplete() const;

private slots:
    void on_selectServer_clicked();

    void on_selfileButton_clicked();

    void on_jarButton_clicked();

private:
    Ui::PgImpParamPage *ui;
};

#endif // PGIMPPARAMPAGE_H
