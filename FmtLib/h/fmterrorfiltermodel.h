#ifndef FMTERRORFILTERMODEL_H
#define FMTERRORFILTERMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class FmtErrors;
class FmtErrorFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FmtErrorFilterModel(QObject *parent = 0);
    ~FmtErrorFilterModel();

    void setFmtErrors(FmtErrors *e);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

public slots:
    void setShowInformation(bool value);
    void setShowWarnings(bool value);
    void setShowErrors(bool value);

private:
    bool m_ShowInformation, m_ShowWarnings, m_ShowErrors;
    FmtErrors *pErrors;
};

#endif // FMTERRORFILTERMODEL_H
