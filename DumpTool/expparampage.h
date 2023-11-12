#ifndef EXPPARAMPAGE_H
#define EXPPARAMPAGE_H

#include <QWizardPage>

namespace Ui {
class ExpParamPage;
}

class QLineEdit;
class OdbcTableModel;
class OdbcTableModelDelegate;
class ExpParamPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExpParamPage(QWidget *parent = nullptr);
    ~ExpParamPage();

    virtual int nextId() const;
    virtual bool isComplete() const;
     virtual bool validatePage();

private:
    Ui::ExpParamPage *ui;

    OdbcTableModel *m_pDsnModel;
    OdbcTableModelDelegate *m_DataSourceDelegate;

    QLineEdit *fakeDirEdit;
};

#endif // EXPPARAMPAGE_H
