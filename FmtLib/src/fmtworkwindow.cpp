#include "fmtworkwindow.h"
#include "geninterfacefactorymodel.h"
#include "ui_fmtworkwindow.h"
#include "fmtfieldsdelegate.h"
#include "fmtindecesdelegate.h"
#include "fmtfieldstableview.h"
#include "fmtindextreeview.h"
#include "fmtindecesmodel.h"
#include "fmtfildsmodel.h"
#include "dbinitdlg.h"
#include "fmtfielddlg.h"
#include "errorsmodel.h"
#include "errordlg.h"
#include "fmtcore.h"
#include "fmtimpexpwrp.h"
#include "fmtfield.h"
#include "fmtworkwndgen.h"
#include "undolistpopup.h"
#include "selectfieldsmodel.h"
#include "selectfiltereddlg.h"
#include <codeeditor/codeeditor.h>
#include <codeeditor/highlighterstyle.h>
#include <codeeditor/codehighlighter.h>
#include "lineeditaction.h"
#include "fmtscriptwindow.h"
#include "fmrichtextwidget.h"
#include "fmteditcontentwindow.h"
#include "fmtindecesmodelitem.h"
#include "fmtsegmentflagsdlg.h"
#include "fmtsegment.h"
#include "widgets/filteredtablewidget.h"
#include "widgets/editcontent/import/importwizard.h"
#include "widgets/comparefmt/comparefmtviewtab.h"
#include "rslexecutors/toolbaractionexecutor.h"
#include "selectfolderdlg.h"
#include "gensqltemplatedlg.h"
#include "mainwindow.h"
#include "fmtapplication.h"
#include "src/widgets/tablestructsqldlg.h"
#include "src/models/generatorsproxymodel.h"
#include "widgets/comparefmt/comparefmtwizard.h"
#include "src/core/fmttablecomparemodel.h"
#include "wizards/RichTextToInsertWizard/richtexttoinsertwizard.h"
#include <SARibbon.h>
#include <QtWidgets>
#include <QClipboard>
#include <QMessageBox>
#include <QShortcut>
#include <QTemporaryFile>
#include <QProgressDialog>
#include <QDirIterator>
#include <QSpacerItem>

// Внешние функции для добавления tooltips
extern void toolAddActionWithTooltip(QAction* action, const QString& description, const QKeySequence& shortcut = QKeySequence());
extern void toolAddActionWithTooltip(QMenu* menu, const QString& description, const QKeySequence& shortcut = QKeySequence());
extern void toolAddActionWithTooltip(QToolButton* button, const QString& description, const QKeySequence& shortcut = QKeySequence());

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
        w->setToolTip(tr("Фильтрация полей по имени"));
    }
    if (index == FmtFildsModel::fld_Comment)
    {
        connect(w, &QLineEdit::textChanged, pDelegate, &FmtFieldsDelegate::setCommentText);
        w->setPlaceholderText(tr("Значение для поиска"));
        w->setToolTip(tr("Фильтрация полей по комментарию"));
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
    pTable(Q_NULLPTR),
    pCopyMenuAction(nullptr),
    pCopyMenu(nullptr),
    pActionsMenu(nullptr),
    pCodeGenMenu(nullptr),
    pUserActionsMenu(nullptr),
    pGenCppCodeMenu(nullptr),
    m_saveToXml(nullptr),
    m_createTableSql(nullptr),
    m_rebuildOffsets(nullptr),
    m_MassRemoveFields(nullptr),
    m_unloadDbf(nullptr),
    m_loadDbf(nullptr),
    m_ImportData(nullptr),
    m_ImportFromTable(nullptr),
    m_AddFieldsToEnd(nullptr),
    m_InsertFields(nullptr),
    m_CopyFields(nullptr),
    m_PasteFields(nullptr),
    m_EditContent(nullptr),
    m_GenDelScript(nullptr),
    m_GenAddScript(nullptr),
    m_GenCreateTbSql(nullptr),
    m_GenModifyScript(nullptr),
    m_GenInsertTemplate(nullptr),
    m_CamelCaseAction(nullptr),
    m_GenDiffToScript(nullptr),
    m_pCompareFmt(nullptr),
    m_TableObjects(nullptr),
    pGenCodeAction(nullptr),
    pGenCppCodeAction(nullptr),
    m_DiffToScript(nullptr),
    m_pInitTableMenu(nullptr),
    m_pLastActiveFmtTab(nullptr)
{
    ui->setupUi(this);

    // Добавляем tooltips для элементов интерфейса
    ui->toolStdBlobSize->setToolTip(tr("Установить стандартный размер BLOB (16384)"));
    ui->blobCombo->setToolTip(tr("Выбрать тип хранения больших данных:\n"
                                 "- Нет - без BLOB полей\n"
                                 "- BLOB - двоичные данные\n"
                                 "- CLOB - символьные данные"));
    ui->nameEdit->setToolTip(tr("Имя таблицы в словаре (максимум %1 символов)").arg(fmtm_TableNameMaxSize));
    ui->commentEdit->setToolTip(tr("Комментарий к таблице (максимум %1 символов)").arg(fmtm_TableCommentMaxSize));
    ui->idEdit->setToolTip(tr("Уникальный идентификатор записи (генерируется автоматически)"));
    ui->keyComboBox->setToolTip(tr("Выбрать первичный ключ таблицы из списка уникальных индексов"));
    ui->isTemp->setToolTip(tr("Отметить таблицу как временную (Temporary)"));
    ui->isRec->setToolTip(tr("Отметить таблицу как запись (Record)"));
    ui->blobLen->setToolTip(tr("Размер BLOB поля в байтах"));

    m_pGeneratorsModel = new GenInterfaceFactoryModel(this);
    m_pGeneratorsProxyModel = new GeneratorsProxyModel(this);
    m_pGeneratorsProxyModel->setSourceModel(m_pGeneratorsModel);

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
    pFilteredTableView->setView(pTableView, pTableView->horizontalHeader());

    ui->tabWidget->widget(0)->layout()->addWidget(pFilteredTableView);

    pAddIndex = new QPushButton(tr("Добавить индекс"), ui->tabWidget->widget(1));
    pAddIndex->setToolTip(tr("Добавить новый индекс для текущей таблицы"));
    pHorizontalSpacer = new QSpacerItem(100, 20, QSizePolicy::Expanding);
    pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->addWidget(pAddIndex);
    pHBoxLayout->addSpacerItem(pHorizontalSpacer);
    ui->tabWidget->widget(1)->layout()->addItem(pHBoxLayout);
    ui->tabWidget->widget(1)->layout()->addWidget(pTreeView);

    ui->toolStdBlobSize->setIcon(QIcon::fromTheme("CatalogZone"));
    connect(ui->toolStdBlobSize, &QToolButton::clicked, [=]()
            {
                pTable->setBlobLen(16384);
                QToolTip::showText(QCursor::pos(),
                                   tr("Установлен стандартный размер BLOB: 16384"),
                                   this, QRect(), 1500);
            });

    connect(ui->blobCombo, qOverload<int>(&QComboBox::currentIndexChanged), [=](int index)
            {
                if (index == COMBO_No)
                    pTable->setBlobType(BT_BLOB_NO);
                else if (index == COMBO_BLOB)
                    pTable->setBlobType(BT_BLOB_VAR);
                else
                    pTable->setBlobType(BT_CLOB);
            });

    QList<QAbstractButton*> pButtomList = ui->buttonBox->buttons();
    foreach (QAbstractButton *pBtn, pButtomList)
    {
        QPushButton *btn = qobject_cast<QPushButton*>(pBtn);
        if (btn)
        {
            btn->setAutoDefault(false);
            btn->setDefault(false);
        }
    }

    pCopyMenu = new QMenu(tr("Шаблон строки"), this);
    toolAddActionWithTooltip(pCopyMenu, tr("Копировать имя таблицы в различных форматах в буфер обмена"));

    ui->nameEdit->setMaxLength(fmtm_TableNameMaxSize);
    ui->commentEdit->setMaxLength(fmtm_TableCommentMaxSize);

    SetUnclosableSystemTabs();

    connect(pAddIndex, SIGNAL(clicked(bool)), SLOT(AddIndex()));
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked(bool)), SLOT(Apply()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), SLOT(OnOk()));
    connect(pTableView, SIGNAL(clicked(QModelIndex)), SLOT(Clicked(QModelIndex)));
    connect(pTreeView, SIGNAL(clicked(QModelIndex)), SLOT(SegmentButtonClicked(QModelIndex)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(TabCloseRequested(int)));
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &FmtWorkWindow::currentTabChanged);
}

