#ifndef COMPAREPARAMSPAGE_H
#define COMPAREPARAMSPAGE_H

#include <QWizardPage>

namespace Ui {
class CompareParamsPage;
}

class QSpinBox;
class QButtonGroup;
class CompareParamsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit CompareParamsPage(QWidget *parent = nullptr);
    ~CompareParamsPage();

    virtual int nextId() const Q_DECL_OVERRIDE;

private:
    Ui::CompareParamsPage *ui;

    QButtonGroup *m_pGroup;
    QSpinBox *fakeBtn;
};

#endif // COMPAREPARAMSPAGE_H
