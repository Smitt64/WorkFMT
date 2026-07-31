#ifndef ADDREGTODATOPERATION_H
#define ADDREGTODATOPERATION_H

#include "operationbase.h"

class AddRegToDatOperation : public OperationBase
{
    Q_OBJECT
public:
    explicit AddRegToDatOperation(QObject *parent = nullptr);

    virtual void run() Q_DECL_OVERRIDE;
};

#endif // ADDREGTODATOPERATION_H
