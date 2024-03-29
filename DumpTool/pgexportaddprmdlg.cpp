#include "pgexportaddprmdlg.h"
#include "ui_pgexportaddprmdlg.h"
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>

PgExportAddPrmDlg::PgExportAddPrmDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PgExportAddPrmDlg)
{
    ui->setupUi(this);

    m_pModel = new QStandardItemModel(this);

    initList();
    ui->treeView->setModel(m_pModel);

    ui->treeView->setRootIsDecorated(false);
}

PgExportAddPrmDlg::~PgExportAddPrmDlg()
{
    delete ui;
}

void PgExportAddPrmDlg::initList()
{
    QFile f(":/res/pg_dump_keys.json");
    if (f.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
        QJsonObject obj = doc.object();

        QJsonArray keys = obj["keys"].toArray();
        for (const QJsonValue &val : keys)
        {
            QJsonObject key = val.toObject();

            QStandardItem *keyItem = new QStandardItem();
            keyItem->setText(key["key"].toString());
            keyItem->setToolTip(key["key"].toString());
            keyItem->setEditable(false);
            keyItem->setCheckable(true);

            QStandardItem *descItem = new QStandardItem();
            descItem->setText(key["description"].toString());
            descItem->setToolTip(key["description"].toString());
            descItem->setEditable(false);

            m_pModel->appendRow({keyItem, descItem});
        }
        f.close();
    }

    m_pModel->setHeaderData(0, Qt::Horizontal, tr("Ключ"));
    m_pModel->setHeaderData(1, Qt::Horizontal, tr("Описание"));
}

QStringList PgExportAddPrmDlg::params() const
{
    QStringList lst;

    for (int i = 0; i < m_pModel->rowCount(); i++)
    {
        if (m_pModel->item(i)->checkState() == Qt::Checked)
            lst.append(m_pModel->item(i)->text());
    }

    return lst;
}

void PgExportAddPrmDlg::setParams(const QString &params)
{
    if (params.isEmpty())
        return;

    QStringList lst = params.split(",");
    for (const QString &str : lst)
    {
        QModelIndexList indeces = m_pModel->match(m_pModel->index(0, 0), Qt::DisplayRole, str, 1);

        for (const QModelIndex &idx : indeces)
            m_pModel->item(idx.row())->setCheckState(Qt::Checked);
    }
}
