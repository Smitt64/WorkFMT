/*-----------------------------------------------------------------------*-

 File Name   : rsgenobj.h

 Sybsystem   :
 Description :    RSL Objects

 Source      :

 Library(s)  :

 VERSION INFO :   $Archive: /RS-Tools/Interpreter RSL/rsl/rsgenobj.h $
                  $Revision: 78 $
                  $Date: 12.09.00 17:29 $
                  $Author: Kubrin $
                  $Modtime: 12.09.00 15:43 $


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 October 23,1996  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __RSGENOBJ_H
#define __RSGENOBJ_H

#ifndef __ITYPES_H
#include "rsl/itypes.h"
#endif


#include <packpshn.h>

#ifdef __cplusplus
extern "C" {
#endif


// -----------------------------------------------------------------------------
typedef struct tagGetObject
      {
       struct tagIRslObject  *vtbl;
      } TGenObject;


// -----------------------------------------------------------------------------
typedef struct tagGenClass
      {
       struct tagIRslTypeInfo2  *vtbl;
       LOBJ                      elem;
       void                     *sym;   // Assoc RSL class object SYMCLASS or SYMEXTCLASS
      } TGenClass;


// -----------------------------------------------------------------------------
RSL_MAKE_HANDLE(CLNT_PRVD_HANDLE);


// -----------------------------------------------------------------------------
typedef struct tagICommonTable
      {
       unsigned         size;   // Size of this structure
       unsigned short (*AddRef) (TGenObject *);
       unsigned short (*Release)(TGenObject *);
      } ICommonTable;

// -----------------------------------------------------------------------------
typedef struct tagIRslTypeInfo2
      {
       unsigned           size;   // Size of this structure
       unsigned short   (*AddRef)       (TGenClass *);
       unsigned short   (*Release)      (TGenClass *);

       int              (*isProp)       (TGenClass *cls, const char *parm, long *id);
       int              (*getNProps)    (TGenClass *cls);
       int              (*getNMethods)  (TGenClass *cls);
       int              (*canInherit)   (TGenClass *cls);
       TGenObject      *(*create)       (TGenClass *cls, TGenObject *ctrl);
       const char      *(*typeName)     (TGenClass *cls);
       int              (*getNumObjects)(TGenClass *cls);
      } IRslTypeInfo2;

// -----------------------------------------------------------------------------
typedef struct
      {
       unsigned       size;   // Size of this structure

       int          (*isProp)     (const char *parm,long *id);
       int          (*getNProps)  (void);
       int          (*getNMethods)(void);
       int          (*canInherit) (void);
       TGenObject  *(*create)     (CLNT_PRVD_HANDLE, TGenObject *ctrl);
       const char  *(*typeName)   (void);
      } IRslTypeInfo;


// -----------------------------------------------------------------------------
typedef int            (*TInitRslProvider)(CLNT_PRVD_HANDLE *);
typedef void           (*TDoneRslProvider)(CLNT_PRVD_HANDLE);
typedef TGenObject    *(*TCreateRslObject)(CLNT_PRVD_HANDLE, const char *typeName);

typedef IRslTypeInfo  *(*TRslTypeProc)    (CLNT_PRVD_HANDLE, const char *typeName);
typedef TGenClass     *(*TRslClassProc)   (CLNT_PRVD_HANDLE, const char *typeName);

// -----------------------------------------------------------------------------
// Add provider for module, like AddStdProc
RSLLINK void RSLAPI AddObjectProviderMod  (TInitRslProvider init, TDoneRslProvider done, TCreateRslObject create);
RSLLINK void RSLAPI AddObjectProviderModEx(TInitRslProvider init, TDoneRslProvider done, TCreateRslObject create, TRslTypeProc tpProc);

#ifdef BUILD_DLM
    void DLMAPI AddObjectProviderModEx2(TInitRslProvider init, TDoneRslProvider done, TCreateRslObject create, TRslClassProc tpProc);
#else
    void        AddObjectProviderModEx2(TInitRslProvider init, TDoneRslProvider done, TCreateRslObject create, TRslClassProc tpProc);
#endif

// -----------------------------------------------------------------------------
typedef struct
      {
       unsigned char   v_type;
       unsigned char   flags;
      } TRslParmsInfo;


// -----------------------------------------------------------------------------
#define  RSLNP(par)  (sizeof(par)/sizeof(TRslParmsInfo))

// -----------------------------------------------------------------------------
typedef struct
      {
       const char           *name;
       int                   id;

       unsigned char         tp;
       unsigned char         flags;
       unsigned char         nParm;
       const TRslParmsInfo  *parInfo;
      } TPropMethTable;

// -----------------------------------------------------------------------------
typedef struct
      {
       int              num;
       TPropMethTable  *tbl;
      } TDescPropMeth;

// -----------------------------------------------------------------------------
typedef struct
      {
       int             nProp;
       TDescPropMeth  *prop;
       int             nMeth;
       TDescPropMeth  *meth;
      } TGenInfo;


// -----------------------------------------------------------------------------
#define  NMP(tp)  (sizeof(tp)/sizeof(tp[0]))

// -----------------------------------------------------------------------------
typedef struct
      {
       int   nOffsProps;  // props with offsets, not get/set methods!
       int   nMethProps;  // props with  get/set
       int   nMethPropGet;
       int   nMethPropPut;
       int   nMeth;
       int   nAutoProps;

       int   nFunc;
       int   nVars;
       int   nProps;
      } TClassInfo;

// -----------------------------------------------------------------------------
typedef struct
      {
       int                   ind;
       long                  id;

       const char           *name;

       unsigned char         declType;
       int                   nParm;    // Number of parms
       const TRslParmsInfo  *parInfo;

       unsigned char         kind;     // RSL_KIND_RUN, RSL_KIND_GET, RSL_KIND_SET
       unsigned char         flags;    // VAL_FLAG_RDONLY, VAL_FLAG_HIDE

       // AV 09.12.2010  наименование,  в исходном регистре, так как написано в макросе
       const char           *name_origin;
      } TMemberInfo;


// -----------------------------------------------------------------------------
#define  GOBJ_GETTYPEINFO    100
#define  GOBJ_FREETYPEINFO   101
#define  GOBJ_GETPROPBYIND   102
#define  GOBJ_GETVARBYIND    103
#define  GOBJ_GETFUNCBYIND   104
#define  GOBJ_MEMBERBYID     105
#define  GOBJ_FREEMEMBER     106
#define  GOBJ_CMPCLSID       107
#define  GOBJ_ISPROXYSTUB    108
#define  GOBJ_FINALRELEASE   109
#define  GOBJ_RSCOMINVOKE    110

#define  GOBJ_USEWEAKREF     111
#define  GOBJ_ADDWEAKREF     112
#define  GOBJ_REMWEAKREF     113

#define  GOBJ_POSTINIT       114

#define  GOBJ_ISEQCLASS      115 // Used by RCW wraper

#define  GOBJ_SETGENERATION  116
#define  GOBJ_MARK_OBJECT    117


// -----------------------------------------------------------------------------
int  GenTypeCalcEx     (TGenInfo *p, TClassInfo *info);
int  GenTypeGetPropEx  (int ind, TGenInfo *p, TMemberInfo *info);
int  GenTypeGetFuncEx  (int ind, TGenInfo *p, TMemberInfo *info);
int  GenTypeGetMemberEx(long id, TGenInfo *p, TMemberInfo *info);
int  GetTypeInfoProc   (TGenInfo *p, int cmd, void *data);
long GenTypeGetUniq    (TGenInfo *p, int id, int tp);
int  GenTypeGetId      (TGenInfo *p, long uniqID, long *id);

// -----------------------------------------------------------------------------
typedef struct
      {
       int             num;
       void           *enumData;

       const char     *name;
       long            id;

       unsigned char   declType;
       int             nParm;     // Number of parms
       unsigned char  *parTypes;  // Types of parms  (get and set have the same parms)


       unsigned char   kind;      // RSL_KIND_RUN, RSL_KIND_GET, RSL_KIND_SET
       unsigned char   flags;     // VAL_FLAG_RDONLY, VAL_FLAG_HIDE

       bool            reserv[10];
      } TEnumProps2;


// -----------------------------------------------------------------------------
#define  RSL_KIND_RUN      1
#define  RSL_KIND_GET      2
#define  RSL_KIND_SET      4
#define  RSL_KIND_VAR      8

#define  VAL_FLAG_RDONLY   1
#define  VAL_FLAG_OPTINAL  2  // For parms

#define  VAL_FLAG_HIDE     4


// -----------------------------------------------------------------------------
typedef struct
      {
       int           num;
       void         *enumData;

       const char   *name;
       long          id;
      } TEnumProps;


// -----------------------------------------------------------------------------
#define  GOBJ_INITENUM    0
#define  GOBJ_FREEENUM    1
#define  GOBJ_GETPROP     2

#define  GOBJ_ERRMODE     3

#define  GOBJ_INITENUM2   4
#define  GOBJ_GETPROP2    5
#define  GOBJ_GETPROPID   6
#define  GOBJ_FREEPROP    7


// -----------------------------------------------------------------------------
struct tagSYMCLASS;
struct tagSYMP;
struct tagSYMMODULE;


// -----------------------------------------------------------------------------
typedef struct tagIRslObject
      {
       unsigned          size;   // Size of this structure
       unsigned short  (*AddRef)            (TGenObject *);
       unsigned short  (*Release)           (TGenObject *);

       int             (*set)               (TGenObject *, const char *parm, VALUE *val, long *id);
       int             (*setId)             (TGenObject *, long id, VALUE *val);

       // return: 0 - property, 1 - method, -1 - undefined parameter
       int              (*get)              (TGenObject *, const char *parm, VALUE *val, long *id);
       int              (*getId)            (TGenObject *, long id, VALUE *val);

       int              (*run)              (TGenObject *, const char *method, long *id);
       int              (*runId)            (TGenObject *, long id);

       // Used by RSL object provider only
       int              (*isParent)         (TGenObject *, struct tagSYMCLASS *sym);

       int              (*canCvtToIDispatch)(TGenObject *);

       const char      *(*typeName)         (TGenObject *);
       const char      *(*typeNameCS)       (TGenObject *);  //AV 09.12.2010  TypeName  case sensitive

       uintptr_t        (*typeID)           (TGenObject *);

       int              (*attach)           (TGenObject *, const char *method, struct tagSYMP *sym);

       long             (*getUniqID)        (TGenObject *pObj, const char *name);
       int              (*memberFromID)     (TGenObject *pObj, long dispid, long *id);

       int              (*genProc)          (TGenObject *pObj, int cmd, void *data);

       void            *(*getInterface)     (TGenObject *pObj, int kind);
      } IRslObject;


// -----------------------------------------------------------------------------
#define  RSL_IID_CLASS     0
#define  RSL_IID_INNER     1
#define  RSL_IID_OUTER     2
#define  RSL_IID_USER      3

// -----------------------------------------------------------------------------
#define  RSL_MAKE_OBJ_TABLE2(name, pref)                                          \
                                          IRslObject  name =                      \
                                                             {                    \
                                                              sizeof(IRslObject), \
                                                              pref##AddRef,       \
                                                              pref##Release,      \
                                                              pref##Set,          \
                                                              pref##SetId,        \
                                                              pref##Get,          \
                                                              pref##GetId,        \
                                                              pref##Run,          \
                                                              pref##RunId,        \
                                                              NULL,               \
                                                              pref##CvtToIDisp,   \
                                                              pref##TypeName,     \
                                                              NULL,               \
                                                              pref##TypeId,       \
                                                              pref##Attach,       \
                                                              pref##GetUniqID,    \
                                                              pref##MemberFromID, \
                                                              pref##GenProc,      \
                                                              pref##GetInterface  \
                                                             }

// -----------------------------------------------------------------------------
#define  RSL_MAKE_OBJ_TABLE(name, pref)                                           \
                                         IRslObject  name =                       \
                                                            {                     \
                                                             sizeof(IRslObject),  \
                                                             pref##AddRef,        \
                                                             pref##Release,       \
                                                             pref##Set,           \
                                                             pref##SetId,         \
                                                             pref##Get,           \
                                                             pref##GetId,         \
                                                             pref##Run,           \
                                                             pref##RunId,         \
                                                             NULL,                \
                                                             NULL,                \
                                                             pref##TypeName,      \
                                                             pref##TypeNameCS,    \
                                                             pref##TypeId,        \
                                                             pref##Attach,        \
                                                             NULL,                \
                                                             NULL,                \
                                                             NULL                 \
                                                            }

// -----------------------------------------------------------------------------
#define  RSL_MAKE_TYPE_TABLE2(name, pref)                                                \
                                           IRslTypeInfo2  name =                         \
                                                                 {                       \
                                                                  sizeof(IRslTypeInfo2), \
                                                                  pref##ClsAddRef,       \
                                                                  pref##ClsRelease,      \
                                                                  pref##ClsFindMember,   \
                                                                  pref##ClsNprops,       \
                                                                  pref##ClsNMethods,     \
                                                                  pref##ClsCanInherit,   \
                                                                  pref##ClsCreate,       \
                                                                  pref##ClsTypeName,     \
                                                                  pref##getNumObjects    \
                                                                 }

// -----------------------------------------------------------------------------
#define  RSL_MAKE_TYPE_TABLE(name, pref)                                                 \
                                          IRslTypeInfo  name =                           \
                                                               {                         \
                                                                sizeof(IRslTypeInfo),    \
                                                                pref##FindMember,        \
                                                                pref##Nprops,            \
                                                                pref##NMethods,          \
                                                                pref##CanInherit,        \
                                                                NULL,                    \
                                                                NULL                     \
                                                               }


#define  P_GOBJ(ptr)   ((TGenObject *)(ptr))
#define  ADDREF(ptr)   (((TGenObject *)(ptr))->vtbl->AddRef((TGenObject *)(ptr)))
#define  RELEASE(ptr)  (((TGenObject *)(ptr))->vtbl->Release((TGenObject *)(ptr)))
#define  RSCLSID(ptr)  ((uintptr_t)((TGenObject *)(ptr))->vtbl)

// -----------------------------------------------------------------------------
// Do not public folowed data to DLM SDK !

#define  OBJFUN(ptr)                (((TGenObject *)(ptr))->vtbl)
#define  OBJTYPENAME(ptr)           (((TGenObject *)(ptr))->vtbl->typeName  ((TGenObject *)(ptr)))

//AV 09.12.2010 case sensitive
#define  OBJTYPENAME_CS(ptr)        (((TGenObject *)(ptr))->vtbl->typeNameCS((TGenObject *)(ptr)))
#define  OBJTYPEID(ptr)             (((TGenObject *)(ptr))->vtbl->typeID    ((TGenObject *)(ptr)))
#define  ISPPARENT(ptr, sym)        (((TGenObject *)(ptr))->vtbl->isParent  ((TGenObject *)(ptr), sym))
#define  PROPGET(ptr, nm, val, id)  (((TGenObject *)(ptr))->vtbl->get       ((TGenObject *)(ptr), nm, val, id))
#define  PROPGETID(ptr, id, val)    (((TGenObject *)(ptr))->vtbl->getId     ((TGenObject *)(ptr), id, val))
#define  PROPSET(ptr, nm, val, id)  (((TGenObject *)(ptr))->vtbl->set       ((TGenObject *)(ptr), nm, val, id))
#define  PROPSETID(ptr, id, val)    (((TGenObject *)(ptr))->vtbl->setId     ((TGenObject *)(ptr), id, val))
#define  MRUN(ptr, nm, id)          (((TGenObject *)(ptr))->vtbl->run       ((TGenObject *)(ptr), nm, id))
#define  MRUNID(ptr, id)            (((TGenObject *)(ptr))->vtbl->runId     ((TGenObject *)(ptr), id))
#define  MATTACH(ptr, nm, sym)      (((TGenObject *)(ptr))->vtbl->attach    ((TGenObject *)(ptr), nm, sym))
#define  INITENUM(ptr, data)        (((TGenObject *)(ptr))->vtbl->genProc   ((TGenObject *)(ptr), GOBJ_INITENUM, data))
#define  FREEENUM(ptr, data)        (((TGenObject *)(ptr))->vtbl->genProc   ((TGenObject *)(ptr), GOBJ_FREEENUM, data))
#define  ENUMPROP(ptr, data)        (((TGenObject *)(ptr))->vtbl->genProc   ((TGenObject *)(ptr), GOBJ_GETPROP, data))
#define  RSERRMODE(ptr, data)       (((TGenObject *)(ptr))->vtbl->genProc   ((TGenObject *)(ptr), GOBJ_ERRMODE, data))

#define  INITENUM2(ptr, data)       (((TGenObject *)(ptr))->vtbl->genProc   ((TGenObject *)(ptr), GOBJ_INITENUM2, data))
#define  ENUMPROP2(ptr, data)       (((TGenObject *)(ptr))->vtbl->genProc   ((TGenObject *)(ptr), GOBJ_GETPROP2, data))
#define  ENUMPROPID(ptr, data)      (((TGenObject *)(ptr))->vtbl->genProc   ((TGenObject *)(ptr), GOBJ_GETPROPID, data))
#define  FREEPROP(ptr, data)        (((TGenObject *)(ptr))->vtbl->genProc   ((TGenObject *)(ptr), GOBJ_FREEPROP, data))


// -----------------------------------------------------------------------------
typedef struct
      {
       LOBJ               elem;
       TInitRslProvider   init;
       TDoneRslProvider   done;
       TCreateRslObject   create;
       TRslTypeProc       tpProc;

       CLNT_PRVD_HANDLE   clntId;

       bool               isInited;

       TRslClassProc      clsProc;

       bool               m_bIsBlockMem;
      } TProviderInfo;


// We must call AddObjectProvider on handle IM_ADD_PROVIDER
RSLLINK void RSLAPI AddObjectProvider  (TInitRslProvider init, TDoneRslProvider done, TCreateRslObject create);
RSLLINK void RSLAPI AddObjectProviderEx(TInitRslProvider init, TDoneRslProvider done, TCreateRslObject create, TRslTypeProc tpProc);

void AddObjectProviderEx2(TInitRslProvider init, TDoneRslProvider done, TCreateRslObject create, TRslClassProc clsProc);

RSLLINK TGenObject  *RSLAPI GenObjCreate    (const char *typeName);
RSLLINK void         RSLAPI InitProvidersMod(struct tagSYMMODULE *sym);
RSLLINK void        *RSLAPI GenObjTypeInfo  (const char *typeName, TProviderInfo **provider, bool *isCls);


#ifndef BUILD_DLM
    TGenObject         *RslGetInnerObj(TGenObject *pObj);
#else
    TGenObject *DLMAPI  RslGetInnerObj(TGenObject *pObj);
#endif

DLMLINK TGenObject *RSLDLMAPI GenObjCreateEx      (const char *typeName, TGenObject *ctrlObj);
DLMLINK int         RSLDLMAPI RslObjMemberFromName(TGenObject *pObj, const char *name, long *id);
DLMLINK int         RSLDLMAPI RslObjInvoke        (TGenObject *pObj, long id, int code, int nPar, VALUE *par, VALUE *propORret);
DLMLINK long        RSLDLMAPI RslObjGetUniqID     (TGenObject *rslObj, const char *name);
DLMLINK int         RSLDLMAPI RslObjMemberFromID  (TGenObject *rslObj, long dispid, long *id);

        int                   RslObjInvokeEx      (TGenObject *pObj, long id, int code, int nPar, VALUE *par, VALUE *propORret, unsigned char *attr);


// -----------------------------------------------------------------------------
#define  RSL_DISP_RUN            0
#define  RSL_DISP_GET            1
#define  RSL_DISP_SET            2
#define  RSL_DISP_GET_OR_RUN     3
#define  RSL_DISP_RUN_NC         4
#define  RSL_DISP_GET_NC         5
#define  RSL_DISP_SET_NC         6
#define  RSL_DISP_GET_OR_RUN_NC  7

#define  RSL_DISP_INITEXT       -2
#define  RSL_DISP_CTRL          -3
#define  RSL_DISP_ENUM          -4
#define  RSL_ADVISE             -5
#define  RSL_UNADVISE           -6
#define  RSL_DISP_DTRL          -7
#define  RSL_DISP_MAKE          -8
#define  RSL_DISP_MAKE2         -9
#define  RSL_DISP_GETEXTCLAS   -10
#define  RSL_DISP_FINAL        -11
#define  RSL_DISP_POSTINIT     -12

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif // __RSGENOBJ_H

/* <-- EOF --> */