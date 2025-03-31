#ifndef COMPARESELECTTABLEPAGE_H
#define COMPARESELECTTABLEPAGE_H

#include <QWizardPage>
#include <QSqlQueryModel>

namespace Ui {
class CompareSelectTablePage;
}

class ConnectionInfo;
class FilteredSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    FilteredSqlModel(ConnectionInfo *connection, QObject *parent = nullptr);
    void updateFilter(const QString &filter);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    ConnectionInfo *m_pInfo;
};

// -------------------------------------------------------------

class CompareSelectTablePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit CompareSelectTablePage(QWidget *parent = nullptr);
    ~CompareSelectTablePage();

    virtual void initializePage() Q_DECL_OVERRIDE;
    virtual bool isComplete() const Q_DECL_OVERRIDE;

private:
    Ui::CompareSelectTablePage *ui;

    QScopedPointer<FilteredSqlModel> m_pModel;
    QScopedPointer<ConnectionInfo> m_Connection;
};

#endif // COMPARESELECTTABLEPAGE_H
