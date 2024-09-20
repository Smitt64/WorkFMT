#ifndef FMTRSLMODULE_H
#define FMTRSLMODULE_H

#include "rsscript/rslstaticmodule.h"
extern void fmtappRegister();

class FmtRslModule : public RslStaticModule
{
public:
    constexpr static const char name[] = "Fmt";

    FmtRslModule();

    //static void rslGetPostgreSQLInstallLocation();

protected:
    void Init() Q_DECL_OVERRIDE;
    void Proc() Q_DECL_OVERRIDE;
};

#endif // FMTRSLMODULE_H
