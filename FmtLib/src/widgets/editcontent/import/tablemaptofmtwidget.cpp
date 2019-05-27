#include "tablemaptofmtwidget.h"
#include "ui_tablemaptofmtwidget.h"
#include "columnalignedlayout.h"
#include "fmtfildsmodel.h"
#include <QScrollBar>
#include <QComboBox>

TableMapToFmtWidget::TableMapToFmtWidget(FmtSharedTablePtr table, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableMapToFmtWidget)
{
    ui->setupUi(this);
    pTable = table;

    alignedLayout->setMargin(0);
    alignedLayout = new ColumnAlignedLayout();
    ui->widget->setLayout(alignedLayout);
    alignedLayout->setTableColumnsToTrack(ui->tableView->horizontalHeader());
    alignedLayout->setParent(ui->widget);

    connect(ui->tableView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), SLOT(invalidateAlignedLayout()));
    connect(ui->tableView->horizontalScrollBar(), SIGNAL(valueChanged(int)), SLOT(invalidateAlignedLayout()));
    connect(ui->tableView->horizontalHeader(), SIGNAL(sectionCountChanged(int,int)), SLOT(onSectionCountChanged(int,int)));
}

TableMapToFmtWidget::~TableMapToFmtWidget()
{
    delete ui;
}

QTableView *TableMapToFmtWidget::tableWidget()
{
    return ui->tableView;
}

QComboBox *TableMapToFmtWidget::columnFilter(int columnIndex)
{
    if (columnIndex >= alignedLayout->count())
        return Q_NULLPTR;
    return qobject_cast<QComboBox*>(alignedLayout->itemAt(columnIndex)->widget());
}

void TableMapToFmtWidget::invalidateAlignedLayout()
{
    alignedLayout->invalidate();
}

int TableMapToFmtWidget::comboBoxIndex(QComboBox *combo)
{
    int index = -1;
    for (int i = 0; i < alignedLayout->count(); i++)
    {
        QComboBox *obj = qobject_cast<QComboBox*>(alignedLayout->itemAt(i)->widget());

        if (obj == combo)
            index = i;
    }
    return index;
}

void TableMapToFmtWidget::onSectionCountChanged(int oldCount, int newCount)
{
    Q_UNUSED(oldCount);

    while (alignedLayout->count() > newCount)
    {
        QWidget *w = alignedLayout->itemAt(alignedLayout->count() - 1)->widget();
        alignedLayout->removeWidget(w);
        delete w;
    }

    while (alignedLayout->count() < newCount)
    {
        QComboBox *combo = new QComboBox(this);
        //combo->setEditable(true);
        combo->setModel(pTable->fieldsModel());
        alignedLayout->addWidget(combo);
    }
}
