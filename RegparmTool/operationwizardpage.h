#ifndef OPERATIONWIZARDPAGE_H
#define OPERATIONWIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
class OperationWizardPage;
}

class OperationWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit OperationWizardPage(QWidget *parent = nullptr);
    ~OperationWizardPage();

    virtual void initializePage() Q_DECL_OVERRIDE;

private:
    Ui::OperationWizardPage *ui;
};

#endif // OPERATIONWIZARDPAGE_H
