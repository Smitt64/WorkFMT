#include "addregtodatoperation.h"
#include "regparmwizard.h"
#include "settingsselectionpage.h"
#include "regparmmodel/settingstablemodel.h"
#include "rsl/reginfoobj.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include <toolsruntime.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QScopedPointer>
#include <QTextCodec>
#include <QDir>
#include <QFile>
#include <QtMath>

AddRegToDatOperation::AddRegToDatOperation(QObject *parent)
    : OperationBase(parent)
{
}

static qint64 findKeyIdByParentAndName(QSqlDatabase &db, qint64 parentId, const QString &name)
{
    QSqlQuery query(db);
    query.prepare("SELECT t_keyid FROM dregparm_dbt WHERE t_parentid = :parentId AND UPPER(t_name) = UPPER(:name)");
    query.bindValue(":parentId", parentId);
    query.bindValue(":name", name);

    if (toolExecuteQuery(&query) != 0 || !query.next())
        return -1;

    return query.value(0).toLongLong();
}

static qint64 nextKeyId(QSqlDatabase &db, const QString &tableName, const QString &idColumn)
{
    QSqlQuery query(db);
    query.prepare(QString("SELECT COALESCE(MAX(%1), 0) + 1 FROM %2").arg(idColumn, tableName));
    if (!query.exec() || !query.next())
        return -1;
    return query.value(0).toLongLong();
}

static QByteArray stringToBlobBytes(const QString &value)
{
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    QByteArray data = codec ? codec->fromUnicode(value) : value.toLocal8Bit();
    data.append('\0');
    return data;
}

static QString escapeSqlLoaderString(const QString &value)
{
    QString escaped = value;
    escaped.remove(QChar(0));
    escaped.replace("'", "''");
    escaped.replace("\n", "chr(10)");
    escaped.replace("\r", "chr(13)");
    return escaped;
}

static QString formatStringField(const QString &value)
{
    if (value.isEmpty())
        return QString(QChar(0x01)); // NULL для VARCHAR2 в существующих DAT-файлах

    return QString("'%1'").arg(escapeSqlLoaderString(value));
}

static QString formatCharField(const QString &value)
{
    if (value.isEmpty())
        return QString("'\x02'");

    return QString("'%1'").arg(escapeSqlLoaderString(value));
}

static QString formatBlobField(const QByteArray &bytes)
{
    if (bytes.isEmpty())
        return QString("'00'");

    return QString("'%1'").arg(QString(bytes.toHex().toUpper()));
}

static QString formatNumberField(int value)
{
    return QString::number(value);
}

static QString formatNumberField(qint64 value)
{
    return QString::number(value);
}

static QString formatNumberField(double value)
{
    // Целые значения выгружаем без дробной части (как в исходных DAT-файлах)
    if (value == qFloor(value))
        return QString::number(static_cast<qint64>(value));

    return QString::number(value, 'f', 6);
}

static QString buildRegparmLine(qint64 keyId, qint64 parentId, const QString &name, qint16 type,
                                 bool isGlobal, bool isSecurity)
{
    QStringList values;
    values << formatNumberField(keyId);
    values << formatNumberField(parentId);
    values << QString("'%1'").arg(escapeSqlLoaderString(name));
    values << formatNumberField(static_cast<int>(type));
    values << formatCharField(isGlobal ? QString("X") : QString());
    values << formatStringField(QString()); // T_DESCRIPTION
    values << formatCharField(isSecurity ? QString("X") : QString());
    values << formatCharField(QString()); // T_ISBRANCH - листовая настройка
    values << formatStringField(QString()); // T_TEMPLATE

    return values.join(",");
}

static QString buildRegvalLine(qint64 keyId, qint16 type, const QVariant &value)
{
    int lintValue = 0;
    double doubleValue = 0.0;
    QByteArray blobData;

    switch (type)
    {
    case RegInfoObj::INTEGER:
        lintValue = value.toInt();
        blobData = QByteArray(1, '\0');
        break;

    case RegInfoObj::DOUBLE:
        doubleValue = value.toDouble();
        blobData = QByteArray(1, '\0');
        break;

    case RegInfoObj::STRING:
        blobData = stringToBlobBytes(value.toString());
        break;

    case RegInfoObj::BINARY:
        blobData = value.toByteArray();
        blobData.append('\0');
        break;

    case RegInfoObj::FLAG:
        lintValue = value.toBool() ? 1 : 0;
        blobData = QByteArray(1, '\0');
        break;

    default:
        blobData = QByteArray(1, '\0');
        break;
    }

    QStringList values;
    values << formatNumberField(keyId);
    values << formatNumberField(0);   // T_REGKIND
    values << formatNumberField(0);   // T_OBJECTID
    values << formatCharField(QString()); // T_BLOCKUSERVALUE
    values << formatNumberField(0);   // T_EXPDEP
    values << formatNumberField(lintValue);
    values << formatNumberField(doubleValue);
    values << formatBlobField(blobData);

    return values.join(",");
}

