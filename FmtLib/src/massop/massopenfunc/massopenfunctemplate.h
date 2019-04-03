#ifndef MASSOPENFUNCTEMPLATE_H
#define MASSOPENFUNCTEMPLATE_H

#include "massopinterface.h"
#include <QScopedPointer>

class MassOpenFucnParams;
class MassOpenFuncTemplate : public MassOpInterface
{
    Q_OBJECT
public:
    MassOpenFuncTemplate(QObject *parent = Q_NULLPTR);
    virtual ~MassOpenFuncTemplate() Q_DECL_OVERRIDE;

    void initPages() Q_DECL_OVERRIDE;
    void deinitPages() Q_DECL_OVERRIDE;

private:
    QScopedPointer<MassOpenFucnParams> pParamPage;
};

#endif // MASSOPENFUNCTEMPLATE_H
