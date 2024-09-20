#include "tablegroupoptions.h"
#include "ui_tablegroupoptions.h"
#include "tablesselectwidget.h"
#include "fmtcore.h"
#include "tablesgroupprovider.h"
#include <QtXml>
#include <QMessageBox>

TableGroupOptions::TableGroupOptions(ConnectionInfo *connection, QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::TableGroupOptions)
{
    ui->setupUi(this);

    m_Tables = new TablesSelectWidget(connection);
    m_Tables->setAddFunc(std::bind(&TableGroupOptions::addTable, this, std::placeholders::_1));
    m_Tables->setRemFunc(std::bind(&TableGroupOptions::remTable, this, std::placeholders::_1));
    m_Tables->layout()->setMargin(0);
    layout()->addWidget(m_Tables);

    ui->comboBox->setModel(TablesGroupProvider::instance()->groupsModel());
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), SLOT(groupSelected(int)));
    ui->comboBox->setModelColumn(1);

    //connect(ui->pushAddGroup, &QPushButton::clicked, this, &TableGroupOptions::on_pushAddGroup_clicked);
}

TableGroupOptions::~TableGroupOptions()
{
    delete ui;
}

void TableGroupOptions::groupSelected(const int &index)
{
    if (index < 0)
        return;

    m_Tables->clearSelected();
    QSharedPointer<QSqlQuery> query = TablesGroupProvider::instance()->getQueryForSeletGroup(index);

    if (!ExecuteQuery(query.data()))
    {
        m_Tables->beginAddTables(tr("Загрузка сведений"), 0, TablesGroupProvider::instance()->getTablesCountInGroup(index));
        while(query->next())
        {
            m_Tables->userAddTable(query->value(1).toString());
        }
    }

    m_Tables->endAddTables();
}

bool TableGroupOptions::addTable(const QString &table)
{
    return TablesGroupProvider::instance()->addTable(ui->comboBox->currentIndex(), table);
}

bool TableGroupOptions::remTable(const QString &table)
{
    return TablesGroupProvider::instance()->removeTable(ui->comboBox->currentIndex(), table);
}

void TableGroupOptions::on_pushAddGroup_clicked()
{
    bool ok = false;
    QString name = QInputDialog::getText(this, tr("Создание группы"), tr("Введите наименование новой группы:"), QLineEdit::Normal, QString(), &ok);

    if (ok)
    {
        name = name.simplified();

        if (!name.isEmpty())
        {
            if (!TablesGroupProvider::instance()->isGroupExists(name))
            {
                int count = TablesGroupProvider::instance()->groupsModel()->rowCount();
                if(TablesGroupProvider::instance()->addGroup(name))
                    ui->comboBox->setCurrentIndex(count);
            }
            else
                QMessageBox::warning(this, tr("Предупреждение"), tr("Такая группа уже существует!"));
        }
    }
}

void TableGroupOptions::on_pushRemoveGroup_clicked()
{
    int index = ui->comboBox->currentIndex();

    QMessageBox::Button btn = QMessageBox::question(this, tr("Удаление группы"), tr("Вы действительно хотите удалить группу <b>%1</b>?").arg(ui->comboBox->currentText()), 
        QMessageBox::Yes | QMessageBox::No);

    if (btn == QMessageBox::Yes)
    {
        if (TablesGroupProvider::instance()->removeGroup(index))
        {
            if (!TablesGroupProvider::instance()->groupsModel()->rowCount())
                m_Tables->clearSelected();
            else
            {
                ui->comboBox->setCurrentIndex(0);
            }
        }
    }
}

void TableGroupOptions::on_pushRename_clicked()
{
    bool ok = false;
    QString nName = QInputDialog::getText(this, tr("Переименование"), tr("Введите новое наименование для группы"),
                                          QLineEdit::Normal, ui->comboBox->currentText(), &ok);

    if (ok)
    {
        TablesGroupProvider::instance()->renameGroup(ui->comboBox->currentIndex(), nName);
    }
}

void TableGroupOptions::on_btnSave_clicked()
{
    QString fname = QFileDialog::getSaveFileName(this, QString(), QString(), tr("XML файл (*.xml)"));

    if (fname.isEmpty())
        return;

    QDomDocument doc("TableGroupXml");
    doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    QDomElement root = doc.createElement("TableGroup");
    root.setAttribute("name", ui->comboBox->currentText());
    root.setAttribute("count", m_Tables->tablesCount());
    doc.appendChild(root);

    for (int i = 0; i < m_Tables->tablesCount(); i++)
    {
        QDomElement tag = doc.createElement("Table");
        QDomText t = doc.createTextNode(m_Tables->tableName(i));
        tag.appendChild(t);
        root.appendChild(tag);
    }

    QFile f(fname);
    if (f.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&f);
        stream.setCodec("UTF-8");
        stream << doc.toString();
        f.close();
    }
}

void TableGroupOptions::on_btLoad_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, QString(), QString(), tr("XML файл (*.xml)"));

    if (fname.isEmpty())
        return;

    QDomDocument doc("TableGroupXml");
    QFile file(fname);

    if (!file.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    if (TablesGroupProvider::instance()->selectGroupUid(docElem.attribute("name")).isEmpty())
    {
        trn(TablesGroupProvider::instance()->db(), [docElem]() -> int
            {
                int stat = 0;
                QString uid;
                TablesGroupProvider::instance()->addGroup(docElem.attribute("name"), &uid);
                if (!uid.isEmpty())
                {
                    QDomNode n = docElem.firstChild();
                    while(!n.isNull())
                    {
                        QDomElement e = n.toElement();
                        if(!e.isNull())
                        {
                            if (!TablesGroupProvider::instance()->addTable(uid, e.text()))
                                stat = 1;
                        }
                        n = n.nextSibling();
                    }
                }
                return stat;
            });
    }
}
