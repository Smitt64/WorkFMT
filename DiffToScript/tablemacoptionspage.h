#ifndef TABLEMACOPTIONSPAGE_H
#define TABLEMACOPTIONSPAGE_H

#include <QWidget>
#include "optionsdlg/OptionsPage.h"

namespace Ui {
class TableMacOptionsPage;
}

class QSettings;
class TableFilesModel;
class TableMacOptionsPage : public OptionsPage
{
    Q_OBJECT

public:
    explicit TableMacOptionsPage(QWidget *parent = nullptr);
    ~TableMacOptionsPage();

private:
    Ui::TableMacOptionsPage *ui;

    QSharedPointer<QSettings> pSettings;
    QScopedPointer<TableFilesModel> m_pModel;
};

#endif // TABLEMACOPTIONSPAGE_H
