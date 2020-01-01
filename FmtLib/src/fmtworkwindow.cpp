#include "fmtworkwindow.h"
#include "ui_fmtworkwindow.h"
#include "fmtfieldsdelegate.h"
#include "fmtindecesdelegate.h"
#include "fmtfieldstableview.h"
#include "fmtindextreeview.h"
#include "fmtindecesmodel.h"
#include "fmtfildsmodel.h"
#include "dbinitdlg.h"
#include "fmtfielddlg.h"
#include "fmtcore.h"
#include "fmterrors.h"
#include "errordlg.h"
#include "fmtimpexpwrp.h"
#include "fmtfield.h"
#include "fmtworkwndgen.h"
#include "undolistpopup.h"
#include "selectfieldsmodel.h"
#include "selectfiltereddlg.h"
#include "codeeditor.h"
#include "lineeditaction.h"
#include "fmtscriptwindow.h"
#include "fmrichtextwidget.h"
#include "fmteditcontentwindow.h"
#include "fmtindecesmodelitem.h"
#include "fmtsegmentflagsdlg.h"
#include "fmtsegment.h"
#include "widgets/filteredtablewidget.h"
#include "widgets/editcontent/import/importwizard.h"
#include <QtWidgets>
#include <QClipboard>
#include <QMessageBox>
#include <QShortcut>
#include <QTemporaryFile>
#include <QProgressDialog>


FmtFieldsTableViewFilterController::FmtFieldsTableViewFilterController(FmtFieldsDelegate *delegate) :
    pDelegate(delegate)
{

}

QWidget *FmtFieldsTableViewFilterController::create(const int &index)
{
    QLineEdit *w = new QLineEdit();
    w->setClearButtonEnabled(true);
    if (index == FmtFildsModel::fld_Name)
    {
        connect(w, &QLineEdit::textChanged, pDelegate, &FmtFieldsDelegate::setNameText);
        w->setPlaceholderText(tr("Значение для поиска"));
    }
    if (index == FmtFildsModel::fld_Comment)
    {
        connect(w, &QLineEdit::textChanged, pDelegate, &FmtFieldsDelegate::setCommentText);
        w->setPlaceholderText(tr("Значение для поиска"));
    }
    else if (index == FmtFildsModel::fld_Type)
        w->setEnabled(false);
    else if (index == FmtFildsModel::fld_Size)
        w->setEnabled(false);
    else if (index == FmtFildsModel::fld_DbName)
        w->setEnabled(false);
    else if (index == FmtFildsModel::fld_DbName + 1)
        w->setEnabled(false);
    return w;
}

// ===============================================================================

