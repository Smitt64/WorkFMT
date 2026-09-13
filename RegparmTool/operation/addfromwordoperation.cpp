#include "addfromwordoperation.h"
#include "regparmwizard.h"
#include "wordpreviewregpage.h"
#include "rsl/reginfoobj.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include <toolsruntime.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QScopedPointer>
#include <QTextCodec>

AddFromWordOperation::AddFromWordOperation(QObject *parent)
    : OperationBase(parent)
{

}

void AddFromWordOperation::run()
{
    if (!wizzard())
    {
        emit errorMessage(tr("Внутренняя ошибка: не задан мастер"));
        emit finished();
        return;
    }

    WordPreviewRegPage *PreviewPage = wizzard()->wordPreviewRegPage();
    QSharedPointer<RegInfoObjModel> model = PreviewPage->model();
    RegInfoObjList RegsInfoList = model->getAllObjects();

    if (RegsInfoList.isEmpty())
    {
        emit finished();
        return;
    }

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

    for (const auto &RegInfo : qAsConst(RegsInfoList))
        processRegInfo(db, RegInfo);

    emit finished();
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

static bool insertRegparmNode(QSqlDatabase &db, qint64 &keyId, qint64 parentId, const QString &name,
                              qint16 type, const QString &description, bool isBranch, QString *error = nullptr)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO dregparm_dbt(t_parentid, t_name, t_type, t_global, t_description, t_security, t_isbranch, t_template) "
                  "VALUES (:parentid, :name, :type, :global, :description, :security, :isbranch, :template)");

    query.bindValue(":parentid", parentId);
    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":global", QString());            // узлы не являются глобальными настройками
    query.bindValue(":description", description);
    query.bindValue(":security", QString());          // узлы не являются настройками безопасности
    query.bindValue(":isbranch", isBranch ? QString("X") : QString());
    query.bindValue(":template", QString());

    if (!query.exec())
    {
        if (error)
            *error = query.lastError().text();
        return false;
    }

    QSqlQuery selectQuery(db);
    selectQuery.prepare("SELECT t_keyid FROM dregparm_dbt WHERE t_parentid = :parentid AND t_name = :name");
    selectQuery.bindValue(":parentid", parentId);
    selectQuery.bindValue(":name", name);

    if (toolExecuteQuery(&selectQuery) != 0 || !selectQuery.next())
    {
        if (error)
            *error = selectQuery.lastError().text();
        return false;
    }

    keyId = selectQuery.value(0).toLongLong();
    return keyId > 0;
}

static QByteArray stringToBlobData(const QString &value)
{
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    QByteArray data = codec ? codec->fromUnicode(value) : value.toLocal8Bit();
    data.append('\0');
    return data;
}

static QByteArray emptyBlobData()
{
    return QByteArray(1, '\0');
}

