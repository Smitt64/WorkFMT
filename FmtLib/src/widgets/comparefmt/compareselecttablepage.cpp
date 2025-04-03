#include "compareselecttablepage.h"
#include "comparefmtwizard.h"
#include "ui_compareselecttablepage.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QIcon>

FilteredSqlModel::FilteredSqlModel(ConnectionInfo *connection, QObject *parent) :
    QSqlQueryModel(parent)
{
    m_pInfo = connection;
}

void FilteredSqlModel::updateFilter(const QString &filter)
{
    // Формируем SQL-запрос с фильтром по тексту из lineEdit
    QString queryStr = "SELECT t_name || ' (' || t_comment || ')', t_name, t_flags FROM fmt_names";
    if (!filter.isEmpty()) {
        queryStr += " WHERE lower(t_name) LIKE lower('%" + filter + "%')";
    }

    QSqlQuery query(m_pInfo->db());
    query.prepare(queryStr);
    if (ExecuteQuery(&query))
    {
        qDebug() << "Query error:" << query.lastError().text();
        return;
    }

    setQuery(query);
}

QVariant FilteredSqlModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole)
        return QIcon("://table");

    return QSqlQueryModel::data(index, role);
}

// -------------------------------------------------------------

CompareSelectTablePage::CompareSelectTablePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CompareSelectTablePage)
{
    ui->setupUi(this);
    ui->tableEdit->setVisible(false);

    setTitle(tr("Таблица для сравнения"));
    registerField("SelectedTable", ui->tableEdit);

    connect(ui->lineEdit, &QLineEdit::returnPressed, [=]()
    {
        m_pModel->updateFilter(ui->lineEdit->text());
        emit completeChanged();
    });
}

CompareSelectTablePage::~CompareSelectTablePage()
{
    delete ui;
}

bool CompareSelectTablePage::isComplete() const
{
    return ui->listView->selectionModel()->hasSelection();
}

void CompareSelectTablePage::initializePage()
{
    CompareFmtWizard *wzrd = (CompareFmtWizard*)wizard();

    int Action = wzrd->field("Action").toInt();
    if (Action == CompareFmtWizard::CompareTable)
    {
        m_Connection.reset();
        m_pModel.reset(new FilteredSqlModel(wzrd->connection()));

        ui->listView->setModel(m_pModel.data());
        m_pModel->updateFilter("");
    }
    else if (Action == CompareFmtWizard::PageConnection)
    {
        QString unicode;
        QString User = wzrd->field("User").toString();
        QString Password = wzrd->field("Password").toString();
        QString Service = wzrd->field("Service").toString();
        bool IsUnicode = wzrd->field("IsUnicode").toBool();

        if (IsUnicode)
            unicode = "RSD_UNICODE";

        QString error;
        m_Connection.reset(new ConnectionInfo());
        m_Connection->open(QRSD_DRIVER, User, Password, Service, unicode, &error);

        m_pModel.reset(new FilteredSqlModel(wzrd->connection()));

        ui->listView->setModel(m_pModel.data());
        m_pModel->updateFilter("");
    }

    connect(ui->listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            [=](const QItemSelection &selected, const QItemSelection &deselected)
    {
        Q_UNUSED(deselected); // Неиспользуемый параметр

        QString name = m_pModel->data(m_pModel->index(ui->listView->currentIndex().row(), 1)).toString();
        ui->tableEdit->setText(name);
        setField("SelectedTable", name);
        emit completeChanged();
    });
}
