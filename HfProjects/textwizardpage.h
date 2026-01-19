#ifndef TEXTWIZARDPAGE_H
#define TEXTWIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
class TextWizardPage;
}

class TextWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit TextWizardPage(QWidget *parent = nullptr);
    ~TextWizardPage();

private:
    Ui::TextWizardPage *ui;
};

#endif // TEXTWIZARDPAGE_H
