#ifndef ACTIONPAGE_H
#define ACTIONPAGE_H

#include <QWizardPage>

namespace Ui {
class ActionPage;
}

class QSpinBox;
class QButtonGroup;
class QPushButton;
class ActionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ActionPage(QWidget *parent = nullptr);
    ~ActionPage();

    virtual bool isComplete() const Q_DECL_OVERRIDE;
    virtual void initializePage() Q_DECL_OVERRIDE;

    QString selectedPath() const;

private slots:
    void on_selFolderBtn_clicked();
    void on_pathEdit_textChanged(const QString &text);

private:
    bool checkRequiredFiles(const QString &basePath) const;

    Ui::ActionPage *ui;
    QButtonGroup *m_pGroup;
    QSpinBox *fakeBtn;
};

#endif // ACTIONPAGE_H
