#ifndef CONTENTTREEITEM_H
#define CONTENTTREEITEM_H

#include <QVariant>
#include <vector>
#include <memory>

enum MakeAction
{
    ActionPrepare = 1,
    ActionMake,
    ActionEnd
};

enum MakeResult
{
    ResultWarning = -1,
    ResultSuccess = 0,
    ResultSuccessSkipChild,
    ResultFail,
};

class QFileIconProvider;
class HotfixContentModel;
class ContentTreeItem : public QObject
{
    Q_OBJECT
    friend class HotfixContentModel;
public:
    ContentTreeItem(ContentTreeItem *parentItem = nullptr);
    ~ContentTreeItem();

    ContentTreeItem *appendChild(std::unique_ptr<ContentTreeItem> &&child);

    ContentTreeItem *findItemByData(const QVariant& value, int column, int role);
    template<class T>T *findItemByData(const QVariant& value, int column, int role)
    {
        ContentTreeItem *found = findItemByData(value, column, role);

        if (!found)
            return nullptr;

        return dynamic_cast<T*>(found);
    }

    ContentTreeItem *child(int row);
    ContentTreeItem *child(int row) const;

    template<class T>T *child(int row)
    {
        ContentTreeItem *item = child(row);

        if (!item)
            return nullptr;

        return dynamic_cast<T*>(item);
    }

    template<class T>T *child(int row) const
    {
        ContentTreeItem *item = child(row);

        if (!item)
            return nullptr;

        return dynamic_cast<T*>(item);
    }

    int childCount() const;

    virtual QVariant data(const int &column, const int &role) const;
    virtual bool setData(const QVariant &value, const int &column = 0, int role = Qt::EditRole);
    virtual MakeResult make(const MakeAction &action, QString &msg) const;

    int row() const;
    ContentTreeItem *parentItem();
    ContentTreeItem *parentItem() const;

    QFileIconProvider *iconProvider();
    QFileIconProvider *iconProvider() const;

    void setCheckable(const bool &checkable);
    void setTristate(const bool &state);
    void setEnable(const bool &state);
    void setShowRowNumber(const bool &state);

    void setOrder(const qint16 &v);
    const qint16 &order() const;

    const bool &checkable() const;
    const bool &tristate() const;
    const bool &isEnable() const;
    const bool &isShowRowNumber() const;

    Qt::CheckState checkState();

signals:
    void itemChanged(const int &column, const QVector<int> &roles);

protected:
    void setModel(HotfixContentModel *model);
    HotfixContentModel *model();

    bool m_fSkipSetDataChild;

private:
    HotfixContentModel *m_pModel;
    ContentTreeItem *m_parentItem;
    std::vector<std::unique_ptr<ContentTreeItem>> m_childItems;

    Qt::CheckState m_Check;
    bool m_Chackable, m_Tristate, m_fEnabled, m_fShowRowNumber;
    qint16 m_Order;
};

#endif // CONTENTTREEITEM_H
