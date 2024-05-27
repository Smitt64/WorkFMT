#include "selectfolderdlg.h"
#include "ui_selectfolderdlg.h"
#include "fmtapplication.h"
#include <QStringListModel>
#include <QFileDialog>
#include <QSortFilterProxyModel>
#include <QDir>
#include <QMessageBox>
#include <QFileIconProvider>
#include <QMap>
#include <QDebug>

#define UsesCount Qt::UserRole + 1
#define DIR_SECTION "dir"
#define USES_SECTION "uses"

class FoldersSortModel Q_DECL_FINAL : public QSortFilterProxyModel
{
    Q_DISABLE_COPY(FoldersSortModel)
public:
    explicit FoldersSortModel(QObject *parent = Q_NULLPTR) :
        QSortFilterProxyModel(parent)
    {

    }

protected:
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const Q_DECL_OVERRIDE
    {
        return source_left.data(UsesCount).toInt() < source_right.data(UsesCount).toInt();
    }
};

class FoldersModel Q_DECL_FINAL : public QStandardItemModel
{
    Q_DISABLE_COPY(FoldersModel)
public:
    explicit FoldersModel(QObject *parent = Q_NULLPTR) :
        QStandardItemModel(parent)
    {
        QFileIconProvider provider;
        m_Folder = provider.icon(QFileIconProvider::Folder);
    }

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE
    {
        if (role == Qt::DecorationRole)
            return m_Folder;
        /*else if (role == UsesCount)
            return m_Uses[index.row()];*/

        return QStandardItemModel::data(index, role);
    }

    /*virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE
    {
        if (role == UsesCount)
            m_Uses[index.row()] = value.toInt();

        return QStringListModel::setData(index, value, role);
    }*/

private:
    //QMap<int, int> m_Uses;
    QIcon m_Folder;
};

SelectFolderDlg::SelectFolderDlg(QSettings *settings, const QString &context, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectFolderDlg),
    m_Context(context),
    m_pSettings(settings)
{
    ui->setupUi(this);
    m_pModel = new FoldersModel(this);

    int size = m_pSettings->beginReadArray(m_Context);
    for (int i = 0; i < size; i++)
    {
        m_pSettings->setArrayIndex(i);

        QString dir = m_pSettings->value(DIR_SECTION).toString();
        int uses = m_pSettings->value(USES_SECTION, 1).toInt();

        QStandardItem *item = new QStandardItem(dir);
        item->setData(uses, UsesCount);
        m_pModel->appendRow(item);
    }
    m_pSettings->endArray();

    m_pSortModel = new FoldersSortModel(this);
    m_pSortModel->setSourceModel(m_pModel);
    m_pSortModel->setSortRole(UsesCount);
    m_pSortModel->sort(0, Qt::DescendingOrder);

    ui->listView->setModel(m_pSortModel);
    connect(ui->listView, &QAbstractItemView::activated, this, &SelectFolderDlg::clicked);
    connect(ui->listView, &QAbstractItemView::clicked, this, &SelectFolderDlg::clicked);
    connect(ui->listView, &QAbstractItemView::doubleClicked, this, &SelectFolderDlg::doubleClicked);
}

SelectFolderDlg::SelectFolderDlg(const QString &context, QWidget *parent) :
    SelectFolderDlg(qobject_cast<FmtApplication*>(qApp)->settings(), context, parent)
{
    m_pModel = new FoldersModel(this);

    FmtApplication *app = qobject_cast<FmtApplication*>(qApp);
    m_pSettings = app->settings();

    int size = m_pSettings->beginReadArray(m_Context);
    for (int i = 0; i < size; i++)
    {
        m_pSettings->setArrayIndex(i);

        QString dir = m_pSettings->value(DIR_SECTION).toString();
        int uses = m_pSettings->value(USES_SECTION, 1).toInt();

        QStandardItem *item = new QStandardItem(dir);
        item->setData(uses, UsesCount);
        m_pModel->appendRow(item);
    }
    m_pSettings->endArray();

}

SelectFolderDlg::SelectFolderDlg(const QString &context, const QString &title, QWidget *parent) :
    SelectFolderDlg(context, parent)
{
    setWindowTitle(title);
}

SelectFolderDlg::~SelectFolderDlg()
{
    delete ui;
}

QString SelectFolderDlg::GetTopelement()
{
    if (!m_pSortModel->rowCount())
        return QString();

    return m_pSortModel->data(m_pSortModel->index(0, 0)).toString();
}

void SelectFolderDlg::on_openDir_clicked()
{
    QString firstElement = GetTopelement();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выбор каталога"), firstElement,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty())
    {
        QDir dpath(dir);
        if (dpath.exists())
        {
            m_SelectedPath = dir;
            ui->lineEdit->setText(m_SelectedPath);
        }
        else
            QMessageBox::critical(this, tr("Выбор каталога"), tr("Каталог '%1' не существует.").arg(dir));
    }
}

void SelectFolderDlg::SaveList()
{
    int size = m_pModel->rowCount();
    QModelIndexList matches = m_pModel->match(m_pModel->index(0, 0), Qt::DisplayRole, m_SelectedPath);

    if (matches.isEmpty())
    {
        QStandardItem *item = new QStandardItem(m_SelectedPath);
        item->setData(1, UsesCount);
        m_pModel->appendRow(item);
        ++size;
    }
    else
    {
        for (const QModelIndex &index : matches)
        {
            int uses = index.data(UsesCount).toInt() + 1;
            m_pModel->setData(index, uses, UsesCount);
        }
    }

    m_pSettings->beginWriteArray(m_Context);
    for (int i = 0; i < size; i++)
    {
        m_pSettings->setArrayIndex(i);

        QString path = m_pModel->data(m_pModel->index(i,0)).toString();
        int uses = m_pModel->data(m_pModel->index(i,0), UsesCount).toInt();

        m_pSettings->setValue(DIR_SECTION, path);
        m_pSettings->setValue(USES_SECTION, uses);
    }
    m_pSettings->endArray();
}

void SelectFolderDlg::clicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        m_SelectedPath = index.data().toString();
        ui->lineEdit->setText(m_SelectedPath);
    }
}

void SelectFolderDlg::doubleClicked(const QModelIndex &index)
{
    clicked(index);
    accept();
}

const QString &SelectFolderDlg::selectedPath() const
{
    return m_SelectedPath;
}

void SelectFolderDlg::accept()
{
    QDir dpath(m_SelectedPath);
    if (!dpath.exists())
    {
        QModelIndexList matches = m_pModel->match(m_pModel->index(0, 0), Qt::DisplayRole, m_SelectedPath);

        if (matches.isEmpty())
            QMessageBox::critical(this, tr("Выбор каталога"), tr("Каталог <b>%1</b> не существует.").arg(m_SelectedPath));
        else
        {
            QMessageBox::StandardButton btn = QMessageBox::critical(this, tr("Выбор каталога"),
                                                                    tr("Каталог <b>%1</b> не существует.<br>Удалить из списка?")
                                                                    .arg(m_SelectedPath),
                                                                    QMessageBox::Yes | QMessageBox::No);
            if (btn == QMessageBox::Yes)
                m_pModel->removeRow(matches[0].row());
        }
    }
    else
    {
        SaveList();
        QDialog::accept();
    }
}
