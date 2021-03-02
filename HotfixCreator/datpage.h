#ifndef DATPAGE_H
#define DATPAGE_H

#include <QWizardPage>

namespace Ui {
class DatPage;
}

class DatPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit DatPage(QWidget *parent = nullptr);
    ~DatPage();

private:
    Ui::DatPage *ui;
};

#endif // DATPAGE_H
