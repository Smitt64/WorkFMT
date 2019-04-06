#ifndef MASSINITTABLEOPERATION_H
#define MASSINITTABLEOPERATION_H

#include "massopinterface.h"
#include <QScopedPointer>

class MassInitTableParams;
class MassInitTablesProgress;
class MassInitTablesParamModel;
class MassInitTableOperation : public MassOpInterface
{
    Q_OBJECT
public:
    MassInitTableOperation(QObject *parant = Q_NULLPTR);
    virtual ~MassInitTableOperation() Q_DECL_OVERRIDE;

    void initPages() Q_DECL_OVERRIDE;
    void deinitPages() Q_DECL_OVERRIDE;

    MassInitTablesParamModel *model();

private:
    QScopedPointer<MassInitTableParams> pParamsPage;
    QScopedPointer<MassInitTablesProgress> pParamsProgress;
    QScopedPointer<MassInitTablesParamModel> pModel;
};

#endif // MASSINITTABLEOPERATION_H
