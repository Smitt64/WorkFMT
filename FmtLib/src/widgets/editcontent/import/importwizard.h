#ifndef IMPORTWIZARD_H
#define IMPORTWIZARD_H

#include <QWizard>

enum
{
    page_Type = 0,
    page_TextEdit,
};

class QStandardItemModel;
class QWizardPage;
class ImportWizard : public QWizard
{
    Q_OBJECT
public:
    ImportWizard(QWidget *parent = nullptr);
    virtual ~ImportWizard();

private:
    QWizardPage *createTypePage();
    QWizardPage *createTextEditPage(QStandardItemModel *model);

    QStandardItemModel *pTableModel;
};

#endif // IMPORTWIZARD_H
