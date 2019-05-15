#ifndef MASSDESTRIBPARAMSPAGE_H
#define MASSDESTRIBPARAMSPAGE_H

#include <QWizardPage>

namespace Ui {
class MassDestribParamsPage;
}

class QSettings;
class MassDestribItemParamDelegate;
class MassDestribParamsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit MassDestribParamsPage(QWidget *parent = nullptr);
    ~MassDestribParamsPage();

    virtual void initializePage() Q_DECL_OVERRIDE;

public slots:
    void selectFolder();

private:
    Ui::MassDestribParamsPage *ui;
    MassDestribItemParamDelegate *pDelegate;
    QSettings *m_pPrm;
};

#endif // MASSDESTRIBPARAMSPAGE_H
