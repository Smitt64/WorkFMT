#include "filelistdialog.h"
#include "ui_filelistdialog.h"
#include <QFileSystemModel>
#include <QDebug>
#include <QSortFilterProxyModel>

FileListDialog::FileListDialog(const QString &path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileListDialog)
{
    ui->setupUi(this);

    m_pModel = new QFileSystemModel(this);

    QModelIndex root = m_pModel->setRootPath(path);
    m_pModel->setNameFilters(QStringList() << "*.dmp");
    m_pModel->setFilter(QDir::Files);
    m_pModel->setNameFilterDisables(false);

    ui->listView->setModel(m_pModel);
    ui->listView->setRootIndex(root);
    ui->dirLabel->setText(path);

    connect(ui->listView, &QListView::doubleClicked, [=](const QModelIndex &index)
    {
        if (index.isValid())
            accept();
    });

    connect(ui->listView, &QListView::clicked, [=](const QModelIndex &index)
    {
        if (index.isValid())
            ui->lineEdit->setText(m_pModel->fileName(index));
    });
}

FileListDialog::~FileListDialog()
{
    delete ui;
}

QString FileListDialog::fileName() const
{
    return m_pModel->fileName(ui->listView->currentIndex());
}
