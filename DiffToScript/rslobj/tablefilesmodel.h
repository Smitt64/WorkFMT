#ifndef TABLEFILESMODEL_H
#define TABLEFILESMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QSet>
#include <QSettings>
#include <QFileInfo>

class TableFilesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableFilesModel(QSettings* settings, QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Основной метод для загрузки данных
    void loadFromDirectories(const QStringList &directories);

    // Методы для работы с выделенными элементами
    QSet<QString> selectedTables() const;
    void restoreSelection();
    void saveSelection();
    QString getSelectedFileForTable(const QString &tableName) const;

private:
    struct TableFile
    {
        QString tableName;
        QString fileName;
        bool isChecked;
    };

    QList<TableFile> m_files;
    QSet<QString> m_uniqueTables;
    QSettings* m_settings;

    void processMacFile(const QFileInfo &fileInfo);
    QString extractTableName(const QString &fileContent) const;
    QString findMacFile(const QString &baseDir, const QString &relativePath) const;
};

#endif // TABLEFILESMODEL_H
