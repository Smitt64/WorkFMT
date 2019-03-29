#include "fmterrorfiltermodel.h"
#include "fmterrors.h"

FmtErrorFilterModel::FmtErrorFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_ShowInformation = true;
    m_ShowWarnings = true;
    m_ShowErrors = true;
    pErrors = NULL;
}

FmtErrorFilterModel::~FmtErrorFilterModel()
{

}

void FmtErrorFilterModel::setShowInformation(bool value)
{
    if (m_ShowInformation == value)
        return;

    m_ShowInformation = value;
    invalidateFilter();
}

void FmtErrorFilterModel::setShowWarnings(bool value)
{
    if (m_ShowWarnings == value)
        return;

    m_ShowWarnings = value;
    invalidateFilter();
}

void FmtErrorFilterModel::setShowErrors(bool value)
{
    if (m_ShowErrors == value)
        return;

    m_ShowErrors = value;
    invalidateFilter();
}

void FmtErrorFilterModel::setFmtErrors(FmtErrors *e)
{
    pErrors = e;
    setSourceModel(pErrors);
}

bool FmtErrorFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent);
    if (!pErrors)
        return false;

    bool result = false;
    qint16 type = pErrors->type(sourceRow);

    if (type == FmtErrors::fmtet_Error && m_ShowErrors)
        result = true;

    if (type == FmtErrors::fmtet_Warning && m_ShowWarnings)
        result = true;

    if (type == FmtErrors::fmtet_Info && m_ShowInformation)
        result = true;

    return result;
}
