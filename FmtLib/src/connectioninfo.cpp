#include "connectioninfo.h"
#include "fmttablesmodel.h"
#include "fmtcore.h"
#include "loggingcategories.h"
#include "toolsruntime.h"
#include <QPainter>
#include <QSqlDriver>
#include <QSqlError>
#include <QImage>
#include <QRegion>
#include <QBitmap>
#include <QUuid>

ConnectionInfo::ConnectionInfo(const QString &dbalias) :
    QObject(Q_NULLPTR),
    pModel(Q_NULLPTR),
    Index(0)
{
    m_SqlDatabaseObj = Q_NULLPTR;
    m_Alias = dbalias;
    if (!dbalias.isEmpty())
        _db = QSqlDatabase::database(m_Alias);
    pModel = addModel();
    //pModel->updateFmtList();
    m_Color = GenerateColor();
    m_Type = CON_NON;
}

ConnectionInfo::~ConnectionInfo()
{
    qDeleteAll(pModels);
    pModels.clear();
    close();
}

FmtTablesModel *ConnectionInfo::tablesModel()
{
    if (pModel == Q_NULLPTR)
        pModel = new FmtTablesModel(this);
    return pModel;
}

void ConnectionInfo::updateFmtList()
{
    //pModel->updateFmtList();
    tablesModel()->updateFmtList();
}

bool ConnectionInfo::isOpen() const
{
    return _db.isOpen();
}

bool ConnectionInfo::isUnicode() const
{
    return _db.connectOptions().contains("RSD_UNICODE");
}

QSqlDatabase &ConnectionInfo::db()
{
    return _db;
}

QSqlDriver *ConnectionInfo::driver()
{
    return _db.driver();
}

QString ConnectionInfo::color()
{
    return m_Color.name();
}

QString ConnectionInfo::typeName() const
{
    QString name = "Unknown";

    switch(m_Type)
    {
    case CON_ORA:
        name = "Oracle";
        break;
    case CON_POSTGRESQL:
        name = "PostgreSql";
        break;
    case CON_SQLITE:
        name = "Sqlite";
        break;
    case CON_NON:
        break;
    }

    return name;
}

QIcon ConnectionInfo::colorIcon(const QSize &size)
{
    const int ratio = 2;
    if (m_Icon.isNull())
    {
        qCInfo(logCore()) << QString("Begin create icon for connection %1. Db type: %2")
                             .arg(m_SchemeName)
                             .arg(typeName());

        QPixmap pix(size.width() * ratio, size.height() * ratio);
        pix.fill(Qt::transparent);

        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);

        int borderWidth = (32.0 / size.width()) * ratio;
        int radius = size.width() / 4;
        QPen pen(m_Color.darker());
        pen.setWidth(borderWidth);
        p.setPen(pen);
        p.setBrush(m_Color);

        QRect rc = pix.rect();
        rc.setX(rc.x() + borderWidth);
        rc.setY(rc.y() + borderWidth);
        rc.setWidth(rc.width() - borderWidth);
        rc.setHeight(rc.height() - borderWidth);

        p.drawRoundedRect(rc, radius, radius);
        //p.drawRect(rc);

        auto LoadImage = [=](const QString &filename) -> QImage
        {
            QImage img;
            qCInfo(logCore()) << QString("Try load image mask: [%1]")
                                 .arg(filename);

            if (!img.load(filename))
                qCInfo(logCore()) << QString("Load failed");
            else
                qCInfo(logCore()) << QString("Load successed");

            return img;
        };

        QImage mskimg;
        if (m_Type == CON_SQLITE)
            mskimg = LoadImage(":/img/dblogomask/sqlite.svg");
        else if (m_Type == CON_ORA)
            mskimg = LoadImage(":/img/dblogomask/oracle.svg");
        else if (m_Type == CON_POSTGRESQL)
            mskimg = LoadImage(":/img/dblogomask/postgresql.svg");

        if (!mskimg.isNull())
        {
            QPixmap mask(mskimg.size());
            mask.fill(pen.color());
            mask.setMask(QPixmap::fromImage(mskimg.createAlphaMask()));
            p.save();
            p.setPen(pen);
            p.drawPixmap(rc, mask);
            p.restore();
        }

        m_Icon = QIcon(pix);

        qCInfo(logCore()) << QString("End create icon for connection %1")
                             .arg(m_SchemeName);
    }

    return m_Icon;
}

int ConnectionInfo::type() const
{
    return m_Type;
}

bool ConnectionInfo::isOracle()
{
    QSqlQuery query(_db);
    query.prepare("SELECT * FROM v$version");

    if (ExecuteQuery(&query) || !query.next())
        return false;

    QString oracleVersion = query.value(0).toString();
    return oracleVersion.startsWith("Oracle Database", Qt::CaseInsensitive);
}

