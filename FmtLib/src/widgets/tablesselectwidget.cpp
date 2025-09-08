#include "tablesselectwidget.h"
#include "connectioninfo.h"
#include "tablesdockwidget.h"
#include "fmttablesmodel.h"

TablesSelectWidget::TablesSelectWidget(ConnectionInfo *connection, QWidget *parent) : QWidget(parent)
{
    pAddFunc = Q_NULLPTR;
    pRemFunc = Q_NULLPTR;
    pDstModel = new QStandardItemModel(this);
    pSourceModel = new FmtTablesModel(connection, this);
    pSourceModel->updateFmtList();

    sourceList = new TablesDockWidget;
    sourceList->setObjectName(QStringLiteral("sourceList"));
    sourceList->setModel(pSourceModel);
    sourceList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    sourceList->listView()->installEventFilter(this);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(sourceList);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(verticalSpacer);

    addButton = new QPushButton(this);
    addButton->setObjectName(QStringLiteral("addButton"));
    addButton->setIcon(QIcon(":/img/RightArrowHS.png"));

    addAllButton = new QPushButton(this);
    addAllButton->setObjectName(QStringLiteral("addAllButton"));
    addAllButton->setIcon(QIcon(":/img/DoubleRightArrowHS.png"));

    verticalLayout->addWidget(addButton);
    verticalLayout->addWidget(addAllButton);

    removeButton = new QPushButton(this);
    removeButton->setObjectName(QStringLiteral("removeButton"));
    removeButton->setIcon(QIcon(":/img/LeftArrowHS.png"));

    removeAllButton = new QPushButton(this);
    removeAllButton->setObjectName(QStringLiteral("removeAllButton"));
    removeAllButton->setIcon(QIcon(":/img/DoubleLeftArrowHS.png"));

    hLine = new QFrame(this);
    hLine->setFrameShape(QFrame::HLine);

    verticalLayout->addWidget(hLine);
    verticalLayout->addWidget(removeButton);
    verticalLayout->addWidget(removeAllButton);

    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(verticalSpacer_2);
    horizontalLayout->addLayout(verticalLayout);

    dstListView = new QListView(this);
    dstListView->setObjectName(QStringLiteral("dstListView"));
    dstListView->setModel(pDstModel);
    dstListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    dstListView->installEventFilter(this);

    horizontalLayout->addWidget(dstListView);
    pUserAddDlg = Q_NULLPTR;
    m_SingleSelection = false;

    setLayout(horizontalLayout);

    connect(addButton, SIGNAL(clicked(bool)), SLOT(addButtonPressed()));
    connect(removeButton, SIGNAL(clicked(bool)), SLOT(removeButtonPressed()));
    connect(addAllButton, SIGNAL(clicked(bool)), SLOT(addAllButtonPressed()));
    connect(removeAllButton, SIGNAL(clicked(bool)), SLOT(removeAllButtonPressed()));

    connect(sourceList->listView(), &QListView::doubleClicked, this, &TablesSelectWidget::doubleAddClicked);
    connect(dstListView, &QListView::doubleClicked, this, &TablesSelectWidget::doubleRemoveClicked);

    connect(sourceList->selection(), &QItemSelectionModel::selectionChanged,
            this, &TablesSelectWidget::updateButtonsState);
    connect(dstListView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &TablesSelectWidget::updateButtonsState);
    connect(pDstModel, &QStandardItemModel::rowsRemoved,
            this, &TablesSelectWidget::updateButtonsState);
    connect(pDstModel, &QStandardItemModel::rowsInserted,
            this, &TablesSelectWidget::updateButtonsState);
}

void TablesSelectWidget::addButtonPressed()
{
    QItemSelectionModel *m = sourceList->selection();

    if (!m->hasSelection())
        return;

    QModelIndexList indexList = m->selectedIndexes();

    QProgressDialog dlg(tr("Добавление записей"), tr("Отмена"), 0, indexList.size(), this);
    dlg.open();
    for (int i = 0; i < indexList.size(); i++)
    {
        const QModelIndex &index = indexList[i];

        dlg.setValue(i);
        dlg.setLabelText(tr("Добавление записей: %1 из %2")
                         .arg(i + 1)
                         .arg(indexList.size()));

        if (dlg.wasCanceled())
            break;

        if (index.isValid())
        {
            CopyToDstList(index);
        }
        QApplication::processEvents();
    }
    dlg.setValue(indexList.size());

    updateButtonsState();
}

