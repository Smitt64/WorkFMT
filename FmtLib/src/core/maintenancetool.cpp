#include "maintenancetool.h"
#include <QThread>
#include <QProcess>
#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QDomDocument>
#include <QLocale>
#include <QTime>

#define CHECK_UPDATES_INTERRVAL_SEC (10L * 60L)

MaintenanceTool::MaintenanceTool(QObject *parent) :
    QObject(parent),
    QRunnable()
{
    setAutoDelete(true);
    m_Program = QString("%1/WorkFmtMaintenanceTool.exe").arg(qApp->applicationDirPath());

    m_Interrupt = 0;
    m_CheckUpdate = 1;
}

MaintenanceTool::~MaintenanceTool()
{

}

void MaintenanceTool::requestInterruption()
{
    m_Interrupt.testAndSetOrdered(0, 1);
}

void MaintenanceTool::setCheckUpdateFlag(bool value)
{
    if (value)
        m_CheckUpdate.testAndSetOrdered(0, 1);
    else
        m_CheckUpdate.testAndSetOrdered(1, 0);
}

void MaintenanceTool::run()
{
    QLocale currentLocale = QLocale::system();

    QScopedPointer<QProcess> pProc(new QProcess());
    pProc->setProgram(m_Program);
    pProc->setArguments(QStringList() << "check-updates");

    QTime lastStartTime = QTime::currentTime();
    forever
    {
        QThread::sleep(5);
        QTime curTime = QTime::currentTime();
        if (lastStartTime.secsTo(curTime) >= CHECK_UPDATES_INTERRVAL_SEC && m_CheckUpdate == 1)
        {
            lastStartTime = QTime::currentTime();
            emit checkStarted();

            pProc->start();
            pProc->waitForFinished();
            QString output = QString::fromLocal8Bit(pProc->readAllStandardOutput());

            if (output.contains("There are currently no updates available"))
                emit checkFinished(false);
            else
            {
                const QString metaString = "Extracting meta information...";
                CheckDataList updatedata;

                int pos = output.indexOf(metaString);
                if (pos != -1)
                {
                    QString xmlcontent = output.mid(pos + metaString.length() + 1).simplified();
                    //qDebug() << xmlcontent;
                    QDomDocument doc;
                    if (doc.setContent(xmlcontent))
                    {
                        QDomElement docElem = doc.documentElement();
                        QDomNode n = docElem.firstChild();
                        while(!n.isNull())
                        {
                            QDomElement e = n.toElement();
                            if(!e.isNull())
                            {
                                CheckUpdateData data;
                                data.id = e.attribute("id");
                                data.name = e.attribute("name");
                                data.version = e.attribute("version");
                                data.size = e.attribute("size").toLongLong();
                                data.sizeString = currentLocale.formattedDataSize(data.size, 2, QLocale::DataSizeTraditionalFormat);
                                updatedata.push_back(data);
                            }
                            n = n.nextSibling();
                        }
                    }
                }
                emit checkFinished(true, updatedata);
            }
        }

        if (m_Interrupt.fetchAndAddAcquire(0) == 1)
            break;
    }
}


