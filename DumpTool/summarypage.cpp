#include "summarypage.h"
#include "ui_summarypage.h"
#include "selectactionpage.h"
#include "dumptoolwizard.h"

SummaryPage::SummaryPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SummaryPage)
{
    ui->setupUi(this);
    setCommitPage(true);

    m_Stream.reset(new QTextStream(&m_Text));
}

SummaryPage::~SummaryPage()
{
    delete ui;
}

void SummaryPage::initializePage()
{
    DumpToolWizard *wzrd = qobject_cast<DumpToolWizard*>(wizard());
    int action = field("Action").toInt();

    m_Text.clear();
    if (action == SelectActionPage::ActionImportOra)
    {
        setTitle(tr("Информация об операции импорта (Oracle)"));
        AddParam(tr("Имя пользователя (куда заливать)"), field("UserName").toString());

        if (field("DropUser").toBool())
            AddParam(tr("Пересодать, если существует"), tr("Да"));
        else
            AddParam(tr("Пересодать, если существует"), tr("Нет"));

        AddParam(tr("База данных (куда заливать)"), field("Service").toString());
        AddParam(tr("Имя системного пользователя"), field("SystemName").toString());
        AddParam(tr("Пароль системного пользователя"), field("SystemPass").toString());

        AddParam(tr("Расположение файла дампа"), field("LocationName").toString());
        AddParam(tr("Каталог с файлом дампа"), field("PathToDump").toString());
        AddParam(tr("Файл дампа"), field("DumpFile").toString());
        AddParam(tr("Владелец схемы"), field("SchemeOwner").toString());
        AddParam(tr("Каталог импорта"), field("ImportPath").toString());
    }
    else if (action == SelectActionPage::ActionExportOra)
    {
        setTitle(tr("Информация об операции экспорта (Oracle)"));
        AddParam(tr("База данных"), field("ExportService").toString());
        AddParam(tr("Имя пользователя, из которого сливаем данные"), field("ExportFrom").toString());
        AddParam(tr("Пароль пользователя, из которого сливаем данные"), field("ExportPass").toString());
        AddParam(tr("Каталог куда выгружается файл дампа"), field("ExportPath").toString());
    }
    else if (action == SelectActionPage::ActionExportPg)
    {
        setTitle(tr("Информация об операции экспорта (PostgreSQL)"));
        AddParam(tr("Адрес сервера"), field("PgExpServer").toString());
        AddParam(tr("Порт сервера"), field("PgExpPort").toString());
        AddParam(tr("Имя администратора"), field("PgExpUser").toString());
        AddParam(tr("Имя базы"), field("PgExpDatabase").toString());
        AddParam(tr("Имя файла дампа"), field("PgExpPath").toString());
        AddParam(tr("Путь к клиенту PostgreSQL"), wzrd->userField("PgBinPath").toString());
    }

    ui->textBrowser->setHtml(m_Text);
}

void SummaryPage::AddParam(const QString &key, const QString &value)
{
    (*m_Stream) << QString("<b>%1: </b>%2<br/>").arg(key, value);
}
