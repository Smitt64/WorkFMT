#include "filetypespage.h"
#include "ui_filetypespage.h"
#include <QDebug>

FileTypesModel::FileTypesModel(HotfixWizard *wizard, QObject *parent) :
    QAbstractTableModel(parent),
    pWizard(wizard)
{
    using ContentFlag = HotfixWizard::ContentFlag;
    m_FlagData =
    {
        {tr("Исходные тексты"), ContentFlag::ContentSource},
        {tr("Файлы ресурсов"), ContentFlag::ContentLbr},
        {tr("Файлы наполнения таблиц"), ContentFlag::ContentDatFile},
        {tr("Макросы"), ContentFlag::ContentMac},
        {tr("Файлы Fmt"), ContentFlag::ContentFmt},
        {tr("Макросы"), ContentFlag::ContentMac}
    };
}

int FileTypesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_FlagData.keys().size();
}

int FileTypesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant FileTypesModel::data(const QModelIndex &index, int role) const
{
    QString value = m_FlagData.keys().at(index.row());

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return value;
    }
    else if (role == Qt::DecorationRole)
    {
        if (index.column() == 1)
        {
            if (pWizard->isSetContentFlag(m_FlagData[value]))
                return QIcon(":/img/complete.png");
            else
                return QIcon(":/img/fail.png");
        }
    }

    return QVariant();
}

FileTypesPage::FileTypesPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::FileTypesPage)
{
    ui->setupUi(this);

    m_pModel = new FileTypesModel(qobject_cast<HotfixWizard*>(parent), this);

    ui->tableView->setModel(m_pModel);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();

    ui->tableView->verticalHeader()->setDefaultSectionSize(25);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

FileTypesPage::~FileTypesPage()
{
    delete ui;
}
