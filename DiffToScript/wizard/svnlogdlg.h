#ifndef SVNLOGDLG_H
#define SVNLOGDLG_H

#include <QDialog>

namespace Ui {
class SvnLogDlg;
}

class QSplitter;
class QTreeView;
class SvnLogModel;
class QListView;
class QPlainTextEdit;
class SvnLogItemsModel;
class SvnLogDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SvnLogDlg(QWidget *parent = nullptr);
    ~SvnLogDlg();

    void setPath(const QString &path);
    QString revision() const;

public slots:
    virtual void accept() Q_DECL_OVERRIDE;

private slots:
    void UpdateDateRange(bool NeedRefresh = true);

    void on_dateBox_stateChanged(int arg1);

private:
    Ui::SvnLogDlg *ui;

    QTreeView *m_pLogView, *m_pFilesView;
    QSplitter *m_pSplitter;
    QPlainTextEdit *m_pComment;

    SvnLogModel *m_pModel;
    SvnLogItemsModel *m_pFilesModel;

    QString m_Url, m_SelectedRevision;
};

#endif // SVNLOGDLG_H
