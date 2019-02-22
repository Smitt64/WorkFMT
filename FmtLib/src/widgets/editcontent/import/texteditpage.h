#ifndef TEXTEDITPAGE_H
#define TEXTEDITPAGE_H

#include <QWizardPage>
#include <QStandardItemModel>
#include "fmttable.h"

namespace Ui {
class TextEditPage;
}

class TableMapToFmtWidget;
class TextEditPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit TextEditPage(FmtSharedTablePtr table, QStandardItemModel *model, QWidget *parent = nullptr);
    ~TextEditPage();

private slots:
    void textChanged();

private:
    QStandardItemModel *pTableModel;
    TableMapToFmtWidget *pTableWidget;
    FmtSharedTablePtr pTable;
    Ui::TextEditPage *ui;
};

#endif // TEXTEDITPAGE_H