FmtWorkWindow::FmtWorkWindow(QWidget *parent) :
    MdiSubInterface(parent),
    ui(new Ui::FmtWorkWindow),
    pTable(Q_NULLPTR)
{
    ui->setupUi(this);
    pMapper = new QDataWidgetMapper(this);

    pFieldsDelegate = new FmtFieldsDelegate(this);
    pTableHeaderDelegate = new FmtFieldsTableHeaderDelegate(Qt::Horizontal, this);
    pTableView = new FmtFieldsTableView;
    pTreeView = new FmtIndexTreeView;
    pFilteredTableView = new FilteredTableWidget();
    pFieldsFilterController = new FmtFieldsTableViewFilterController(pFieldsDelegate);
    pFilteredTableView->setController(pFieldsFilterController);

    pTableView->setItemDelegate(pFieldsDelegate);
    pTableView->verticalHeader()->setDefaultSectionSize(25);

    pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    pTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    pTableView->setHorizontalHeader(pTableHeaderDelegate);
    pFilteredTableView->setTableView(pTableView);

    ui->tabWidget->widget(0)->layout()->addWidget(pFilteredTableView);

    pAddIndex = new QPushButton("Добавить индекс", ui->tabWidget->widget(1));
    pHorizontalSpacer = new QSpacerItem(100, 20, QSizePolicy::Expanding);
    pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->addWidget(pAddIndex);
    pHBoxLayout->addSpacerItem(pHorizontalSpacer);
    ui->tabWidget->widget(1)->layout()->addItem(pHBoxLayout);
    ui->tabWidget->widget(1)->layout()->addWidget(pTreeView);

    QList<QAbstractButton*> pButtomList = ui->buttonBox->buttons();
    foreach (QAbstractButton *pBtn, pButtomList) {
        QPushButton *btn = qobject_cast<QPushButton*>(pBtn);
        if (btn)
        {
            btn->setAutoDefault(false);
            btn->setDefault(false);
        }
    }

    pCopyMenu = new QMenu(this);
    ui->copyTool->setMenu(pCopyMenu);

    SetupActionsMenu();

    pCodeGenWidget = new FmtWorkWndGen(this);
    ui->tabWidget->addTab(pCodeGenWidget, tr("Код"));

    ui->nameEdit->setMaxLength(fmtm_TableNameMaxSize);
    ui->commentEdit->setMaxLength(fmtm_TableCommentMaxSize);
    ui->scriptButton->setEnabled(false);

    SetUnclosableSystemTabs();

    connect(pAddIndex, SIGNAL(clicked(bool)), SLOT(AddIndex()));
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked(bool)), SLOT(Apply()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), SLOT(OnOk()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), SLOT(InitDB()));
    connect(pTableView, SIGNAL(clicked(QModelIndex)), SLOT(Clicked(QModelIndex)));
    connect(pTreeView, SIGNAL(clicked(QModelIndex)), SLOT(SegmentButtonClicked(QModelIndex)));
    connect(ui->checkButton, SIGNAL(clicked(bool)), SLOT(CheckTable()));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(TabCloseRequested(int)));
    connect(ui->scriptButton, SIGNAL(clicked(bool)), SLOT(OpenScriptEditorWindow()));
}

FmtWorkWindow::~FmtWorkWindow()
{
    delete ui;
    delete pTreeView;
    delete pTableView;
    delete pFieldsDelegate;
}

void FmtWorkWindow::SetupActionsMenu()
{
    pActionsMenu = new QMenu(this);
    m_AddFieldsToEnd = pActionsMenu->addAction(tr("Добавить поля в конец"));
    m_InsertFields = pActionsMenu->addAction(tr("Добавить поля перед..."));
    pActionsMenu->addSeparator();
    m_CopyFields = pActionsMenu->addAction(QIcon(":/img/CopyHS.png"), tr("Копировать поля в буффер обмена..."));
    m_PasteFields = pActionsMenu->addAction(QIcon(":/img/PasteHS.png"), tr("Вставить поля из буффера обмена..."));
    pActionsMenu->addSeparator();
    m_saveToXml = pActionsMenu->addAction(QIcon(":/img/savexml.png"), tr("Экспорт в XML"));
    pActionsMenu->addSeparator();
    m_MassRemoveFields = pActionsMenu->addAction(QIcon(":/img/remove.png"), tr("Удалить поля из таблицы"));
    pActionsMenu->addSeparator();
    m_EditContent = pActionsMenu->addAction(QIcon(":/img/EditTableHS.png"), tr("Редактировать содержимое"));
    m_unloadDbf = pActionsMenu->addAction(tr("Выгрузить содержимое в *.dat"));
    m_loadDbf = pActionsMenu->addAction(tr("Загрузить содержимое из *.dat"));
    pActionsMenu->addSeparator();
    m_ImportData = pActionsMenu->addAction(QIcon(":/img/ImportContent.png"), tr("Загрузить данные"));
    pActionsMenu->addSeparator();
    m_createTableSql = pActionsMenu->addAction(QIcon(":/img/savesql.png"), tr("Сохранить CreateTablesSql скрипт"));
    m_rebuildOffsets = pActionsMenu->addAction(tr("Перестроить смещения"));
    pActionsMenu->addSeparator();
    pCodeGenMenu = pActionsMenu->addMenu(tr("Скрипты обновления"));
    m_GenCreateTbSql = pCodeGenMenu->addAction(tr("Создание таблицы"));
    m_GenAddScript = pCodeGenMenu->addAction(tr("Добавление полей"));
    m_GenModifyScript = pCodeGenMenu->addAction(tr("Изменение полей"));
    m_GenDelScript = pCodeGenMenu->addAction(tr("Удаления полей"));
    ui->pushActions->setMenu(pActionsMenu);

    connect(m_saveToXml, SIGNAL(triggered(bool)), SLOT(ExportXml()));
    connect(m_createTableSql, SIGNAL(triggered(bool)), SLOT(CreateTableSql()));
    connect(m_unloadDbf, SIGNAL(triggered(bool)), SLOT(UnloadToDbf()));
    connect(m_loadDbf, SIGNAL(triggered(bool)), SLOT(LoadFromDbf()));
    connect(m_GenDelScript, SIGNAL(triggered(bool)), SLOT(GenDeleteFiledsScript()));
    connect(m_GenAddScript, SIGNAL(triggered(bool)), SLOT(GenAddFiledsScript()));
    connect(m_GenModifyScript, SIGNAL(triggered(bool)), SLOT(GenModifyTableFields()));
    connect(m_GenCreateTbSql, SIGNAL(triggered(bool)), SLOT(GenCreateTableScript()));
    connect(m_MassRemoveFields, SIGNAL(triggered(bool)), SLOT(RemoveTableFields()));
    connect(m_AddFieldsToEnd, SIGNAL(triggered(bool)), SLOT(AddFieldsToEnd()));
    connect(m_InsertFields, SIGNAL(triggered(bool)), SLOT(InsertFieldsBefore()));
    connect(m_EditContent, SIGNAL(triggered(bool)), SLOT(EditContent()));
    connect(m_CopyFields, SIGNAL(triggered(bool)), SLOT(CopyFields()));
    connect(m_PasteFields, SIGNAL(triggered(bool)), SLOT(PasteFields()));
    connect(m_ImportData, SIGNAL(triggered(bool)), SLOT(OnImport()));
}

