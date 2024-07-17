#include "fmtrslmodule.h"
#include "rsscript/registerobjlist.hpp"
#include "rslexecutor.h"
#include "connectioninfo.h"
#include "fmtcore.h"
#include "fmtfield.h"
#include "fmtindex.h"
#include "fmttable.h"
#include "fmtsegment.h"
#include "fmtnamespace.h"

Q_GLOBAL_STATIC(FmtNamespace, pFmtNamespace)
Q_GLOBAL_STATIC(FmtTypesNamespace, pFmtTypesNamespace)
Q_GLOBAL_STATIC(FmtKeyFlagsNamespace, pFmtKeyFlags)
Q_GLOBAL_STATIC(FmtKeyNullValNamespace, pFmtKeyNullValFlags)

FmtRslModule::FmtRslModule() :
    RslStaticModule()
{
    FmtInit();
    RegisterObjList::inst()->RegisterRslObject<FmtNamespace>();
    RegisterObjList::inst()->RegisterRslObject<FmtTypesNamespace>();
    RegisterObjList::inst()->RegisterRslObject<FmtKeyFlagsNamespace>();
    RegisterObjList::inst()->RegisterRslObject<FmtKeyNullValNamespace>();
    RegisterObjList::inst()->RegisterRslObject<ConnectionInfo>();
    RegisterObjList::inst()->RegisterRslObject<FmtTable>();
    RegisterObjList::inst()->RegisterRslObject<FmtField>();
    RegisterObjList::inst()->RegisterRslObject<FmtIndex>();
    RegisterObjList::inst()->RegisterRslObject<FmtSegment>();
}

void FmtRslModule::Init()
{

}

void FmtRslModule::Proc()
{
    addConstant("QRSD_DRIVER", QRSD_DRIVER);

    RegisterObjList::inst()->AddObject<ConnectionInfo>();
    RegisterObjList::inst()->AddObject<FmtTable>();
    RegisterObjList::inst()->AddObject<FmtField>(false);
    RegisterObjList::inst()->AddObject<FmtIndex>(false);
    RegisterObjList::inst()->AddObject<FmtSegment>(false);
    RegisterObjList::inst()->AddObject<FmtNamespace>(false);
    RegisterObjList::inst()->AddObject<FmtTypesNamespace>(false);
    RegisterObjList::inst()->AddObject<FmtKeyFlagsNamespace>(false);
    RegisterObjList::inst()->AddObject<FmtKeyNullValNamespace>(false);

    addConstant("Fmt", QVariant::fromValue<QObject*>(pFmtNamespace));
    addConstant("FmtType", QVariant::fromValue<QObject*>(pFmtTypesNamespace));
    addConstant("FmtKeyFlags", QVariant::fromValue<QObject*>(pFmtKeyFlags));
    addConstant("FmtKeyNullVal", QVariant::fromValue<QObject*>(pFmtKeyNullValFlags));
}
