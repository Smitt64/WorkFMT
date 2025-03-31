#ifndef COMPARECPPSTRUCTPAGE_H
#define COMPARECPPSTRUCTPAGE_H

#include <QWizardPage>

namespace Ui {
class CompareCppStructPage;
}

class CompareCppStructPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit CompareCppStructPage(QWidget *parent = nullptr);
    ~CompareCppStructPage();

    virtual int nextId() const Q_DECL_OVERRIDE;
    virtual bool isComplete() const Q_DECL_OVERRIDE;

private:
    Ui::CompareCppStructPage *ui;
};

#endif // COMPARECPPSTRUCTPAGE_H
