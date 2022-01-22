#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include "fmtcore.h"
#include <Windows.h>
#include <QDomDocument>
#include <QStandardItemModel>
#include <QFile>

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

    ui->labelBased->setText(tr("Основан на Qt %1").arg(QT_VERSION_STR));
    ui->textBrowser->setSource(QUrl("qrc:/about.html"));
    ui->tabWidget->setCurrentIndex(0);
    ui->componentsView->setModel(pComponentsModel);
    ui->componentsView->header()->resizeSection(0, 150);
    ui->componentsView->header()->resizeSection(1, 150);
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
    QDir dir = qApp->applicationDirPath();
    QString filename = dir.absoluteFilePath("components.xml");
    if (QFile::exists(filename))
    {
        QDomDocument doc("components");
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
            ui->tabWidget->setTabVisible(1, false);
            return;
        }

        if (!doc.setContent(&file))
        {
            ui->tabWidget->setTabVisible(1, false);
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
        ui->tabWidget->setTabVisible(1, false);
}
