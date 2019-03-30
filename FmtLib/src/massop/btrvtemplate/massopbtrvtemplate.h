#ifndef MASSOPBTRVTEMPLATE_H
#define MASSOPBTRVTEMPLATE_H

#include "massopinterface.h"
#include <QScopedPointer>

class MassOpBtrvTemplateResultPage;
class MassOpBtrvTemplate : public MassOpInterface
{
    Q_OBJECT
public:
    MassOpBtrvTemplate(QObject *parent = Q_NULLPTR);
    virtual ~MassOpBtrvTemplate() Q_DECL_OVERRIDE;

    void initPages() Q_DECL_OVERRIDE;
    void deinitPages() Q_DECL_OVERRIDE;

private:
    QScopedPointer<MassOpBtrvTemplateResultPage> pResultPage;
};

#endif // MASSOPBTRVTEMPLATE_H
