#ifndef SELECTTYPEPAGE_H
#define SELECTTYPEPAGE_H

#include <QWizardPage>

namespace Ui {
class SelectTypePage;
}

class SelectTypePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit SelectTypePage(QWidget *parent = nullptr);
    ~SelectTypePage();

    int nextId() const Q_DECL_OVERRIDE;

private:
    Ui::SelectTypePage *ui;
};

#endif // SELECTTYPEPAGE_H
