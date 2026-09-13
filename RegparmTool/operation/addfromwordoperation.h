#ifndef ADDFROMWORDOPERATION_H
#define ADDFROMWORDOPERATION_H

#include "operationbase.h"
#include "rsl/reginfoobj.h"
#include <QSqlDatabase>

class AddFromWordOperation : public OperationBase
{
    Q_OBJECT
public:
    explicit AddFromWordOperation(QObject *parent = nullptr);

    virtual void run() Q_DECL_OVERRIDE;

private:
    void processRegInfo(QSqlDatabase &db, const QSharedPointer<RegInfoObj> &obj);
};

#endif // ADDFROMWORDOPERATION_H
