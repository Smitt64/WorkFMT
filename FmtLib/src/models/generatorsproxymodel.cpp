#include "generatorsproxymodel.h"
#include "fmtgeninterface.h"
#include "geninterfacefactorymodel.h"
#include "rslexecutors/generatorrslexecutor.h"
#include <QDir>
#include <QRegularExpression>

class FmtGenMacroExecutor : public FmtGenInterface
{
public:
    FmtGenMacroExecutor(const GeneratorsMacroElement *element) :
        FmtGenInterface()
    {
        m_Data = *element;
    }

    virtual ~FmtGenMacroExecutor()
    {

    }

protected:
    virtual QMap<QString, QByteArray> makeContent(FmtSharedTablePtr pTable)
    {
        GeneratorRslExecutor executor(m_Data.macro, pTable.data());
        executor.execute();

        return QMap<QString, QByteArray>{{QString(), executor.data()}};
    }

private:
    GeneratorsMacroElement m_Data;
};

void GetGenMacroExecutor(const GeneratorsMacroElement *element, FmtGenInterface **ppObj)
{
    if (!ppObj)
        return;

    (*ppObj) = new FmtGenMacroExecutor(element);
}

// ----------------------------------------------------------------------------------

GeneratorsProxyModel::GeneratorsProxyModel(QObject *parent)
    : QAbstractProxyModel{parent}
{
    QDir appdir(qApp->applicationDirPath());

    if (appdir.cd("mac") && appdir.cd("fmtcore") && appdir.cd("generators"))
    {
        QFileInfoList generators = appdir.entryInfoList({"*.mac"}, QDir::Files);

        for (const QFileInfo &info : qAsConst(generators))
        {
            m_MacroList.append({info.absoluteFilePath(), info.baseName()});
            ReadFileInfo(m_MacroList.back());
        }
    }
}

GeneratorsProxyModel::~GeneratorsProxyModel()
{

}

QString GeneratorsProxyModel::ReadValue(const QString &value, const QString &content)
{
    int pos = content.indexOf(value + ":");

    if (pos!= -1)
    {
        int pos2 = content.indexOf("\n", pos);
        return content.mid(pos + value.length() + 1, pos2 - pos - value.length() - 1).simplified().trimmed();
    }

    return QString();
}

void GeneratorsProxyModel::ReadFileInfo(GeneratorsMacroElement &elem)
{
    QFile f(elem.macro);
    if (f.open(QIODevice::ReadOnly))
    {
        QRegularExpression rx("\\/\\*[\\s\\S]*?\\*\\/");

        QTextStream stream(&f);
        stream.setCodec("IBM 866");
        QString content = stream.readAll();

        QRegularExpressionMatch match = rx.match(content);
        if (match.hasMatch())
        {
            QString matched = match.captured();

            elem.alias = ReadValue("Title", matched);
            elem.highlighter = ReadValue("Highlighter", matched);
        }
        f.close();
    }
}

QModelIndex GeneratorsProxyModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int GeneratorsProxyModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return sourceModel()->columnCount();
}

QModelIndex GeneratorsProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if(!sourceModel())
        return QModelIndex();

    return index(sourceIndex.row(), sourceIndex.column(), QModelIndex());
}

QModelIndex GeneratorsProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if(!sourceModel())
        return QModelIndex();

    if (proxyIndex.row() < sourceModel()->rowCount())
        return sourceModel()->index(proxyIndex.row(), proxyIndex.column());

    return QModelIndex();
}

int GeneratorsProxyModel::rowCount(const QModelIndex& parent) const
{
    if (!sourceModel() || parent.isValid())
        return 0;

    return sourceModel()->rowCount() + m_MacroList.size();
}

QModelIndex GeneratorsProxyModel::index(int row, int column, const QModelIndex& parent) const
{
    if (parent.isValid())
        return QModelIndex();

    return createIndex(row, column);
}

Qt::ItemFlags GeneratorsProxyModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
}

bool GeneratorsProxyModel::hasChildren(const QModelIndex &parent) const
{
    return false;
}

QVariant GeneratorsProxyModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < sourceModel()->rowCount())
    {
        if (role == Qt::DisplayRole)
            return sourceModel()->data(mapToSource(index), role);
        else if (role == Qt::DecorationRole)
            return QIcon(":/img/codewin.png");

        return sourceModel()->data(mapToSource(index), role);
    }
    else
    {
        int idx = index.row() - sourceModel()->rowCount();
        if (role == Qt::DisplayRole)
        {
            if (index.column() == GenInterfaceFactoryModel::FieldAlias)
                return m_MacroList[idx].alias;
            else
                return QString("macro:%1").arg(m_MacroList[idx].macro);
        }
        else if (role == Qt::EditRole)
        {
            if (index.column() == GenInterfaceFactoryModel::FieldAlias)
                return m_MacroList[idx].macro;
        }
        else if (role == Qt::DecorationRole)
            return QIcon(":/img/script.png");
    }

    return QVariant();
}

GeneratorsMacroElement GeneratorsProxyModel::getMacroElement(const QModelIndex &index)
{
    if (index.row() >= sourceModel()->rowCount())
        return m_MacroList[sourceModel()->rowCount() - index.row()];

    return GeneratorsMacroElement();
}
