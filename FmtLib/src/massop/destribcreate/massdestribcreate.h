#ifndef MASSDESTRIBCREATE_H
#define MASSDESTRIBCREATE_H

#include "massopinterface.h"
#include <QScopedPointer>

class MassDestribParamsPage;
class MassDestribParamModel;
class MassDestribProgress;
class MassDestribCreate: public MassOpInterface
{
    Q_OBJECT
public:
    MassDestribCreate(QObject *parent = Q_NULLPTR);
    virtual ~MassDestribCreate() Q_DECL_OVERRIDE;

    void initPages() Q_DECL_OVERRIDE;
    void deinitPages() Q_DECL_OVERRIDE;

    MassDestribParamModel *model();

private:
    QScopedPointer<MassDestribParamsPage> mParams;
    QScopedPointer<MassDestribProgress> mProgress;
    MassDestribParamModel *pModel;
};

#endif // MASSDESTRIBCREATE_H