void FmtWorkWindow::setupFind()
{
    /*pFindLineEdit = new LineEditAction(this);
    pFindBtn = new QToolButton(this);
    pFindMenu = new QMenu(this);
    pFindShortcut = new QShortcut(QKeySequence::Find, this);

    pFindBtn->setIcon(QIcon("://img/FindHS.png"));
    pFindBtn->setPopupMode(QToolButton::InstantPopup);
    pFindBtn->setMenu(pFindMenu);
    pUndoRedoLayout->addWidget(pFindBtn);
    pFindMenu->addAction(pFindLineEdit);

    connect(pFindLineEdit, SIGNAL(textChanged(QString)), pFieldsDelegate, SLOT(setHighlightText(QString)));
    connect(pFindMenu, SIGNAL(aboutToShow()), pFindLineEdit, SLOT(setFocus()));
    connect(pFindShortcut, SIGNAL(activated()), pFindBtn, SLOT(showMenu()));*/
}

void FmtWorkWindow::SetUnclosableSystemTabs()
{
    QTabBar *tabBar = ui->tabWidget->findChild<QTabBar*>();
    tabBar->setTabButton(FmtWinTabs_Columns, QTabBar::RightSide, Q_NULLPTR);
    tabBar->setTabButton(FmtWinTabs_Columns, QTabBar::LeftSide, Q_NULLPTR);

    tabBar->setTabButton(FmtWinTabs_Index, QTabBar::RightSide, Q_NULLPTR);
    tabBar->setTabButton(FmtWinTabs_Index, QTabBar::LeftSide, Q_NULLPTR);

    tabBar->setTabButton(FmtWinTabs_Code, QTabBar::RightSide, Q_NULLPTR);
    tabBar->setTabButton(FmtWinTabs_Code, QTabBar::LeftSide, Q_NULLPTR);
}

