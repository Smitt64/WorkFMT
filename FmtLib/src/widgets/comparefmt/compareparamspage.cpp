#include "compareparamspage.h"
#include "ui_compareparamspage.h"
#include "comparefmtwizard.h"
#include <QSpinBox>
#include <QButtonGroup>

CompareParamsPage::CompareParamsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CompareParamsPage)
{
    ui->setupUi(this);
    setTitle(tr("Сравнить структуру"));

    fakeBtn = new QSpinBox();
    m_pGroup = new QButtonGroup(this);
    m_pGroup->addButton(ui->radioText, 0);
    m_pGroup->addButton(ui->radioTable, 1);
    m_pGroup->addButton(ui->radioConnection, 2);

    registerField("Action", fakeBtn, "value");

    connect(m_pGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), [=](int id, bool checked)
    {
        if (checked)
            setField("Action", id);
    });
}

CompareParamsPage::~CompareParamsPage()
{
    delete ui;
}

int CompareParamsPage::nextId() const
{
    if (ui->radioText->isChecked())
        return CompareFmtWizard::PageStruct;
    else if (ui->radioTable->isChecked())
        return CompareFmtWizard::PageSelectTable;

    return -1;
}
