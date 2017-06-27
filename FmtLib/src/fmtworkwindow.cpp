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
#include <QMessageBox>
#include <QTemporaryFile>
#include <QProgressDialog>

FmtWorkWindow::FmtWorkWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FmtWorkWindow),
    pTable(NULL),
    pParentWnd(NULL)
{
    ui->setupUi(this);
    pMapper = new QDataWidgetMapper(this);

    pFieldsDelegate = new FmtFieldsDelegate;
    pTableView = new FmtFieldsTableView;
    pTreeView = new FmtIndexTreeView;

    pTableView->setItemDelegate(pFieldsDelegate);
    pTableView->verticalHeader()->setDefaultSectionSize(25);

    pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    pTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->tabWidget->widget(0)->layout()->addWidget(pTableView);
    ui->tabWidget->widget(1)->layout()->addWidget(pTreeView);

    pAddIndex = new QPushButton("Добавить индекс", ui->tabWidget->widget(1));
    pHorizontalSpacer = new QSpacerItem(100, 20, QSizePolicy::Expanding);
    pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->addSpacerItem(pHorizontalSpacer);
    pHBoxLayout->addWidget(pAddIndex);
    ui->tabWidget->widget(1)->layout()->addItem(pHBoxLayout);

    ui->buttonBox->button(QDialogButtonBox::Apply)->setDefault(true);

    pCopyMenu = new QMenu(this);
    ui->copyTool->setMenu(pCopyMenu);

    pActionsMenu = new QMenu(this);
    QAction *saveToXml = pActionsMenu->addAction(QIcon(":/img/savexml.png"), tr("Экспорт в XML"));
    QAction *createTableSql = pActionsMenu->addAction(QIcon(":/img/savesql.png"), tr("Сохранить CreateTablesSql скрипт"));
    QAction *rebuildOffsets = pActionsMenu->addAction(tr("Перестроить смещения"));
    ui->pushActions->setMenu(pActionsMenu);

    pCodeGenWidget = new FmtWorkWndGen(this);
    ui->tabWidget->addTab(pCodeGenWidget, tr("Код"));

    ui->nameEdit->setMaxLength(fmtm_TableNameMaxSize);
    ui->commentEdit->setMaxLength(fmtm_TableCommentMaxSize);

    connect(pAddIndex, SIGNAL(clicked(bool)), SLOT(AddIndex()));
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked(bool)), SLOT(Apply()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), SLOT(OnOk()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), SLOT(InitDB()));
    connect(pTableView, SIGNAL(clicked(QModelIndex)), SLOT(Clicked(QModelIndex)));
    connect(ui->checkButton, SIGNAL(clicked(bool)), SLOT(CheckTable()));
    connect(saveToXml, SIGNAL(triggered(bool)), SLOT(ExportXml()));
    connect(createTableSql, SIGNAL(triggered(bool)), SLOT(CreateTableSql()));
    connect(rebuildOffsets, SIGNAL(triggered(bool)), SLOT(on_rebuildOffsetsBtn_clicked()));
}

FmtWorkWindow::~FmtWorkWindow()
{
    delete ui;
    delete pTreeView;
    delete pTableView;
    delete pFieldsDelegate;
}

ConnectionInfo *FmtWorkWindow::connection()
{
    return pTable->connection();
}

void FmtWorkWindow::setFmtTable(QSharedPointer<FmtTable> &table)
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
    pTableView->setModel((QAbstractItemModel*)fieldsModel);
    pTreeView->setModel((QAbstractItemModel*)pTable->indecesModel());

    static QList<int> m_Width = QList<int>()
            << 150 << 65 << 50 << 200 << 150 << 50;

    static QList<int> m_iWidth = QList<int>()
            << 175 << 65 << 35 << 40 << 70 << 35 << 40 << 70 << 50 << 100;

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

    ConnectionInfo *info = pTable->connection();
    dcolor = info->color().darker();
    color = info->color();

    connect(pTable->indecesModel(), SIGNAL(modelReset()), SLOT(indexModelReseted()));
    connect(pTable->indecesModel(), SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(indexModelInserted(QModelIndex,int,int)));
    connect(pTable.data(), SIGNAL(indexAdded(FmtIndex*)), SLOT(FillIndecesList()));
    connect(pTable.data(), SIGNAL(indexRemoved(quint32)), SLOT(FillIndecesList()));
    connect(pTable.data(), SIGNAL(indexChanged(FmtIndex*)), SLOT(OnIndexChanged(FmtIndex*)));
    connect(pTable.data(), SIGNAL(nameChanged(QString)), SLOT(UpdateCopyMenu(QString)));
    connect(fieldsModel, SIGNAL(AddedToLast(FmtField*, const QModelIndex&)), SLOT(AddedToLast(FmtField*,QModelIndex)));
    connect(pTable.data(), SIGNAL(isTemporaryChanged(bool)), SLOT(isTemporaryTableChanged(bool)));
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
        {
            msg = tr("Сохранение не возможно, т.к имеются ошибки наполнения Fmt словаря.");
        }
        else
        {
            msg = tr("Имеются предупреждения по структуре Fmt словаря. Сохранить изменения не смотря на сообщения?");
        }
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

    return stat;
}

void FmtWorkWindow::Apply()
{
    int stat = CheckAppy();

    if (!stat)
        stat = SaveTable();
}

void FmtWorkWindow::OnOk()
{
    int stat = CheckAppy();

    if (!stat)
        stat = SaveTable();

    if (!stat)
        emit QDialog::accepted();
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
    {
        ui->copyTool->setEnabled(false);
    }
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
    //p.setRenderHint(QPainter::Antialiasing, true);
    p.save();
    QPen pen(dcolor);
    pen.setWidth(2);
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

void FmtWorkWindow::on_rebuildOffsetsBtn_clicked()
{
    pTable->rebuildOffsets();
}

QUndoStack *FmtWorkWindow::tableUndoStack()
{
    return pTable->undoStack();
}

void FmtWorkWindow::setParentWnd(QMdiSubWindow *wnd)
{
    pParentWnd = wnd;
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

    pParentWnd->setWindowIcon(icon);
}
