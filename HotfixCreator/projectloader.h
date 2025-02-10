#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include <QObject>
#include <QMap>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QStringList>
#include <QSet>
#include <QDebug>

class ProjectLoader
{
public:
    ProjectLoader(const QString& filePath);
    bool getProjectInfo(const QString& name, QString& vcxproj, QString& dir, QString& targetName, QString& targetExt);
    QStringList getModules(const QString& name);
    QSet<QString> getModulesFromNames(const QStringList& names);

    QString generateTextForProjects(const QStringList &projectNames, const QStringList &modules);

private:
    void loadJson(const QString& filePath);
    QMap<QString, QJsonValue> m_projects;
};

#endif // PROJECTLOADER_H
