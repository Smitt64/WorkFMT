#include "diffwizard.h"
#include "wizard/actionpage.h"
#include "wizard/connactionpage.h"
#include "wizard/scriptspage.h"
#include <QIcon>
#include <QAbstractButton>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QDir>
#include <QMenu>
#include <QPushButton>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>
#include "task.h"
#include "diffoptionsdlg.h"
#include <QLibrary>
#include <QDebug>

DiffWizard::DiffWizard(QWidget *parent) :
    QWizard(parent)
{
    setWindowIcon(QIcon(":/img/DiffToScript.png"));
    setFixedSize(800, 600);

    m_pActionPage = new ActionPage(this);
    m_pConnactionPage = new ConnactionPage(this);
    m_pScriptsPage = new ScriptsPage(this);

    setWindowTitle(tr("Diff to Script"));
    addPage(m_pActionPage);
    addPage(m_pConnactionPage);
    addPage(m_pScriptsPage);

    setOption(QWizard::HaveHelpButton);
    setOption(QWizard::HelpButtonOnRight, false);
    setOption(QWizard::HaveCustomButton1);
    setOption(QWizard::HaveCustomButton2);
    setOption(QWizard::NoCancelButtonOnLastPage);
    setOption(QWizard::DisabledBackButtonOnLastPage);

    QPushButton *helpbtn = (QPushButton*)button(QWizard::HelpButton);
    QAbstractButton *settingsbtn = button(QWizard::CustomButton1);
    QAbstractButton *restartbtn = button(QWizard::CustomButton2);

    m_pHelpMenu = new QMenu(helpbtn);
    helpbtn->setMenu(m_pHelpMenu);

    QAction *aboutAction = m_pHelpMenu->addAction(tr("Утилита для создания запросов по изменениям в dat файлах"));
    m_pHelpMenu->addSeparator();
    QAction *rslInfoAction = m_pHelpMenu->addAction(tr("Реализация пользовательских операций в WorkFMT с помощью RSL"));
    QAction *regParmInfo = m_pHelpMenu->addAction(tr("Генератор SQL-скриптов для работы с реестром настроек"));

    QList<QWizard::WizardButton> layout;
    layout << QWizard::BackButton << QWizard::HelpButton << QWizard::CustomButton1 << QWizard::Stretch
            << QWizard::NextButton << QWizard::CustomButton2 << QWizard::FinishButton << QWizard::CancelButton;
    setButtonLayout(layout);

    setButtonText(QWizard::CustomButton1, tr("Параметры"));
    setButtonText(QWizard::CustomButton2, tr("Рестарт"));

    /*QLibrary lib("PgConvWrapper.dll");
    lib.setLoadHints(QLibrary::ResolveAllSymbolsHint);

    if (!lib.load())
        qDebug() << "!lib.load()";
    else
    {
        typedef void (*ConvertFuncPtr)(const wchar_t* Sql, wchar_t** PgSql, wchar_t** Error, wchar_t** Tail);
        //ConvertFuncPtr func = (ConvertFuncPtr)lib.resolve("?convertSqlOraToPg@@YA?AV?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@std@@ABV12@@Z");
        ConvertFuncPtr func = (ConvertFuncPtr)lib.resolve("convertSqlOraToPg");

        if (!func)
            qDebug() << "!func";
        else
        {
            wchar_t *PgSql = nullptr;
            wchar_t *Error = nullptr;
            wchar_t *Tail = nullptr;
            std::wstring sql = L"DROP FUNCTION InsertIntoRegparmPath__ (pPath IN VARCHAR, pNAME IN VARCHAR, pTYPE IN INTEGER, pGLOBAL IN CHAR, pDESCRIPTION IN VARCHAR, pSECURITY IN CHAR, pISBRANCH IN CHAR, pTEMPLATE IN VARCHAR);";
            func(sql.c_str(), &PgSql, &Error, &Tail);
            qDebug() << "PgSql:" << QString::fromWCharArray(PgSql);
            qDebug() << "Error:" << QString::fromWCharArray(Error);
            qDebug() << "Tail:" << QString::fromWCharArray(Tail);

            free(PgSql);
            free(Error);
            free(Tail);
        }
    }*/

    connect(aboutAction, &QAction::triggered, [=]()
    {
        QDesktopServices::openUrl(QUrl("https://confluence.softlab.ru/pages/viewpage.action?pageId=609715197"));
    });

    connect(rslInfoAction, &QAction::triggered, [=]()
    {
        QDesktopServices::openUrl(QUrl("https://confluence.softlab.ru/pages/viewpage.action?pageId=610865722"));
    });

    connect(regParmInfo, &QAction::triggered, [=]()
    {
        QDesktopServices::openUrl(QUrl("https://confluence.softlab.ru/pages/viewpage.action?pageId=629571588"));
    });

    connect(settingsbtn, &QAbstractButton::clicked, [=]()
    {
        QSharedPointer<QSettings> settings = diffGetSettings();
        DiffOptionsDlg dlg(settings.data(), this);
        dlg.exec();
    });

    connect(restartbtn, &QAbstractButton::clicked, this, &DiffWizard::restart);
}

DiffWizard::~DiffWizard()
{

}

void DiffWizard::addUserField(const QString &name, const QVariant &value)
{
    m_Fields.insert(name, value);
}

const QVariant &DiffWizard::userField(const QString &name)
{
    return m_Fields[name];
}

QString DiffWizard::userValName(const QString &value) const
{
    return m_Fields.key(value);
}
