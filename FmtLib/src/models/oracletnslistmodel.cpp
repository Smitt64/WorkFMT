#include "oracletnslistmodel.h"
#include "loggingcategories.h"
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>

OracleTnsListModel::OracleTnsListModel(QObject *parent)
    :QStandardItemModel(parent)
{
    setColumnCount(6);

    setHorizontalHeaderLabels(QStringList()
                              << tr("Name")
                              << tr("SERVICE_NAME")
                              << tr("PROTOCOL")
                              << tr("HOST")
                              << tr("PORT")
                              << tr("File"));

    QStringList OracleHomes = getOracleHomes();

    foreach (QString key, OracleHomes)
    {
        QString name = getTNSNAMESORAFilePath(key);
        if (!name.isEmpty())
        {
            loadTNSNames(name);
        }
    }
}

OracleTnsListModel::~OracleTnsListModel()
{

}

QStringList OracleTnsListModel::getOracleHomes()
{
    QStringList oracleHomes;
    QSettings softs("HKEY_LOCAL_MACHINE\\Software\\ORACLE", QSettings::NativeFormat);
    QStringList subKeys = softs.allKeys();

    foreach (QString key, subKeys)
    {
        if (key.startsWith("KEY_"))
        {
            oracleHomes.append(key);
        }
    }
    return oracleHomes;
}

QString OracleTnsListModel::getOracleHomePath(QString OracleHomeRegistryKey)
{
    QSettings softs(QString("HKEY_LOCAL_MACHINE\\Software\\ORACLE"), QSettings::NativeFormat);

    return softs.value(OracleHomeRegistryKey).toString();
}

QString OracleTnsListModel::getTNSNAMESORAFilePath(QString OracleHomeRegistryKey)
{
    QString oracleHomePath = getOracleHomePath(OracleHomeRegistryKey);
    QString tnsNamesOraFilePath = "";
    QSettings softs(QString("HKEY_LOCAL_MACHINE\\Software\\ORACLE"), QSettings::NativeFormat);

    if (!oracleHomePath.isEmpty())
    {
        tnsNamesOraFilePath = oracleHomePath + "\\NETWORK\\ADMIN\\TNSNAMES.ORA";
        if (!QFileInfo::exists(tnsNamesOraFilePath))
        {
            tnsNamesOraFilePath = oracleHomePath + "\\NET80\\ADMIN\\TNSNAMES.ORA";
        }
    }

    if (!QFileInfo::exists(tnsNamesOraFilePath))
    {
        tnsNamesOraFilePath = "";
    }
    return tnsNamesOraFilePath;
}

void OracleTnsListModel::loadTNSNames(const QString &strTNSNAMESORAFilePath)
{
    QString RegExPattern = "[\\n]*[\\s]*[^\\(][a-zA-Z0-9_.-]+[\\s]*=[\\s]*\\(";

    QFile f(strTNSNAMESORAFilePath);

    if (f.open(QIODevice::ReadOnly))
    {
        int pos = 0;
        QString in = f.readAll();
        QRegExp rx(RegExPattern);
        while ((pos = rx.indexIn(in, pos)) != -1)
        {
            int row = rowCount();
            QString name = rx.cap(0);
            QStandardItem *item = new QStandardItem(clearString(name));//name.remove("\n").remove("=").remove("\r").remove("(")
            QStandardItem *fileitem = new QStandardItem(QFileInfo(strTNSNAMESORAFilePath).fileName());
            setItem(row, mtns_Name, item);
            setItem(row, mtns_TnsNamesFilePath, fileitem);

            QRegExp rx2("[^\\(][a-zA-Z0-9_.-]+[\\s]*=[\\s]*[a-zA-Z0-9_.-]*\\)");
            int pos2 = 0;
            QString subStr = in.mid(pos, findClosePose(in, pos) - pos);
            while ((pos2 = rx2.indexIn(subStr, pos2, QRegExp::CaretAtZero)) != -1)
            {
                QString cap = rx2.cap(0);

                if (cap.contains("PROTOCOL", Qt::CaseInsensitive))
                {
                    setItem(row, mtns_Protocol, new QStandardItem(clearString(cap).remove("PROTOCOL")));
                }

                if (cap.contains("SERVICE_NAME", Qt::CaseInsensitive))
                {
                    setItem(row, mtns_ServiceName, new QStandardItem(clearString(cap).remove("SERVICE_NAME")));
                }

                if (cap.contains("HOST", Qt::CaseInsensitive))
                {
                    setItem(row, mtns_Host, new QStandardItem(clearString(cap).remove("HOST")));
                }

                if (cap.contains("PORT", Qt::CaseInsensitive))
                {
                    setItem(row, mtns_Port, new QStandardItem(clearString(cap).remove("PORT")));
                }
                pos2 += rx2.matchedLength();
                //pos += rx2.matchedLength() + 1;
            }

            pos += subStr.length();
        }
        qCInfo(logCore()) << "TNSNAMES.ORA loaded: " << strTNSNAMESORAFilePath;
    }
}

QString OracleTnsListModel::clearString(QString str)
{
    QString s = str.remove("\n").remove("\r").remove(")").remove("(").remove("=").remove(" ").trimmed().simplified();
    return s;
}

quint32 OracleTnsListModel::getIndexByDsn(const QString &str)
{
    QModelIndexList list = match(index(0, 0), Qt::DisplayRole, str);

    if (list.size())
        return list.at(0).row();

    return -1;
}

int OracleTnsListModel::findClosePose(const QString &str, int pos)
{
    int opened = 0;
    int mpos = pos + 1;

    bool next = true;
    while(next)
    {
        if (mpos >= str.length())
        {
            next = false;
        }
        else
        {
            if (str[mpos] == '(')
            {
                opened ++;
            }
            else if (str[mpos] == ')')
            {
                opened --;

                if (opened == 0)
                {
                    next = false;
                }
            }

            mpos ++;
        }
    }

    return mpos;
}
