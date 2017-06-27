#include "connectioninfo.h"
#include "fmttablesmodel.h"
#include "fmtcore.h"
#include <QPainter>

ConnectionInfo::ConnectionInfo(const QString &dbalias)
{
    m_Alias = dbalias;
    _db = QSqlDatabase::database(m_Alias);
    pModel = new FmtTablesModel(this);
    //pModel->updateFmtList();
    m_Color = GenerateColor();
}

FmtTablesModel *ConnectionInfo::tablesModel()
{
    return pModel;
}

void ConnectionInfo::updateFmtList()
{
    pModel->updateFmtList();
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
