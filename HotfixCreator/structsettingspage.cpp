#include "structsettingspage.h"
#include "ui_structsettingspage.h"
#include "hotfixcontentmodel.h"
#include "hotfixwizard.h"
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include "model/contenttreeitem.h"

StructSettingsPage::StructSettingsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::StructSettingsPage)
{
    ui->setupUi(this);

    setTitle(tr("Структура"));

    connect(ui->pushButton, &QPushButton::clicked, this, &StructSettingsPage::OnTest);
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

void StructSettingsPage::OnTest()
{
    HotfixWizard *wzrd = (HotfixWizard*)wizard();

    QString msg;
    MakeParams params;
    params[PARAM_USER] = wzrd->field("User");
    params[PARAM_PASSWORD] = wzrd->field("Password");
    params[PARAM_SERVICE] = wzrd->field("Service");
    params[PARAM_IS_UNICODE] = wzrd->field("IsUnicode");

    params[PARAM_SOURCE_DIR] = wzrd->field("sourceEdit");
    params[PARAM_HOTFIX_DIR] = wzrd->field("hotfixEdit");
    params[PARAM_HOTFIX_NAME] = wzrd->field("hotfixName");
    params[PARAM_ORA_PG] = wzrd->field("checkOraPg");
    params[PARAM_UNPACKDBEXE] = wzrd->field("unpackDbExe");
    params[PARAM_BUILDINSTRUCTION] = wzrd->field("buildInstruction");

    ContentTreeItem *item = m_pModel->findItemByData("readme.txt", 0, Qt::DisplayRole);
    item->make(ActionMake, msg, params);
}
