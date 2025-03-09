#ifndef HOTFIXCONTENTMODEL_H
#define HOTFIXCONTENTMODEL_H

#include <QAbstractItemModel>
#include <memory>
#include <QSet>

enum
{
    UsrMakeBegin = 0,
    UsrMakeEnd
};

class ContentTreeItem;
class FolderContentTreeItem;
class ProjectLoader;
class ErrorsModel;
class SvnStatusModel;

typedef std::function<void(const qint16 &action, ContentTreeItem *item)> UsrMakeHandle;
class HotfixContentModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(HotfixContentModel)

    enum
    {
        ColumnName = 0,
        ColumnAction
    };
    HotfixContentModel(QObject *parent = nullptr);
    virtual ~HotfixContentModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = {}) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = {}) const Q_DECL_OVERRIDE;

    void sort(int column, Qt::SortOrder order) Q_DECL_OVERRIDE;

    void makeModel(const QString &source, const QString &dst, const QString &hfname, const bool &NewFormat);
    void makeHotFix(ErrorsModel *logs, const QMap<QString, QVariant> &params, UsrMakeHandle usr);
    QStringList projects() const;
    ProjectLoader *projectLoader();

    int totalChildCount() const;

public slots:
    void contentItemChanged(const int &column, const QVector<int> &roles);

private:
    typedef QMap<QString, FolderContentTreeItem*> FolderParents;
    typedef std::function<ContentTreeItem*(FolderContentTreeItem *parent, const QString &name, const QString &fullname)> PathMaker;

    ContentTreeItem *stdMakePathFunc(FolderContentTreeItem *parent, const QString &name, const QString &fullname);

    bool getTFStructValue(const QString &fileName, bool &tfStruct);
    ContentTreeItem *makePath(FolderParents &Parents, const QString &path, Qt::HANDLE element, FolderContentTreeItem *parent);
    ContentTreeItem *makePathEx(FolderParents &Parents, const QString &path, Qt::HANDLE elem, FolderContentTreeItem *parent, PathMaker maker);
    void makeAddFiles(FolderParents &Parents, const QString &path, Qt::HANDLE elem, FolderContentTreeItem *AddFiles);

    void makePgRoutes(SvnStatusModel *svn, FolderParents Parents, FolderContentTreeItem *AddFiles, const QString &hfname);

    bool isFile(const QString &name);
    bool isExcludeElement(const QString &name);
    bool isExcludePath(const QString &name);

    QSet<QString> m_Projects;
    std::unique_ptr<ContentTreeItem> rootItem;
    QScopedPointer<ProjectLoader> m_pLoader;
    QStringList m_DatFiles;

    bool m_NewFormat;
};

#endif // HOTFIXCONTENTMODEL_H
