#ifndef STRINGLISTDLG_H
#define STRINGLISTDLG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class StringListDlg;
}

class StringListDlg : public QDialog
{
    Q_OBJECT

public:
    explicit StringListDlg(QWidget *parent = 0);
    virtual ~StringListDlg();

    void setList(const QStringList &lst);
    QString selected() const;

private slots:
    void selectionChanged();

private:
    Ui::StringListDlg *ui;
    QStringListModel model;
};

#endif // STRINGLISTDLG_H
