#ifndef SETTINGSTABLEMODEL_H
#define SETTINGSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QSharedPointer>
#include "rsl/reginfoobj.h"

class SettingsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Column {
        FullNameColumn = 0,
        TypeNameColumn,
        ValueColumn,
        ColumnCount
    };
    Q_ENUM(Column)

    explicit SettingsTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void addSetting(const QString &fullName, qint16 type = RegInfoObj::INTEGER, const QVariant &value = QVariant());
    void addSetting(QSharedPointer<RegInfoObj> obj);
    void removeSetting(int row);
    void clear();
    QSharedPointer<RegInfoObj> setting(int row) const;
    RegInfoObjList settings() const;
    bool containsFullName(const QString &fullName) const;

private:
    RegInfoObjList m_objects;
};

#endif // SETTINGSTABLEMODEL_H
