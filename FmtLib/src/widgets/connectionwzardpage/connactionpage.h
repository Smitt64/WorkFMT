#ifndef CONNACTIONPAGE_H
#define CONNACTIONPAGE_H

#include "fmtlib_global.h"
#include <QWizardPage>

namespace Ui {
class ConnactionPage;
}

class OdbcTableModel;
class OdbcTableModelDelegate;
class RecentConnectionList;
class FMTLIBSHARED_EXPORT ConnactionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConnactionPage(QWidget *parent = nullptr);
    ~ConnactionPage();

    virtual bool isComplete() const Q_DECL_OVERRIDE;
    virtual bool validatePage() Q_DECL_OVERRIDE;

private slots:
    void on_connectionsTree_clicked(const QModelIndex &index);

private:
    Ui::ConnactionPage *ui;

    QScopedPointer<RecentConnectionList> m_pRecentList;
    QScopedPointer<OdbcTableModel> m_DataSources;
    QScopedPointer<OdbcTableModelDelegate> m_m_DataSourceDelegate;
};

#endif // CONNACTIONPAGE_H