void FmtWorkWindow::setFmtTable(FmtSharedTablePtr &table)
{
    pTable = table;
    pCodeGenWidget->setTable(pTable);
    FillIndecesList();
    UpdateCopyMenu(pTable->name());

    pMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    pMapper->setModel(pTable.data());
    pMapper->addMapping(ui->idEdit, FmtTable::fld_Id);
    pMapper->addMapping(ui->nameEdit, FmtTable::fld_Name);
    pMapper->addMapping(ui->commentEdit, FmtTable::fld_Comment);
    pMapper->addMapping(ui->blobLen, FmtTable::fld_BlobLen);
    pMapper->addMapping(ui->isTemp, FmtTable::fld_isTemporary);
    pMapper->addMapping(ui->isRec, FmtTable::fld_isRecord);
    pMapper->addMapping(ui->keyComboBox, FmtTable::fld_PkIndx, "currentText");

    pMapper->toFirst();

    QAbstractItemModel *fieldsModel = pTable->fieldsModel();
    pTableView->setModel(fieldsModel);
    pTreeView->setModel(pTable->indecesModel());

    static QList<int> m_Width = QList<int>()
            << 150 << 65 << 50 << 200 << 150 << 50;

    static QList<int> m_iWidth = QList<int>()
            << 175 << 65 << 35 << 40 << 70 << 35 << 40 << 70 << 50 << 50 << 175;

    for (int i = 0; i < m_iWidth.size(); i++)
        pTreeView->setColumnWidth(i, m_iWidth[i]);

    for (int i = 0; i < m_Width.size(); i++)
        pTableView->setColumnWidth(i, m_Width[i]);

    if (pTable->id() != 0)
        ui->nameEdit->setReadOnly(true);

    pTreeView->setFmtTable(pTable.data());
    pTreeView->expandAll();

    isTemporaryTableChanged(pTable->isTemporary());
    setupUndoRedo();
    setupFind();

    ConnectionInfo *info = pTable->connection();
    dcolor = info->color().darker();
    color = info->color();

    if (info->type() != ConnectionInfo::CON_ORA)
    {
        ui->pushButton->setEnabled(false);
        m_saveToXml->setEnabled(false);
    }

    ui->scriptButton->setEnabled(true);

    /*pTableView->horizontalHeader()->setSectionResizeMode(FmtIndecesModelItem::fld_Comment, QHeaderView::Stretch);
    pTableView->horizontalHeader()->setSectionResizeMode(FmtIndecesModelItem::fld_Panel, QHeaderView::Fixed);*/

    connect(pTable->indecesModel(), SIGNAL(modelReset()), SLOT(indexModelReseted()));
    connect(pTable->indecesModel(), SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(indexModelInserted(QModelIndex,int,int)));
    connect(pTable.data(), SIGNAL(indexAdded(FmtIndex*)), SLOT(FillIndecesList()));
    connect(pTable.data(), SIGNAL(indexRemoved(quint32)), SLOT(FillIndecesList()));
    connect(pTable.data(), SIGNAL(indexChanged(FmtIndex*)), SLOT(OnIndexChanged(FmtIndex*)));
    connect(pTable.data(), SIGNAL(nameChanged(QString)), SLOT(UpdateCopyMenu(QString)));
    connect(fieldsModel, SIGNAL(AddedToLast(FmtField*, const QModelIndex&)), SLOT(AddedToLast(FmtField*,QModelIndex)));
    connect(pTable.data(), SIGNAL(isTemporaryChanged(bool)), SLOT(isTemporaryTableChanged(bool)));
    connect(m_rebuildOffsets, SIGNAL(triggered(bool)), pTable.data(), SLOT(rebuildOffsets()));
}

void FmtWorkWindow::setupUndoRedo()
{
    pUndoMenu = new QMenu(this);
    pUndoActionWrp = new QAction(tr("Отменить"), this);
    pUndoPopup = new UndoListPopup(this);

    pUndoRedoBtnContainer = new QFrame();
    pUndoBtn = new QToolButton(this);
    pUndoBtn->setMenu(pUndoMenu);
    pUndoBtn->setDefaultAction(pUndoActionWrp);
    pUndoBtn->setPopupMode(QToolButton::MenuButtonPopup);
    pUndoBtn->setMinimumHeight(16);
    pRedoBtn = new QToolButton(this);
    pRedoBtn->setMinimumHeight(16);

    pUndoRedoLayout = new QHBoxLayout();
    pUndoRedoLayout->addWidget(pUndoBtn);
    pUndoRedoLayout->addWidget(pRedoBtn);
    pUndoRedoLayout->setContentsMargins(0, 0, 4, 0);

    pUndoRedoBtnContainer->setLayout(pUndoRedoLayout);
    ui->tabWidget->setCornerWidget(pUndoRedoBtnContainer);

    pUndoMenu->clear();
    pUndoPopup->setStack(pTable->undoStack());

    pUndoAction = pTable->undoAction();
    pRedoAction = pTable->redoAction();
    pRedoBtn->setDefaultAction(pRedoAction);

    pUndoMenu->addAction(pUndoActionWrp);
    pUndoMenu->addAction(pUndoPopup);

    pUndoAction->setIcon(QIcon(":/img/Edit_UndoHS.png"));
    pRedoAction->setIcon(QIcon(":/img/Edit_RedoHS.png"));
    undoDisabled = QIcon(pUndoAction->icon().pixmap(16, QIcon::Disabled));
    pUndoActionWrp->setIcon(undoDisabled);

    connect(pUndoActionWrp, SIGNAL(triggered(bool)), pUndoAction, SLOT(trigger()));
    connect(pUndoAction, SIGNAL(changed()), SLOT(undoActionChanged()));
}

