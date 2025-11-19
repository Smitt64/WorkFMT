#include "regparmwizard.h"
#include "actionpage.h"
#include "connactionpage.h"
#include "recordparser.h"
#include "difftableinfo.h"
#include "fmtcore.h"
#include <QIcon>
#include <QVariant>
#include "linesparsermain.h"
#include "regparmmodel/regparmmodel.h"
#include "viewdatpage.h"
#include "wordcontentpage.h"
#include "wordpreviewregpage.h"
#include <toolsruntime.h>
#include <rsscript/registerobjlist.hpp>

RegParmWizard::RegParmWizard(QWidget *parent) :
    QWizard(parent),
    m_pActionPage(nullptr),
    m_pConnectionPage(nullptr),
    m_pViewPage(nullptr),
    m_pHelpMenu(nullptr)
{
    QDir trDir(qApp->applicationDirPath());
    if (trDir.cd("translations"))
    {
        QString translatorFile = QString("qt_%1").arg("ru");
        if (qt_translator.load(translatorFile, trDir.absolutePath()))
            qApp->installTranslator(&qt_translator);
    }

    FmtInit();
    setupUi();
    setupConnections();

    rslAddStaticMacroDir(".\\mac\\regparmtool");
}

RegParmWizard::~RegParmWizard()
{
    // Деструктор автоматически удаляет дочерние виджеты
}

void RegParmWizard::setupUi()
{
    setWindowIcon(QIcon(":/img/RegParmWizard.png"));
    setFixedSize(800, 600);

    // Создание страниц
    m_pActionPage = new ActionPage(this);
    m_pConnectionPage = new ConnactionPage(this);
    m_pViewPage = new ViewDatPage(this);
    m_pWordContentPage = new WordContentPage(this);
    m_pWordPreviewRegPage = new WordPreviewRegPage(this);

    setWindowTitle(tr("Реестр параметров - Мастер"));

    // Добавление страниц
    addPage(m_pActionPage);
    addPage(m_pConnectionPage);
    addPage(m_pViewPage);
    addPage(m_pWordContentPage);
    addPage(m_pWordPreviewRegPage);
   // addPage(m_pScriptsPage);

    // Настройка кнопок
    setOption(QWizard::HaveHelpButton);
    setOption(QWizard::HaveCustomButton1);
    setOption(QWizard::HaveCustomButton2);
    setOption(QWizard::NoCancelButtonOnLastPage);
    setOption(QWizard::DisabledBackButtonOnLastPage);

    // Кастомный layout кнопок
    QList<QWizard::WizardButton> layout;
    layout << QWizard::BackButton << QWizard::HelpButton << QWizard::CustomButton1
            << QWizard::Stretch << QWizard::NextButton << QWizard::CustomButton2
            << QWizard::FinishButton << QWizard::CancelButton;
    setButtonLayout(layout);

    setButtonText(QWizard::CustomButton1, tr("Настройки"));
    setButtonText(QWizard::CustomButton2, tr("Рестарт"));

    // Настройка меню помощи
   /* QPushButton *helpBtn = dynamic_cast<QPushButton*>(button(QWizard::HelpButton));
    if (helpBtn) {
        m_pHelpMenu = new QMenu(helpBtn);
        helpBtn->setMenu(m_pHelpMenu);

        QAction *aboutAction = m_pHelpMenu->addAction(tr("О мастере реестра параметров"));
        QAction *docsAction = m_pHelpMenu->addAction(tr("Документация"));
        m_pHelpMenu->addSeparator();
        QAction *examplesAction = m_pHelpMenu->addAction(tr("Примеры использования"));

        connect(aboutAction, &QAction::triggered, []() {
            QDesktopServices::openUrl(QUrl("https://confluence.softlab.ru/pages/viewpage.action?pageId=629571588"));
        });

        connect(docsAction, &QAction::triggered, []() {
            QDesktopServices::openUrl(QUrl("https://confluence.softlab.ru/display/DOC/RegParm+Wizard"));
        });

        connect(examplesAction, &QAction::triggered, []() {
            QDesktopServices::openUrl(QUrl("https://confluence.softlab.ru/display/EXAMPLES/RegParm+Examples"));
        });
    }*/
}

void RegParmWizard::setupConnections()
{
    /*QAbstractButton *settingsBtn = button(QWizard::CustomButton1);
    QAbstractButton *restartBtn = button(QWizard::CustomButton2);

    if (settingsBtn) {
        connect(settingsBtn, &QAbstractButton::clicked, this, &RegParmWizard::onSettingsClicked);
    }

    if (restartBtn) {
        connect(restartBtn, &QAbstractButton::clicked, this, &RegParmWizard::onRestartClicked);
    }*/
}

void RegParmWizard::addUserField(const QString &name, const QVariant &value)
{
    m_Fields.insert(name, value);
}

const QVariant &RegParmWizard::userField(const QString &name)
{
    return m_Fields[name];
}

QString RegParmWizard::userValName(const QVariant &value) const
{
    return m_Fields.key(value);
}

RegParmWizard::ActionType RegParmWizard::selectedAction() const
{
    return static_cast<ActionType>(field("Action").toInt());
}

void RegParmWizard::onSettingsClicked()
{
    // TODO: Реализовать диалог настроек
    //qDebug() << "Settings button clicked";
}

void RegParmWizard::onRestartClicked()
{
    // TODO: Реализовать рестарт мастера
    //qDebug() << "Restart button clicked";
}

QAbstractItemModel *RegParmWizard::datModel()
{
    return m_pDatModel.data();
}

void RegParmWizard::resetDatModel()
{
    if (m_DatDatabase.isOpen())
        m_DatDatabase.close();

    m_DatDatabase = QSqlDatabase::addDatabase("QSQLITE");
    m_DatDatabase.setDatabaseName("1_DatDebugDb.sqlite");
    m_DatDatabase.open();

    LinesParserMain linesParser;

    VcsType m_VcsType = (VcsType)userField("VcsType").toInt();
    if (m_VcsType == VcsType::Svn)
        linesParser.setTableParser(new LinesTablePareser("Index: "));
    else
        linesParser.setTableParser(new LinesTablePareser("diff --git "));

    QDir dir(field("RepositoryPath").toString());
    QString dataPath = dir.absoluteFilePath("Distrib/DBFile/Data");
    QDir dataDir(dataPath);

    QScopedPointer<DiffTable> table(new DiffTable());
    QString regparmdat = dataDir.absoluteFilePath("DREGPARM_DBT.dat");
    table->loadFromFmtXml("://xml/dregparm_dbt.xml", regparmdat);

    diffLoadDatToSqlite(regparmdat, m_DatDatabase, table.data(), false);

    m_pDatModel.reset(new RegParmModel(m_DatDatabase));
}

QTextDocument *RegParmWizard::wordContentDocument()
{
    return m_pWordContentPage->document();
}
