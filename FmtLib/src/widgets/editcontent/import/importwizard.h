#ifndef IMPORTWIZARD_H
#define IMPORTWIZARD_H

#include <QWizard>
#include "fmttable.h"

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
    ImportWizard(FmtSharedTablePtr table, QWidget *parent = nullptr);
    virtual ~ImportWizard();

private:
    QWizardPage *createTypePage();
    QWizardPage *createTextEditPage(QStandardItemModel *model);

    QStandardItemModel *pTableModel;
    FmtSharedTablePtr pTable;
};

#endif // IMPORTWIZARD_H