static bool insertRegvalDefault(QSqlDatabase &db, qint64 keyId, qint16 type, const QVariant &defaultValue, QString *error = nullptr)
{
    QSqlQuery query(db);
    // BLOB параметр передаётся позиционно: в примерах RSD драйвер связывает BLOB
    // только с '?', именованные placeholder'ы для BLOB не работают.
    query.prepare("INSERT INTO dregval_dbt(t_keyid, t_regkind, t_objectid, t_blockuservalue, t_expdep, t_lintvalue, t_ldoublevalue, t_fmtblobdata_xxxx) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

    int lintvalue = 0;
    double ldoublevalue = 0.0;
    QByteArray blobdata = emptyBlobData();

    switch (type)
    {
    case RegInfoObj::INTEGER:
        lintvalue = defaultValue.toInt();
        ldoublevalue = 0.0;
        break;

    case RegInfoObj::DOUBLE:
        lintvalue = 0;
        ldoublevalue = defaultValue.toDouble();
        break;

    case RegInfoObj::STRING:
        lintvalue = 0;
        ldoublevalue = 0.0;
        blobdata = stringToBlobData(defaultValue.toString());
        break;

    case RegInfoObj::BINARY:
    {
        lintvalue = 0;
        ldoublevalue = 0.0;
        blobdata = defaultValue.toByteArray();
        blobdata.append('\0');
        break;
    }

    case RegInfoObj::FLAG:
        lintvalue = defaultValue.toBool() ? 1 : 0;
        ldoublevalue = 0.0;
        break;

    default:
        break;
    }

    query.addBindValue(keyId);
    query.addBindValue(0);
    query.addBindValue(0);
    query.addBindValue(QString("0"));
    query.addBindValue(0);
    query.addBindValue(lintvalue);
    query.addBindValue(ldoublevalue);
    query.addBindValue(blobdata);

    if (toolExecuteQuery(&query) != 0)
    {
        if (error)
            *error = query.lastError().text();
        return false;
    }

    return true;
}

void AddFromWordOperation::processRegInfo(QSqlDatabase &db, const QSharedPointer<RegInfoObj> &obj)
{
    if (!obj)
        return;

    QString fullName = obj->fullName();
    if (fullName.isEmpty())
    {
        emit errorMessage(tr("Пустое полное имя настройки"));
        return;
    }

    QStringList parts = fullName.split('\\', Qt::SkipEmptyParts);
    if (parts.isEmpty())
    {
        emit errorMessage(tr("Не удалось разобрать полное имя настройки: %1").arg(fullName));
        return;
    }

    if (!db.transaction())
    {
        emit errorMessage(tr("Не удалось начать транзакцию для настройки %1: %2")
                          .arg(fullName)
                          .arg(db.lastError().text()));
        return;
    }

    qint64 parentId = 0;
    bool ok = true;
    QString error;

    // Создаем/находим промежуточные узлы
    for (int i = 0; i < parts.size() - 1 && ok; ++i)
    {
        const QString &part = parts.at(i);
        qint64 keyId = findKeyIdByParentAndName(db, parentId, part);

        if (keyId < 0)
        {
            if (!insertRegparmNode(db, keyId, parentId, part, 0, QString(), true, &error))
            {
                ok = false;
                break;
            }
        }

        parentId = keyId;
    }

    // Вставляем собственно настройку
    const QString &name = parts.last();
    qint64 keyId = -1;

    if (ok)
    {
        qint64 existingKeyId = findKeyIdByParentAndName(db, parentId, name);
        if (existingKeyId >= 0)
        {
            error = tr("Настройка уже существует в БД");
            ok = false;
        }
    }

    if (ok)
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO dregparm_dbt(t_parentid, t_name, t_type, t_global, t_description, t_security, t_isbranch, t_template) "
                      "VALUES (:parentid, :name, :type, :global, :description, :security, :isbranch, :template)");

        query.bindValue(":parentid", parentId);
        query.bindValue(":name", name);
        query.bindValue(":type", obj->type());
        query.bindValue(":global", obj->isGlobal() ? QString("X") : QString());
        query.bindValue(":description", obj->comment());
        query.bindValue(":security", obj->isSecurity() ? QString("X") : QString());
        query.bindValue(":isbranch", QString());  // листовая настройка
        query.bindValue(":template", obj->regTemplate());

        if (!query.exec())
        {
            error = query.lastError().text();
            ok = false;
        }
    }

    if (ok)
    {
        QSqlQuery selectQuery(db);
        selectQuery.prepare("SELECT t_keyid FROM dregparm_dbt WHERE t_parentid = :parentid AND t_name = :name");
        selectQuery.bindValue(":parentid", parentId);
        selectQuery.bindValue(":name", name);

        if (toolExecuteQuery(&selectQuery) != 0 || !selectQuery.next())
        {
            error = selectQuery.lastError().text();
            ok = false;
        }
        else
        {
            keyId = selectQuery.value(0).toLongLong();
        }
    }

    // Вставляем значение по умолчанию
    if (ok && keyId > 0)
    {
        if (!insertRegvalDefault(db, keyId, obj->type(), obj->defaultValue(), &error))
            ok = false;
    }

    if (!ok)
    {
        db.rollback();
        emit errorMessage(tr("Не удалось создать настройку %1: %2").arg(fullName).arg(error));
        return;
    }

    if (!db.commit())
    {
        db.rollback();
        emit errorMessage(tr("Не удалось зафиксировать транзакцию для настройки %1: %2")
                          .arg(fullName)
                          .arg(db.lastError().text()));
        return;
    }

    emit informationMessage(tr("Создана настройка %1").arg(fullName));
}
