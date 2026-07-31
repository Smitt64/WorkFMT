#ifndef SETTINGSSELECTIONPAGE_H
#define SETTINGSSELECTIONPAGE_H

#include <QWizardPage>

namespace Ui {
class SettingsSelectionPage;
}

class ConnectionInfo;
class QTreeView;
class SettingsPathCompleter;
class SettingsTableModel;
class TypeComboBoxDelegate;
class SettingsValueDelegate;

class SettingsSelectionPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit SettingsSelectionPage(QWidget *parent = nullptr);
    ~SettingsSelectionPage();

    bool isComplete() const override;
    void initializePage() override;
    void cleanupPage() override;
    int nextId() const override;

    SettingsTableModel *model() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onAddClicked();
    void onDeleteClicked();
    void onSelectionChanged();

private:
    void setupTree();
    void setupCompleter();
    bool openDatabase();
    void closeDatabase();
    bool validateAndAddName(const QString &fullName);
    qint64 findKeyIdByParentAndName(qint64 parentId, const QString &name) const;

    Ui::SettingsSelectionPage *ui;
    SettingsTableModel *m_pModel;
    TypeComboBoxDelegate *m_pTypeDelegate;
    SettingsValueDelegate *m_pValueDelegate;
    SettingsPathCompleter *m_pCompleter;
    QTreeView *m_pCompleterTree;
    ConnectionInfo *m_pConnection;
};

#endif // SETTINGSSELECTIONPAGE_H
