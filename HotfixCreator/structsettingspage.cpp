#include "structsettingspage.h"
#include "ui_structsettingspage.h"
#include "hotfixcontentmodel.h"
#include "hotfixwizard.h"
#include <QHeaderView>
#include <QSortFilterProxyModel>

StructSettingsPage::StructSettingsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::StructSettingsPage)
{
    ui->setupUi(this);

    setTitle(tr("Структура"));
}

StructSettingsPage::~StructSettingsPage()
{
    delete ui;
}

void StructSettingsPage::initializePage()
{
    HotfixWizard *wzrd = (HotfixWizard*)wizard();
    m_pModel = wzrd->structModel();

    ui->treeView->setModel(m_pModel);
    m_pModel->makeModel(field("sourceEdit").toString(), field("hotfixEdit").toString(),
                        field("hotfixName").toString(),
                        field("checkOraPg").toBool());

    m_pModel->sort(0, Qt::AscendingOrder);
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->expandAll();
}
