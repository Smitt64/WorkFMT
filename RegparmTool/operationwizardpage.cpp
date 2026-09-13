#include "operationwizardpage.h"
#include "ui_operationwizardpage.h"
#include <errorsmodel.h>
#include <errorwidget.h>
#include <QVBoxLayout>
#include <QThreadPool>
#include "regparmwizard.h"
#include "operation/addfromwordoperation.h"
#include "operation/addregtodatoperation.h"

OperationWizardPage::OperationWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::OperationWizardPage),
    m_pErrorsModel(new ErrorsModel(this))
{
    ui->setupUi(this);
    ui->widget->setErrors(m_pErrorsModel);
}

OperationWizardPage::~OperationWizardPage()
{
    delete ui;
}

void OperationWizardPage::initializePage()
{
    RegParmWizard *wzrd = qobject_cast<RegParmWizard*>(wizard());
    if (!wzrd)
        return;

    m_pErrorsModel->clear();

    if (wzrd->selectedAction() == RegParmWizard::ActionAddFromWord)
    {
        setTitle(tr("Создание новых настроек"));

        AddFromWordOperation *op = new AddFromWordOperation();
        op->setWizzard(wzrd);
        op->setAutoDelete(false);

        connect(op, &AddFromWordOperation::errorMessage, this, [this](const QString &msg) {
            m_pErrorsModel->addError(msg);
        }, Qt::QueuedConnection);
        connect(op, &AddFromWordOperation::warningMessage, this, [this](const QString &msg) {
            m_pErrorsModel->appendError(msg, ErrorsModel::TypeWarning);
        }, Qt::QueuedConnection);
        connect(op, &AddFromWordOperation::informationMessage, this, [this](const QString &msg) {
            m_pErrorsModel->addMessage(msg);
        }, Qt::QueuedConnection);
        connect(op, &AddFromWordOperation::finished, this, [this]() {
            m_pErrorsModel->addMessage(tr("Операция завершена"));
        }, Qt::QueuedConnection);
        connect(op, &AddFromWordOperation::finished, op, &AddFromWordOperation::deleteLater, Qt::QueuedConnection);

        QThreadPool::globalInstance()->start(op);
    }
    else if (wzrd->selectedAction() == RegParmWizard::ActionAddRegToDat)
    {
        setTitle(tr("Добавление настроек в DAT файлы"));

        AddRegToDatOperation *op = new AddRegToDatOperation();
        op->setWizzard(wzrd);
        op->setAutoDelete(false);

        connect(op, &AddRegToDatOperation::errorMessage, this, [this](const QString &msg) {
            m_pErrorsModel->addError(msg);
        }, Qt::QueuedConnection);
        connect(op, &AddRegToDatOperation::warningMessage, this, [this](const QString &msg) {
            m_pErrorsModel->appendError(msg, ErrorsModel::TypeWarning);
        }, Qt::QueuedConnection);
        connect(op, &AddRegToDatOperation::informationMessage, this, [this](const QString &msg) {
            m_pErrorsModel->addMessage(msg);
        }, Qt::QueuedConnection);
        connect(op, &AddRegToDatOperation::finished, this, [this]() {
            m_pErrorsModel->addMessage(tr("Операция завершена"));
        }, Qt::QueuedConnection);
        connect(op, &AddRegToDatOperation::finished, op, &AddRegToDatOperation::deleteLater, Qt::QueuedConnection);

        QThreadPool::globalInstance()->start(op);
    }
}
