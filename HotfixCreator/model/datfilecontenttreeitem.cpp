#include "datfilecontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include "toolsruntime.h"
#include <QIcon>
#include <QFileInfo>

DatFileContentTreeItem::DatFileContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    FileContentTreeItem(file, parentItem)
{

}

DatFileContentTreeItem::~DatFileContentTreeItem()
{

}

QVariant DatFileContentTreeItem::data(const int &column, const int &role) const
{
    if (column == HotfixContentModel::ColumnName)
    {
        if (role == Qt::DecorationRole)
            return QIcon("://img/ext_sql.png");
        else if (role == Qt::DisplayRole)
        {
            QFileInfo fi(fileName());

            if (!parentItem()->isShowRowNumber())
                return fi.baseName().toLower() + ".sql";
            else
               return QString("%1_").arg(row() + 1, 2, 10, QChar('0')) + fi.baseName().toLower() + ".sql";
        }
        else if (role == Qt::EditRole)
        {
            QFileInfo fi(fileName());
            return fi.baseName().toLower();
        }
    }
    else if (column == HotfixContentModel::ColumnAction)
    {
        if (role == Qt::DisplayRole)
            return "diff to script";
    }

    return FileContentTreeItem::data(column, role);
}

void DatFileContentTreeItem::setChunks(const QStringList &lst)
{
    m_Chunks = lst;
}

MakeResult DatFileContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    MakeResult result = ResultFail;

    //SvnInfoMap info = SvnGetRepoInfo(Path);

    /*QStringList argtmpl;
    argtmpl << "--delete"
        << "--insert"
        << "--update"
        << "--cs"
        << connectionString;

    if (IsUnicode)
        argtmpl.append("--unicodedb");*/

    return result;
}
