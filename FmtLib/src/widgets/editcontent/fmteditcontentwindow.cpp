#include "fmteditcontentwindow.h"
#include "fmteditcontentdialog.h"
#include "fmteditcontentfilter.h"
#include <QFontMetrics>
#include "fmtfield.h"
#include "import/importwizard.h"
#include "connectioninfo.h"

FmtEditContentWindow::FmtEditContentWindow(FmtSharedTablePtr table, QWidget *parent) :
    QMainWindow(parent)
{
    pToolBar = addToolBar(tr("База данных"));
    pToolBar->setMovable(false);
    pToolBar->setFloatable(false);
    pToolBar->setIconSize(QSize(16, 16));

    pTableView = new QTableView(this);
    pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    setCentralWidget(pTableView);

    pFilter = pToolBar->addAction(QIcon(":/img/Filter2HS.png"), tr("Фильтр"));
    pToolBar->addSeparator();
    pAddRecord = pToolBar->addAction(QIcon(":/img/DataContainer_NewRecordHS.png"), tr("Добавить запись"));
    pRemoveRecord = pToolBar->addAction(QIcon(":/img/XSDSchema_RemoveAllButSelectionFromWorkspaceCmd.png"), tr("Удалить запись"));
    pRefrash = pToolBar->addAction(QIcon(":/img/RepeatHS.png"), tr("Обновить записи"));
    pToolBar->addSeparator();
    pImportAction = pToolBar->addAction(QIcon(":/img/RepeatHS.png"), tr("Загрузить данные"));
    pTable = table;

    try
    {
        pModel = new FmtEditContentModel(table, this);
        connect(pAddRecord, SIGNAL(triggered(bool)), SLOT(OnAddRecord()));
        connect(pRemoveRecord, SIGNAL(triggered(bool)), SLOT(OnDeleteRecord()));
        connect(pRefrash, SIGNAL(triggered(bool)), pModel, SLOT(select()));
        connect(pFilter, SIGNAL(triggered(bool)), SLOT(OnFilter()));
        connect(pImportAction, SIGNAL(triggered(bool)), SLOT(OnImport()));
        connect(pTableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(OnDoubleClicked(QModelIndex)));

        if (table->connection()->type() == ConnectionInfo::CON_ORA)
        {
            pModel->select();
            pTableView->setModel(pModel);
            pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            pTableView->setColumnHidden(pModel->columnCount() - 1, true);
            pTableView->verticalHeader()->setDefaultSectionSize(25);
            SetupColWidth();
        }
        else
        {
            pAddRecord->setEnabled(false);
            pRemoveRecord->setEnabled(false);
            pRefrash->setEnabled(false);
        }
    }
    catch (const QString err)
    {
        pAddRecord->setEnabled(false);
        pRemoveRecord->setEnabled(false);
        pRefrash->setEnabled(false);
        QMessageBox::critical(this, tr("Ошибка выполнения запроса."), err);
    }
}

void FmtEditContentWindow::OnAddRecord()
{
    qint32 row = pModel->rowCount();
    if (pModel->insertRow(row))
    {
        QModelIndex index = pModel->index(row, 0);
        pTableView->scrollTo(index);
        FmtEditContentDialog dlg(pTable, pModel, this);
        dlg.setCurrentIndex(index);
        if (dlg.exec() == QDialog::Rejected)
            pModel->removeRow(dlg.currentIndex());
        else
        {
            if(pModel->submitAll())
                pModel->database().commit();
            else
                pModel->database().rollback();
            pModel->select();
            pTableView->reset();
        }
    }
}

void FmtEditContentWindow::OnDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    bool needSubmit = false;
    FmtEditContentDialog dlg(pTable, pModel, this);
    dlg.setCurrentIndex(index);
    if (dlg.exec() == QDialog::Accepted)
        needSubmit = true;
    else
        needSubmit = dlg.hasSubmit();

    if (needSubmit)
    {
        if(pModel->submitAll())
            pModel->database().commit();
        else
            pModel->database().rollback();
        pModel->select();
    }
}

void FmtEditContentWindow::SetupColWidth()
{
    QFontMetrics fm(pTableView->font());

    qint32 max = 0;
    pTableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    for (int i = 0; i < pTable->fieldsCount(); i++)
    {
        int w = fm.boundingRect(pTable->field(i)->name()).width();
        max = qMax(max, w);
        pTableView->setColumnWidth(i, w);

    }
    pTableView->horizontalHeader()->setMaximumSectionSize(500);
    pTableView->horizontalHeader()->setMinimumSectionSize(max);
    pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void FmtEditContentWindow::OnDeleteRecord()
{
    QItemSelectionModel *selection = pTableView->selectionModel();

    if (selection->hasSelection())
    {
        QModelIndexList lst = selection->selectedRows();
        if (QMessageBox::question(this, tr("Удаление записей"),
                                  tr("Удалить %1 строк?").arg(lst.count()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            qSort(lst.begin(), lst.end(), QModelIndexGrThanRow);

            foreach (const QModelIndex &index, lst) {
                pModel->removeRow(index.row());
            }

            if(pModel->submitAll())
                pModel->database().commit();
            else
                pModel->database().rollback();
            pModel->select();
        }
    }
}

void FmtEditContentWindow::OnFilter()
{
    FmtEditContentFilter dlg(pTable, this);
    dlg.setFilter(pModel->filter());
    dlg.setSort(pModel->order());
    if (dlg.exec() == QDialog::Accepted)
    {
        pModel->setOrder(dlg.sort());
        pModel->setFilter(dlg.filter());
    }
}

void FmtEditContentWindow::OnImport()
{
    ImportWizard dlg(pTable);
    dlg.exec();
}
