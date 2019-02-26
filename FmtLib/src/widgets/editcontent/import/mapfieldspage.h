#ifndef MAPFIELDSPAGE_H
#define MAPFIELDSPAGE_H

#include <QWizardPage>
#include <QStandardItemModel>
#include "fmttable.h"

namespace Ui {
class MapFieldsPage;
}

class MapFildsModel;
class MapFieldsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit MapFieldsPage(FmtSharedTablePtr table, QStandardItemModel *model, QWidget *parent = nullptr);
    virtual ~MapFieldsPage();

private:
    Ui::MapFieldsPage *ui;
    FmtSharedTablePtr pTable;
    QStandardItemModel *pModel;
    MapFildsModel *pMapModel;
};

#endif // MAPFIELDSPAGE_H
