#include "massopenfucnparams.h"
#include "ui_massopenfucnparams.h"
#include "codeeditor.h"

MassOpenFucnParams::MassOpenFucnParams(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MassOpenFucnParams)
{
    ui->setupUi(this);
    pEditor = new CodeEditor(this);
    pEditor->setReadOnly(true);
    ui->verticalLayout->addWidget(pEditor);
}

MassOpenFucnParams::~MassOpenFucnParams()
{
    delete ui;
}
