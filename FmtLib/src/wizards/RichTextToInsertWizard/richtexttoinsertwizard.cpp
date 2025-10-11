#include "richtexttoinsertwizard.h"
#include "src/massop/massopselecttablespage.h"
#include "fmttable.h"
#include "fmtfield.h"
#include "src/wizards/RichTextToInsertWizard/existsconditionpage.h"
#include "src/wizards/RichTextToInsertWizard/fmtimportfldsourcepage.h"
#include "src/wizards/RichTextToInsertWizard/fmtsourcemodel.h"
#include "src/wizards/RichTextToInsertWizard/richtexttoinsertresultpage.h"
#include "src/wizards/richtextpage.h"
#include <QTextDocument>

class RichTextToInsertSelectTablesPage : public MassOpSelectTablesPage
{
public:
    RichTextToInsertSelectTablesPage(ConnectionInfo *info, QWidget *parent) :
        MassOpSelectTablesPage(info, parent)
    {
        setSingleSelectionMode(true);
    }

    virtual bool AddFunc(const QString &str) Q_DECL_OVERRIDE
    {
        RichTextToInsertWizard *pWizard = qobject_cast<RichTextToInsertWizard*>(wizard());
        return pWizard->addTables(str);
    }

    virtual bool RemoveFunc(const QString &str) Q_DECL_OVERRIDE
    {
        RichTextToInsertWizard *pWizard = qobject_cast<RichTextToInsertWizard*>(wizard());
        return pWizard->removeTables(str);
    }
};

RichTextToInsertWizard::RichTextToInsertWizard(ConnectionInfo *info, QWidget *parent, Qt::WindowFlags flags) :
    QWizard(parent, flags),
    m_pTable(nullptr)
{
    Init(info);
}

RichTextToInsertWizard::RichTextToInsertWizard(FmtTable *Table, QWidget *parent, Qt::WindowFlags flags) :
    QWizard(parent, flags),
    m_pTable(Table)
{
    Init(m_pTable->connection());
}

void RichTextToInsertWizard::Init(ConnectionInfo *info)
{
    setMinimumSize(800, 600);
    setWindowTitle(tr("Создание скрипта загрузки содержимого"));

    if (!m_pTable)
    {
        m_pSelectTables = new RichTextToInsertSelectTablesPage(info, this);
        addPage(m_pSelectTables);
    }

    m_pRichText = new QTextDocument(this);
    m_pRichTextPage = new RichTextPage(this);
    m_pRichTextPage->setDocument(m_pRichText);
    m_pRichTextPage->setTitle(tr("Таблица с данными"));
    addPage(m_pRichTextPage);

    m_pSourcePage = new FmtImportFldSourcePage(this);
    addPage(m_pSourcePage);

    m_pExistsConditionPage = new ExistsConditionPage(this);
    addPage(m_pExistsConditionPage);

    m_pResultPage = new RichTextToInsertResultPage(this);
    addPage(m_pResultPage);
}

bool RichTextToInsertWizard::addTables(const QString &str)
{
    if (m_Tables.contains(str))
        return false;

    m_Tables.append(str);
    return true;
}

bool RichTextToInsertWizard::removeTables(const QString &str)
{
    m_Tables.removeOne(str);
    return true;
}

FmtTable *RichTextToInsertWizard::table() const
{
    return m_pTable;
}

QTextDocument *RichTextToInsertWizard::document()
{
    return m_pRichText;
}

QMap<QString, QString> RichTextToInsertWizard::fieldMapping() const
{
    QMap<QString, QString> mapping;

    if (!m_pSourcePage || !m_pTable)
        return mapping;

    FmtSourceModel *model = m_pSourcePage->sourceModel();
    if (!model)
        return mapping;

    int fieldCount = model->rowCount();

    for (int fieldIndex = 0; fieldIndex < fieldCount; ++fieldIndex)
    {
        FmtField *field = model->field(fieldIndex);
        if (!field)
            continue;

        // Получаем источник данных из свойства поля
        QVariant sourceVar = field->property("dataSource");
        QString source = sourceVar.isValid() ? sourceVar.toString() : "";

        // Если источник не установлен, используем значение по умолчанию
        if (source.isEmpty())
        {
            QModelIndex sourceIndex = model->index(fieldIndex, FmtSourceModel::col_Source);
            source = model->data(sourceIndex, Qt::DisplayRole).toString();
        }

        QString fieldName = field->name(); // Используем настоящее имя поля из БД

        // Ключ = имя поля в БД таблице
        // Значение = источник данных

        if (source.startsWith("{column_") && source.endsWith("}"))
        {
            // Колонка Word документа
            QString columnStr = source.mid(8, source.length() - 9);
            bool ok;
            int columnIndex = columnStr.toInt(&ok) - 1;

            if (ok && columnIndex >= 0)
            {
                mapping[fieldName] = QString("COLUMN|%1").arg(columnIndex);
            }
        }
        else if (source.startsWith("sql:"))
        {
            // SQL выражение
            QString sqlValue = source.mid(4);
            mapping[fieldName] = QString("SQL|%1").arg(sqlValue);
        }
        else if (source.startsWith("func:"))
        {
            // Функция
            QString funcValue = source.mid(5);
            mapping[fieldName] = QString("FUNC|%1").arg(funcValue);
        }
        else
        {
            // Простое значение или выражение
            mapping[fieldName] = QString("VAL|%1").arg(source);
        }
    }

    return mapping;
}

FmtIndex* RichTextToInsertWizard::selectedIndex() const
{
    if (!m_pExistsConditionPage)
        return nullptr;

    return m_pExistsConditionPage->getSelectedIndex();
}

QString RichTextToInsertWizard::customCondition() const
{
    if (!m_pExistsConditionPage)
        return QString();

    return m_pExistsConditionPage->getCustomCondition();
}

bool RichTextToInsertWizard::useCustomCondition() const
{
    if (!m_pExistsConditionPage)
        return false;

    return m_pExistsConditionPage->useCustomCondition();
}
