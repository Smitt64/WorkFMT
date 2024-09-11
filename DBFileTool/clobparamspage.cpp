#include "clobparamspage.h"
#include "ui_clobparamspage.h"
#include <QButtonGroup>
#include <QSpinBox>

ClobParamsPage::ClobParamsPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ClobParamsPage)
{
    ui->setupUi(this);

    fakeBtn = new QSpinBox();
    m_pGroup = new QButtonGroup(this);
    m_pGroup->addButton(ui->radioSimplified, 0);
    m_pGroup->addButton(ui->radioSimplifiedTrimmed, 1);
    m_pGroup->addButton(ui->radioSplitFile, 2);

    setTitle(tr("Режим выгрузки таблиц с CLOB полями"));
    registerField("ClobMode", fakeBtn, "value");
    connect(m_pGroup, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), [=](int id, bool checked)
    {
        if (checked)
            setField("ClobMode", id);
    });
}

ClobParamsPage::~ClobParamsPage()
{
    delete ui;
}
