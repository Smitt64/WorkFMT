#ifndef SELECTFOLDERDLG_H
#define SELECTFOLDERDLG_H

#include <QDialog>

namespace Ui {
class SelectFolderDlg;
}

class QSettings;
class FoldersModel;
class FoldersSortModel;
class SelectFolderDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectFolderDlg(const QString &context, QWidget *parent = nullptr);
    explicit SelectFolderDlg(const QString &context, const QString &title, QWidget *parent = nullptr);
    ~SelectFolderDlg();

    const QString &selectedPath() const;
    virtual void accept();

private slots:
    void on_openDir_clicked();
    void clicked(const QModelIndex &index);
    void doubleClicked(const QModelIndex &index);
    //void on_SelectFolderDlg_accepted();

private:
    void SaveList();
    QString GetTopelement();
    Ui::SelectFolderDlg *ui;
    QString m_Context;
    QSettings *m_pSettings;
    FoldersModel *m_pModel;
    FoldersSortModel *m_pSortModel;
    QString m_SelectedPath;
};

#endif // SELECTFOLDERDLG_H
