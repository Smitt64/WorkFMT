#include "fmtcontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include <QIcon>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QFile>

FmtContentTreeItem::FmtContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    FileContentTreeItem(file, parentItem),
    m_Elem(ElementTable)
{
}

FmtContentTreeItem::~FmtContentTreeItem()
{

}

void FmtContentTreeItem::setElementType(const FmtElement &type)
{
    m_Elem = type;
}

void FmtContentTreeItem::setWidth(const int &w)
{
    m_Width = w;
}

QVariant FmtContentTreeItem::data(const int &column, const int &role) const
{
    if (column == HotfixContentModel::ColumnName)
    {
        if (role == Qt::DecorationRole)
        {
            if (m_Elem == ElementTable)
                return QIcon("://img/fmtable.png");
            else
                return QIcon("://img/index.png");
        }
        else if (role == Qt::DisplayRole)
        {
            if (m_Elem == ElementTable)
            {
                QFileInfo fi(fileName());
                return fi.baseName().toUpper();
            }
        }
    }
    else if (column == HotfixContentModel::ColumnAction)
    {
        if (role == Qt::DisplayRole)
        {
            if (m_Elem == ElementIndex)
                return QObject::tr("create index");
            else
            {
                if (svnAction() == "modified")
                    return "alter table";
                else
                    return "create table";
            }
        }
    }

    return FileContentTreeItem::data(column, role);
}

MakeResult FmtContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (checkable() && checkState() != Qt::Checked && checkState() != Qt::PartiallyChecked)
        return ResultSuccess;

    if (m_Elem == ElementTable)
        return FileContentTreeItem::make(action, msg, params);

    if (action == ActionMake)
    {
        QDir d(fileName());
        QFileInfo fi(fileName());

        QString dbtname = fi.baseName().toLower();
        dbtname = dbtname.mid(1);

        int pos = dbtname.lastIndexOf("_");
        if (pos != -1)
            dbtname = dbtname.replace(pos, 1, ".");

        d.cdUp();
        QFile file(d.absoluteFilePath("create_index.cmd"));
        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            msg = QString("Не удалось добавить таблицу <b>%1</b> в файл <b>%2</b>").arg(dbtname, file.fileName());
            return ResultWarning;
        }

        QTextStream stream(&file);
        stream.setCodec("IBM 866");
        stream.setFieldAlignment(QTextStream::AlignLeft);

        stream << "dbinit %%user%%:%%pass%%@%%dsn%% -dt:ora -log:\".\\log\\";
        stream.setFieldWidth(m_Width + 5);
        stream << dbtname + ".log\"";
        stream.setFieldWidth(0);
        stream << " -op:idx -tsn:%%indx_tsn%% -tbli:";
        stream.setFieldWidth(m_Width);
        stream << dbtname;
        stream.setFieldWidth(0);
        stream << " >> create_index.log" << Qt::endl;

        file.close();
    }

    return ResultSuccess;
}
