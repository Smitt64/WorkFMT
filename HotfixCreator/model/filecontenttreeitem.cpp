#include "filecontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include <QFileIconProvider>
#include <QFileInfo>
#include <QDebug>

FileContentTreeItem::FileContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    ContentTreeItem(parentItem)
{
    m_FileName = file;
    m_Dirs = m_FileName.split('\\');
}

FileContentTreeItem::~FileContentTreeItem()
{

}

QVariant FileContentTreeItem::data(const int &column, const int &role) const
{
    if (column == HotfixContentModel::ColumnName)
    {
        if (role == Qt::DisplayRole)
            return m_Dirs.back();
        else if (role == Qt::DecorationRole)
        {
            if (!m_FullFileName.isEmpty())
            {
                QFileInfo fi(m_FullFileName);
                QIcon ico = getIconForExtension(fi.suffix());

                if (!ico.isNull())
                    return ico;

                return iconProvider()->icon(fi);
            }
            else
                return iconProvider()->icon(QFileIconProvider::File);
        }
    }
    else if (column == HotfixContentModel::ColumnAction)
    {
        if (role == Qt::DisplayRole)
            return m_SvnAction;
    }

    return ContentTreeItem::data(column, role);
}

void FileContentTreeItem::setSvnAction(const QString &action)
{
    m_SvnAction = action;
}

void FileContentTreeItem::setFullFileName(const QString &action)
{
    m_FullFileName = action;
}

QIcon FileContentTreeItem::getIconForExtension(const QString &ext)
{
    static QMap<QString, QIcon> icons =
    {
        {"c", QIcon("://img/ext_c.png")},
        {"cpp", QIcon("://img/ext_cpp.png")},
        {"h", QIcon("://img/ext_h.png")},
        {"hpp", QIcon("://img/ext_hpp.png")},
        {"vcxproj", QIcon("://img/ext_vcproj.png")},
        {"txt", QIcon("://img/ext_txt.png")},
        {"hss", QIcon("://img/ext_txt.png")},
        {"s362", QIcon("://img/ext_txt.png")},
        {"def", QIcon("://img/ext_def.png")},
        {"xml", QIcon("://img/ext_xml.png")},
        {"mac", QIcon("://img/ext_mac.png")},
        {"xls", QIcon("://img/ext_xls.png")},
        {"xlsx", QIcon("://img/ext_xls.png")},
        {"doc", QIcon("://img/ext_doc.png")},
        {"docx", QIcon("://img/ext_doc.png")},
        {"dot", QIcon("://img/ext_dot.png")},
    };

    if (icons.contains(ext))
        return icons[ext];

    return QIcon();
}
