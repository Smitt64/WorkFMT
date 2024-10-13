#include "generaloptions.h"
#include "ui_generaloptions.h"
#include "fmtcore.h"
#include <QSettings>
#include <QStandardItemModel>
#include <QDir>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QLineEdit>

static const QStringList ExpTableModelTitles = QStringList()
                                               << QObject::tr("Один файл, упрощенный текст")
                                               << QObject::tr("Один файл, упрощенный текст c удалением лишних пробелов")
                                               << QObject::tr("Разделение на два файла");

class ExpTableModelDelegate : public QStyledItemDelegate
{
public:
    ExpTableModelDelegate(QObject *parent = nullptr) :
        QStyledItemDelegate(parent)
    {

    }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if (index.column() == 1)
        {
            QComboBox *combo = new QComboBox(parent);
            combo->addItems(ExpTableModelTitles);
            return combo;
        }
        else if (index.column() == 0)
        {
            QLineEdit *edit = new QLineEdit(parent);
            return edit;
        }
        return nullptr;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        if (index.column() == 1)
        {
            QComboBox *combo = qobject_cast<QComboBox*>(editor);
            combo->setCurrentIndex(index.data(Qt::UserRole + 1).toInt());
        }
        else if (index.column() == 0)
        {
            QLineEdit *edit = qobject_cast<QLineEdit*>(editor);
            edit->setText(index.data().toString());
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        if (index.column() == 1)
        {
            QComboBox *combo = qobject_cast<QComboBox*>(editor);

            int action = combo->currentIndex();
            model->setData(index, action, Qt::UserRole + 1);
            model->setData(index, action, Qt::UserRole + 1);
            model->setData(index, combo->currentText());
        }
        else if (index.column() == 0)
        {
            QLineEdit *edit = qobject_cast<QLineEdit*>(editor);
            model->setData(index, edit->text());
        }
    }
};

GeneralOptions::GeneralOptions(QWidget *parent)
    : OptionsPage(parent)
    , ui(new Ui::GeneralOptions)
{
    ui->setupUi(this);

    m_StdFolderListHandler = new StdFolderListHandler();
    ui->pathListWidget->setHandler(m_StdFolderListHandler);
    ui->pathListWidget->setCategoryes({
        tr("Каталоги *.dat файлов"),
        tr("Каталоги *.xml файлов структуры"),
        tr("Каталоги Create Tables Sql"),
        tr("Репозитории для DiffToScript")
        });

    ui->pathsGroup->layout()->setMargin(0);

    QAbstractButton *pFolderBtn = ui->pathListWidget->button(StringListEditor::ButtonAddFolder);
    pFolderBtn->setVisible(true);

    setupExpTableModel();

    connect(pFolderBtn, &QAbstractButton::clicked, [=]()
    {
        QString text;
        if (m_StdFolderListHandler->click(text, this))
            ui->pathListWidget->append(text);
    });
}

GeneralOptions::~GeneralOptions()
{
    delete ui;

    if (m_StdFolderListHandler)
        delete m_StdFolderListHandler;
}

void GeneralOptions::setupExpTableModel()
{
    ui->expDefaultClob->addItems(ExpTableModelTitles);

    m_pExpTableDelegate = new ExpTableModelDelegate(this);
    m_pExpTableModel = new QStandardItemModel(this);
    m_pExpTableModel->setHorizontalHeaderLabels({tr("Таблица"), tr("Действие")});

    ui->exportTableSettingsView->setModel(m_pExpTableModel);
    ui->exportTableSettingsView->setItemDelegate(m_pExpTableDelegate);
    ui->exportTableSettingsView->header()->setDefaultSectionSize(200);

    connect(ui->expOldCheckBox, &QCheckBox::clicked, this, &GeneralOptions::updateExpOldFlag);
    connect(ui->newExpTableBtn, &QToolButton::clicked, this, &GeneralOptions::newExpTableBtnClicked);
    connect(ui->removeExpTableBtn, &QToolButton::clicked, this, &GeneralOptions::removeExpTableBtnClicked);
}

void GeneralOptions::removeExpTableBtnClicked()
{
    QModelIndex index = ui->exportTableSettingsView->currentIndex();

    if (!index.isValid())
        return;

    m_pExpTableModel->removeRow(index.row());
}

void GeneralOptions::newExpTableBtnClicked()
{
    int currentAction = ui->expDefaultClob->currentIndex();
    QStandardItem *allTable = new QStandardItem("");
    QStandardItem *allTitles = new QStandardItem(ExpTableModelTitles[currentAction]);

    allTable->setData(currentAction);
    allTitles->setData(currentAction);
    m_pExpTableModel->appendRow({allTable, allTitles});
}

void GeneralOptions::updateExpOldFlag()
{
    bool enable = !ui->expOldCheckBox->isChecked();
    ui->expInfoLabel->setEnabled(enable);
    ui->horizontalLayout->setEnabled(enable);
    ui->expDefaultLabel->setEnabled(enable);
    ui->expDefaultClob->setEnabled(enable);
    ui->newExpTableBtn->setEnabled(enable);
    ui->removeExpTableBtn->setEnabled(enable);
    ui->exportTableSettingsView->setEnabled(enable);
}

