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
    //m_pTable = new FmtTable(info, this);
    Init(info);
}

RichTextToInsertWizard::RichTextToInsertWizard(FmtTable *Table, QWidget *parent, Qt::WindowFlags flags) :
    QWizard(parent, flags)
{
    m_pTable = Table;

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

QMap<int, QString> RichTextToInsertWizard::fieldMapping() const
{
    QMap<int, QString> mapping;

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

        QString fieldName = field->undecorateName();

        // Анализируем тип источника
        if (source.startsWith("{column_") && source.endsWith("}"))
        {
            // Колонка Word документа
            QString columnStr = source.mid(8, source.length() - 9);
            bool ok;
            int columnIndex = columnStr.toInt(&ok) - 1;

            if (ok && columnIndex >= 0)
            {
                mapping[columnIndex] = fieldName;
            }
        }
        else if (source.startsWith("sql:"))
        {
            // SQL выражение
            QString sqlValue = source.mid(4);
            mapping[-fieldIndex - 1000] = QString("SQL|%1|%2").arg(fieldName).arg(sqlValue);
        }
        else if (source.startsWith("func:"))
        {
            // Функция
            QString funcValue = source.mid(5);
            mapping[-fieldIndex - 2000] = QString("FUNC|%1|%2").arg(fieldName).arg(funcValue);
        }
        else
        {
            // Простое значение или выражение
            mapping[-fieldIndex - 3000] = QString("VAL|%1|%2").arg(fieldName).arg(source);
        }
    }

    return mapping;
}
