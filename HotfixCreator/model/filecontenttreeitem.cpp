#include "filecontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include <QFileIconProvider>
#include <QFileInfo>
#include <QDir>
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
        {
            if (!parentItem()->isShowRowNumber())
                return m_Dirs.back();
            else
                return QString("%1_").arg(row() + 1, 2, 10, QChar('0')) + m_Dirs.back();
        }
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
        else if (role == Qt::ToolTipRole)
            return QDir::toNativeSeparators(m_FullFileName);
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

const QString &FileContentTreeItem::fileName() const
{
    return m_FileName;
}

const QString &FileContentTreeItem::fullFileName() const
{
    return m_FullFileName;
}

const QString &FileContentTreeItem::svnAction() const
{
    return m_SvnAction;
}

MakeResult FileContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (action != ActionMake)
        return ResultSuccess;

    if (checkable() && checkState() != Qt::Checked && checkState() != Qt::PartiallyChecked)
        return ResultSuccess;

    QFileInfo fi(m_FileName);
    QDir d(fi.absolutePath());
    QString filename = d.absoluteFilePath(data(0, Qt::DisplayRole).toString());

    bool hr = QFile::copy(m_FullFileName, filename);

    if (hr)
        msg = QString("Файл <b>%1</b> успешно скопирован в <b>%2</b>")
                .arg(QDir::toNativeSeparators(m_FullFileName))
                .arg(QDir::toNativeSeparators(filename));
    else
        msg = QString("Не удалось скопировать файл <b>%1</b> в <b>%2</b>")
                .arg(QDir::toNativeSeparators(m_FullFileName))
                .arg(QDir::toNativeSeparators(filename));

    return hr ? ResultSuccess : ResultFail;
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
        {"sql", QIcon("://img/ext_sql.png")},
        {"pks", QIcon("://img/ext_sql.png")},
        {"pkb", QIcon("://img/ext_sql.png")},
        {"trg", QIcon("://img/ext_sql.png")},
        {"trn", QIcon("://img/ext_sql.png")},
    };

    if (icons.contains(ext))
        return icons[ext];

    return QIcon();
}