int GeneralOptions::save() 
{
    QSettings *setting = settings();
    setting->setValue("AutoCamelCase", ui->autoCamelCase->isChecked());

    setting->beginGroup("FmtXml");
    setting->setValue("CodePage", ui->fmtXmlEncode->currentText());
    setting->setValue("NoXsd", ui->fmtXmlXsdCheck->isChecked());
    setting->endGroup();

    auto WriteListToSettrings = [=](const int &id, const QString &context)
    {
        ui->pathListWidget->categoryWidget()->setCurrentIndex(id);

        QStringList list = ui->pathListWidget->stringList();  
        setting->beginWriteArray(context);
        for (int i = 0; i < list.size(); ++i)
        {
            setting->setArrayIndex(i);

            setting->setValue("dir", list[i]);
            setting->setValue("uses", m_DirUses[id][list[i]]);
        }
        setting->endArray();
    };

    WriteListToSettrings(0, RsExpUnlDirContext);
    WriteListToSettrings(1, RsFmtUnlDirContext);
    WriteListToSettrings(2, RsCreateSqlContext);
    WriteListToSettrings(3, RsDiffToScriptContext);

    setting->beginGroup(RsExportDatContext);
    setting->setValue("UseRSexp", ui->expOldCheckBox->isChecked());
    setting->setValue("DefaultAction", ui->expDefaultClob->currentIndex());
    setting->endGroup();

    setting->beginWriteArray(RsExportDatContext, m_pExpTableModel->rowCount());
    for (int i = 0, realindex = 0; i < m_pExpTableModel->rowCount(); i++)
    {
        QStandardItem *allTable = m_pExpTableModel->item(i, 0);
        QStandardItem *allTitles = m_pExpTableModel->item(i, 1);

        QString table = allTable->text().simplified().trimmed();

        if (!table.isEmpty())
        {
            setting->setArrayIndex(realindex);
            setting->setValue("table", allTable->text());
            setting->setValue("action", allTitles->data().toInt());
            realindex ++;
        }
    }
    setting->endArray();

    return 0;
}

void GeneralOptions::restore() 
{
    QSettings *setting = settings();
    ui->autoCamelCase->setChecked(setting->value("AutoCamelCase", true).toBool());

    setting->beginGroup("FmtXml");
    ui->fmtXmlPath->setText("fmtxml.exe");
    ui->fmtXmlEncode->setCurrentText(setting->value("CodePage", "utf8").toString());
    ui->fmtXmlXsdCheck->setChecked(setting->value("NoXsd", false).toBool());
    setting->endGroup();

    ui->pathListWidget->categoryWidget()->setCurrentIndex(0);
    int size = setting->beginReadArray(RsExpUnlDirContext);

    UsesMap RsExpUnlDirUses;
    for (int i = 0; i < size; ++i) 
    {
        setting->setArrayIndex(i);

        QString dir = setting->value("dir").toString();
        int uses = setting->value("uses").toInt();
        ui->pathListWidget->append(dir);

        RsExpUnlDirUses[dir] = uses;
    }
    setting->endArray();
    m_DirUses.insert(ui->pathListWidget->categoryWidget()->currentIndex(), RsExpUnlDirUses);

    ui->pathListWidget->categoryWidget()->setCurrentIndex(1);
    size = setting->beginReadArray(RsFmtUnlDirContext);
    UsesMap RsFmtUnlDirUses;

    for (int i = 0; i < size; ++i) 
    {
        setting->setArrayIndex(i);

        QString dir = setting->value("dir").toString();
        int uses = setting->value("uses").toInt();
        ui->pathListWidget->append(dir);

        RsFmtUnlDirUses[dir] = uses;
    }
    setting->endArray();
    m_DirUses.insert(ui->pathListWidget->categoryWidget()->currentIndex(), RsFmtUnlDirUses);

    ui->pathListWidget->categoryWidget()->setCurrentIndex(2);
    size = setting->beginReadArray(RsCreateSqlContext);
    UsesMap RsCreateSqlUses;
    for (int i = 0; i < size; ++i)
    {
        setting->setArrayIndex(i);

        QString dir = setting->value("dir").toString();
        int uses = setting->value("uses").toInt();
        ui->pathListWidget->append(dir);

        RsCreateSqlUses[dir] = uses;
    }
    setting->endArray();
    m_DirUses.insert(ui->pathListWidget->categoryWidget()->currentIndex(), RsCreateSqlUses);

    ui->pathListWidget->categoryWidget()->setCurrentIndex(3);
    size = setting->beginReadArray(RsDiffToScriptContext);
    UsesMap RsDiffToScriptUses;
    for (int i = 0; i < size; ++i)
    {
        setting->setArrayIndex(i);

        QString dir = setting->value("dir").toString();
        int uses = setting->value("uses").toInt();
        ui->pathListWidget->append(dir);

        RsDiffToScriptUses[dir] = uses;
    }
    setting->endArray();
    m_DirUses.insert(ui->pathListWidget->categoryWidget()->currentIndex(), RsDiffToScriptUses);
    ui->pathListWidget->categoryWidget()->setCurrentIndex(0);

    setting->beginGroup(RsExportDatContext);
    ui->expOldCheckBox->setChecked(setting->value("UseRSexp", false).toBool());
    ui->expDefaultClob->setCurrentIndex(setting->value("DefaultAction", 0).toInt());
    setting->endGroup();

    size = setting->beginReadArray(RsExportDatContext);
    for (int i = 0; i < size; i++)
    {
        setting->setArrayIndex(i);

        int action = setting->value("action").toInt();
        QStandardItem *allTable = new QStandardItem(setting->value("table").toString());
        QStandardItem *allTitles = new QStandardItem(ExpTableModelTitles[action]);

        allTable->setData(action);
        allTitles->setData(action);
        m_pExpTableModel->appendRow({allTable, allTitles});
    }
    setting->endArray();

    updateExpOldFlag();
}
