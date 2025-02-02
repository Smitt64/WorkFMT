#include "hotfixwizard.h"
#include "seldirspage.h"
#include "structsettingspage.h"
#include "fmtcore.h"
#include <QSettings>
#include <connectioninfo.h>

HotfixWizard::HotfixWizard() :
    QWizard()
{
    setWindowTitle(tr("Мастер создания хотфиксов"));
    setWindowIcon(QIcon(":/img/DiffToScript.png"));
    setFixedSize(800, 600);

    QDir settingsDir = QDir(qApp->applicationDirPath());
    m_pSettings.reset(new QSettings(settingsDir.absoluteFilePath("hfcreator.ini"), QSettings::IniFormat));

    m_pSelDirsPage = new SelDirsPage(this);
    m_pStructPage = new StructSettingsPage(this);

    addPage(m_pSelDirsPage);
    addPage(m_pStructPage);
}

HotfixWizard::~HotfixWizard()
{

}

QSettings *HotfixWizard::settings()
{
    return m_pSettings.data();
}

void HotfixWizard::setFileListText(const QString &text)
{
    QString str = text;
    QTextStream stream(&str);

    QString sourceEdit = field("sourceEdit").toString();
    QString hotfixEdit = field("hotfixEdit").toString();

    m_FileList.clear();
    m_Contents.clear();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        QFileInfo fi(QDir(sourceEdit), line);

        if (fi.isFile())
        {
            line = line.replace('\\', '/');

            if (line.contains(sourceEdit, Qt::CaseInsensitive))
            {
                line = line.remove(sourceEdit, Qt::CaseInsensitive);
                line = line.remove(hotfixEdit, Qt::CaseInsensitive);
                line = line.mid(1);
            }

            if (line.midRef(0, 6).contains("Source", Qt::CaseInsensitive))
                m_Contents.insert(ContentFlag::ContentSource);
            else
            {
                if (line.contains(".dat", Qt::CaseInsensitive))
                    m_Contents.insert(ContentFlag::ContentDatFile);
                else if (line.contains("CreateTablesSql", Qt::CaseInsensitive))
                    m_Contents.insert(ContentFlag::ContentCreateTables);
                else if (line.contains("Mac", Qt::CaseInsensitive))
                    m_Contents.insert(ContentFlag::ContentMac);
                else if (line.contains("Fmt", Qt::CaseInsensitive))
                    m_Contents.insert(ContentFlag::ContentFmt);
                else if (line.contains("LBR", Qt::CaseInsensitive))
                    m_Contents.insert(ContentFlag::ContentLbr);
            }

            m_FileList.append(line);
        }
    }

    if (!m_FileList.isEmpty())
    {
        m_FileList.erase(std::unique(m_FileList.begin(), m_FileList.end()), m_FileList.end());
        m_FileList.sort(Qt::CaseInsensitive);
    }
}

QString HotfixWizard::fileListText() const
{
    QString text;

    QTextStream stream(&text);
    for (const QString &str : m_FileList)
    {
        if (!text.isEmpty())
            stream << endl;

        stream << str;
    }

    return text;
}

void HotfixWizard::setContentFlag(const QString &filename)
{

}

bool HotfixWizard::isSetContentFlag(const HotfixWizard::ContentFlag &flag)
{
    return m_Contents.contains(flag);
}

ConnectionInfo *HotfixWizard::connection()
{
    return pConnetion;
}
