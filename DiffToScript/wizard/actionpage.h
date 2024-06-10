#ifndef ACTIONPAGE_H
#define ACTIONPAGE_H

#include <QWizardPage>
#include <QSortFilterProxyModel>

namespace Ui {
class ActionPage;
}

class QButtonGroup;
class QSpinBox;
class SvnSatatusModel;
class DatSatatusModel;
class ActionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ActionPage(QWidget *parent = nullptr);
    ~ActionPage();

    virtual bool isComplete() const Q_DECL_OVERRIDE;

private slots:
    void on_selFolderBtn_clicked();

    void on_logButton_clicked();

private:
    Ui::ActionPage *ui;

    QButtonGroup *m_pGroup;
    QSpinBox *fakeBtn;

    DatSatatusModel *m_pStatusModel;
    SvnSatatusModel *m_pModel;
};

class DatSatatusModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    DatSatatusModel(QObject *parent);
    QStringList files();

    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

signals:
    void checkChanged();

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

    QMap<QString, Qt::CheckState> m_CheckSate;
};

#endif // ACTIONPAGE_H