static bool appendDatLine(const QString &filePath, const QString &line, QString *error)
{
    // Без QIODevice::Text: переводы строк пишем сами, иначе Qt превратит "\r\n" в "\r\r\n"
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite))
    {
        if (error)
            *error = file.errorString();
        return false;
    }

    // Если файл не заканчивается переводом строки - добавляем разделитель,
    // чтобы новая строка не склеилась с последней существующей.
    // Перевод строки пишется ПЕРЕД записью: исходные DAT-файлы не имеют
    // завершающего перевода строки, иначе получится пустая строка в конце.
    bool needNewLine = false;
    if (file.size() > 0)
    {
        file.seek(file.size() - 1);
        needNewLine = (file.read(1) != "\n");
    }
    file.seek(file.size());

    QTextStream stream(&file);
    stream.setCodec("IBM 866");
    if (needNewLine)
        stream << "\r\n";
    stream << line;
    file.close();
    return true;
}

// Читает актуальные данные настройки (тип, флаги, значение) из подключенной БД
static bool loadSettingFromDb(QSqlDatabase &db, const QString &fullName, QSharedPointer<RegInfoObj> obj)
{
    QStringList parts = fullName.split('\\', Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return false;

    qint64 parentId = 0;
    for (int i = 0; i < parts.size() - 1; ++i)
    {
        qint64 keyId = findKeyIdByParentAndName(db, parentId, parts.at(i));
        if (keyId < 0)
            return false;
        parentId = keyId;
    }

    QSqlQuery query(db);
    query.prepare("SELECT p.t_type, p.t_global, p.t_security, "
                  "v.t_lintvalue, v.t_ldoublevalue, v.t_fmtblobdata_xxxx "
                  "FROM dregparm_dbt p "
                  "LEFT JOIN dregval_dbt v ON p.t_keyid = v.t_keyid AND v.t_regkind = 0 AND v.t_objectid = 0 "
                  "WHERE p.t_parentid = :parentId AND UPPER(p.t_name) = UPPER(:name)");
    query.bindValue(":parentId", parentId);
    query.bindValue(":name", parts.last());

    if (toolExecuteQuery(&query) != 0 || !query.next())
        return false;

    qint16 type = static_cast<qint16>(query.value(0).toInt());
    obj->setType(type);
    obj->setIsGlobal(query.value(1).toString().trimmed() == QLatin1String("X"));
    obj->setIsSecurity(query.value(2).toString().trimmed() == QLatin1String("X"));

    int lintValue = query.value(3).toInt();
    double doubleValue = query.value(4).toDouble();
    QByteArray blobBytes = query.value(5).toByteArray(); // QRSD возвращает BLOB как сырые байты

    QVariant value;
    switch (type)
    {
    case RegInfoObj::INTEGER:
        value = lintValue;
        break;
    case RegInfoObj::DOUBLE:
        value = doubleValue;
        break;
    case RegInfoObj::FLAG:
        value = lintValue != 0;
        break;
    case RegInfoObj::STRING:
    case RegInfoObj::BINARY:
    {
        QByteArray bytes = blobBytes;
        while (!bytes.isEmpty() && bytes.endsWith('\0'))
            bytes.chop(1);

        if (type == RegInfoObj::STRING) {
            QTextCodec *codec = QTextCodec::codecForName("IBM 866");
            value = codec ? codec->toUnicode(bytes) : QString::fromLocal8Bit(bytes);
        } else {
            value = bytes;
        }
        break;
    }
    default:
        break;
    }

    obj->setDefaultValue(value);
    return true;
}

void AddRegToDatOperation::run()
{
    if (!wizzard())
    {
        emit errorMessage(tr("Внутренняя ошибка: не задан мастер"));
        emit finished();
        return;
    }

    SettingsSelectionPage *page = wizzard()->settingsSelectionPage();
    SettingsTableModel *model = page ? page->model() : nullptr;
    if (!model || model->rowCount() == 0)
    {
        emit finished();
        return;
    }

    // Подключаемся к БД - источнику переносимых настроек
    QScopedPointer<ConnectionInfo> connection(new ConnectionInfo());
    QString error;

    QString options;
    if (wizzard()->field("IsUnicode").toBool())
        options = "RSD_UNICODE";

    if (!connection->open(QRSD_DRIVER,
                          wizzard()->field("User").toString(),
                          wizzard()->field("Password").toString(),
                          wizzard()->field("Service").toString(),
                          options,
                          &error))
    {
        emit errorMessage(tr("Не удалось подключиться к БД: %1").arg(error));
        emit finished();
        return;
    }

    QSqlDatabase &db = connection->db();

    // Загружаем DAT-файлы в SQLite: по ним ищем родительские узлы,
    // проверяем дубликаты и определяем новые T_KEYID
    wizzard()->resetDatModel();
    QSqlDatabase datDb = wizzard()->datDatabase();

    if (!datDb.isOpen())
    {
        emit errorMessage(tr("Не удалось загрузить DAT-файлы для определения ключей"));
        emit finished();
        return;
    }

    qint64 nextParmId = nextKeyId(datDb, "DREGPARM_DBT", "T_KEYID");
    if (nextParmId < 0)
    {
        emit errorMessage(tr("Не удалось определить следующий T_KEYID для DREGPARM_DBT"));
        emit finished();
        return;
    }

    // Пути к DAT-файлам
    QDir dataDir(wizzard()->field("RepositoryPath").toString());
    QString parmDatPath = dataDir.absoluteFilePath("Distrib/DBFile/Data/DREGPARM_DBT.dat");
    QString valDatPath = dataDir.absoluteFilePath("Distrib/DBFile/Data/DREGVAL_DBT.dat");

    RegInfoObjList settings = model->settings();

    for (const auto &obj : settings)
    {
        if (!obj)
            continue;

        QString fullName = obj->fullName();
        if (fullName.isEmpty())
        {
            emit errorMessage(tr("Пустое полное имя настройки"));
            continue;
        }

        QStringList parts = fullName.split('\\', Qt::SkipEmptyParts);
        if (parts.isEmpty())
        {
            emit errorMessage(tr("Не удалось разобрать полное имя настройки: %1").arg(fullName));
            continue;
        }

        // Читаем актуальные данные настройки из БД
        if (!loadSettingFromDb(db, fullName, obj))
        {
            emit errorMessage(tr("Настройка не найдена в БД: %1").arg(fullName));
            continue;
        }

        qint64 parentId = 0;
        bool ok = true;

        // Находим родительские узлы в DAT-файле
        for (int i = 0; i < parts.size() - 1; ++i)
        {
            qint64 keyId = findKeyIdByParentAndName(datDb, parentId, parts.at(i));
            if (keyId < 0)
            {
                emit errorMessage(tr("Родительский узел не найден в DAT файле: %1")
                                  .arg(parts.mid(0, i + 1).join("\\")));
                ok = false;
                break;
            }
            parentId = keyId;
        }

        if (!ok)
            continue;

        const QString &name = parts.last();

        // Проверяем, что настройки еще нет в DAT-файле
        if (findKeyIdByParentAndName(datDb, parentId, name) >= 0)
        {
            emit warningMessage(tr("Настройка уже существует в DAT файле: %1").arg(fullName));
            continue;
        }

        qint64 keyId = nextParmId++;

        // Дописываем строку в DREGPARM_DBT.dat
        QString parmLine = buildRegparmLine(keyId, parentId, name, obj->type(),
                                             obj->isGlobal(), obj->isSecurity());
        if (!appendDatLine(parmDatPath, parmLine, &error))
        {
            emit errorMessage(tr("Не удалось записать в DREGPARM_DBT.dat: %1").arg(error));
            continue;
        }

        // Дописываем строку в DREGVAL_DBT.dat
        QString valLine = buildRegvalLine(keyId, obj->type(), obj->defaultValue());
        if (!appendDatLine(valDatPath, valLine, &error))
        {
            emit errorMessage(tr("Не удалось записать в DREGVAL_DBT.dat: %1").arg(error));
            continue;
        }

        emit informationMessage(tr("Добавлена настройка %1").arg(fullName));
    }

    emit finished();
}