void FmtWorkWindow::undoActionChanged()
{
    QString undoText = pTable->undoStack()->undoText();
    pUndoActionWrp->setToolTip(undoText);

    if (pTable->undoStack()->canUndo())
    {
        pUndoActionWrp->setIcon(QIcon(":/img/Edit_UndoHS.png"));
        pUndoActionWrp->setText(tr("Отменить: %1...")
                                .arg(undoText.mid(0, 10)));
    }
    else
    {
        pUndoActionWrp->setIcon(undoDisabled);
        pUndoActionWrp->setText(tr("Отменить"));
    }
}

void FmtWorkWindow::indexModelReseted()
{
    pTreeView->expandAll();
}

void FmtWorkWindow::indexModelInserted(const QModelIndex &parent, const int &first, const int &last)
{
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)
    pTreeView->expandAll();
}

void FmtWorkWindow::AddIndex()
{
    pTable->addIndex();
}

void FmtWorkWindow::FillIndecesList()
{
    QStringList lst = pTable->uniqueIndeces();
    lst.prepend("-1");
    ui->keyComboBox->clear();
    ui->keyComboBox->insertItems(0, lst);
}

void FmtWorkWindow::OnIndexChanged(FmtIndex *index)
{
    Q_UNUSED(index)
    FillIndecesList();
}

int FmtWorkWindow::CheckAppy()
{
    FmtErrors err;
    pTable->checkErrors(&err);

    int stat = 0;
    if (!err.isEmpty())
    {
        QString msg;
        if (err.hasErrors())
            msg = tr("Сохранение не возможно, т.к имеются ошибки наполнения Fmt словаря.");
        else
            msg = tr("Имеются предупреждения по структуре Fmt словаря. Сохранить изменения не смотря на сообщения?");
        ErrorDlg dlg(ErrorDlg::mode_MessageBox, this);
        dlg.setErrors(&err);
        dlg.setWindowTitle(tr("Сохранение структуры"));
        dlg.setMessage(msg);

        if (dlg.exec() == QDialog::Rejected)
            stat = 1;
    }

    return stat;
}

int FmtWorkWindow::SaveTable()
{
    int stat = pTable->save();

    if (!stat)
    {
        pMapper->toFirst();
        if (pTable->id() != 0)
            ui->nameEdit->setReadOnly(true);
    }
    update();

    return stat;
}

void FmtWorkWindow::Apply()
{
    int stat = CheckAppy();

    if (!stat)
        stat = SaveTable();

    if (!stat)
        emit needUpdateTableList();
}

void FmtWorkWindow::OnOk()
{
    int stat = CheckAppy();

    if (!stat)
        stat = SaveTable();

    if (!stat)
    {
        emit needUpdateTableList();
        emit QDialog::accepted();
    }
}

void FmtWorkWindow::Clicked(const QModelIndex &index)
{
    if (index.column() == FmtFildsModel::fld_Custom && index.row() < pTable->fieldsCount())
    {
        FmtFieldDlg dlg(this);
        dlg.setModel(pTable->fieldsModel());
        dlg.setCurrentIndex(index.row());
        dlg.exec();
    }
}

void FmtWorkWindow::rebuildOffsets()
{
    pTable->rebuildOffsets();
}

void FmtWorkWindow::setDialogButtonsVisible(bool f)
{
    ui->buttonBox->setVisible(f);
}

