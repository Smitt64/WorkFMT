#ifndef EXPSETTINGSPAGE_H
#define EXPSETTINGSPAGE_H

#include <QtWidgets>

class ExportToXmlWizard;
class FmtTablesModel;
class TablesDockWidget;
class TablesSelectWidget;
class ExpSettingsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ExpSettingsPage(ExportToXmlWizard *parent = 0);
    virtual bool isComplete() const;

signals:

private slots:
    void dirComboItemActivated(int index);

private:
    bool AddTable(const QString table);
    bool RemTable(const QString table);
    void UpdateDirCombo();
    void UpdateButtons();
    void CopyToDstList(const QModelIndex &index);

    ExportToXmlWizard *pWizard;

    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *dirComboBox;
    QHBoxLayout *horizontalLayout;

    TablesSelectWidget *pSelectWidget;
};

#endif // EXPSETTINGSPAGE_H
