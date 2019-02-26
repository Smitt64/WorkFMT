#ifndef FMTWORKWINDOW_H
#define FMTWORKWINDOW_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QHBoxLayout>
#include <QToolButton>
#include "fmttable.h"
#include "fmtlib_global.h"
#include "mdisubinterface.h"

namespace Ui {
class FmtWorkWindow;
}

class FmtFieldsDelegate;
class FmtIndecesDelegate;
class FmtFieldsTableView;
class FmtIndexTreeView;
class FmtWorkWndGen;
class UndoListPopup;
class LineEditAction;
class QMdiSubWindow;
class QShortcut;
class FmtFieldsTableHeaderDelegate;
class FMTLIBSHARED_EXPORT FmtWorkWindow : public MdiSubInterface
{
    Q_OBJECT

public:
    enum FmtWinTabs
    {
        FmtWinTabs_Columns = 0,
        FmtWinTabs_Index,
        FmtWinTabs_Code,
    };

    explicit FmtWorkWindow(QWidget *parent = Q_NULLPTR);
    virtual ~FmtWorkWindow();

    void setFmtTable(FmtSharedTablePtr &table);
    FmtSharedTablePtr table() const { return pTable; }
    void rebuildOffsets();

    void setDialogButtonsVisible(bool f);
    void setInitButtonVisible(bool f);

    ConnectionInfo *connection() const;

    QUndoStack *tableUndoStack();

signals:

public slots:
    void EditContent();
    void GenDeleteFiledsScript();
    void GenAddFiledsScript();
    void GenModifyTableFields();
    void GenCreateTableScript();

private slots:
    void indexModelReseted();
    void indexModelInserted(const QModelIndex &parent, const int &first, const int &last);
    void AddIndex();
    void Apply();
    void ExportXml();
    void UnloadToDbf();
    void LoadFromDbf();
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

    void undoActionChanged();
    void isTemporaryTableChanged(bool value);

    void TabCloseRequested(int index);
    void RemoveTableFields();
    void OpenScriptEditorWindow();

    void AddFieldsToEnd();
    void InsertFieldsBefore();

    void CopyFields();
    void PasteFields();

protected:
    void paintEvent(QPaintEvent *paintEvent);

private:
    int CheckAppy();
    int SaveTable();
    void setupUndoRedo();
    void setupFind();
    void SetUnclosableSystemTabs();
    int SelectTableFieldsDailog(const QString &title, QList<FmtField*> *pFldList);
    void AddSqlCodeTab(const QString &title, const QString &code, bool OpenTab = true);
    void SetupActionsMenu();
    Ui::FmtWorkWindow *ui;
    QSharedPointer<FmtTable> pTable;

    FmtFieldsTableView *pTableView;
    FmtIndexTreeView   *pTreeView;
    FmtFieldsTableHeaderDelegate *pTableHeaderDelegate;

    QDataWidgetMapper *pMapper;
    FmtFieldsDelegate *pFieldsDelegate;

    QHBoxLayout *pHBoxLayout;
    QSpacerItem *pHorizontalSpacer;
    QPushButton *pAddIndex;

    QMenu *pCopyMenu, *pActionsMenu, *pCodeGenMenu;
    QAction *m_saveToXml, *m_createTableSql, *m_rebuildOffsets, *m_unloadDbf, *m_loadDbf, *m_MassRemoveFields;
    QAction *m_AddFieldsToEnd, *m_InsertFields, *m_CopyFields, *m_PasteFields, *m_EditContent;
    QAction *m_GenDelScript, *m_GenAddScript, *m_GenCreateTbSql, *m_GenModifyScript;
    FmtWorkWndGen *pCodeGenWidget;

    QFrame *pUndoRedoBtnContainer;
    QHBoxLayout *pUndoRedoLayout;
    QToolButton *pUndoBtn, *pRedoBtn, *pFindBtn;

    QAction *pUndoAction, *pRedoAction, *pUndoActionWrp;
    UndoListPopup *pUndoPopup;
    LineEditAction *pFindLineEdit;
    QIcon undoDisabled;
    QMenu *pUndoMenu, *pFindMenu;

    QShortcut *pFindShortcut;

    QColor dcolor;
    QColor color;
};

#endif // FMTWORKWINDOW_H