void FmtWorkWindow::setInitButtonVisible(bool f)
{
    ui->pushButton->setVisible(f);
}

void FmtWorkWindow::FieldAdded(FmtField *fld)
{
    Q_UNUSED(fld)
}

void FmtWorkWindow::InitDB()
{
    InitFmtTable(pTable, this);
}

void FmtWorkWindow::CheckTable()
{
    FmtErrors e;
    if (!pTable->checkErrors(&e))
    {
        ErrorDlg dlg(ErrorDlg::mode_Information, this);
        dlg.setErrors(&e);
        dlg.setMessage(tr("Результат проверки: "));
        dlg.exec();
    }
    else
    {
        QMessageBox::information(this, tr("Результат"), tr("Проверка ошибок не выявила!"));
    }
}

void FmtWorkWindow::ExportXml()
{
    FmtImpExpWrp imp(pTable->connection(), this);
    QString dir = QFileDialog::getExistingDirectory(this, tr("Экспорт файла"), imp.lastImportDir());

    if (dir.isEmpty() || !pTable->isExists())
        return;

    ExportFmtToXml(pTable->connection(), QStringList()
                   << pTable->name(), dir, true, true, this);
}

void FmtWorkWindow::AddedToLast(FmtField *fld, const QModelIndex &index)
{
    Q_UNUSED(fld);
    Q_UNUSED(index);
    pTableView->scrollToBottom();
}

void FmtWorkWindow::CreateTableSql()
{
    SaveFmtTableSql(pTable, this);
}

void FmtWorkWindow::UpdateCopyMenu(const QString &table)
{
    pCopyMenu->clear();

    if (table.isEmpty())
        ui->copyTool->setEnabled(false);
    else
    {
        ui->copyTool->setEnabled(true);
        QStringList lst = FmtTableStringList(table);

        foreach (const QString &str, lst)
        {
            QAction *action = pCopyMenu->addAction(str);
            connect(action, SIGNAL(triggered(bool)), SLOT(CopyAction()));
        }
    }
    QMdiSubWindow *mdiWnd = mdiWindow();

    if (mdiWnd)
        mdiWnd->setWindowTitle(makeWindowTitle());
}

void FmtWorkWindow::CopyAction()
{
    QAction *action = qobject_cast<QAction*>(sender());
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(action->text());
    QToolTip::showText(mapToGlobal(ui->copyTool->pos()), tr("Текст скопирован: %1").arg(action->text()), this, QRect(), 1000);
}

void FmtWorkWindow::paintEvent(QPaintEvent *paintEvent)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.save();
    QPen pen(dcolor);
    pen.setWidth(2);
    if (!pTable->id())
        pen.setStyle(Qt::DotLine);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);

    QRect rc = rect();
    rc.setX(rc.x() + 1);
    rc.setY(rc.y() + 1);
    rc.setWidth(rc.width() - 1);
    rc.setHeight(rc.height() - 1);
    p.drawRect(rc);

    QPainterPath path;
    int startx = ui->idEdit->x() + ui->idEdit->width();
    int tmpy = ui->idEdit->y() + ui->idEdit->height();

    p.setBrush(color);
    path.moveTo(startx, -1);
    path.cubicTo(startx + 50, 0, startx + 25, tmpy / 4, startx + 50, tmpy / 2);
    path.lineTo(width(), tmpy / 2);
    p.drawPath(path);
    p.restore();

    p.fillRect(startx + 50, 0, width(), tmpy / 2 - 1, color);

    QDialog::paintEvent(paintEvent);
}

QUndoStack *FmtWorkWindow::tableUndoStack()
{
    return pTable->undoStack();
}

void FmtWorkWindow::isTemporaryTableChanged(bool value)
{
    if (!pParentWnd)
        return;

    QIcon icon;
    if (!value)
        icon = QIcon(":/table");
    else
        icon = QIcon(":/tablet");

    if (pParentWnd)
        pParentWnd->setWindowIcon(icon);
}

void FmtWorkWindow::UnloadToDbf()
{
    StartUnloadDbf(pTable->connection(), pTable->name(), this);
}

