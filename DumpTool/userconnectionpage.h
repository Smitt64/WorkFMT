#ifndef USERCONNECTIONPAGE_H
#define USERCONNECTIONPAGE_H

#include <QWizardPage>

namespace Ui {
class UserConnectionPage;
}

class QLineEdit;
class OdbcTableModel;
class OdbcTableModelDelegate;
class UserConnectionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit UserConnectionPage(QWidget *parent = nullptr);
    ~UserConnectionPage();

    virtual bool validatePage() Q_DECL_OVERRIDE;
    virtual bool isComplete() const Q_DECL_OVERRIDE;

private:
    Ui::UserConnectionPage *ui;
    OdbcTableModel *m_pDsnModel;
    OdbcTableModelDelegate *m_DataSourceDelegate;
};

#endif // USERCONNECTIONPAGE_H
