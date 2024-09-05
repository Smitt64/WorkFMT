#ifndef ORACONNECTIONPAGE_H
#define ORACONNECTIONPAGE_H

#include <QWizardPage>

namespace Ui {
class OraConnectionPage;
}

class OdbcTableModel;
class OdbcTableModelDelegate;
class RecentConnectionList;
class OraConnectionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit OraConnectionPage(QWidget *parent = nullptr);
    ~OraConnectionPage();

    virtual int nextId() const;
    virtual bool isComplete() const;
    virtual bool validatePage();

private:
    Ui::OraConnectionPage *ui;

    OdbcTableModel *m_pDsnModel;
    OdbcTableModelDelegate *m_DataSourceDelegate;

    RecentConnectionList *m_pRecentList;
};

#endif // ORACONNECTIONPAGE_H
