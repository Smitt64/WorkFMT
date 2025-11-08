#include "regparmwizard.h"
#include "actionpage.h"
#include "connactionpage.h"
#include <QIcon>
#include <QVariant>

RegParmWizard::RegParmWizard(QWidget *parent) :
    QWizard(parent),
    m_pActionPage(nullptr),
    m_pConnectionPage(nullptr),
    m_pHelpMenu(nullptr)
{
    setupUi();
    setupConnections();
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

    setWindowTitle(tr("Реестр параметров - Мастер"));

    // Добавление страниц
    addPage(m_pActionPage);
    addPage(m_pConnectionPage);
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
    return static_cast<ActionType>(field("actionType").toInt());
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