FmtWorkWindow::~FmtWorkWindow()
{
    delete ui;
    delete pTreeView;
    delete pTableView;
    delete pFieldsDelegate;
}

void FmtWorkWindow::setupFind()
{
    // Метод для настройки поиска
}

void FmtWorkWindow::SetUnclosableSystemTabs()
{
    QTabBar *tabBar = ui->tabWidget->findChild<QTabBar*>();
    tabBar->setTabButton(FmtWinTabs_Columns, QTabBar::RightSide, Q_NULLPTR);
    tabBar->setTabButton(FmtWinTabs_Columns, QTabBar::LeftSide, Q_NULLPTR);

    tabBar->setTabButton(FmtWinTabs_Index, QTabBar::RightSide, Q_NULLPTR);
    tabBar->setTabButton(FmtWinTabs_Index, QTabBar::LeftSide, Q_NULLPTR);
}

void FmtWorkWindow::setFmtTable(FmtSharedTablePtr &table)
{
    pTable = table;

    FillIndecesList();
    UpdateCopyMenu(pTable->name());

    pMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    pMapper->setModel(pTable.data());
    pMapper->addMapping(ui->idEdit, FmtTable::fld_Id);
    pMapper->addMapping(ui->nameEdit, FmtTable::fld_Name);
    pMapper->addMapping(ui->commentEdit, FmtTable::fld_Comment);
    pMapper->addMapping(ui->blobLen, FmtTable::fld_BlobLen, "value");
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

    UpdateTableIcon();
    setupUndoRedo();
    setupFind();

    ConnectionInfo *info = pTable->connection();
    dcolor = QColor(info->color()).darker();
    color = info->color();

    if (info->type() != ConnectionInfo::CON_ORA)
    {
        m_EditContent->setEnabled(false);

        if (m_TableObjects)
            m_TableObjects->setEnabled(false);
    }

    if (!info->hasFeature(ConnectionInfo::CanCreateTable))
    {
        pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        pTableView->setButtonsVisible(false);

        pTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        pTreeView->setButtonsVisible(false);

        ui->buttonBox->setEnabled(false);
        ui->nameEdit->setReadOnly(true);
        ui->commentEdit->setReadOnly(true);

        ui->isTemp->setEnabled(false);
        ui->isRec->setEnabled(false);
        ui->keyComboBox->setEnabled(false);
        ui->blobLen->setEnabled(false);

        m_AddFieldsToEnd->setEnabled(false);
        m_InsertFields->setEnabled(false);
        m_CamelCaseAction->setEnabled(false);
        m_PasteFields->setEnabled(false);
        m_MassRemoveFields->setEnabled(false);
        m_rebuildOffsets->setEnabled(false);

        pAddIndex->setEnabled(false);
        m_rebuildOffsets->setEnabled(false);
    }

    if (!info->hasFeature(ConnectionInfo::CanSaveToXml) && m_saveToXml)
        m_saveToXml->setEnabled(false);

    if (!info->hasFeature(ConnectionInfo::CanLoadUnloadDbf))
    {
        if (m_unloadDbf)
            m_unloadDbf->setEnabled(false);

        if (m_loadDbf)
            m_loadDbf->setEnabled(false);
    }

    ui->blobCombo->blockSignals(true);
    if (pTable->blobType() == BT_BLOB_NO)
        ui->blobCombo->setCurrentIndex(COMBO_No);
    else if (pTable->blobType() == BT_BLOB_VAR || pTable->blobType() == BT_BLOB_STREAM)
        ui->blobCombo->setCurrentIndex(COMBO_BLOB);
    else
        ui->blobCombo->setCurrentIndex(COMBO_CLOB);
    ui->blobCombo->blockSignals(false);

    connect(pTable->indecesModel(), SIGNAL(modelReset()), SLOT(indexModelReseted()));
    connect(pTable->indecesModel(), SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(indexModelInserted(QModelIndex,int,int)));
    connect(pTable.data(), SIGNAL(indexAdded(FmtIndex*)), SLOT(FillIndecesList()));
    connect(pTable.data(), SIGNAL(indexRemoved(quint32)), SLOT(FillIndecesList()));
    connect(pTable.data(), SIGNAL(indexChanged(FmtIndex*)), SLOT(OnIndexChanged(FmtIndex*)));
    connect(pTable.data(), SIGNAL(nameChanged(QString)), SLOT(UpdateCopyMenu(QString)));
    connect(fieldsModel, SIGNAL(AddedToLast(FmtField*, const QModelIndex&)), SLOT(AddedToLast(FmtField*,QModelIndex)));

    connect(ui->isRec, &QCheckBox::toggled, this, &FmtWorkWindow::isRecordTableChanged);
    connect(ui->isTemp, &QCheckBox::toggled, this, &FmtWorkWindow::isTemporaryTableChanged);
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
    pUndoBtn->setToolTip(tr("Отменить последнее действие"));

    pRedoBtn = new QToolButton(this);
    pRedoBtn->setMinimumHeight(16);
    pRedoBtn->setToolTip(tr("Повторить отмененное действие"));

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

    pUndoAction->setIcon(QIcon::fromTheme("Undo"));
    pRedoAction->setIcon(QIcon::fromTheme("Redo"));
    undoDisabled = QIcon(QIcon::fromTheme("Undo").pixmap(16, QIcon::Disabled));
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
        pUndoActionWrp->setIcon(QIcon::fromTheme("Undo"));
        pUndoActionWrp->setText(tr("Отменить: %1...")
                                    .arg(undoText.mid(0, 10)));
        pUndoBtn->setToolTip(tr("Отменить: %1").arg(undoText));
    }
    else
    {
        pUndoActionWrp->setIcon(undoDisabled);
        pUndoActionWrp->setText(tr("Отменить"));
        pUndoBtn->setToolTip(tr("Отменить последнее действие"));
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
    pTreeView->scrollToBottom();
}

void FmtWorkWindow::FillIndecesList()
{
    QString saveindex = ui->keyComboBox->currentText();

    QStringList lst = pTable->uniqueIndeces();
    lst.prepend("-1");
    ui->keyComboBox->clear();
    ui->keyComboBox->insertItems(0, lst);

    if (lst.contains(saveindex))
        ui->keyComboBox->setCurrentIndex(lst.indexOf(saveindex));
}

