#ifndef SELECTACTIONPAGE_H
#define SELECTACTIONPAGE_H

#include <QWizardPage>

namespace Ui {
class SelectActionPage;
}

class QButtonGroup;
class QSpinBox;
class SelectActionPage : public QWizardPage
{
    Q_OBJECT

public:
    enum
    {
        ActionExportOraOld = 0,
        ActionExportOra,
        ActionImportOra,
    }Action;

    explicit SelectActionPage(QWidget *parent = nullptr);
    ~SelectActionPage();

    virtual int nextId() const;

private:
    Ui::SelectActionPage *ui;

    QButtonGroup *m_pGroup;
    QSpinBox *fakeBtn;
};

#endif // SELECTACTIONPAGE_H