void FmtWorkWindow::LoadFromDbf()
{
    QString RsExpUnlDir = settings()->value("RsExpUnlDir").toString();
    QString file = QFileDialog::getOpenFileName(this, QString(), RsExpUnlDir, QString("%1.dat (%1.dat)").arg(pTable->name()));

    if (!file.isEmpty())
        StartLoadDbf(pTable->connection(), file, this);
}

void FmtWorkWindow::TabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

int FmtWorkWindow::SelectTableFieldsDailog(const QString &title, QList<FmtField*> *pFldList)
{
    int stat = 0;

    SelectFieldsModel selFldModel(pTable.data(), this);
    SelectFilteredDlg dlg(this);
    dlg.setWindowTitle(title);
    dlg.setFilteredModel(&selFldModel);
    dlg.setHidenColumns(QList<int>()
                        << FmtFildsModel::fld_Size
                        << FmtFildsModel::fld_DbName
                        << FmtFildsModel::fld_Custom
                        << FmtFildsModel::fld_Offset
                        << FmtFildsModel::fld_Outlen
                        << FmtFildsModel::fld_Decpoint
                        << FmtFildsModel::fld_Hidden
                        << FmtFildsModel::fld_Id
                        << FmtFildsModel::fld_FmtId
                        << FmtFildsModel::fld_TypeIndex);
    dlg.setColumnWidth(FmtFildsModel::fld_Name, 150);
    connect(&dlg, SIGNAL(textChanged(QString)), &selFldModel, SLOT(setFilterFieldName(QString)));
    stat = dlg.exec();

    if (pFldList && stat == QDialog::Accepted)
        *pFldList = selFldModel.checkedFields();

    return stat;
}

void FmtWorkWindow::AddSqlCodeTab(const QString &title, const QString &code, bool OpenTab)
{
    QSyntaxHighlighter *pCurrentHighlighter = Q_NULLPTR;
    CodeEditor *editor = new CodeEditor();
    pCurrentHighlighter = new SqlHighlighter(editor->document());
    editor->setReadOnly(true);
    editor->setPlainText(code);

    int tab = ui->tabWidget->addTab(editor, title);
    if (OpenTab) ui->tabWidget->setCurrentIndex(tab);
}

void FmtWorkWindow::GenDeleteFiledsScript()
{
    QList<FmtField*> FldList;
    if (SelectTableFieldsDailog(tr("Выбор полей для скрипта обновления"), &FldList) == QDialog::Accepted)
    {
        QString code = FmtGenUpdateDeleteColumnScript(FldList);
        AddSqlCodeTab(tr("Скрипт удаления полей"), code);
    }
}

void FmtWorkWindow::GenAddFiledsScript()
{
    QList<FmtField*> FldList;
    if (SelectTableFieldsDailog(tr("Выбор полей для скрипта обновления"), &FldList) == QDialog::Accepted)
    {
        QString code = FmtGenUpdateAddColumnScript(FldList);
        AddSqlCodeTab(tr("Скрипт добавления полей"), code);
    }
}

void FmtWorkWindow::GenModifyTableFields()
{
    QList<FmtField*> FldList;
    if (SelectTableFieldsDailog(tr("Выбор полей для скрипта изменения типа"), &FldList) == QDialog::Accepted)
    {
            QString code = FmtGenModifyColumnScript(FldList);
            AddSqlCodeTab(tr("Скрипт изменения типа полей"), code);
    }
}

void FmtWorkWindow::GenCreateTableScript()
{
    QString code = FmtGenUpdateCreateTableScript(pTable);
    AddSqlCodeTab(tr("Скрипт создания таблицы"), code);
}

void FmtWorkWindow::RemoveTableFields()
{
    QList<FmtField*> FldList;
    if (SelectTableFieldsDailog(tr("Выбор полей для удаления"), &FldList) == QDialog::Accepted)
    {
        foreach (FmtField *fld, FldList) {
            qint32 row = pTable->fieldNum(fld);
            pTable->removeField(row);
        }
    }
}

void FmtWorkWindow::OpenScriptEditorWindow()
{
    FmtScriptWindow *script = new FmtScriptWindow(pTable);
    int tab = ui->tabWidget->addTab(script, tr("Скрипт"));
    ui->tabWidget->setCurrentIndex(tab);
}

