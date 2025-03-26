#ifndef COMPAREPARAMSPAGE_H
#define COMPAREPARAMSPAGE_H

#include <QWizardPage>

namespace Ui {
class CompareParamsPage;
}

class CompareParamsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit CompareParamsPage(QWidget *parent = nullptr);
    ~CompareParamsPage();

private:
    Ui::CompareParamsPage *ui;
};

#endif // COMPAREPARAMSPAGE_H
