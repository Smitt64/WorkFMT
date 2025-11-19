#include "viewdatpage.h"
#include "ui_viewdatpage.h"
#include "regparmwizard.h"
#include "regparmmodel/regparmproxymodel.h"
#include <QKeyEvent>
#include <QPushButton>

ViewDatPage::ViewDatPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ViewDatPage)
{
    ui->setupUi(this);
    setTitle(tr("Просмотр содержимого DAT файла"));
    m_pProxyModel = new RegParmProxyModel(this);
    //m_pProxyModel->setShowOnlyMatches(true);
    ui->treeView->setModel(m_pProxyModel);
    ui->lineEdit->installEventFilter(this);

    connect(ui->lineEdit, &QLineEdit::returnPressed, [=]()
    {
        if (ui->lineEdit->text().isEmpty())
            ui->treeView->clearSelection();

        m_pProxyModel->setFilterString(ui->lineEdit->text());
    });
}

ViewDatPage::~ViewDatPage()
{
    delete ui;
}

void ViewDatPage::initializePage()
{
    RegParmWizard *wzrd = (RegParmWizard*)wizard();
    wzrd->resetDatModel();

    m_pProxyModel->setSourceModel(wzrd->datModel());
    //ui->treeView->expandToDepth(0);

    for (int i = 0; i < m_pProxyModel->columnCount(); ++i)
        ui->treeView->resizeColumnToContents(i);
}

bool ViewDatPage::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            applyFilter();
            return true;
        }
    }
    return QWizardPage::eventFilter(obj, event);
}

void ViewDatPage::applyFilter()
{
    QString filterText = ui->lineEdit->text().trimmed();
    m_pProxyModel->setFilterString(filterText);

    if (!filterText.isEmpty())
        ui->treeView->expandAll();
    else
    {
        ui->treeView->collapseAll();
        ui->treeView->expandToDepth(0);
    }

    ui->treeView->setFocus();
}

void ViewDatPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (!qobject_cast<QPushButton*>(focusWidget()))
        {
            event->ignore();
            return;
        }
    }

    QWizardPage::keyPressEvent(event);
}
