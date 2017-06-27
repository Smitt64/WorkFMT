#include "tablesgroupsdlg.h"
#include "ui_tablesgroupsdlg.h"
#include "tablesgroupprovider.h"
#include "fmtcore.h"
#include "tablesselectwidget.h"
#include "connectioninfo.h"
#include <QInputDialog>
#include <QtXml>
#include <QTextStream>
#include <QFile>
#include <functional>

TablesGroupsDlg::TablesGroupsDlg(ConnectionInfo *connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TablesGroupsDlg)
{
    ui->setupUi(this);
    m_Tables = new TablesSelectWidget(connection);
    m_Tables->setAddFunc(std::bind(&TablesGroupsDlg::addTable, this, std::placeholders::_1));
    m_Tables->setRemFunc(std::bind(&TablesGroupsDlg::remTable, this, std::placeholders::_1));
    layout()->addWidget(m_Tables);

    ui->comboBox->setModel(TablesGroupProvider::instance()->groupsModel());
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), SLOT(groupSelected(int)));
    ui->comboBox->setModelColumn(1);
}

TablesGroupsDlg::~TablesGroupsDlg()
{
    delete ui;
}

bool TablesGroupsDlg::addTable(const QString &table)
{
    return TablesGroupProvider::instance()->addTable(ui->comboBox->currentIndex(), table);
}

bool TablesGroupsDlg::remTable(const QString &table)
{
    return TablesGroupProvider::instance()->removeTable(ui->comboBox->currentIndex(), table);
}

void TablesGroupsDlg::groupSelected(const int &index)
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

void TablesGroupsDlg::on_pushAddGroup_clicked()
{
    bool ok = false;
    QString name = QInputDialog::getText(this, tr("Создание группы"), tr("Введите наименование новой группы:"), QLineEdit::Normal, QString(), &ok);

    if (ok)
    {
        name = name.simplified();

        if (!name.isEmpty())
        {
            int count = TablesGroupProvider::instance()->groupsModel()->rowCount();
            if(TablesGroupProvider::instance()->addGroup(name))
            {
                ui->comboBox->setCurrentIndex(count);
            }
        }
    }
}

void TablesGroupsDlg::on_pushRemoveGroup_clicked()
{
    int index = ui->comboBox->currentIndex();
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

void TablesGroupsDlg::on_pushRename_clicked()
{
    bool ok = false;
    QString nName = QInputDialog::getText(this, tr("Переименование"), tr("Введите новое наименование для группы"),
                                          QLineEdit::Normal, ui->comboBox->currentText(), &ok);

    if (ok)
    {
        TablesGroupProvider::instance()->renameGroup(ui->comboBox->currentIndex(), nName);
    }
}

void TablesGroupsDlg::on_btnSave_clicked()
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

void TablesGroupsDlg::on_btLoad_clicked()
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
