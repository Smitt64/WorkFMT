#include "errordlg.h"
#include "ui_errordlg.h"
#include "fmterrorfiltermodel.h"
#include <QPushButton>

ErrorDlg::ErrorDlg(const qint16 &mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDlg),
    pErrors(NULL)
{
    ui->setupUi(this);
    pFilterModel = new FmtErrorFilterModel(this);
    ui->tableView->verticalHeader()->setDefaultSectionSize(30);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_Mode = mode;
    if (mode == mode_Information)
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok);
    else if (mode == mode_MessageBox)
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Abort);
    else if (mode == mode_Widget)
        ui->buttonBox->setVisible(false);

    setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));

    ui->toolButtonMsg->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    ui->toolButtonWarning->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
    ui->toolButtonError->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), SLOT(onActivated(QModelIndex)));
    connect(ui->tableView, SIGNAL(activated(QModelIndex)), SLOT(onActivated(QModelIndex)));

    connect(ui->toolButtonMsg, SIGNAL(toggled(bool)), pFilterModel, SLOT(setShowInformation(bool)));
    connect(ui->toolButtonError, SIGNAL(toggled(bool)), pFilterModel, SLOT(setShowErrors(bool)));
    connect(ui->toolButtonWarning, SIGNAL(toggled(bool)), pFilterModel, SLOT(setShowWarnings(bool)));
}

ErrorDlg::~ErrorDlg()
{
    delete ui;
}

void ErrorDlg::setErrors(FmtErrors *e)
{
    pErrors = e;
    pFilterModel->setFmtErrors(e);
    ui->tableView->setModel(pFilterModel);
    ui->tableView->setColumnWidth(0, 150);
    ui->tableView->setColumnWidth(1, 320);

    ui->tableView->selectRow(0);
    onActivated(e->index(0, 1));

    ui->toolButtonMsg->setText(tr("Сообщения: %1")
                               .arg(e->infoCount()));
    ui->toolButtonWarning->setText(tr("Предупреждения: %1")
                                   .arg(e->warningsCount()));
    ui->toolButtonError->setText(tr("Ошибки: %1")
                                 .arg(e->errorsCount()));

    if (m_Mode == mode_MessageBox)
    {
        if (e->hasErrors())
            ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    }

    connect(e, SIGNAL(errorsCountChanged(quint32)), SLOT(errorsCountChanged(quint32)));
    connect(e, SIGNAL(warningsCountChanged(quint32)), SLOT(warningsCountChanged(quint32)));
    connect(e, SIGNAL(infoCountChanged(quint32)), SLOT(infoCountChanged(quint32)));
}

void ErrorDlg::onActivated(const QModelIndex &index)
{
    ui->textEdit->setText(pErrors->textAt(index.row()));
}

void ErrorDlg::setMessage(const QString &msg)
{
    ui->msgLabel->setText(msg);
}

QDialogButtonBox *ErrorDlg::buttons()
{
    return ui->buttonBox;
}

void ErrorDlg::errorsCountChanged(const quint32 &value)
{
    ui->toolButtonError->setText(tr("Ошибки: %1").arg(value));
}

void ErrorDlg::warningsCountChanged(const quint32 &value)
{
    ui->toolButtonWarning->setText(tr("Предупреждения: %1").arg(value));
}

void ErrorDlg::infoCountChanged(const quint32 &value)
{
    ui->toolButtonMsg->setText(tr("Сообщения: %1").arg(value));
}
