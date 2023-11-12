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
    m_pGroup->addButton(ui->radioButton, 0);
    m_pGroup->addButton(ui->radioButton_2, 1);

    setTitle(tr("Выбор действия"));
    registerField("Action", fakeBtn, "value");
    connect(m_pGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), [=](int id, bool checked)
    {
        if (checked)
            setField("Action", id);
    });
}

SelectActionPage::~SelectActionPage()
{
    delete ui;
    delete fakeBtn;
}

int SelectActionPage::nextId() const
{
    if (field("Action").toInt() == ActionImport)
        return DumpToolWizard::eConnectionPage;

    return DumpToolWizard::eExpParamPage;
}
