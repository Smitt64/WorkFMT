#include "connectioninfo.h"
#include "fmttablesmodel.h"
#include "fmtcore.h"
#include "loggingcategories.h"
#include <QPainter>
#include <QSqlDriver>
#include <QSqlError>
#include <QImage>
#include <QRegion>
#include <QBitmap>

ConnectionInfo::ConnectionInfo(const QString &dbalias) :
    QObject(Q_NULLPTR),
    pModel(Q_NULLPTR),
    Index(0)
{
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

QSqlDatabase &ConnectionInfo::db()
{
    return _db;
}

QSqlDriver *ConnectionInfo::driver()
{
    return _db.driver();
}

QColor ConnectionInfo::color()
{
    return m_Color;
}

QIcon ConnectionInfo::colorIcon(const QSize &size)
{
    const int ratio = 2;
    if (m_Icon.isNull())
    {
        QPixmap pix(size.width() * ratio, size.height() * ratio);
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(m_Color);
        p.setPen(m_Color);
        p.fillRect(pix.rect(), m_Color);

        int borderWidth = (1.0 / 4.0) * ratio * 2;
        QPen pen(m_Color.darker());
        pen.setWidth(borderWidth);
        p.setPen(pen);

        QRect rc = pix.rect();
        rc.setX(rc.x() + borderWidth);
        rc.setY(rc.y() + borderWidth);
        rc.setWidth(rc.width() - borderWidth);
        rc.setHeight(rc.height() - borderWidth);
        p.drawRect(rc);

        QImage mskimg;
        if (m_Type == CON_SQLITE)
            mskimg = QImage(":/img/dblogomask/sqlite.svg");
        else if (m_Type == CON_ORA)
            mskimg = QImage(":/img/dblogomask/oracle.svg");
        else if (m_Type == CON_POSTGRESQL)
            mskimg = QImage(":/img/dblogomask/postgresql.svg");

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

bool ConnectionInfo::isSqlite()
{
    QSqlQuery query(_db);
    query.prepare("SELECT sqlite_version()");

    if (ExecuteQuery(&query) || !query.next())
        return false;

    return false;
}

bool ConnectionInfo::open(const QString &drv, const QString &user, const QString &password, const QString &dsn, QString *error)
{
    bool hr = false;
    m_Alias = QString("%1@%2#%3").arg(user, dsn, QDateTime::currentDateTime().toString(Qt::RFC2822Date));
    m_SchemeName = QString("%1@%2").arg(user, dsn);
    m_DSN = dsn;

    _db = QSqlDatabase::addDatabase(drv, m_Alias);
    _db.setUserName(user);
    _db.setPassword(password);
    _db.setDatabaseName(dsn);
    hr = _db.open();

    if (hr)
    {
        m_User = user;
        m_Password = password;

        if (isOracle())
            m_Type = CON_ORA;
        else if (isSqlite())
            m_Type = CON_SQLITE;

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
    m_Alias = QString("%1%2").arg(filename, QDateTime::currentDateTime().toString(Qt::RFC2822Date));
    _db = QSqlDatabase::addDatabase("QSQLITE", m_Alias);
    _db.setDatabaseName(filename);
    m_SchemeName = fi.fileName();
    bool hr = _db.open();

    if (hr)
    {
        m_Type = CON_SQLITE;
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
