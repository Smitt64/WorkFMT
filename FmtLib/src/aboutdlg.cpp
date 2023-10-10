#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include "fmtcore.h"
#include <Windows.h>
#include <QDomDocument>
#include <QStandardItemModel>
#include <QFile>
#include <QScrollBar>
#include <QTextBrowser>
#include <QDomDocument>
#include <QTemporaryFile>
#include <QWebEngineView>
#include "loggingcategories.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);

    currentLocale = QLocale::system();
    pComponentsModel = new QStandardItemModel(this);
    pComponentsModel->setHorizontalHeaderLabels(QStringList()
                                                << tr("Компонент")
                                                << tr("ID")
                                                << tr("Версия")
                                                << tr("Размер")
                                                << tr("Установлен/Обновлен"));

    QHBoxLayout *pLayout = new QHBoxLayout(ui->tab);
    pLayout->setMargin(0);
    qInfo(logCore()) << "Creating QWebEngineView";
    m_WebView = new QWebEngineView(ui->tab);
    qInfo(logCore()) << "QWebEngineView creation finish";

    ui->tab->setLayout(pLayout);
    ui->tab->layout()->addWidget(m_WebView);
    ui->labelBased->setText(tr("<b>Версия Qt: </b>%1").arg(QT_VERSION_STR));

    ui->tabWidget->setCurrentIndex(0);
    ui->componentsView->setModel(pComponentsModel);
    ui->componentsView->header()->resizeSection(0, 200);
    ui->componentsView->header()->resizeSection(1, 250);

    QString appTitle = ui->applicationTitle->text();
#ifdef _DEBUG
    ui->applicationTitle->setText(appTitle.arg(" (Debug)"));
#else
    ui->applicationTitle->setText(appTitle.arg(""));
#endif

    PutVersion();
    ReadComponents();
    ReadVersionsTitles();

    RenderHtmlForProject("Work Fmt");

    connect(m_WebView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
}

AboutDlg::~AboutDlg()
{
    delete ui;
}

void AboutDlg::PutVersion()
{
    ui->labelVersion->setText(tr("<b>Версия: </b>%1").arg(GetVersionNumberString()));
}

void AboutDlg::ReadComponentPackage(QDomElement *Elem)
{
    QMap<QString,QString> Package;

    QDomNode n = Elem->firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();
        if(!e.isNull())
            Package.insert(e.tagName(), e.text());

        n = n.nextSibling();
    }

    QStandardItem *TitleItem = new QStandardItem();
    TitleItem->setText(Package["Title"]);

    QStandardItem *NameItem = new QStandardItem();
    NameItem->setText(Package["Name"]);

    QStandardItem *VersionItem = new QStandardItem();
    VersionItem->setText(Package["Version"]);

    qint64 Size = Package["Size"].toLongLong();
    QStandardItem *SizeItem = new QStandardItem();
    SizeItem->setText(currentLocale.formattedDataSize(Size, 2, QLocale::DataSizeTraditionalFormat));

    QDate InstallDate = QDate::fromString(Package["InstallDate"], Qt::ISODate);
    QString datesString = InstallDate.toString(Qt::SystemLocaleShortDate);

    if (!Package["LastUpdateDate"].isEmpty())
    {
        QDate LastUpdateDate = QDate::fromString(Package["LastUpdateDate"], Qt::ISODate);
        datesString += QString("/%1")
                .arg(LastUpdateDate.toString(Qt::SystemLocaleShortDate));
    }

    QStandardItem *DatesItem = new QStandardItem();
    DatesItem->setText(datesString);

    pComponentsModel->appendRow(QList<QStandardItem*>()
                                << TitleItem
                                << NameItem
                                << VersionItem
                                << SizeItem
                                << DatesItem);
}

