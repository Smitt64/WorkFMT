#include "structsettingspage.h"
#include "ui_structsettingspage.h"
#include "hotfixcontentmodel.h"
#include <QHeaderView>
#include <QSortFilterProxyModel>

StructSettingsPage::StructSettingsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::StructSettingsPage)
{
    ui->setupUi(this);
    m_pSortModel.reset(new QSortFilterProxyModel());
    m_pModel.reset(new HotfixContentModel());

    //m_pSortModel->setSourceModel(m_pModel.data());
    ui->treeView->setModel(m_pModel.data());

    setSubTitle(tr("Структура"));
}

StructSettingsPage::~StructSettingsPage()
{
    delete ui;
}

void StructSettingsPage::initializePage()
{
    m_pModel->makeModel(field("sourceEdit").toString(), field("hotfixEdit").toString(),
                        field("hotfixName").toString(),
                        field("checkOraPg").toBool());
    m_pModel->sort(0, Qt::AscendingOrder);
    //m_pSortModel->sort(HotfixContentModel::ColumnName);
    //m_pModel->sort(HotfixContentModel::ColumnName);

    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->expandAll();
}
