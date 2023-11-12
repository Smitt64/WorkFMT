#ifndef IMPFILESELECTPAGE_H
#define IMPFILESELECTPAGE_H

#include <QWizardPage>

namespace Ui {
class ImpFileSelectPage;
}

class ImpFileSelectPage : public QWizardPage
{
    Q_OBJECT

public:
    enum
    {
        Local = 0,
        SameServer,
        ImpdpServer,
        OtherServer,
    }FileLocation;
    explicit ImpFileSelectPage(QWidget *parent = nullptr);
    ~ImpFileSelectPage();

    virtual void initializePage();
    virtual bool validatePage();
    virtual bool isComplete() const;

private slots:
    void LoacationChanged(int index);
    void on_expSelectButton_clicked();
    void on_fileButton_clicked();

private:
    void PumpOwner();
    Ui::ImpFileSelectPage *ui;
};

#endif // IMPFILESELECTPAGE_H
