#ifndef MASSDESTRIBPARAMSPAGE_H
#define MASSDESTRIBPARAMSPAGE_H

#include <QWizardPage>

namespace Ui {
class MassDestribParamsPage;
}

class MassDestribItemParamDelegate;
class MassDestribParamsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit MassDestribParamsPage(QWidget *parent = nullptr);
    ~MassDestribParamsPage();

    virtual void initializePage() Q_DECL_OVERRIDE;

private:
    Ui::MassDestribParamsPage *ui;
    MassDestribItemParamDelegate *pDelegate;
};

#endif // MASSDESTRIBPARAMSPAGE_H
