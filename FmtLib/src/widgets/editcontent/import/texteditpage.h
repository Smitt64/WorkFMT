#ifndef TEXTEDITPAGE_H
#define TEXTEDITPAGE_H

#include <QWizardPage>
#include <QStandardItemModel>
#include <QMap>
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

    int nextId() const Q_DECL_OVERRIDE;
    int getColumnForFmtField(const int &fieldId);

signals:
    void tableMapChanged();

private slots:
    void textChanged();
    void updateFieldsMap();

private:
    QStandardItemModel *pTableModel;
    TableMapToFmtWidget *pTableWidget;
    FmtSharedTablePtr pTable;
    QMap<qint16, qint32> fieldsMap;
    Ui::TextEditPage *ui;
};

#endif // TEXTEDITPAGE_H