void FmtWorkWindow::AddFieldsToEnd()
{
    QDialog dlg(this);
    dlg.setWindowTitle(m_AddFieldsToEnd->text());
    FmRichTextWidget rich;
    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QVBoxLayout *l = new QVBoxLayout;
    l->addWidget(&rich);
    l->addWidget(&buttons);
    dlg.setLayout(l);

    connect(&buttons, SIGNAL(accepted()), &dlg, SLOT(accept()));
    connect(&buttons, SIGNAL(rejected()), &dlg, SLOT(reject()));
    if (dlg.exec() == QDialog::Accepted)
    {
        FmRichTextReadParam prm;
        prm.insertBefore = -1;
        prm.readComment = false;
        prm.readTableName = false;
        prm.ignoreFirstRow = false;
        rich.ReadConetent(pTable, prm);
    }
    delete l;
}

void FmtWorkWindow::InsertFieldsBefore()
{
    QDialog dlg(this);
    dlg.setWindowTitle(m_InsertFields->text());
    FmRichTextWidget rich;
    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QComboBox combo;

    combo.setModel(pTable->fieldsModel());
    rich.AddCustomWidget(tr("Вставить перед:"), &combo);

    QVBoxLayout *l = new QVBoxLayout;
    l->addWidget(&rich);
    l->addWidget(&buttons);
    dlg.setLayout(l);

    connect(&buttons, SIGNAL(accepted()), &dlg, SLOT(accept()));
    connect(&buttons, SIGNAL(rejected()), &dlg, SLOT(reject()));
    if (dlg.exec() == QDialog::Accepted)
    {
        FmRichTextReadParam prm;
        prm.insertBefore = combo.currentIndex();
        prm.readComment = false;
        prm.readTableName = false;
        prm.ignoreFirstRow = false;
        rich.ReadConetent(pTable, prm);
    }
    delete l;
}

void FmtWorkWindow::EditContent()
{
    if (!pTable->isRecord())
    {
        FmtEditContentWindow *content = new FmtEditContentWindow(pTable);
        int tab = ui->tabWidget->addTab(content, tr("Содержимое"));
        ui->tabWidget->setCurrentIndex(tab);
    }
}

void FmtWorkWindow::CopyFields()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QList<FmtField*> FldList;
    if (SelectTableFieldsDailog(tr("Выбор полей для копирования в буффер"), &FldList) == QDialog::Accepted)
    {
        QByteArray itemData;
        QDataStream stream(&itemData, QIODevice::WriteOnly);
        QMimeData* mimeData = new QMimeData;

        stream << (quint16)FldList.size();
        foreach (FmtField *fld, FldList) {
            stream << fld;
        }
        mimeData->setData(FmtField::getMimeType(), itemData);
        clipboard->clear();
        clipboard->setMimeData(mimeData);
    }
}

void FmtWorkWindow::PasteFields()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (!mimeData)
        return;

    if (!mimeData->hasFormat(FmtField::getMimeType()))
        return;

    QByteArray itemData = mimeData->data(FmtField::getMimeType());
    QDataStream stream(&itemData, QIODevice::ReadOnly);

    quint16 size = 0;
    stream >> size;

    for (int i = 0; i < size; i++)
    {
        QMap<quint16,QVariant> fldDataMap;
        stream >> fldDataMap;
        pTable->addField(fldDataMap);
        //qDebug() << fldDataMap;
    }
}

void FmtWorkWindow::OnImport()
{
    ImportWizard dlg(pTable);
    dlg.exec();
}

void FmtWorkWindow::SegmentButtonClicked(const QModelIndex &index)
{
    if (index.parent().isValid() && index.column() == FmtIndecesModelItem::fld_Flag)
    {
        FmtSegment *pSegment = static_cast<FmtSegment*>(index.internalPointer());
        FmtSegmentFlagsDlg dlg(pSegment, this);
        dlg.exec();
    }
}

QString FmtWorkWindow::makeWindowTitle()
{
    QString schemeName = connection()->schemeName();
    return QString("%1@%2")
            .arg(schemeName.trimmed())
            .arg(pTable->name());
}
