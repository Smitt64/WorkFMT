#include "regparmwizard.h"
#include "actionpage.h"
#include "connactionpage.h"
#include "settingsselectionpage.h"
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
#include "operationwizardpage.h"
#include "regparmoptionsdlg.h"
#include <toolsruntime.h>
#include <rsscript/registerobjlist.hpp>
#include <QMenu>
#include <QPushButton>
#include <QAbstractButton>
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>

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
    setWindowIcon(QIcon(":/res/regparmtool_icon.svg"));
    setFixedSize(800, 600);

    // Создание страниц
    m_pActionPage = new ActionPage(this);
    m_pConnectionPage = new ConnactionPage(this);
    m_pSettingsSelectionPage = new SettingsSelectionPage(this);
    m_pViewPage = new ViewDatPage(this);
    m_pWordContentPage = new WordContentPage(this);
    m_pWordPreviewRegPage = new WordPreviewRegPage(this);
    m_pOperationWizardPage = new OperationWizardPage(this);

    setWindowTitle(tr("Реестр параметров - Мастер"));

    // Добавление страниц
    addPage(m_pActionPage);
    addPage(m_pConnectionPage);
    addPage(m_pSettingsSelectionPage);
    addPage(m_pViewPage);
    addPage(m_pWordContentPage);
    addPage(m_pWordPreviewRegPage);
    addPage(m_pOperationWizardPage);
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

    // Настройка меню помощи (по аналогии с DiffToScript)
    QPushButton *helpBtn = qobject_cast<QPushButton*>(button(QWizard::HelpButton));
    if (helpBtn) {
        m_pHelpMenu = new QMenu(helpBtn);
        helpBtn->setMenu(m_pHelpMenu);

        QAction *aboutAction = m_pHelpMenu->addAction(tr("Мастер работы с реестром параметров"));
        m_pHelpMenu->addSeparator();
        QAction *rslInfoAction = m_pHelpMenu->addAction(tr("Реализация пользовательских операций в WorkFMT с помощью RSL"));

        connect(aboutAction, &QAction::triggered, []() {
            QDesktopServices::openUrl(QUrl("https://confluence.softlab.ru/pages/viewpage.action?pageId=629571588"));
        });

        connect(rslInfoAction, &QAction::triggered, []() {
            QDesktopServices::openUrl(QUrl("https://confluence.softlab.ru/pages/viewpage.action?pageId=610865722"));
        });
    }
}

void RegParmWizard::setupConnections()
{
    QAbstractButton *settingsBtn = button(QWizard::CustomButton1);
    QAbstractButton *restartBtn = button(QWizard::CustomButton2);

    if (settingsBtn) {
        connect(settingsBtn, &QAbstractButton::clicked, this, &RegParmWizard::onSettingsClicked);
    }

    if (restartBtn) {
        connect(restartBtn, &QAbstractButton::clicked, this, &RegParmWizard::restart);
    }
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
    QSharedPointer<QSettings> settings = regparmGetSettings();
    RegparmOptionsDlg dlg(settings.data(), this);
    dlg.exec();
}

QAbstractItemModel *RegParmWizard::datModel()
{
    return m_pDatModel.data();
}

QSqlDatabase RegParmWizard::datDatabase() const
{
    return m_DatDatabase;
}

void RegParmWizard::resetDatModel()
{
    if (m_DatDatabase.isOpen())
        m_DatDatabase.close();

    m_DatDatabase = QSqlDatabase();
    const QString defaultConnection = QSqlDatabase::defaultConnection;
    if (QSqlDatabase::contains(defaultConnection))
        QSqlDatabase::removeDatabase(defaultConnection);

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

    QScopedPointer<DiffTable> valTable(new DiffTable());
    QString regvaldat = dataDir.absoluteFilePath("DREGVAL_DBT.dat");
    valTable->loadFromFmtXml("://xml/dregval_dbt.xml", regvaldat);

    diffLoadDatToSqlite(regvaldat, m_DatDatabase, valTable.data(), false);

    m_pDatModel.reset(new RegParmModel(m_DatDatabase));
}

QTextDocument *RegParmWizard::wordContentDocument()
{
    return m_pWordContentPage->document();
}
