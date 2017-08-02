#ifndef SELECTCONNECTIONDLG_H
#define SELECTCONNECTIONDLG_H

#include <QDialog>

namespace Ui {
class SelectConnectionDlg;
}

class ConnectionInfo;
class ConnectionListModel;
class SelectConnectionDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectConnectionDlg(const QList<ConnectionInfo*> list, QWidget *parent = Q_NULLPTR);
    virtual ~SelectConnectionDlg();

    QList<ConnectionInfo*> сheckedItems() const;

private:
    Ui::SelectConnectionDlg *ui;
    ConnectionListModel *pModel;
};

#endif // SELECTCONNECTIONDLG_H
