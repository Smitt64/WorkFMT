#include "tablesdockwidget.h"
#include "fmttablesmodel.h"
#include "tablesgroupprovider.h"
#include "fmtcore.h"
#include <tablecomboaction.h>
#include <QtSql>
#include <QInputDialog>

static int Index = 0;

TablesDockWidget::TablesDockWidget(QWidget *parent) :
    QMainWindow(parent),
    pInitTableAction(Q_NULLPTR),
    pRebuildOffsets(Q_NULLPTR),
    pFilterByGroup(Q_NULLPTR)
{
    pModel = Q_NULLPTR;
    pTables = new QListView(this);
    //pTables->setResizeMode(QListView::Adjust);
    pTables->setContextMenuPolicy(Qt::ActionsContextMenu);
    setCentralWidget(pTables);
    setIconSize(QSize(16, 16));

    pFilterLabel = new QLabel(tr("Фильтр: "), this);
    pFilterEdit = new QLineEdit(this);
    pFilterEdit->setClearButtonEnabled(true);

    pMainToolBar = addToolBar(tr("Фильтр"));
    pMainToolBar->setMovable(false);
    pMainToolBar->setAutoFillBackground(true);
    pMainToolBar->addWidget(pFilterLabel);
    pMainToolBar->addWidget(pFilterEdit);

    pFilterMenu = new QMenu(this);
    pFilterAction = new QToolButton(this);
    pFilterAction->setCheckable(true);
    pFilterAction->setPopupMode(QToolButton::MenuButtonPopup);
    pFilterAction->setMenu(pFilterMenu);
    pMainToolBar->addWidget(pFilterAction);

    pApplyFilterAction = pFilterMenu->addAction(QIcon(":/img/Filter2HS.png"), tr("Фильтр"), this, SLOT(filterTriggered(bool)));//new QAction(QIcon(":/img/Filter2HS.png"), tr("Фильтр"), this);
    pApplyFilterAction->setCheckable(true);
    connect(pApplyFilterAction, SIGNAL(triggered(bool)), SLOT(filterTriggered(bool)));

    pFilterMenu->addSection("Группы таблиц");
    pFilterByGroupAction = new TableComboAction(this);
    pFilterByGroupAction->setModel(TablesGroupProvider::instance()->groupsModel());
    pFilterByGroupAction->setDisplayColumn(1);
    pFilterMenu->addAction(pFilterByGroupAction);

    pFilterAction->addAction(pApplyFilterAction);
    pFilterAction->setDefaultAction(pApplyFilterAction);

    pAddToGroupAction = pFilterMenu->addAction(tr("Добавить в группу"));
    pCreateGroupAction = pFilterMenu->addAction(tr("Создать группу"));
    pFilterMenu->addSeparator();
    pMuiltyLineText = pFilterMenu->addAction(QIcon(":/img/text.png"), tr("Из многострочногго текста"), this, SLOT(fromMuiltyLineText()));

    Index ++;
    QString name = QString("TablesDock%1").arg(Index);
    setObjectName(name);

    pFilterAction->setEnabled(false);
    pFilterEdit->setEnabled(false);
    pTables->setEnabled(false);
    pTables->installEventFilter(this);
    pTables->setIconSize(QSize(16, 16));
    pTables->setResizeMode(QListView::Fixed);
    pTables->setMouseTracking(true);

    connect(pFilterEdit, SIGNAL(textChanged(QString)), SLOT(filterTextChanged(QString)));
    connect(pFilterEdit, SIGNAL(returnPressed()), SLOT(filterReturnPressed()));
    connect(pTables, SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked(QModelIndex)));

    connect(pFilterAction, SIGNAL(toggled(bool)), pApplyFilterAction, SLOT(setChecked(bool)));
    connect(pApplyFilterAction, SIGNAL(toggled(bool)), pFilterAction, SLOT(setChecked(bool)));
    connect(pAddToGroupAction, SIGNAL(triggered(bool)), SLOT(addToGroup()));
    connect(pCreateGroupAction, SIGNAL(triggered(bool)), SLOT(createGroup()));
    connect(pFilterByGroupAction, SIGNAL(activated(int)), SLOT(filterByGroupCombo(int)));
}

void TablesDockWidget::setModel(FmtTablesModel *model)
{
    pTables->setModel(model);
    pModel = model;
    if (model)
    {
        pTables->setModelColumn(fnc_Name);
        pFilterAction->setChecked(false);

        pFilterAction->setEnabled(true);
        pFilterEdit->setEnabled(true);
        pTables->setEnabled(true);
    }
    else
    {
        pFilterAction->setEnabled(false);
        pFilterEdit->setEnabled(false);
        pTables->setEnabled(false);
    }
}

void TablesDockWidget::filterTriggered(bool checked)
{
    FmtTablesModel *model = qobject_cast<FmtTablesModel*>(pTables->model());

    if (model)
    {
        if (!checked)
            model->setFilterString("");
        else
            model->setFilterString(pFilterEdit->text());
        model->updateFmtList();
    }
}

