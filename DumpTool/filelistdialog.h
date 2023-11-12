#ifndef FILELISTDIALOG_H
#define FILELISTDIALOG_H

#include <QDialog>

namespace Ui {
class FileListDialog;
}

class QFileSystemModel;
class QSortFilterProxyModel;
class FileListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileListDialog(const QString &path, QWidget *parent = nullptr);
    ~FileListDialog();

    QString fileName() const;

private:
    Ui::FileListDialog *ui;

    QFileSystemModel *m_pModel;
    //QSortFilterProxyModel *m_pSort;
};

#endif // FILELISTDIALOG_H