void TablesSelectWidget::removeButtonPressed()
{
    QItemSelectionModel *m = dstListView->selectionModel();

    if (!m->hasSelection())
        return;

    QModelIndexList indexList = m->selectedIndexes();

    QProgressDialog dlg(tr("Удаление записей"), tr("Отмена"), 0, 0, this);
    dlg.open();

    QList<qint32> rows;
    for (int i = 0; i < indexList.size(); i++)
    {
        rows.append(indexList[i].row());
    }
    qSort(rows);

    int prev = -1;
    int progress = 0;
    dlg.setRange(0, rows.count());
    for(int i = rows.count() - 1; i >= 0; i -= 1 )
    {
        int current = rows[i];
        dlg.setValue(progress);
        dlg.setLabelText(tr("Удаление записей: %1 из %2")
                         .arg(i + 1)
                         .arg(rows.size()));
        if(current != prev)
        {
            QString table = pDstModel->data(pDstModel->index(current, 0), Qt::UserRole).toString();
            if (pRemFunc && pRemFunc(table))
            {
                pDstModel->removeRow(current);
            }
            /*pWizard->pExp->removeTable();
            pDstModel->removeRow(current);*/
            prev = current;
        }
        QApplication::processEvents();
        progress ++;
    }

    dlg.setValue(indexList.size());

    updateButtonsState();
}

void TablesSelectWidget::CopyToDstList(const QModelIndex &index)
{
    if (m_SingleSelection && pDstModel->rowCount() > 0)
        return;

    QString table = index.data(Qt::UserRole).toString();
    if (pAddFunc && pAddFunc(table))
    {
        if (m_SingleSelection)
            clearSelected();

        QStandardItem *item = new QStandardItem();
        item->setData(index.data(Qt::DisplayRole), Qt::DisplayRole);
        item->setData(index.data(Qt::DecorationRole), Qt::DecorationRole);
        item->setData(index.data(Qt::UserRole), Qt::UserRole);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        pDstModel->appendRow(item);
        emit tableAdded(table);
        emit selectionChanged();
        updateButtonsState();
    }
}

void TablesSelectWidget::setAddFunc(TablesSelectAddFunc func)
{
    pAddFunc = func;
}

void TablesSelectWidget::setRemFunc(TablesSelectRemFunc func)
{
    pRemFunc = func;
}

void TablesSelectWidget::addAllButtonPressed()
{
    QProgressDialog dlg(tr("Добавление записей"), tr("Отмена"), 0, pSourceModel->rowCount(), this);

    dlg.open();
    int rows = pSourceModel->rowCount();
    for (int i = 0; i < rows; i++)
    {
        dlg.setValue(i);
        if (dlg.wasCanceled())
            break;

        QModelIndex index = pSourceModel->index(i, 0);
        CopyToDstList(index);
        dlg.setLabelText(tr("Добавление записей: %1 из %2")
                         .arg(i + 1)
                         .arg(rows));
        QApplication::processEvents();
    }
    dlg.setValue(rows);
}

void TablesSelectWidget::removeAllButtonPressed()
{
    QProgressDialog dlg(tr("Удаление записей"), tr("Отмена"), 0, pDstModel->rowCount(), this);

    dlg.open();
    int rows = pDstModel->rowCount();
    for (int i = 0; i < rows; i++)
    {
        dlg.setValue(i);
        if (dlg.wasCanceled())
            break;

        QModelIndex index = pDstModel->index(0, 0);
        dlg.setLabelText(tr("Удаление записей: %1 из %2")
                         .arg(i + 1)
                         .arg(rows));

        QString table = pDstModel->data(index, Qt::UserRole).toString();
        if (pRemFunc && pRemFunc(table))
        {
            pDstModel->removeRow(index.row());
        }
        /*pWizard->pExp->removeTable(pDstModel->data(index, Qt::UserRole).toString());
        pDstModel->removeRow(index.row());*/
        QApplication::processEvents();
    }
    dlg.setValue(rows);
}

