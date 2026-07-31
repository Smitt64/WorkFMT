#include "settingspathcompleter.h"
#include "regparmmodel/regparmitem.h"

SettingsPathCompleter::SettingsPathCompleter(QObject *parent)
    : QCompleter(parent)
{
}

QString SettingsPathCompleter::pathFromIndex(const QModelIndex &index) const
{
    QStringList path;
    QModelIndex current = index;
    while (current.isValid()) {
        QModelIndex nameIndex = current.sibling(current.row(), RegParmItem::T_NAME);
        path.prepend(nameIndex.data(completionRole()).toString());
        current = current.parent();
    }
    return path.join("\\");
}

QString SettingsPathCompleter::completionString(const QModelIndex &index) const
{
    return pathFromIndex(index);
}

QStringList SettingsPathCompleter::splitPath(const QString &path) const
{
    return path.split("\\", Qt::KeepEmptyParts);
}