bool ConnectionInfo::isPostgre()
{
    QSqlQuery query(_db);
    query.prepare("SELECT version() from dual");

    if (ExecuteQuery(&query) || !query.next())
        return false;

    QString oracleVersion = query.value(0).toString();
    return oracleVersion.startsWith("PostgreSQL", Qt::CaseInsensitive);
}

bool ConnectionInfo::isSqlite()
{
    QSqlQuery query(_db);
    query.prepare("SELECT sqlite_version()");

    if (ExecuteQuery(&query) || !query.next())
        return false;

    return false;
}

void ConnectionInfo::close()
{
    if (_db.isOpen())
        _db.close();
}

bool ConnectionInfo::open(const QString &drv, const QString &user, const QString &password, const QString &dsn, const QString &options, QString *error)
{
    bool hr = false;
    m_Alias = QUuid::createUuid().toString();
        //QString("%1@%2#%3").arg(user, dsn, QDateTime::currentDateTime().toString(Qt::RFC2822Date));
    m_SchemeName = QString("%1@%2").arg(user, dsn);
    m_DSN = dsn;

    _db = QSqlDatabase::addDatabase(drv, m_Alias);
    _db.setUserName(user);
    _db.setPassword(password);
    _db.setDatabaseName(dsn);
    _db.setConnectOptions(options);

    hr = _db.open();

    if (hr)
    {
        if (m_SqlDatabaseObj)
            delete m_SqlDatabaseObj;

        toolMakeSqlDatabaseObj(_db, &m_SqlDatabaseObj);

        m_User = user;
        m_Password = password;

        qCInfo(logCore()) << QString("Try determine Db type...");

        if (isOracle())
            m_Type = CON_ORA;
        else if (isPostgre())
             m_Type = CON_POSTGRESQL;
        else if (isSqlite())
            m_Type = CON_SQLITE;

        QString DbType = typeName();
        qCInfo(logCore()) << QString("Db type: ") << DbType;
        qCInfo(logCore()) << QString("Connected to %1@%2").arg(user, dsn);
    }
    else
    {
        QString err = _db.lastError().text();
        qCInfo(logCore()) << QString("Can't connect to %1@%2").arg(user, dsn);

        if (err.contains("Driver not loaded"))
            err = tr("Driver not loaded");

        qCInfo(logCore()) << err;

        if (error)
            *error = err;
    }

    return hr;
}

bool ConnectionInfo::openSqlite(const QString &filename)
{
    QFileInfo fi(filename);
    m_Alias = QUuid::createUuid().toString();
    _db = QSqlDatabase::addDatabase("QSQLITE", m_Alias);
    _db.setDatabaseName(filename);
    m_SchemeName = fi.fileName();
    bool hr = _db.open();

    if (hr)
    {
        m_Type = CON_SQLITE;
        m_DSN = fi.fileName();
        qCInfo(logCore()) << QString("Connected to %1").arg(fi.baseName());
    }
    else
    {
        qCInfo(logCore()) << QString("Can't connect to %1").arg(fi.baseName());
        qCInfo(logCore()) << _db.driver()->lastError().text().toLocal8Bit();
    }

    return hr;
}

bool ConnectionInfo::openSqlteMemory()
{
    m_SchemeName = ":memory:";
    _db = QSqlDatabase::addDatabase("QSQLITE", m_Alias);
    _db.setDatabaseName(m_SchemeName);
    bool hr = _db.open();
    return hr;
}

FmtTablesModel *ConnectionInfo::addModel()
{
    FmtTablesModel *model = new FmtTablesModel(this);
    pModels.append(model);
    Index ++;
    m_Index.append(Index);
    return model;
}

int ConnectionInfo::getFilterIndex(const int &index)
{
    return m_Index[index];
}

int ConnectionInfo::lastIndex() const
{
    return Index;
}

void ConnectionInfo::deleteModel(const int &index)
{
    FmtTablesModel *model = pModels.takeAt(index);
    delete model;
}

FmtTablesModel *ConnectionInfo::getModel(const int &index)
{
    if (index < 0 || index >= pModels.size())
        return Q_NULLPTR;
    return pModels[index];
}

int ConnectionInfo::modelCount() const
{
    return pModels.size();
}

ConnectionInfo::operator int() const
{
    return reinterpret_cast<qintptr>(this);
}

bool ConnectionInfo::hasFeature(ConnectionInfo::ConnectionFeature feature) const
{
    bool result = true;
    if (m_Type == CON_POSTGRESQL)
    {
        if (feature == ConnectionInfo::CanLoadUnloadDbf)
            result = false;
    }
    /*if (m_Type == CON_ORA)
        return true;

    return false;*/
    return result;
}
