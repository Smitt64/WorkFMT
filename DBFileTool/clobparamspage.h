#ifndef CLOBPARAMSPAGE_H
#define CLOBPARAMSPAGE_H

#include <QWizardPage>

namespace Ui {
class ClobParamsPage;
}

class QSpinBox;
class QButtonGroup;
class ClobParamsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ClobParamsPage(QWidget *parent = nullptr);
    ~ClobParamsPage();

private:
    Ui::ClobParamsPage *ui;

    QButtonGroup *m_pGroup;
    QSpinBox *fakeBtn;
};

#endif // CLOBPARAMSPAGE_H
