#include "projectloader.h"

ProjectLoader::ProjectLoader(const QString& filePath)
{
    loadJson(filePath);
}

QString ProjectLoader::getRealProjectName(const QString& name) const
{
    QString proj = name;
    proj = proj.remove(".lib", Qt::CaseInsensitive).remove(".www", Qt::CaseInsensitive);

    QStringList keys = m_projects.keys();
    for (int i = 0; i < keys.size(); i++)
    {
        if (keys[i].startsWith(proj, Qt::CaseInsensitive))
        {
           proj = keys[i];
           break;
        }
    }

    return proj;
}

bool ProjectLoader::getProjectInfo(const QString& name, QString& vcxproj, QString& dir, QString& targetName, QString& targetExt)
{
    QString proj = getRealProjectName(name);

    if (m_projects.contains(proj))
    {
        QJsonObject project = m_projects[proj].toObject();
        vcxproj = project["vcxproj"].toString();
        dir = project["dir"].toString();
        targetName = project["target_name"].toString();
        targetExt = project["target_ext"].toString();
        return true;
    }

    return false;
}

QStringList ProjectLoader::getModules(const QString& name)
{
    QString proj = getRealProjectName(name);

    if (m_projects.contains(proj))
    {
        QJsonObject project = m_projects[proj].toObject();
        QJsonArray modulesArray = project["module"].toArray();
        QStringList modules;
        for (const QJsonValue& module : qAsConst(modulesArray))
            modules.append(module.toString());

        if (!project["target_ext"].toString().compare(".dll", Qt::CaseInsensitive))
            modules.append(proj);

        return modules;
    }

    return QStringList();
}

QSet<QString> ProjectLoader::getModulesFromNames(const QStringList& names)
{
    QSet<QString> modulesSet;
    for (const QString& name : names)
    {
        QStringList modules = getModules(name);
        for (const QString& module : qAsConst(modules))
            modulesSet.insert(module);
    }

    return modulesSet;
}

void ProjectLoader::loadJson(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Не удалось открыть файл:" << filePath;
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isArray())
    {
        QJsonArray jsonArray = jsonDoc.array();
        for (const QJsonValue& value : qAsConst(jsonArray))
        {
            QJsonObject obj = value.toObject();
            QString name = obj["name"].toString();
            m_projects[name] = obj;
        }
    }
    else
        qWarning() << "JSON файл не является массивом.";
}

QString ProjectLoader::generateTextForProjects(const QStringList &projectNames, const QStringList& modules)
{
    QString result;

    // 1. Генерация текста для библиотек
    result += "1. Библиотеки\n";
    for (const QString& projectName : projectNames)
    {
        QString vcxproj, dir, targetName, targetExt;
        if (getProjectInfo(projectName, vcxproj, dir, targetName, targetExt))
        {
            // Пропускаем элементы, которые заканчиваются на ".www"
            if (!dir.endsWith(".www", Qt::CaseInsensitive))
            {
                result += QString("  %1%2 (%3\\%4)\n")
                        .arg(targetName)
                        .arg(targetExt)
                        .arg(dir)
                        .arg(vcxproj);
            }
        }
    }

    // 2. Генерация текста для модулей
    result += "\n2. Модули\n";
    for (const QString& module : modules)
    {
        QString vcxproj, dir, targetName, targetExt;
        if (getProjectInfo(module, vcxproj, dir, targetName, targetExt))
        {
            result += QString("  %1%2 (%3\\%4)\n")
                    .arg(targetName)
                    .arg(targetExt)
                    .arg(dir)
                    .arg(vcxproj);
        }
    }

    return result;
}