void FmtWorkWindow::OnIndexChanged(FmtIndex *index)
{
    Q_UNUSED(index)
    FillIndecesList();
}

int FmtWorkWindow::CheckAppy()
{
    QScopedPointer<ErrorsModel> err(new ErrorsModel());
    pTable->checkErrors(err.data());

    int stat = 0;
    if (!err->isEmpty())
    {
        QString msg;
        if (err->hasErrors())
            msg = tr("Сохранение не возможно, т.к имеются ошибки наполнения Fmt словаря.");
        else
            msg = tr("Имеются предупреждения по структуре Fmt словаря. Сохранить изменения не смотря на сообщения?");

        ErrorDlg dlg(ErrorDlg::ModeMessageBox, this);
        dlg.setErrors(err.data());
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
    ConnectionInfo *info = pTable->connection();

    if (index.column() == FmtFildsModel::fld_Custom && index.row() < pTable->fieldsCount())
    {
        FmtFieldDlg dlg(this);
        dlg.setModel(pTable->fieldsModel());
        dlg.setCurrentIndex(index.row());

        if (!info->hasFeature(ConnectionInfo::CanCreateTable))
            dlg.setReadOnly(true);

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
    if (m_pInitTableMenu)
        m_pInitTableMenu->setVisible(f);
}

void FmtWorkWindow::FieldAdded(FmtField *fld)
{
    Q_UNUSED(fld)
}

void FmtWorkWindow::InitDB()
{
    InitFmtTable(pTable.data(), this, "TableAdapter");
}

void FmtWorkWindow::CheckTable()
{
    ErrorsModel e;
    if (!pTable->checkErrors(&e))
    {
        ErrorDlg dlg(ErrorDlg::ModeInformation, this);
        dlg.setErrors(&e);
        dlg.setMessage(tr("Результат проверки: "));
        dlg.exec();
    }
    else
        QMessageBox::information(this, tr("Результат"), tr("Проверка ошибок не выявила!"));
}

void FmtWorkWindow::ExportXml()
{
    SelectFolderDlg folder(RsFmtUnlDirContext, tr("Экспорт в xml файл"), this);
    folder.setWindowIcon(QIcon::fromTheme("ExportFmtXml"));

    if (folder.exec() == QDialog::Accepted)
    {
        ExportFmtToXml(pTable->connection(),QStringList()
                       << pTable->name(), folder.selectedPath(), true, true, this);
    }
}

void FmtWorkWindow::AddedToLast(FmtField *fld, const QModelIndex &index)
{
    Q_UNUSED(fld);
    Q_UNUSED(index);
    pTableView->scrollToBottom();
}

void FmtWorkWindow::CreateTableSql()
{
    SaveFmtTableSql(pTable, this, "CreateTablesSql");
}

void FmtWorkWindow::UpdateCopyMenu(const QString &table)
{
    pCopyMenu->clear();

    if (table.isEmpty())
    {
        if (pCopyMenuAction)
            pCopyMenuAction->setEnabled(false);
    }
    else
    {
        if (pCopyMenuAction)
            pCopyMenuAction->setEnabled(true);

        QStringList lst = FmtTableStringList(table);
        QStringList descriptions = {
            tr("Копировать имя таблицы как есть"),
            tr("Копировать имя таблицы в верхнем регистре"),
            tr("Копировать имя таблицы в нижнем регистре"),
            tr("Копировать имя таблицы в CamelCase нотации"),
            tr("Копировать имя таблицы для использования в C++ коде"),
            tr("Копировать полное имя с префиксом")
        };

        for (int i = 0; i < lst.size(); i++)
        {
            QAction *action = pCopyMenu->addAction(lst[i]);
            if (i < descriptions.size())
                action->setToolTip(descriptions[i]);
            connect(action, SIGNAL(triggered(bool)), SLOT(CopyAction()));
        }

        pCopyMenu->setDefaultAction(pCopyMenu->actions().first());
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

    QToolTip::showText(QCursor::pos(),
                       tr("Текст скопирован: %1").arg(action->text()),
                       nullptr, QRect(), 2000);
}

void FmtWorkWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        return;

    return MdiSubInterface::keyPressEvent(event);
}

void FmtWorkWindow::paintEvent(QPaintEvent *paintEvent)
{
    QMdiSubWindow* parentMdiSubWin = qobject_cast<QMdiSubWindow*>(parentWidget());
    bool isActiveMdiSubWin = parentMdiSubWin ? parentMdiSubWin->isActiveWindow() : false;
    const QPalette& widgetPalette = qApp->style()->standardPalette();

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.save();

    QPainterPath path;
    int startx = ui->idEdit->x() + ui->idEdit->width();
    int tmpy = ui->idEdit->y() + ui->idEdit->height();

    QColor frameColor;
    if (isActiveMdiSubWin)
        frameColor = widgetPalette.color(QPalette::Active, QPalette::Highlight);
    else
        frameColor = widgetPalette.color(QPalette::Inactive, QPalette::Highlight);

    QPen pen(frameColor);
    pen.setWidth(2);
    if (!pTable->id())
        pen.setStyle(Qt::DotLine);
    p.setPen(pen);

    QPointF gradientStart(startx, 0);
    QPointF gradientEnd(startx, tmpy / 2.0);

    p.setBrush(color);

    path.moveTo(0, -1);
    path.moveTo(0, 2);
    path.lineTo(startx, 2);
    path.cubicTo(startx + 50, 0, startx + 25, tmpy / 4, startx + 50, tmpy / 2);
    path.lineTo(width() + 1, tmpy / 2);
    path.lineTo(width() + 1, 0);

    path.lineTo(0, 0);
    path.lineTo(0, -1);

    p.drawPath(path);
    p.restore();

    QDialog::paintEvent(paintEvent);
}

QUndoStack *FmtWorkWindow::tableUndoStack()
{
    return pTable->undoStack();
}

void FmtWorkWindow::UpdateTableIcon()
{
    QIcon icon;

    if (!ui->isRec->isChecked() && !ui->isTemp->isChecked())
        icon = QIcon::fromTheme("Table");
    else
    {
        if (ui->isTemp->isChecked())
            icon = QIcon::fromTheme("TemporalTable");
        else
            icon = QIcon::fromTheme("FmtRecord");
    }
    if (pParentWnd)
        pParentWnd->setWindowIcon(icon);
}

void FmtWorkWindow::isTemporaryTableChanged(bool value)
{
    pTable->setIsTemporary(value);
    UpdateTableIcon();
}

void FmtWorkWindow::isRecordTableChanged(bool value)
{
    pTable->setIsRecord(value);
    UpdateTableIcon();
}

void FmtWorkWindow::UnloadToDbf()
{
    QSettings *settings = ((FmtApplication*)qApp)->settings();
    StartUnloadDbf(pTable->connection(), pTable->name(), this, settings);
}

void FmtWorkWindow::LoadFromDbf()
{
    StartLoadDbfSelectFile(pTable->connection(), pTable->name(), this);
}

void FmtWorkWindow::TabCloseRequested(int index)
{
    QWidget *tabWidget = ui->tabWidget->widget(index);

    FmtWindowTabInterface *fmtTab = qobject_cast<FmtWindowTabInterface*>(tabWidget);
    if (fmtTab)
    {
        fmtTab->deactivateRibbon();

        if (m_pLastActiveFmtTab == tabWidget)
        {
            m_pLastActiveFmtTab = nullptr;
        }
    }

    ui->tabWidget->removeTab(index);

    int newIndex = ui->tabWidget->currentIndex();
    currentTabChanged(newIndex);
}

void FmtWorkWindow::currentTabChanged(int index)
{
    if (index < 0)
    {
        if (ribbon())
        {
            SARibbonCategory *mainCategory = ribbon()->categoryByName("Главная");
            if (mainCategory)
            {
                ribbon()->raiseCategory(mainCategory);
            }

            SARibbonContextCategory *contextCategory = findCategoryByName(FMTTABLE_CONTEXTCATEGORY);
            if (contextCategory)
            {
                if (contextCategory->categoryCount() == 0)
                {
                    ribbon()->hideContextCategory(contextCategory);
                }
                else
                {
                    QList<SARibbonCategory*> categories = contextCategory->categoryList();
                    for (SARibbonCategory *cat : categories)
                    {
                        if (cat->categoryName() != FMTTABLE_CONTEXTCATEGORY)
                        {
                            contextCategory->takeCategory(cat);
                        }
                    }

                    if (contextCategory->categoryCount() == 1)
                    {
                        ribbon()->showContextCategory(contextCategory);
                        ribbon()->raiseCategory(contextCategory->categoryList().first());
                    }
                    else
                    {
                        ribbon()->hideContextCategory(contextCategory);
                    }
                }
            }
        }
        return;
    }

    QWidget *currentWidget = ui->tabWidget->widget(index);

    if (m_pLastActiveFmtTab && m_pLastActiveFmtTab != currentWidget)
    {
        FmtWindowTabInterface *lastTab = qobject_cast<FmtWindowTabInterface*>(m_pLastActiveFmtTab);
        if (lastTab)
        {
            lastTab->deactivateRibbon();
        }
        m_pLastActiveFmtTab = nullptr;
    }

    FmtWindowTabInterface *fmtTab = qobject_cast<FmtWindowTabInterface*>(currentWidget);

    if (fmtTab)
    {
        fmtTab->activateRibbon();
        m_pLastActiveFmtTab = currentWidget;
    }
    else
    {
        SARibbonContextCategory *contextCategory = findCategoryByName(FMTTABLE_CONTEXTCATEGORY);
        if (contextCategory && ribbon())
        {
            QList<SARibbonCategory*> categories = contextCategory->categoryList();
            for (SARibbonCategory *cat : categories)
            {
                if (cat->categoryName() != FMTTABLE_CONTEXTCATEGORY)
                {
                    contextCategory->takeCategory(cat);
                }
            }

            if (contextCategory->categoryCount() > 0)
            {
                ribbon()->showContextCategory(contextCategory);

                for (SARibbonCategory *cat : contextCategory->categoryList())
                {
                    if (cat->categoryName() == FMTTABLE_CONTEXTCATEGORY)
                    {
                        ribbon()->raiseCategory(cat);
                        break;
                    }
                }
            }
            else
            {
                ribbon()->hideContextCategory(contextCategory);

                SARibbonCategory *mainCategory = ribbon()->categoryByName("Главная");
                if (mainCategory)
                {
                    ribbon()->raiseCategory(mainCategory);
                }
            }
        }
    }
}

int FmtWorkWindow::SelectTableFieldsDailog(const QString &title, QList<FmtField*> *pFldList, QWidget *userwidget, const QString &icon)
{
    return SelectTableFieldsDlg(pTable, title, pFldList, this, userwidget, icon);
}

void FmtWorkWindow::AddSqlCodeTab(const QString &title, const QString &code, bool OpenTab, bool WordWrap)
{
    OpenCodeTab(title, HighlighterSql, code, OpenTab, WordWrap);
}

void FmtWorkWindow::AddCppCodeTab(const QString &title, const QString &code, bool OpenTab, bool WordWrap)
{
    OpenCodeTab(title, HighlighterCpp, code, OpenTab, WordWrap);
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

    bool newAlg = true;
    QWidget *containert = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(containert);
    layout->setMargin(0);

    QButtonGroup group;
    QRadioButton *newAlgRadio = new QRadioButton(tr("Использовать DEFAULT для полей"));
    QRadioButton *oldAlgRadio = new QRadioButton(tr("Обновлять значения после создания полей (UPDATE)"));

    layout->addWidget(newAlgRadio);
    layout->addWidget(oldAlgRadio);
    containert->setLayout(layout);

    group.addButton(newAlgRadio);
    group.addButton(oldAlgRadio);

    oldAlgRadio->setChecked(true);
    if (SelectTableFieldsDailog(tr("Выбор полей для скрипта обновления"), &FldList, containert) == QDialog::Accepted)
    {
        QString code;

        if (!group.checkedId())
            code = FmtGenUpdateAddColumnScript(FldList);
        else
            code = FmtGenUpdateAddColumnScriptOld(FldList);

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
    if (SelectTableFieldsDailog(tr("Выбор полей для удаления"), &FldList, nullptr, "RemoveRow") == QDialog::Accepted)
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

    SARibbonContextCategory *sharedContextCategory = findCategoryByName(FMTTABLE_CONTEXTCATEGORY);
    if (ribbon() && sharedContextCategory)
    {
        script->setRibbonBar(ribbon());
        script->setParentContextCategory(sharedContextCategory);
        script->initRibbonPanels();
    }

    int tab = ui->tabWidget->addTab(script, tr("Скрипт"));
    ui->tabWidget->setCurrentIndex(tab);
    currentTabChanged(ui->tabWidget->currentIndex());
}

void FmtWorkWindow::AddFieldsToEnd()
{
    QDialog dlg(this);
    dlg.setWindowTitle(m_AddFieldsToEnd->text());
    dlg.setWindowIcon(QIcon::fromTheme("AddField"));
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
    dlg.setWindowIcon(QIcon::fromTheme("InsertField"));
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
    if (SelectTableFieldsDailog(tr("Выбор полей для копирования в буффер"), &FldList, nullptr, "CopyField") == QDialog::Accepted)
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
    }
}

void FmtWorkWindow::OnImport()
{
    ImportWizard dlg(pTable);
    dlg.exec();
}

void FmtWorkWindow::SegmentButtonClicked(const QModelIndex &index)
{
    ConnectionInfo *info = pTable->connection();

    if (index.parent().isValid() && index.column() == FmtIndecesModelItem::fld_Flag)
    {
        FmtSegment *pSegment = static_cast<FmtSegment*>(index.internalPointer());
        FmtSegmentFlagsDlg dlg(pSegment, this);

        if (!info->hasFeature(ConnectionInfo::CanCreateTable))
            dlg.setReadOnly(true);

        dlg.exec();
    }
}

QString FmtWorkWindow::makeWindowTitle()
{
    QString schemeName = connection()->dsn();
    QString tableName = pTable->name();
    QString name = QString("%1@%2")
                       .arg(tableName)
                       .arg(schemeName.trimmed());
    return name;
}

void FmtWorkWindow::GenInsertTemplateSql()
{
    GenSqlTemplateDlg dlg(pTable, this);

    if (dlg.exec() == QDialog::Accepted)
    {
        int templ = dlg.templateID();
        QString code = dlg.sqlTemplate();

        switch(templ)
        {
        case GenSqlTemplateDlg::sqlInsertWithDefaults:
        case GenSqlTemplateDlg::sqlInsertWithPlaceholders:
        case GenSqlTemplateDlg::sqlUpdateWithDefaults:
        case GenSqlTemplateDlg::sqlUpdateWithPlaceholders:
            AddSqlCodeTab(dlg.templateName(), code, true, true);
            break;
        default:
            AddCppCodeTab(dlg.templateName(), code, true, true);
        }
    }
}

void FmtWorkWindow::CamelCaseAction()
{
    QList<FmtField*> FldList;
    if (SelectTableFieldsDailog(tr("Выбор полей для преобразования в CamelCase"), &FldList) == QDialog::Accepted)
    {
        QStringList lst;
        for (auto fld : FldList)
            lst.append(fld->name());

        lst = FmtCapitalizeField(lst, true);

        tableUndoStack()->beginMacro(tr("Преобразование в CamelCase"));

        for (int i = 0; i < lst.size(); i++)
            FldList[i]->setName(lst[i]);

        tableUndoStack()->endMacro();
    }
}

void FmtWorkWindow::DiffToScript()
{
    ConnectionInfo *conn = pTable->connection();
    SelectFolderDlg folder(RsDiffToScriptContext, tr("Выбор каталога с репозиторием"), this);
    folder.setWindowIcon(QIcon::fromTheme("DiffToScript"));

    if (folder.exec() != QDialog::Accepted)
        return;

    QString datname = QString("%1.dat").arg(pTable->name().toUpper());

    QDir dir(folder.selectedPath());
    QString filename = dir.absoluteFilePath(datname);

    auto ShowErrMsg = [=](const QString &msg)
    {
        QMessageBox dlg(this);
        dlg.setIcon(QMessageBox::Critical);
        dlg.setText(tr("Не удалось сформировать скрипт обновления записей"));
        dlg.setDetailedText(msg);

        dlg.exec();
    };

    if (!QFile::exists(filename))
    {
        filename = QString();
        QDirIterator iter(dir.path(), {"*.dat"},
                          QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
                          QDirIterator::Subdirectories);

        QStringList files;
        while (iter.hasNext())
        {
            iter.next();
            if (!iter.fileName().compare(datname, Qt::CaseInsensitive))
            {
                filename = iter.filePath();
                break;
            }
        }
    }

    if (filename.isEmpty())
    {
        ShowErrMsg(tr("Файл <b>%1</b> не найден").arg(datname));
        return;
    }

    QString err;
    QString connectionString = QString("CONNSTRING=dsn=%1;user id=%2;password=%3")
                                   .arg(conn->dsn(), conn->user(), conn->password());
    QString result = FmtGenDiffToScript(filename,
                                        connectionString,
                                        conn->isUnicode(),
                                        &err);

    if (!err.isEmpty())
        ShowErrMsg(err);
    else
    {
        if (!result.isEmpty())
            AddSqlCodeTab(tr("Diff To Script"), result);
    }
}

void FmtWorkWindow::CheckAction()
{
    ErrorsModel err;
    pTable->checkErrors(&err);

    if (!err.isEmpty())
    {
        QString msg = tr("Имеются предупреждения по структуре Fmt словаря");
        ErrorDlg dlg(ErrorDlg::ModeInformation, this);
        dlg.setErrors(&err);
        dlg.setWindowTitle(tr("Результаты проверки"));
        dlg.setMessage(msg);
        dlg.exec();
    }
    else
    {
        QMessageBox::information(this, tr("Результат"), tr("Проверка ошибок не обнаружила"));
    }
}

void FmtWorkWindow::OpenCodeTab(const QString &title, int syntax, const QString &code, bool OpenTab, bool WordWrap)
{
    FmtCodeTabBase *editor = new FmtCodeTabBase();
    editor->AddTab(title, code, syntax);

    SARibbonContextCategory *sharedContextCategory = findCategoryByName(FMTTABLE_CONTEXTCATEGORY);
    if (ribbon() && sharedContextCategory)
    {
        editor->setRibbonBar(ribbon());
        editor->setParentContextCategory(sharedContextCategory);
        editor->initRibbonPanels();
    }

    editor->setWordWrap(WordWrap);

    int tab = ui->tabWidget->addTab(editor, title);
    if (OpenTab) ui->tabWidget->setCurrentIndex(tab);
    currentTabChanged(ui->tabWidget->currentIndex());
}

void FmtWorkWindow::execUserAction(const QString &macro)
{
    MainWindow *window = qobject_cast<MainWindow*>(qApp->activeWindow());

    if (!window)
        return;

    QScopedPointer<ToolbarActionExecutor> executor(new ToolbarActionExecutor(window));
    executor->playRep(macro);
}

void FmtWorkWindow::onUserActionTriggered()
{
    QAction *action = qobject_cast<QAction*>(sender());

    if (!action)
        return;

    execUserAction(action->data().toString());
}

void FmtWorkWindow::TableObjects()
{
    TableStructSqlDlg dlg(pTable->connection(), pTable->name(), this);
    if (dlg.exec() == QDialog::Accepted)
        AddSqlCodeTab(tr("Объекты таблицы"), dlg.getObjectsSql());
}

void FmtWorkWindow::CompareStruct()
{
    CompareFmtWizard dlg(pTable->connection(), this);
    if (dlg.exec() == QDialog::Accepted)
    {
        CompareFmtViewTab *tabView = new CompareFmtViewTab(ui->tabWidget);

        int Action = dlg.field("Action").toInt();
        if (Action == CompareFmtWizard::CompareStruct)
            tabView->setLists(pTable.data(), dlg.structString());
        else if (Action == CompareFmtWizard::CompareTable)
        {
            QString SelectedTable = dlg.field("SelectedTable").toString();

            FmtTable tbl(pTable->connection());
            tbl.load(SelectedTable);

            tabView->setLists(pTable.data(), &tbl);
        }
        else if (Action == CompareFmtWizard::CompareXml)
        {
            QString SelectedTable = dlg.field("XmlDump").toString();

            FmtFldElementVector table2;
            FmtTableCompareModel::parseFieldsFromXml(SelectedTable, table2);
            tabView->setLists(pTable.data(), table2);
        }

        int tab = ui->tabWidget->addTab(tabView, tr("Результат сравнения"));
        ui->tabWidget->setCurrentIndex(tab);
    }
}

void FmtWorkWindow::ImportFromTable()
{
    RichTextToInsertWizard wzrd(pTable.data(), this);
    wzrd.setWindowIcon(QIcon::fromTheme("ImportRichText"));
    wzrd.exec();
}

void FmtWorkWindow::initRibbonFmtPanel()
{
    SARibbonPannel* FmtPannel = new SARibbonPannel(tr("Запись словаря"));
    m_pFmtCategory->addPannel(FmtPannel);

    m_pInitTableMenu = new QMenu(tr("Создать в БД"), this);
    m_pInitTableMenu->setIcon(QIcon::fromTheme("TableAdapter"));
    toolAddActionWithTooltip(m_pInitTableMenu,
                             tr("Создание объектов базы данных на основе записи в словаре"));

    m_pInitCreateTableAction = m_pInitTableMenu->addAction(tr("Создать/пересоздать таблицу"));
    toolAddActionWithTooltip(m_pInitCreateTableAction,
                             tr("Создать или пересоздать таблицу в базе данных. ВНИМАНИЕ: пересоздание приведет к потере данных!"));

    m_pInitCreateIndexAction = m_pInitTableMenu->addAction(tr("Создать/пересоздать индексы"));
    toolAddActionWithTooltip(m_pInitCreateIndexAction,
                             tr("Создать или обновить индексы для таблицы в базе данных"));

    m_pInitTableMenuAction = FmtPannel->addLargeMenu(m_pInitTableMenu, QToolButton::MenuButtonPopup);
    m_pInitTableMenuAction->setIcon(QIcon::fromTheme("TableAdapter"));

    m_pCheckAction = createAction(tr("Проверить"), "Test");
    toolAddActionWithTooltip(m_pCheckAction,
                             tr("Проверить структуру таблицы на наличие ошибок и предупреждений"));
    FmtPannel->addSmallAction(m_pCheckAction);

    m_pCompareFmt = createAction(tr("Сравнить структуры"), "CompareFmt");
    toolAddActionWithTooltip(m_pCompareFmt,
                             tr("Сравнить структуру текущей таблицы с другой таблицей, XML файлом или эталонной структурой"));
    FmtPannel->addSmallAction(m_pCompareFmt);

    m_rebuildOffsets = createAction(tr("Перестроить смещения"), "Step");
    toolAddActionWithTooltip(m_rebuildOffsets,
                             tr("Пересчитать смещения полей в структуре таблицы"));
    FmtPannel->addSmallAction(m_rebuildOffsets);
    FmtPannel->addSeparator();

    pCopyMenuAction = FmtPannel->addLargeMenu(pCopyMenu, QToolButton::MenuButtonPopup);
    pCopyMenuAction->setIcon(QIcon::fromTheme("DictionaryContains"));
    toolAddActionWithTooltip(pCopyMenuAction,
                             tr("Скопировать имя таблицы в различных форматах в буфер обмена"));
    FmtPannel->addSeparator();

    m_saveToXml = createAction(tr("Экспорт в XML"), "ExportFmtXml");
    toolAddActionWithTooltip(m_saveToXml,
                             tr("Экспортировать структуру таблицы в XML файл"));
    FmtPannel->addLargeAction(m_saveToXml);

    m_createTableSql = createAction(tr("Экспорт CreateTablesSql"), "CreateTablesSql");
    toolAddActionWithTooltip(m_createTableSql,
                             tr("Сгенерировать SQL скрипт CREATE TABLE для создания таблицы"));
    FmtPannel->addLargeAction(m_createTableSql);

    connect(m_pCheckAction, &QAction::triggered, this, &FmtWorkWindow::CheckTable);
    connect(m_pInitTableMenuAction, &QAction::triggered, this, &FmtWorkWindow::InitDB);
    connect(m_saveToXml, &QAction::triggered, this, &FmtWorkWindow::ExportXml);
    connect(m_pCompareFmt, &QAction::triggered, this, &FmtWorkWindow::CompareStruct);
    connect(m_createTableSql, &QAction::triggered, this, &FmtWorkWindow::CreateTableSql);
    connect(m_rebuildOffsets, &QAction::triggered, this, &FmtWorkWindow::rebuildOffsets);
    connect(m_pInitCreateTableAction, &QAction::triggered, [=]() {
        if (pTable->isRecord())
            return;

        bool CreateIndex = false;
        if (pTable->isExistsInDb())
        {
            QCheckBox *check = new QCheckBox(tr("Создать индексы"));
            QMessageBox msg(this);
            msg.setWindowTitle(QObject::tr("Создать/пересоздать таблицу"));
            msg.setText(tr("Таблица уже существует в БД."));
            msg.setInformativeText(tr("Пересоздание приведет к потери данных в таблице. Пересоздать?"));
            msg.setCheckBox(check);
            msg.setIcon(QMessageBox::Warning);
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

            if (msg.exec() == QMessageBox::No)
                return;

            CreateIndex = check->isChecked();
        }

        QString err;
        int stat = pTable->createDbTable(&err);

        if (stat)
        {
            QMessageBox msg(this);
            msg.setWindowTitle(QObject::tr("Ошибка"));
            msg.setIcon(QMessageBox::Critical);
            msg.setText(QObject::tr("Не удалось создать/пересоздать таблицу"));
            msg.setDetailedText(err);
            msg.exec();
        }
        else
        {
            if (CreateIndex)
            {
                err = QString();
                stat = InitFmtTableExec(pTable.data(), &err);

                QApplication::processEvents();
                QMessageBox msg(this);

                if (stat)
                {
                    msg.setWindowTitle(QObject::tr("Ошибка"));
                    msg.setIcon(QMessageBox::Critical);
                    msg.setText(QObject::tr("<b>DbInit вернул код ошибки [%1]: %2</b>").arg(stat).arg(DbInitTextError(stat)));
                }
                else
                {
                    msg.setWindowTitle(QObject::tr("Результат выполнения"));
                    msg.setIcon(QMessageBox::Information);
                    msg.setText(QObject::tr("<b>DbInit завершился с кодом [%1]: %2</b>").arg(stat).arg(DbInitTextError(stat)));
                }

                if (err.size())
                {
                    msg.setDetailedText(err);
                    QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
                    QGridLayout* layout = (QGridLayout*)msg.layout();
                    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
                }
                msg.exec();
            }
        }
    });

    connect(m_pInitCreateIndexAction, &QAction::triggered, [=]() {
        QString err;
        int stat = InitFmtTableExec(pTable.data(), &err);
        QApplication::processEvents();

        QMessageBox msg(this);

        if (stat)
        {
            msg.setWindowTitle(QObject::tr("Ошибка"));
            msg.setIcon(QMessageBox::Critical);
            msg.setText(QObject::tr("<b>DbInit вернул код ошибки [%1]: %2</b>").arg(stat).arg(DbInitTextError(stat)));
        }
        else
        {
            msg.setWindowTitle(QObject::tr("Результат выполнения"));
            msg.setIcon(QMessageBox::Information);
            msg.setText(QObject::tr("<b>DbInit завершился с кодом [%1]: %2</b>").arg(stat).arg(DbInitTextError(stat)));
        }

        if (err.size())
        {
            msg.setDetailedText(err);
            QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
            QGridLayout* layout = (QGridLayout*)msg.layout();
            layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
        }
        msg.exec();
    });
}

void FmtWorkWindow::initRibbonFieldsPanel()
{
    SARibbonPannel* Pannel = new SARibbonPannel(tr("Колонки"));
    m_pFmtCategory->addPannel(Pannel);

    m_MassRemoveFields = createAction(tr("Удалить"), "RemoveRow");
    toolAddActionWithTooltip(m_MassRemoveFields,
                             tr("Удалить выбранные поля из структуры таблицы"));

    m_AddFieldsToEnd = createAction(tr("Добавить в конец"), "AddField");
    toolAddActionWithTooltip(m_AddFieldsToEnd,
                             tr("Добавить новые поля в конец структуры таблицы на основе текстового описания"));

    m_InsertFields = createAction(tr("Добавить перед..."), "InsertField");
    toolAddActionWithTooltip(m_InsertFields,
                             tr("Вставить новые поля перед выбранным полем на основе текстового описания"));

    m_CamelCaseAction = createAction(tr("Преобразовать в CamelCase"), "HighlightText");
    toolAddActionWithTooltip(m_CamelCaseAction,
                             tr("Преобразовать имена выбранных полей в CamelCase нотацию"));

    Pannel->addLargeAction(m_MassRemoveFields);
    Pannel->addMediumAction(m_AddFieldsToEnd);
    Pannel->addMediumAction(m_InsertFields);
    Pannel->addSeparator();

    m_CopyFields = createAction(tr("Копировать"), "CopyField");
    toolAddActionWithTooltip(m_CopyFields,
                             tr("Копировать выбранные поля в буфер обмена для последующей вставки"));

    Pannel->addLargeAction(m_CopyFields);

    m_PasteFields = createAction(tr("Вставить"), "PasteField");
    toolAddActionWithTooltip(m_PasteFields,
                             tr("Вставить ранее скопированные поля в текущую таблицу"));

    Pannel->addLargeAction(m_PasteFields);
    Pannel->addSeparator();

    Pannel->addLargeAction(m_CamelCaseAction);

    connect(m_MassRemoveFields, &QAction::triggered, this, &FmtWorkWindow::RemoveTableFields);
    connect(m_AddFieldsToEnd, &QAction::triggered, this, &FmtWorkWindow::AddFieldsToEnd);
    connect(m_InsertFields, &QAction::triggered, this, &FmtWorkWindow::InsertFieldsBefore);
    connect(m_CopyFields, &QAction::triggered, this, &FmtWorkWindow::CopyFields);
    connect(m_PasteFields, &QAction::triggered, this, &FmtWorkWindow::PasteFields);
    connect(m_CamelCaseAction, &QAction::triggered, this, &FmtWorkWindow::CamelCaseAction);
}

void FmtWorkWindow::initRibbonDataPanel()
{
    SARibbonPannel* Pannel = new SARibbonPannel(tr("Содержимое"));
    m_pFmtCategory->addPannel(Pannel);

    m_unloadDbf = createAction(tr("Выгрузить в *.dat"), "UnloadDbf");
    toolAddActionWithTooltip(m_unloadDbf,
                             tr("Выгрузить данные таблицы в DAT файл (DBF формат) для загрузки через SQL*Loader"));
    Pannel->addLargeAction(m_unloadDbf);

    m_loadDbf = createAction(tr("Загрузить из *.dat"), "LoadDbf");
    toolAddActionWithTooltip(m_loadDbf,
                             tr("Загрузить данные таблицы из DAT файла (DBF формат) с помощью SQL*Loader"));
    Pannel->addSmallAction(m_loadDbf);

    m_ImportFromTable = createAction(tr("Загрузить из Word"), "ImportRichText");
    toolAddActionWithTooltip(m_ImportFromTable,
                             tr("Импортировать структуру таблицы из документа Word или текстового описания"));
    Pannel->addSmallAction(m_ImportFromTable);

    m_EditContent = createAction(tr("Редактировать содержимое"), "EditTableRow");
    toolAddActionWithTooltip(m_EditContent,
                             tr("Открыть редактор содержимого для просмотра и редактирования данных таблицы"));
    Pannel->addSmallAction(m_EditContent);

    connect(m_unloadDbf, &QAction::triggered, this, &FmtWorkWindow::UnloadToDbf);
    connect(m_loadDbf, &QAction::triggered, this, &FmtWorkWindow::LoadFromDbf);
    connect(m_ImportFromTable, &QAction::triggered, this, &FmtWorkWindow::ImportFromTable);
    connect(m_EditContent, &QAction::triggered, this, &FmtWorkWindow::EditContent);
}

void FmtWorkWindow::initRibbonCodeTemplatesPanel()
{
    SARibbonPannel* Pannel = new SARibbonPannel(tr("Генерация кода"));
    m_pFmtCategory->addPannel(Pannel);

    m_DiffToScript = createAction(tr("Diff To Script"), "DiffToScript");
    toolAddActionWithTooltip(m_DiffToScript,
                             tr("Создать скрипт обновления записей на основе DAT файла из репозитория"));
    Pannel->addLargeAction(m_DiffToScript);

    pGenCppCodeMenu = new QMenu(tr("Шаблоны"), this);
    pGenCppCodeMenu->setIcon(QIcon::fromTheme("GenerateAndRecordCode"));
    toolAddActionWithTooltip(pGenCppCodeMenu,
                             tr("Генерация C++ кода и пользовательских макросов на основе структуры таблицы"));

    pGenCppCodeAction = Pannel->addLargeMenu(pGenCppCodeMenu, QToolButton::MenuButtonPopup);
    pGenCppCodeAction->setIcon(QIcon::fromTheme("GenerateAndRecordCode"));
    toolAddActionWithTooltip(pGenCppCodeAction,
                             tr("Генерация C++ кода и пользовательских макросов"));

    bool HasMacroSeparator = false;
    for (int i = 0; i < m_pGeneratorsProxyModel->rowCount(); i++)
    {
        QModelIndex indexAlias = m_pGeneratorsProxyModel->index(i, GenInterfaceFactoryModel::FieldAlias);
        QModelIndex indexKey = m_pGeneratorsProxyModel->index(i, GenInterfaceFactoryModel::FieldKey);
        QString id = m_pGeneratorsProxyModel->data(indexKey).toString();

        if (id.startsWith("macro:") && !HasMacroSeparator)
        {
            pGenCppCodeMenu->addSeparator();
            HasMacroSeparator = false;
        }

        QAction *action = nullptr;
        if (id != "FmtGenCppTemplate")
            action = pGenCppCodeMenu->addAction(m_pGeneratorsProxyModel->data(indexAlias).toString());
        else
        {
            action = new QAction(m_pGeneratorsProxyModel->data(indexAlias).toString());
            pGenCppCodeMenu->insertAction(pGenCppCodeMenu->actions().at(0), action);
            pGenCppCodeMenu->setDefaultAction(action);
        }

        // Добавляем tooltip для каждого шаблона
        QString description;
        if (id == "FmtGenCppTemplate")
            description = tr("Сгенерировать C++ структуру данных для таблицы");
        else if (id == "FmtGenCppDaoTemplate")
            description = tr("Сгенерировать DAO класс для работы с таблицей");
        else if (id == "FmtGenCppClassTemplate")
            description = tr("Сгенерировать C++ класс с методами доступа");
        else if (id.startsWith("macro:"))
            description = tr("Выполнить пользовательский макрос");
        else
            description = tr("Сгенерировать код на основе шаблона");

        toolAddActionWithTooltip(action, description);

        action->setData(id);
        connect(action, &QAction::triggered, [=]()
                {
                    OpenGeneratorTab(action->data().toString(), action->text());
                });
    }

    pCodeGenMenu = new QMenu(tr("Скрипты SQL"), this);
    pCodeGenMenu->setIcon(QIcon::fromTheme("GenerateChangeScript"));
    toolAddActionWithTooltip(pCodeGenMenu,
                             tr("Генерация SQL скриптов для модификации структуры таблицы"));

    m_GenCreateTbSql = pCodeGenMenu->addAction(tr("Создание таблицы"));
    toolAddActionWithTooltip(m_GenCreateTbSql,
                             tr("Сгенерировать SQL скрипт CREATE TABLE для создания таблицы"));
    pCodeGenMenu->setDefaultAction(m_GenCreateTbSql);

    m_GenAddScript = pCodeGenMenu->addAction(tr("Добавление полей"));
    toolAddActionWithTooltip(m_GenAddScript,
                             tr("Сгенерировать SQL скрипт ALTER TABLE ADD для добавления новых полей"));

    m_GenModifyScript = pCodeGenMenu->addAction(tr("Изменение полей"));
    toolAddActionWithTooltip(m_GenModifyScript,
                             tr("Сгенерировать SQL скрипт ALTER TABLE MODIFY для изменения типов полей"));

    m_GenDelScript = pCodeGenMenu->addAction(tr("Удаления полей"));
    toolAddActionWithTooltip(m_GenDelScript,
                             tr("Сгенерировать SQL скрипт ALTER TABLE DROP для удаления полей"));

    pCodeGenMenu->addSeparator();

    m_TableObjects = pCodeGenMenu->addAction(tr("Объекты таблицы"));
    toolAddActionWithTooltip(m_TableObjects,
                             tr("Показать все объекты базы данных, связанные с таблицей (индексы, триггеры, ограничения)"));

    pCodeGenMenu->addSeparator();

    m_GenInsertTemplate = pCodeGenMenu->addAction(tr("Прочие инструкции"));
    toolAddActionWithTooltip(m_GenInsertTemplate,
                             tr("Сгенерировать шаблоны INSERT, UPDATE и C++ кода для работы с данными"));

    pGenCodeAction = Pannel->addLargeMenu(pCodeGenMenu, QToolButton::MenuButtonPopup);
    pGenCodeAction->setIcon(QIcon::fromTheme("GenerateChangeScript"));
    toolAddActionWithTooltip(pGenCodeAction,
                             tr("Генерация SQL скриптов для модификации структуры таблицы"));

    pUserActionsMenu = nullptr;
    CreateUserCommandsMenu(&pUserActionsMenu, tr("Польз. действия"),
                           this, SLOT(onUserActionTriggered()));

    if (pUserActionsMenu)
    {
        toolAddActionWithTooltip(pUserActionsMenu,
                                 tr("Выполнить пользовательские макросы и действия"));

        // UserCode
        m_pUserCode = Pannel->addLargeMenu(pUserActionsMenu);
        m_pUserCode->setIcon(QIcon::fromTheme("UserCode"));
        toolAddActionWithTooltip(m_pUserCode,
                                 tr("Выполнить пользовательские макросы"));
    }

    m_pEditMacro = createAction(tr("Редактировать макрос"), "ScriptManager");
    toolAddActionWithTooltip(m_pEditMacro,
                             tr("Открыть редактор макросов для создания и модификации пользовательских скриптов"));
    Pannel->addLargeAction(m_pEditMacro);

    connect(m_DiffToScript, &QAction::triggered, this, &FmtWorkWindow::DiffToScript);
    connect(m_GenDelScript, &QAction::triggered, this, &FmtWorkWindow::GenDeleteFiledsScript);
    connect(m_GenAddScript, &QAction::triggered, this, &FmtWorkWindow::GenAddFiledsScript);
    connect(m_GenInsertTemplate, &QAction::triggered, this, &FmtWorkWindow::GenInsertTemplateSql);
    connect(m_GenModifyScript, &QAction::triggered, this, &FmtWorkWindow::GenModifyTableFields);
    connect(m_TableObjects, &QAction::triggered, this, &FmtWorkWindow::TableObjects);
    connect(m_GenCreateTbSql, &QAction::triggered, this, &FmtWorkWindow::GenCreateTableScript);
    connect(pGenCodeAction, &QAction::triggered, this, &FmtWorkWindow::GenCreateTableScript);
    connect(m_pEditMacro, &QAction::triggered, this, &FmtWorkWindow::OpenScriptEditorWindow);

    connect(pGenCppCodeAction, &QAction::triggered, [=]
            {
                OpenGeneratorTab("FmtGenCppTemplate", fmtGenInterfaceAlias("FmtGenCppTemplate"));
            });
}

void FmtWorkWindow::initRibbonPanels()
{
    m_pFmtCategory = new SARibbonCategory(FMTTABLE_CONTEXTCATEGORY, ribbon());

    initRibbonFmtPanel();
    initRibbonDataPanel();
    initRibbonCodeTemplatesPanel();
    initRibbonFieldsPanel();
}

void FmtWorkWindow::updateRibbonTabs()
{
    SARibbonContextCategory *context = findCategoryByName(FMTTABLE_CONTEXTCATEGORY);

    if (!context)
        return;

    QList<SARibbonCategory*> oldCategories = context->categoryList();
    for (SARibbonCategory *cat : qAsConst(oldCategories))
        context->takeCategory(cat);

    if (m_pFmtCategory)
    {
        if (!context->isHaveCategory(m_pFmtCategory))
            context->addCategoryPage(m_pFmtCategory);
    }

    FmtWindowTabInterface *TabInterface = qobject_cast<FmtWindowTabInterface*>(ui->tabWidget->currentWidget());
    if (TabInterface)
    {
        QList<SARibbonCategory*> lst = TabInterface->categoryList();

        for (auto category : qAsConst(lst))
        {
            context->addCategoryPage(category);
            ribbon()->showCategory(category);
        }

        if (!lst.isEmpty())
            ribbon()->raiseCategory(lst.last());
    }

    ribbon()->showContextCategory(context);
    ribbon()->showCategory(m_pFmtCategory);
}

void FmtWorkWindow::clearRibbonTabs()
{
    SARibbonContextCategory *context = findCategoryByName(FMTTABLE_CONTEXTCATEGORY);

    if (!context)
        return;

    if (m_pFmtCategory)
    {
        ribbon()->removeCategory(m_pFmtCategory);
        ribbon()->hideCategory(m_pFmtCategory);
    }

    FmtWindowTabInterface *TabInterface = qobject_cast<FmtWindowTabInterface*>(ui->tabWidget->currentWidget());
    if (TabInterface)
    {
        QList<SARibbonCategory*> lst = TabInterface->categoryList();

        for (auto category : qAsConst(lst))
        {
            ribbon()->removeCategory(category);
            ribbon()->hideCategory(category);
        }
    }

    bool hasVisible = false;
    QList<SARibbonCategory*> oldCategories = context->categoryList();
    for (SARibbonCategory *cat : qAsConst(oldCategories))
    {
        if (ribbon()->isCategoryVisible(cat))
        {
            hasVisible = true;
            break;
        }
    }

    if (!hasVisible)
        ribbon()->hideContextCategory(context);
}

void FmtWorkWindow::OpenGeneratorTab(const QString &interfaceid, const QString &title)
{
    for (int i = 0; i < ui->tabWidget->count(); i ++)
    {
        FmtWorkWndGen *GenTmp = qobject_cast<FmtWorkWndGen*>(ui->tabWidget->widget(i));
        if (GenTmp && GenTmp->interfaceId() == interfaceid)
        {
            ui->tabWidget->setCurrentIndex(i);
            currentTabChanged(i);
            return;
        }
    }

    FmtWorkWndGen *GenWnd = new FmtWorkWndGen(this);
    SARibbonContextCategory *sharedContextCategory = findCategoryByName(FMTTABLE_CONTEXTCATEGORY);

    if (ribbon() && sharedContextCategory)
    {
        GenWnd->setRibbonBar(ribbon());
        GenWnd->setParentContextCategory(sharedContextCategory);
        GenWnd->initRibbonPanels();
    }

    GenWnd->setInterfaceID(interfaceid);
    GenWnd->setTable(pTable);
    GenWnd->generate();

    int tabIndex = ui->tabWidget->addTab(GenWnd, title);

    if (!m_pLastActiveFmtTab)
        m_pLastActiveFmtTab = GenWnd;

    ui->tabWidget->setCurrentIndex(tabIndex);
}
