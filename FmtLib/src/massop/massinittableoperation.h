#ifndef MASSINITTABLEOPERATION_H
#define MASSINITTABLEOPERATION_H

#include "massopinterface.h"

class MassInitTableOperation : public MassOpInterface
{
    Q_OBJECT
public:
    MassInitTableOperation();
    virtual ~MassInitTableOperation() Q_DECL_OVERRIDE;
};

#endif // MASSINITTABLEOPERATION_H
