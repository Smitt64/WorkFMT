#include "generaloptions.h"
#include "ui_generaloptions.h"
#include "fmtcore.h"
#include <QSettings>

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

    QAbstractButton *pFolderBtn = ui->pathListWidget->button(StringListEditor::ButtonAddFolder);
    pFolderBtn->setVisible(true);

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

int GeneralOptions::save() 
{
    QSettings *setting = settings();

    setting->beginGroup("FmtXml");
    setting->setValue("CodePage", ui->fmtXmlEncode->currentText());
    setting->setValue("NoXsd", ui->fmtXmlXsdCheck->isChecked());
    setting->endGroup();

    return 0;
}

void GeneralOptions::restore() 
{
    QSettings *setting = settings();
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
}