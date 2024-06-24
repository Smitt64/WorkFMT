#ifndef FMTWORKWINDOW_H
#define FMTWORKWINDOW_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QHBoxLayout>
#include <QToolButton>
#include "fmttable.h"
#include "fmtlib_global.h"
#include "mdisubinterface.h"
#include "src/widgets/filteredtablewidget.h"
#include "fmtfieldstableheaderdelegate.h"

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
class FilteredTableWidget;
class FmtFieldsTableViewFilterController;

class FmtFieldsTableViewFilterController: public QObject, public FilteredControlHandler
{
    Q_OBJECT
public:
    FmtFieldsTableViewFilterController(FmtFieldsDelegate *delegate);
    QWidget *create(const int &index) Q_DECL_OVERRIDE;

private:
    FmtFieldsDelegate *pDelegate;
};

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

    QUndoStack *tableUndoStack();
    QString makeWindowTitle() Q_DECL_OVERRIDE;

signals:

public slots:
    void EditContent();
    void GenDeleteFiledsScript();
    void GenAddFiledsScript();
    void GenModifyTableFields();
    void GenCreateTableScript();
    void GenInsertTemplateSql();
    void DiffToScript();

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
    void SegmentButtonClicked(const QModelIndex &index);
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
    void OnImport();
    void AutoCamelCase(bool checked);
    void CamelCaseAction();

protected:
    void paintEvent(QPaintEvent *paintEvent);

private:
    int CheckAppy();
    int SaveTable();
    void setupUndoRedo();
    void setupFind();
    void SetUnclosableSystemTabs();
    int SelectTableFieldsDailog(const QString &title, QList<FmtField*> *pFldList);
    void AddSqlCodeTab(const QString &title, const QString &code, bool OpenTab = true, bool WordWrap = false);
    void AddCppCodeTab(const QString &title, const QString &code, bool OpenTab = true, bool WordWrap = false);
    void SetupActionsMenu();
    Ui::FmtWorkWindow *ui;
    QSharedPointer<FmtTable> pTable;

    FmtFieldsTableView *pTableView;
    FmtIndexTreeView   *pTreeView;
    FmtFieldsTableHeaderDelegate *pTableHeaderDelegate;
    FilteredTableWidget *pFilteredTableView;
    FmtFieldsTableViewFilterController *pFieldsFilterController;

    QDataWidgetMapper *pMapper;
    FmtFieldsDelegate *pFieldsDelegate;

    QHBoxLayout *pHBoxLayout;
    QSpacerItem *pHorizontalSpacer;
    QPushButton *pAddIndex;

    QMenu *pCopyMenu, *pActionsMenu, *pCodeGenMenu;
    QAction *m_saveToXml, *m_createTableSql, *m_rebuildOffsets, *m_MassRemoveFields;
    QAction *m_unloadDbf, *m_loadDbf, *m_ImportData;
    QAction *m_AddFieldsToEnd, *m_InsertFields, *m_CopyFields, *m_PasteFields, *m_EditContent;
    QAction *m_GenDelScript, *m_GenAddScript, *m_GenCreateTbSql, *m_GenModifyScript;
    QAction *m_GenInsertTemplate, *m_CamelCaseAction, *m_GenDiffToScript;
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