void TablesSelectWidget::beginAddTables(const QString &msg, const int &min, const int &max)
{
    pUserAddDlg = new QProgressDialog(msg, tr("Отмена"), min, max, this);
    m_UserAddIndex = min;
    pUserAddDlg->open();
}

void TablesSelectWidget::endAddTables()
{
    pUserAddDlg->close();
    delete pUserAddDlg;
    pUserAddDlg = NULL;
}

bool TablesSelectWidget::userAddTable(const QString &table)
{
    bool result = true;

    if (pUserAddDlg->wasCanceled())
        result = false;

    if (result)
    {
        pUserAddDlg->setValue(m_UserAddIndex);

        QStandardItem *item = new QStandardItem();
        item->setData(table, Qt::DisplayRole);
        item->setData(QIcon(":/table"), Qt::DecorationRole);
        item->setData(table, Qt::UserRole);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        pDstModel->appendRow(item);
        m_UserAddIndex ++;
        QApplication::processEvents();
    }

    return result;
}

void TablesSelectWidget::clearSelected()
{
    pDstModel->clear();
    emit selectionChanged();
    updateButtonsState();
}

int TablesSelectWidget::tablesCount() const
{
    return pDstModel->rowCount();
}

QString TablesSelectWidget::tableName(const int &index) const
{
    return pDstModel->item(index)->text();
}

bool TablesSelectWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == sourceList->listView() && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = dynamic_cast<QKeyEvent*>(event);

        if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Space)
            addButtonPressed();
    }
    else if (obj == dstListView && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *key = dynamic_cast<QKeyEvent*>(event);

        if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Space)
            removeButtonPressed();
    }
    return QWidget::eventFilter(obj, event);
}

void TablesSelectWidget::doubleAddClicked(const QModelIndex &index)
{
    addButtonPressed();
}

void TablesSelectWidget::doubleRemoveClicked(const QModelIndex &index)
{
    removeButtonPressed();
}

void TablesSelectWidget::setSingleSelectionMode(bool single)
{
    m_SingleSelection = single;
    if (single)
    {
        dstListView->setSelectionMode(QAbstractItemView::SingleSelection);
        sourceList->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    else
    {
        dstListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        sourceList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    updateButtonsState();
}

QPushButton* TablesSelectWidget::getButton(int buttonType) const
{
    switch (buttonType)
    {
    case AddButton: return addButton;
    case AddAllButton: return addAllButton;
    case RemoveButton: return removeButton;
    case RemoveAllButton: return removeAllButton;
    default: return Q_NULLPTR;
    }
}

void TablesSelectWidget::addTable(const QString &tableName)
{
    if (m_SingleSelection)
        clearSelected();

    QStandardItem *item = new QStandardItem();
    item->setData(tableName, Qt::DisplayRole);
    item->setData(QIcon(":/table"), Qt::DecorationRole);
    item->setData(tableName, Qt::UserRole);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    pDstModel->appendRow(item);
    emit tableAdded(tableName);
    emit selectionChanged();
    updateButtonsState();
}

void TablesSelectWidget::updateButtonsState()
{
    // В режиме одной таблицы скрываем кнопки массового добавления/удаления
    if (m_SingleSelection)
    {
        addAllButton->setVisible(false);
        removeAllButton->setVisible(false);
        hLine->setVisible(false);
    }
    else
    {
        addAllButton->setVisible(true);
        removeAllButton->setVisible(true);
        hLine->setVisible(true);
    }

    // Обновляем состояние кнопок в зависимости от выбора
    bool hasSourceSelection = sourceList->selection()->hasSelection();
    bool hasDestSelection = dstListView->selectionModel()->hasSelection();
    bool hasDestItems = pDstModel->rowCount() > 0;

    addButton->setEnabled(hasSourceSelection && (!m_SingleSelection || pDstModel->rowCount() == 0));
    removeButton->setEnabled(hasDestSelection);
    removeAllButton->setEnabled(hasDestItems);
}
