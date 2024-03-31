#ifndef PGADDJARSDLG_H
#define PGADDJARSDLG_H

#include <QDialog>
#include <QAbstractItemDelegate>

namespace Ui {
class PgAddJarsDlg;
}

class EditableJarDelegate;
class QStandardItemModel;
class PgAddJarsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PgAddJarsDlg(QWidget *parent = nullptr);
    ~PgAddJarsDlg();

    void addStatic(const QString &value, const QIcon &icon = QIcon());
    void addList(const QStringList &lst);
    QStringList files();

private slots:
    void on_newBtn_clicked();
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

    void on_addFilesBtn_clicked();

    void on_remBtn_clicked();

private:
    Ui::PgAddJarsDlg *ui;

    QStandardItemModel *m_pModel;
    EditableJarDelegate *m_pDelegate;
};

#endif // PGADDJARSDLG_H
