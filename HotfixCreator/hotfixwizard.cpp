#include "hotfixwizard.h"
#include "seldirspage.h"
#include "filelistpage.h"
#include "filetypespage.h"
#include <datfile.h>
#include "fmtcore.h"
#include <connectioninfo.h>

HotfixWizard::HotfixWizard()
{
    FmtInit();
    setWindowTitle(tr("Мастер создания хотфиксов"));

    pConnetion = new ConnectionInfo();
    pConnetion->openSqlteMemory();

    m_pSelDirsPage = new SelDirsPage(this);
    m_FileListPage = new FileListPage(this);
    m_FileTypesPage = new FileTypesPage(this);

    addPage(m_pSelDirsPage);
    addPage(m_FileListPage);
    addPage(m_FileTypesPage);

    DatFile file;
    file.read("D:/svn/UranRSBankV6/Main/Distrib/DBFile/Data/DPDNKIND_DBT.dat");
}

HotfixWizard::~HotfixWizard()
{

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

            if (line.mid(0, 6).contains("Source", Qt::CaseInsensitive))
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
