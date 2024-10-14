#include "selectactionpage.h"
#include "ui_selectactionpage.h"
#include "dbttoolwizard.h"
#include <QButtonGroup>
#include <QSpinBox>
#include <QDebug>

SelectActionPage::SelectActionPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SelectActionPage)
{
    ui->setupUi(this);
    setTitle(tr("Выбор действия"));

    fakeBtn = new QSpinBox();
    m_pGroup = new QButtonGroup(this);
    m_pGroup->addButton(ui->radioExpOld, 0);
    m_pGroup->addButton(ui->radioExp, 1);
    m_pGroup->addButton(ui->radioImp, 2);

    setTitle(tr("Выбор действия"));
    registerField("Action", fakeBtn, "value");
    connect(m_pGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), [=](int id, bool checked)
    {
        if (checked)
            fakeBtn->setValue(id);
    });

    fakeBtn->setValue(1);
    ui->radioExp->setChecked(true);
}

SelectActionPage::~SelectActionPage()
{
    delete ui;
    delete fakeBtn;
}

int SelectActionPage::nextId() const
{
    if (field("Action").toInt() == ActionExportOraOld)
        return DbtToolWizard::eConnectionPage;
    if (field("Action").toInt() == ActionExportOra)
        return DbtToolWizard::eClobParamsPage;
    if (field("Action").toInt() == ActionImportOra)
        return DbtToolWizard::eConnectionPage;

    return -1;
}
