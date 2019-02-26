#ifndef IMPORTWIZARD_H
#define IMPORTWIZARD_H

#include <QWizard>
#include "fmttable.h"

enum
{
    page_Type = 0,
    page_TextEdit,
    page_MapFields
};

class QStandardItemModel;
class QWizardPage;
class ImportWizard : public QWizard
{
    Q_OBJECT
public:
    ImportWizard(FmtSharedTablePtr table, QWidget *parent = nullptr);
    virtual ~ImportWizard();

private:
    QWizardPage *createTypePage();
    QWizardPage *createTextEditPage(QStandardItemModel *model);
    QWizardPage *createMapFieldsPage(QStandardItemModel *model);

    QStandardItemModel *pTableModel;
    FmtSharedTablePtr pTable;
};

#endif // IMPORTWIZARD_H
