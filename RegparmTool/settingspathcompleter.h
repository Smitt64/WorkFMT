#ifndef SETTINGSPATHCOMPLETER_H
#define SETTINGSPATHCOMPLETER_H

#include <QCompleter>

class SettingsPathCompleter : public QCompleter
{
    Q_OBJECT
public:
    explicit SettingsPathCompleter(QObject *parent = nullptr);
    QString completionString(const QModelIndex &index) const;

protected:
    QString pathFromIndex(const QModelIndex &index) const override;
    QStringList splitPath(const QString &path) const override;
};

#endif // SETTINGSPATHCOMPLETER_H