void AboutDlg::ReadComponents()
{
    QString filename = getFullFileNameFromDir("components.xml");
    if (QFile::exists(filename))
    {
        QDomDocument doc("components");
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
            ui->tabWidget->setTabVisible(1, false);
#else
            ui->tabWidget->removeTab(1);
#endif
            return;
        }

        if (!doc.setContent(&file))
        {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
            ui->tabWidget->setTabVisible(1, false);
#else
            ui->tabWidget->removeTab(1);
#endif
            file.close();
            return;
        }
        file.close();

        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();
        while(!n.isNull())
        {
            QDomElement e = n.toElement();
            if(!e.isNull())
            {
                if (e.tagName() == "Package")
                    ReadComponentPackage(&e);
            }
            n = n.nextSibling();
        }
    }
    else
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
            ui->tabWidget->setTabVisible(1, false);
#else
            ui->tabWidget->removeTab(1);
#endif
    }
}

void AboutDlg::ReadVersionsTitles()
{
    QDir current = QDir::current();
    current.cd("changelog");

    qInfo(logCore()) << "Scan changelog folder...";
    for (auto fi : current.entryInfoList(QStringList() << "com.rs.fmt*.xml"))
    {
        QDomDocument doc(fi.absoluteFilePath());
        QString body = ReadTextFileContent(fi.absoluteFilePath());
        doc.setContent(body);

        QDomElement docElem = doc.documentElement();
        m_Projects.insert(docElem.attribute("project"), fi);

        qInfo(logCore()) << QString("Found project [%1] version information").arg(docElem.attribute("project"));
    }
}

void AboutDlg::RenderHtmlForProject(const QString &project)
{
    if (project.isEmpty() || !m_Projects.contains(project))
        return;

    qInfo(logCore()) << "Begin create chanhelog html for project" << project;
    QString projects;
    QString body_template = ReadTextFileContent(":/about/index_template.html");
    QString version_template = ReadTextFileContent(":/about/index_template.html");

    QString add_template = ReadTextFileContent(":/about/add_template.html");
    QString fix_template = ReadTextFileContent(":/about/fixed_template.html");

    QMapIterator<QString,QFileInfo> iter(m_Projects);
    while(iter.hasNext())
    {
        iter.next();

        if (project == iter.key())
            projects += QString("<li><a class=\"active\" href=\"#%1\">%1</a></li>").arg(iter.key());
        else
            projects += QString("<li><a href=\"#%1\">%1</a></li>").arg(iter.key());
    }

    body_template = body_template.replace("{%projects%}", projects);
    body_template = body_template.replace("{%Project%}", project);

    QDomDocument doc;
    QString body = ReadTextFileContent(m_Projects[project].absoluteFilePath());
    doc.setContent(body);

    QString version;
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();
        version += QString("<h2 class=\"hs-docs-heading\"><code>v%1</code></h2>").arg(e.attribute("ver"));

        QDomNode fix = e.firstChild();
        while(!fix.isNull())
        {
            QDomElement efix = fix.toElement();

            if (efix.tagName() == "fix")
                version += fix_template.arg(efix.text());
            else if (efix.tagName() == "add")
                version += add_template.arg(efix.text());

            fix = fix.nextSibling();
        }

        n = n.nextSibling();
    }

    QString content_divider = QString("<div class=\"content-divider\">%1</div>").arg(version);
    body_template = body_template.replace("{%content_divider%}", content_divider);

    QDir tmp = QDir::temp();
    QFile tempfile(tmp.absoluteFilePath(QString("%1.html").arg(project)));
    if (tempfile.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&tempfile);
        stream.setCodec("Utf-8");
        stream << body_template.toUtf8();
        tempfile.close();

        m_WebView->load(tempfile.fileName());
    }
}

void AboutDlg::urlChanged(const QUrl &url)
{
    QString fragment = url.fragment();

    if (fragment.isEmpty())
        return;

    m_WebView->blockSignals(true);
    RenderHtmlForProject(fragment);
    m_WebView->blockSignals(false);
}
