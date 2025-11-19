#ifndef CONNACTIONPAGE_H
#define CONNACTIONPAGE_H

#include <QWizardPage>

namespace Ui {
class ConnactionPage;
}

class OdbcTableModel;
class OdbcTableModelDelegate;
class RecentConnectionList;
class ConnactionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ConnactionPage(QWidget *parent = nullptr);
    ~ConnactionPage();

    virtual bool isComplete() const Q_DECL_OVERRIDE;
    virtual bool validatePage() Q_DECL_OVERRIDE;
    virtual void initializePage() Q_DECL_OVERRIDE;
    virtual int nextId() const Q_DECL_OVERRIDE; // Добавляем переопределение

private slots:
    void on_connectionsTree_clicked(const QModelIndex &index);

private:
    Ui::ConnactionPage *ui;

    QScopedPointer<RecentConnectionList> m_pRecentList;
    QScopedPointer<OdbcTableModel> m_DataSources;
    QScopedPointer<OdbcTableModelDelegate> m_m_DataSourceDelegate;
};

#endif // CONNACTIONPAGE_H
