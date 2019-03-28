#ifndef MASSOPSELECTOPERATIONPAGE_H
#define MASSOPSELECTOPERATIONPAGE_H

#include <QWizardPage>
#include "src/core/abstractfactorymodel.hpp"
#include "massopinterface.h"

namespace Ui {
class MassOpSelectOperationPage;
}

class MassOpSelectOperationPage : public QWizardPage
{
    Q_OBJECT
public:
    MassOpSelectOperationPage(QWidget *parent = Q_NULLPTR);
    virtual ~MassOpSelectOperationPage() Q_DECL_OVERRIDE;

    virtual bool isComplete() const Q_DECL_OVERRIDE;
    QString selectedItem() const;
    QString selectedInterface() const;

private:
    Ui::MassOpSelectOperationPage *ui;
    AbstractFactoryModel<MassOpInterface,QString> *pModel;
};

#endif // MASSOPSELECTOPERATIONPAGE_H
