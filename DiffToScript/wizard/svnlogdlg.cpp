#include "svnlogdlg.h"
#include "ui_svnlogdlg.h"
#include "svnlogmodel.h"
#include <QVBoxLayout>
#include <QSplitter>
#include <QLayout>
#include <QTreeView>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>

SvnLogDlg::SvnLogDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SvnLogDlg)
{
    ui->setupUi(this);

    setWindowTitle(tr("Выбор ревизии"));
    setWindowFlag(Qt::WindowMaximizeButtonHint);

    m_pModel = new SvnLogModel(this);
    m_pFilesModel = new SvnLogItemsModel(this);

    m_pLogView = new QTreeView(this);
    m_pLogView->setRootIsDecorated(false);
    m_pLogView->setUniformRowHeights(true);
    m_pLogView->setModel(m_pModel);

    m_pFilesView = new QTreeView(this);
    m_pFilesView->setRootIsDecorated(false);
    m_pFilesView->setUniformRowHeights(true);
    m_pFilesView->setModel(m_pFilesModel);

    m_pComment = new QPlainTextEdit(this);
    m_pComment->setReadOnly(true);

    m_pSplitter = new QSplitter(Qt::Vertical, this);
    m_pSplitter->addWidget(m_pLogView);
    m_pSplitter->addWidget(m_pComment);
    m_pSplitter->addWidget(m_pFilesView);

    QDate curr = QDate::currentDate();
    ui->fromEdit->setDate(QDate(curr.year(), curr.month(), 1));
    ui->toEdit->setDate(QDate(curr.year(), curr.month(), curr.daysInMonth()));

    ui->verticalLayout->insertWidget(1, m_pSplitter);

    connect(m_pLogView, &QTreeView::clicked, [=](const QModelIndex &index)
    {
        if (!index.isValid())
        {
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            return;
        }

        QModelIndex comment = m_pModel->index(index.row(), SvnLogModel::fld_message);
        QModelIndex revision = m_pModel->index(index.row(), SvnLogModel::fld_revision);
        m_pComment->setPlainText(m_pModel->data(comment).toString());

        m_SelectedRevision = revision.data().toString();
        m_pFilesModel->refresh(m_SelectedRevision);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    });

    connect(ui->fromEdit, &QDateEdit::dateChanged, [=](const QDate &dt)
    {
        UpdateDateRange();
    });

    connect(ui->toEdit, &QDateEdit::dateChanged, [=](const QDate &dt)
    {
        UpdateDateRange();
    });

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    QSettings settings("DiffToScript.ini", QSettings::IniFormat);

    settings.beginGroup("SvnLogDlg");
    restoreGeometry(settings.value("geometry").toByteArray());
    m_pSplitter->restoreState(settings.value("splitter").toByteArray());
    settings.endGroup();
}

SvnLogDlg::~SvnLogDlg()
{
    delete ui;
}

QString SvnLogDlg::revision() const
{
    return m_SelectedRevision;
}

void SvnLogDlg::setPath(const QString &path)
{
    SvnInfoMap info = SvnGetRepoInfo(path);
    m_Url = info["url"];

    m_pModel->setPath(path, m_Url);
    m_pFilesModel->setPath(path, m_Url);
    //UpdateDateRange(false);
    m_pModel->refresh();
}

void SvnLogDlg::accept()
{
    QSettings settings("DiffToScript.ini", QSettings::IniFormat);

    QByteArray data = saveGeometry();
    settings.beginGroup("SvnLogDlg");
    settings.setValue("geometry", data);
    settings.setValue("splitter", m_pSplitter->saveState());
    settings.endGroup();
    settings.sync();

    QDialog::accept();
}

void SvnLogDlg::UpdateDateRange(bool NeedRefresh)
{
    m_pModel->setDateRange(ui->fromEdit->date(), ui->toEdit->date());

    if (NeedRefresh)
        m_pModel->refresh();
}

void SvnLogDlg::on_dateBox_stateChanged(int arg1)
{
    bool isChecked = arg1 == Qt::Checked;
    ui->fromEdit->setEnabled(isChecked);
    ui->toEdit->setEnabled(isChecked);

    if (!isChecked)
        m_pModel->setDateRange(QDate(), QDate());

    m_pModel->refresh();
}

