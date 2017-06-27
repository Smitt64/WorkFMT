#ifndef FMTWORKWINDOW_H
#define FMTWORKWINDOW_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QHBoxLayout>
#include <QToolButton>
#include "fmttable.h"
#include "fmtlib_global.h"

namespace Ui {
class FmtWorkWindow;
}

class FmtFieldsDelegate;
class FmtIndecesDelegate;
class FmtFieldsTableView;
class FmtIndexTreeView;
class FmtWorkWndGen;
class UndoListPopup;
class QMdiSubWindow;
class FMTLIBSHARED_EXPORT FmtWorkWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FmtWorkWindow(QWidget *parent = 0);
    ~FmtWorkWindow();

    void setFmtTable(QSharedPointer<FmtTable> &table);
    void rebuildOffsets();

    void setDialogButtonsVisible(bool f);
    void setInitButtonVisible(bool f);

    void setParentWnd(QMdiSubWindow *wnd);

    ConnectionInfo *connection();

    QUndoStack *tableUndoStack();

signals:

private slots:
    void indexModelReseted();
    void indexModelInserted(const QModelIndex &parent, const int &first, const int &last);
    void AddIndex();
    void Apply();
    void ExportXml();
    void InitDB();
    void FillIndecesList();
    void FieldAdded(FmtField *fld);
    void OnIndexChanged(FmtIndex *index);
    void AddedToLast(FmtField *fld, const QModelIndex &index);

    void CheckTable();
    void Clicked(const QModelIndex &index);
    void OnOk();
    void CreateTableSql();
    void CopyAction();
    void UpdateCopyMenu(const QString &table);

    void on_rebuildOffsetsBtn_clicked();
    void undoActionChanged();
    void isTemporaryTableChanged(bool value);

protected:
    void paintEvent(QPaintEvent *paintEvent);

private:
    int CheckAppy();
    int SaveTable();
    void setupUndoRedo();
    Ui::FmtWorkWindow *ui;
    QMdiSubWindow *pParentWnd;
    QSharedPointer<FmtTable> pTable;

    FmtFieldsTableView *pTableView;
    FmtIndexTreeView   *pTreeView;

    QDataWidgetMapper *pMapper;
    FmtFieldsDelegate *pFieldsDelegate;

    QHBoxLayout *pHBoxLayout;
    QSpacerItem *pHorizontalSpacer;
    QPushButton *pAddIndex;

    QMenu *pCopyMenu, *pActionsMenu;

    FmtWorkWndGen *pCodeGenWidget;

    QFrame *pUndoRedoBtnContainer;
    QHBoxLayout *pUndoRedoLayout;
    QToolButton *pUndoBtn, *pRedoBtn;

    QAction *pUndoAction, *pRedoAction, *pUndoActionWrp;
    UndoListPopup *pUndoPopup;
    QIcon undoDisabled;
    QMenu *pUndoMenu;

    QColor dcolor;
    QColor color;
};

#endif // FMTWORKWINDOW_H
