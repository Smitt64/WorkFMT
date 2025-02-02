#ifndef STRUCTSETTINGSPAGE_H
#define STRUCTSETTINGSPAGE_H

#include <QWizardPage>

namespace Ui {
class StructSettingsPage;
}

class HotfixContentModel;
class QSortFilterProxyModel;
class StructSettingsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit StructSettingsPage(QWidget *parent = nullptr);
    ~StructSettingsPage();

    virtual void initializePage() Q_DECL_OVERRIDE;

private:
    Ui::StructSettingsPage *ui;

    QScopedPointer<QSortFilterProxyModel> m_pSortModel;
    QScopedPointer<HotfixContentModel> m_pModel;
};

#endif // STRUCTSETTINGSPAGE_H
