#ifndef PROJECTSWIZARDPAGE_H
#define PROJECTSWIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
class ProjectsWizardPage;
}

class ModuleListModel;
class QSortFilterProxyModel;
class ProjectsWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProjectsWizardPage(QWidget *parent = nullptr);
    ~ProjectsWizardPage();

    virtual void initializePage() Q_DECL_OVERRIDE;

private slots:
    void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

private:
    Ui::ProjectsWizardPage *ui;

    QScopedPointer<QSortFilterProxyModel> m_Proxy;
    QScopedPointer<ModuleListModel> m_pModel;
};

#endif // PROJECTSWIZARDPAGE_H
