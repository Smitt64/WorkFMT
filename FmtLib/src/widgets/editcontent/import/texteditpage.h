#ifndef TEXTEDITPAGE_H
#define TEXTEDITPAGE_H

#include <QWizardPage>
#include <QStandardItemModel>

namespace Ui {
class TextEditPage;
}

class TextEditPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit TextEditPage(QStandardItemModel *model, QWidget *parent = nullptr);
    ~TextEditPage();

private slots:
    void textChanged();

private:
    QStandardItemModel *pTableModel;
    Ui::TextEditPage *ui;
};

#endif // TEXTEDITPAGE_H
