#include "connectioninfo.h"
#include "fmttablesmodel.h"
#include "fmtcore.h"
#include <QPainter>

ConnectionInfo::ConnectionInfo(const QString &dbalias) :
    pModel(Q_NULLPTR)
{
    m_Alias = dbalias;
    if (!dbalias.isEmpty())
        _db = QSqlDatabase::database(m_Alias);
    //pModel = new FmtTablesModel(this);
    //pModel->updateFmtList();
    m_Color = GenerateColor();
    m_Type = CON_NON;
}

ConnectionInfo::~ConnectionInfo()
{
    delete pModel;
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

QSqlDatabase ConnectionInfo::db()
{
    return _db;
}

QColor ConnectionInfo::color()
{
    return m_Color;
}

QIcon ConnectionInfo::colorIcon()
{
    if (m_Icon.isNull())
    {
        QPixmap pix(32, 32);
        QPainter p(&pix);
        p.setBrush(m_Color);
        p.setPen(m_Color);
        p.fillRect(pix.rect(), m_Color);

        QPen pen(m_Color.darker());
        pen.setWidth(2);
        p.setPen(pen);

        QRect rc = pix.rect();
        rc.setX(rc.x() + 2);
        rc.setY(rc.y() + 2);
        rc.setWidth(rc.width() - 2);
        rc.setHeight(rc.height() - 2);
        p.drawRect(rc);
        m_Icon = QIcon(pix);
    }
    return m_Icon;
}

int ConnectionInfo::type() const
{
    return m_Type;
}

bool ConnectionInfo::openSqlite(const QString &filename)
{
    QFileInfo fi(filename);
    m_Alias = QString("%1%2").arg(filename, QDateTime::currentDateTime().toString(Qt::RFC2822Date));
    _db = QSqlDatabase::addDatabase("QSQLITE", m_Alias);
    _db.setDatabaseName(filename);
    m_SchemeName = fi.fileName();
    bool hr = _db.open();
    return hr;
}
