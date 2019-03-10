#ifndef MASSOPERATIONWIZARD_H
#define MASSOPERATIONWIZARD_H

#include <QWizard>

class MassOperationWizard : public QWizard
{
    Q_OBJECT
public:
    MassOperationWizard(QWidget *parent = Q_NULLPTR);
    virtual ~MassOperationWizard();

private:
    QWizardPage *createSelectOperationPage();
};

#endif // MASSOPERATIONWIZARD_H