void TablesDockWidget::filterTextChanged(const QString &text)
{
    Q_UNUSED(text);
    if (pFilterAction->isChecked())
    {
        pFilterAction->setChecked(false);
        filterTriggered(false);
    }
}

void TablesDockWidget::filterReturnPressed()
{
    QString text = pFilterEdit->text().simplified();
    if (!text.isEmpty())
    {
        pFilterAction->setChecked(true);
        filterTriggered(true);
    }
}

void TablesDockWidget::doubleClicked(const QModelIndex &index)
{
    FmtTablesModel *model = qobject_cast<FmtTablesModel*>(pTables->model());
    QString table = model->data(index, Qt::UserRole).toString();

    quint32 id = 0;
    ConnectionInfo *info = model->connection();
    QSqlQuery q(info->db());
    q.prepare("select T_ID from FMT_NAMES where T_NAME = ?");
    q.bindValue(0, table);

    if (!ExecuteQuery(&q) && q.next())
        id = q.value(0).toInt();

    if (id)
    {
        emit tableDbClicked(id);
    }
}

QItemSelectionModel *TablesDockWidget::selection()
{
    return pTables->selectionModel();
}

void TablesDockWidget::setSelectionMode(const QAbstractItemView::SelectionMode &mode)
{
    pTables->setSelectionMode(mode);
}

bool TablesDockWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == pTables)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *e = (QKeyEvent*)event;
            if (e->key() == Qt::Key_A && (e->modifiers() & Qt::AltModifier))
            {
                pTables->selectionModel()->clearSelection();
                return true;
            }
        }
    }
    return false;
}

QListView *TablesDockWidget::listView()
{
    return pTables;
}

bool TablesDockWidget::isFiltered()
{
    return pFilterAction->isChecked();
}

void TablesDockWidget::filterByGroupCombo(const int &id)
{
    QSharedPointer<QSqlQuery> q = TablesGroupProvider::instance()->getQueryForSeletGroup(id);
    if (!ExecuteQuery(q.data()))
    {
        QString tables;
        bool f = true;
        while(q->next())
        {
            if (!f)
                tables += ",";
            f = false;
            tables += q->value(1).toString();
        }

        if (!tables.isEmpty())
        {
            pFilterEdit->setText(tables);
            pFilterAction->setChecked(true);
            filterTriggered(true);
        }
    }
}

void TablesDockWidget::fromMuiltyLineText()
{
    bool ok = true;
    QString text = QInputDialog::getMultiLineText(this, tr("Текст"), tr("Текст с наименованием таблиц:"), QString(), &ok);

    if (ok)
    {
        int pos = 0;
        QRegExp rx("\\bd(\\w+)\\_(dbt|tmp|rec)\\b");
        QProgressDialog dlg(tr("Обработка тектса"), tr("Прервать"), 0, -1, this);

        bool canceled = false;
        bool f = true;
        QString tables;

        dlg.open();
        while ((pos = rx.indexIn(text, pos)) != -1)
        {
            if (dlg.wasCanceled())
            {
                canceled = true;
                break;
            }

            if (!f)
                tables += ",";
            f = false;
            tables += rx.cap(1);

            QApplication::processEvents();
            pos += rx.matchedLength();
        }
        dlg.close();

        if (!canceled && !tables.isEmpty())
        {
            pFilterEdit->setText(tables);
            pFilterAction->setChecked(true);
            filterTriggered(true);
        }
    }
}

void TablesDockWidget::AddTablesToGroup(const QString &GroupUid)
{
    QProgressDialog dlg(tr("Добавление в группу"), tr("Прервать"), 0, pModel->rowCount(), this);
    dlg.open();
    for (int i = 0; i < pModel->rowCount(); i++)
    {
        if (dlg.wasCanceled())
            break;

        TablesGroupProvider::instance()->addTable(GroupUid, pModel->data(pModel->index(i, 0), Qt::UserRole).toString());
        QApplication::processEvents();
        dlg.setValue(i);
    }
    dlg.close();
}

void TablesDockWidget::addToGroup()
{
    bool ok = true;
    QStringList lst = TablesGroupProvider::instance()->getGroupsList();
    QString item = QInputDialog::getItem(this, tr("Группы"), tr("Показать таблицы из"), lst, 0, false, &ok);
    int index = lst.indexOf(item);

    if (ok && index != -1)
    {
        QString uid = TablesGroupProvider::instance()->groupUid(index);
        AddTablesToGroup(uid);
    }
}

void TablesDockWidget::createGroup()
{
    bool ok = true;
    QString item = QInputDialog::getText(this, tr("Группы"), tr("Наименование новой группы"), QLineEdit::Normal, QString(), &ok);

    if (ok)
    {
        QString uid;
        if (TablesGroupProvider::instance()->addGroup(item, &uid))
        {
            AddTablesToGroup(uid);
        }
    }
}

void TablesDockWidget::updateList()
{
    pModel->updateFmtList();
}

void TablesDockWidget::applyFilter(const QString &str)
{
    pFilterEdit->setText(str);
    filterTriggered(true);

    if (!str.isEmpty())
        pFilterAction->setChecked(true);
}
