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

#define FMTTABLE_CONTEXTCATEGORY tr("Таблица")

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
class SARibbonCategory;
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
    Q_PROPERTY(QObject *table READ tableProp CONSTANT)
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

    QObject* tableProp() const { return pTable.data(); }

    void execUserAction(const QString &macro);

signals:

public slots:
    void EditContent();
    void GenDeleteFiledsScript();
    void GenAddFiledsScript();
    void GenModifyTableFields();
    void GenCreateTableScript();
    void GenInsertTemplateSql();
    void DiffToScript();
    void ExportXml();
    void UnloadToDbf();
    void LoadFromDbf();
    void InitDB();
    void CreateTableSql();
    void CompareStruct();
    void OpenCodeTab(const QString &title, int syntax,
                     const QString &code,
                     bool OpenTab = true, bool WordWrap = false);

private slots:
    void indexModelReseted();
    void indexModelInserted(const QModelIndex &parent, const int &first, const int &last);
    void AddIndex();
    void Apply();

    void FillIndecesList();
    void FieldAdded(FmtField *fld);
    void OnIndexChanged(FmtIndex *index);
    void AddedToLast(FmtField *fld, const QModelIndex &index);

    void CheckTable();
    void Clicked(const QModelIndex &index);
    void SegmentButtonClicked(const QModelIndex &index);
    void OnOk();

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
    void CamelCaseAction();
    void CheckAction();
    void TableObjects();
    void ImportFromTable();

    void onUserActionTriggered();

protected:
    void paintEvent(QPaintEvent *paintEvent) override;
    void keyPressEvent(QKeyEvent *event) override;

    virtual void initRibbonPanels() Q_DECL_OVERRIDE;
    virtual void updateRibbonTabs() Q_DECL_OVERRIDE;
    virtual void clearRibbonTabs() Q_DECL_OVERRIDE;

private:
    void initRibbonFmtPanel();
    void initRibbonFieldsPanel();
    void initRibbonDataPanel();

    int CheckAppy();
    int SaveTable();
    void setupUndoRedo();
    void setupFind();
    void SetUnclosableSystemTabs();
    int SelectTableFieldsDailog(const QString &title, QList<FmtField*> *pFldList, QWidget *userwidget = nullptr, const QString &icon = QString());
    void AddSqlCodeTab(const QString &title, const QString &code, bool OpenTab = true, bool WordWrap = false, bool AddConvertButton = true);
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

    QMenu *pCopyMenu, *pActionsMenu, *pCodeGenMenu, *pUserActionsMenu;
    QAction *m_saveToXml, *m_createTableSql, *m_rebuildOffsets, *m_MassRemoveFields;
    QAction *m_unloadDbf, *m_loadDbf, *m_ImportData, *m_ImportFromTable;
    QAction *m_AddFieldsToEnd, *m_InsertFields, *m_CopyFields, *m_PasteFields, *m_EditContent;
    QAction *m_GenDelScript, *m_GenAddScript, *m_GenCreateTbSql, *m_GenModifyScript;
    QAction *m_GenInsertTemplate, *m_CamelCaseAction, *m_GenDiffToScript, *m_pCompareFmt;
    QAction *m_TableObjects;
    QAction *m_CheckAction;
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

    SARibbonCategory* m_pFmtCategory;

    QMenu *m_pInitTableMenu;
    QAction *m_pInitTableMenuAction, *m_pInitCreateTableAction, *m_pInitCreateIndexAction;
    QAction *m_pCheckAction;
};

#endif // FMTWORKWINDOW_H
