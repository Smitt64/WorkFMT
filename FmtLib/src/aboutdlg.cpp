#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include "fmtcore.h"
#include <Windows.h>
#include <QDomDocument>
#include <QStandardItemModel>
#include <QFile>
#include <QScrollBar>
#include <QTextBrowser>

class AboutTextBrowser : public QTextBrowser
{
public:
    AboutTextBrowser(QWidget *parent = nullptr) :
        QTextBrowser(parent)
    {

    }

    virtual QVariant loadResource(int type, const QUrl &name) Q_DECL_OVERRIDE
    {
        if (type == QTextDocument::ImageResource)
        {
            QPixmap pix(name.toString());
            return pix;
        }

        return QTextBrowser::loadResource(type, name);
    }
};

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
    m_pTextBrowser = new AboutTextBrowser(this);
    m_pTextBrowser->setSource(QUrl("qrc:/about.html"));
    //ui->tabWidget->insertTab(0, m_pTextBrowser, tr("История изменений"));

    ui->tab->setLayout(pLayout);
    ui->tab->layout()->addWidget(m_pTextBrowser);

    ui->labelBased->setText(tr("Основан на Qt %1").arg(QT_VERSION_STR));
    //ui->textBrowser->setSource(QUrl("qrc:/about.html"));
    ui->tabWidget->setCurrentIndex(0);
    ui->componentsView->setModel(pComponentsModel);
    ui->componentsView->header()->resizeSection(0, 150);
    ui->componentsView->header()->resizeSection(1, 150);

    QScrollBar *sb = m_pTextBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());

    QString appTitle = ui->applicationTitle->text();
#ifdef _DEBUG
    ui->applicationTitle->setText(appTitle.arg(" (Debug)"));
#else
    ui->applicationTitle->setText(appTitle.arg(""));
#endif

    PutVersion();
    ReadComponents();
}

AboutDlg::~AboutDlg()
{
    delete ui;
}

void AboutDlg::PutVersion()
{
    ui->labelVersion->setText(tr("Версия: %1").arg(GetVersionNumberString()));
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
