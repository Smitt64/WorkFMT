#include "tablefilesmodel.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>

TableFilesModel::TableFilesModel(QSettings* settings, QObject *parent)
    : QAbstractTableModel(parent), m_settings(settings)
{
    if (!m_settings)
    {
        qWarning() << "Settings object is null, creating default";
        m_settings = new QSettings("YourCompany", "YourApp", this);
    }
}

int TableFilesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_files.size();
}

int TableFilesModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant TableFilesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_files.size())
    {
        return QVariant();
    }

    const TableFile &file = m_files.at(index.row());

    if (index.column() == 0)
    {
        if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
            return QString("%1 (%2)").arg(file.tableName.toLower(), file.fileName);
        else if (role == Qt::CheckStateRole)
            return file.isChecked ? Qt::Checked : Qt::Unchecked;
    }

    return QVariant();
}

bool TableFilesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= m_files.size())
        return false;

    if (role == Qt::CheckStateRole)
    {
        bool newValue = value.toBool();
        TableFile &file = m_files[index.row()];

        // Если пытаемся установить чекбокс для таблицы, которая уже отмечена
        if (newValue)
        {
            // Снимаем все чекбоксы для этой таблицы
            for (int i = 0; i < m_files.size(); ++i)
            {
                if (QString::compare(m_files[i].tableName, file.tableName, Qt::CaseInsensitive) == 0)
                {
                    m_files[i].isChecked = false;
                    QModelIndex changedIndex = createIndex(i, 1);
                    emit dataChanged(changedIndex, changedIndex, {Qt::CheckStateRole});
                }
            }
        }

        file.isChecked = newValue;
        emit dataChanged(index, index, {Qt::CheckStateRole});

        // Сохраняем состояние
        saveSelection();

        return true;
    }

    return false;
}

QVariant TableFilesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    {
        return QVariant();
    }

    switch (section)
    {
    case 0: return tr("Таблица (файл)");
    case 1: return tr("Активна");
    default: return QVariant();
    }
}

Qt::ItemFlags TableFilesModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if (index.isValid())
    {
        // Разрешаем проверять элементы (для чекбоксов)
        flags |= Qt::ItemIsUserCheckable;
    }
    return flags;
}

void TableFilesModel::loadFromDirectories(const QStringList &directories)
{
    beginResetModel();
    m_files.clear();
    m_uniqueTables.clear();

    for (const QString &dirPath : directories)
    {
        QDir dir(dirPath);
        if (!dir.exists())
        {
            qWarning() << "Directory does not exist:" << dirPath;
            continue;
        }

        QStringList macFiles = dir.entryList(QStringList() << "*.mac", QDir::Files);
        for (const QString &fileName : qAsConst(macFiles))
        {
            QFileInfo fileInfo(dir.absoluteFilePath(fileName));
            processMacFile(fileInfo);
        }
    }

    // Восстанавливаем сохраненные состояния чекбоксов
    restoreSelection();

    endResetModel();
}

void TableFilesModel::processMacFile(const QFileInfo &fileInfo)
{
    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Cannot open file:" << fileInfo.absoluteFilePath();
        return;
    }

    QString content = file.readAll();
    file.close();

    QString tableName = extractTableName(content);
    if (tableName.isEmpty())
    {
        return;
    }

    TableFile tf;
    tf.tableName = tableName;
    tf.fileName = fileInfo.fileName();
    tf.isChecked = false;

    m_files.append(tf);
    m_uniqueTables.insert(tableName.toLower());
}

QString TableFilesModel::extractTableName(const QString &fileContent) const
{
    QRegularExpression re("\\$Table:\\s*([^\\s]+)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(fileContent);
    if (match.hasMatch())
    {
        return match.captured(1).trimmed();
    }
    return QString();
}

QSet<QString> TableFilesModel::selectedTables() const
{
    QSet<QString> selected;
    for (const TableFile &file : m_files)
    {
        if (file.isChecked)
        {
            selected.insert(file.tableName);
        }
    }
    return selected;
}

void TableFilesModel::restoreSelection()
{
    if (!m_settings)
        return;

    m_settings->beginGroup("macrofile");

    // Собираем информацию о таблицах, для которых есть сохраненные значения
    QSet<QString> tablesWithSavedValues;
    for (const QString &key : m_settings->childKeys())
        tablesWithSavedValues.insert(key.toLower());

    // Проходим по всем файлам и устанавливаем чекбоксы
    QSet<QString> processedTables;
    for (int i = 0; i < m_files.size(); ++i)
    {
        TableFile &file = m_files[i];
        QString tableLower = file.tableName.toLower();

        // Проверяем сохраненное значение
        QString savedFile = m_settings->value(tableLower).toString();

        if (!savedFile.isEmpty())
        {
            // Если есть сохраненное значение - устанавливаем только для нужного файла
            file.isChecked = (savedFile == file.fileName);
        }
        /*else if (!processedTables.contains(tableLower))
        {
            // Если это первая встреча таблицы и нет сохраненного значения - выбираем первый файл
            file.isChecked = true;
            processedTables.insert(tableLower);

            // Сохраняем выбор по умолчанию
            m_settings->setValue(tableLower, file.fileName);
        }*/
        else
            file.isChecked = false;
    }

    m_settings->endGroup();
    m_settings->sync();
}

void TableFilesModel::saveSelection()
{
    if (!m_settings)
        return;

    m_settings->beginGroup("macrofile");

    // Сохраняем только выбранные файлы
    for (const TableFile &file : qAsConst(m_files))
    {
        if (file.isChecked)
            m_settings->setValue(file.tableName.toLower(), file.fileName);
        else
            m_settings->setValue(file.tableName.toLower(), QString());
    }

    m_settings->endGroup();
    m_settings->sync();
}

QString TableFilesModel::getSelectedFileForTable(const QString &tableName) const
{
    if (!m_settings)
        return QString();

    m_settings->beginGroup("macrofile");
    QString fileName = m_settings->value(tableName.toLower()).toString();
    m_settings->endGroup();

    return fileName;
}
