#include "selectactionpage.h"
#include "ui_selectactionpage.h"
#include "dumptoolwizard.h"
#include "fmtcore.h"
#include <QButtonGroup>
#include <QSpinBox>
#include <QDebug>

SelectActionPage::SelectActionPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SelectActionPage)
{
    ui->setupUi(this);

    fakeBtn = new QSpinBox();
    m_pGroup = new QButtonGroup(this);
    m_pGroup->addButton(ui->oraImport, 0);
    m_pGroup->addButton(ui->oraExport, 1);
    m_pGroup->addButton(ui->pgImport, 2);
    m_pGroup->addButton(ui->pgExport, 3);

    setTitle(tr("Выбор действия"));
    registerField("Action", fakeBtn, "value");
    connect(m_pGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), [=](int id, bool checked)
    {
        if (checked)
            setField("Action", id);
    });

    ui->oraImport->setChecked(true);
}

SelectActionPage::~SelectActionPage()
{
    delete ui;
    delete fakeBtn;
}

int SelectActionPage::nextId() const
{
    if (field("Action").toInt() == ActionImportOra)
        return DumpToolWizard::eConnectionPage;
    if (field("Action").toInt() == ActionExportOra)
        return DumpToolWizard::eExpParamPage;
    if (field("Action").toInt() == ActionExportPg)
        return DumpToolWizard::eExpPgParamPage;

    return -1;
}
