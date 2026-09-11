#ifndef REPOPAGE_H
#define REPOPAGE_H

#include <QWizardPage>
#include <QSortFilterProxyModel>

namespace Ui {
class RepoPage;
}

class QButtonGroup;
class SvnStatusModel;
class MacSatatusModel;
class RepoPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit RepoPage(QWidget *parent = nullptr);
    ~RepoPage();

    virtual bool isComplete() const Q_DECL_OVERRIDE;
    virtual void initializePage() Q_DECL_OVERRIDE;

    QStringList selectedFiles() const;

private slots:
    void on_selFolderBtn_clicked();
    void on_logButton_clicked();

private:
    Ui::RepoPage *ui;

    SvnStatusModel *m_pModel;
    MacSatatusModel *m_pStatusModel;
    QButtonGroup *m_pGroup;
};

class MacSatatusModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    MacSatatusModel(QObject *parent);
    QStringList files();

    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    void resetCheckstate();

signals:
    void checkChanged();

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

    QMap<QString, Qt::CheckState> m_CheckSate;
};

#endif // REPOPAGE_H