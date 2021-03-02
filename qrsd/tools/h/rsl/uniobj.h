/*-----------------------------------------------------------------------*-

 File Name   : uniobj.h  

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: /RS-Tools/Interpreter RSL/rsl/uniobj.h $
               $Revision: 24 $
               $Date: 5.06.00 19:39 $
               $Author: Kubrin $
               $Modtime: 5.06.00 19:17 $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 January 14,1999  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __UNIOBJ_H
#define __UNIOBJ_H

#include <packpshn.h>


// -----------------------------------------------------------------------------
#define  RSLoffsetofcls(base, derived)  ((size_t)(static_cast<base *>((derived *)RSL_STD_ALIGNMENT)) - RSL_STD_ALIGNMENT)

// -----------------------------------------------------------------------------
typedef int   (*TElemProc)(void *objData, VALUE *val);
typedef void *(*TRslCtr)  (void *objData, TGenObject *pThis);

// -----------------------------------------------------------------------------
typedef struct
      {
       unsigned    size;

       int       (*getIdOfName)(void *, const char *parm, long *id);

       // return: 0 - ok, -1 error
       int       (*setId)      (void *, long id, VALUE *val);

       // return: 0 - property, 1 - method, 2 - method and already called, -1 - undefined parameter
       int       (*getId)      (void *, long id, VALUE *val);

       int       (*enumProc)   (void *, int cmd, TMemberInfo *info);
       int       (*getNProps)  (void *);
      } IRslAutoProps;


// -----------------------------------------------------------------------------
#define  RSL_MAKE_AUTOPROP(name, pref)                                             \
                                        IRslAutoProps  name =                      \
                                                              {                    \
                                                               sizeof(IRslObject), \
                                                               pref##GetIdOfName,  \
                                                               pref##SetId,        \
                                                               pref##GetId,        \
                                                               pref##EnumProc,     \
                                                               pref##NProps,       \
                                                              };

// -----------------------------------------------------------------------------
enum
   {
    RSL_TAG_METHOD,
    RSL_TAG_UNUSED,
    RSL_TAG_METHOD_PROP,
    RSL_TAG_CHAIN,
    RSL_TAG_PROPOFFS
   };

// -----------------------------------------------------------------------------
typedef struct
      {
       const char           *name;
       signed char           iniId; 

       unsigned char         tag;
       TElemProc             propGetOrRun;
       TElemProc             propPut;    // Can be NULL for read only property

       unsigned char         v_type;    // declared type of property or return val

       int                   nParm;    // Number of parms
       const TRslParmsInfo  *parInfo;    // Types of parms  (get and set have the same parms)

       long                  id;
      } TMethodEntry;

// -----------------------------------------------------------------------------
typedef struct
      {
       const char     *clsName;
       int             userSize;
       TMethodEntry   *entrys;

       bool            canInherit;
       bool            canCvtToIDisp;

       // was reserved 10 bytes
       unsigned char   ver; 
       unsigned char   alradyInited;
       IRslAutoProps  *autoPtr;
       int             numAuto;

       // New data
       long            id;  // first ID for autoproprs

       int             nOffsProps;
       int             nMethProps;
       int             nPropInd;
       int             nMethInd;
       int             nMethPropGet;
       int             nMethPropPut;
       int             nMeth;
       long            maxNumAuto;
      } TMethodTable;


// -----------------------------------------------------------------------------
typedef void         *(*TRslChainProc) (void *obj);
typedef TMethodTable *(*TGetParentTable)(void *obj);


// -----------------------------------------------------------------------------
#define  CHAIN_BASE_OFFS    0
#define  CHAIN_MEMBER_OFFS  1
#define  CHAIN_FUNC         2

// -----------------------------------------------------------------------------
#if defined(__cplusplus) && !defined(RSL_USE_C_IMPL)
    //  Must be the fist entry in table
    #define  RSL_CHAIN_BASE(BaseCls)                            { "Chain", -1, RSL_TAG_CHAIN, (TElemProc)&BaseCls##::_Table, NULL, CHAIN_BASE_OFFS, 0, NULL, (long)RSLoffsetofcls(BaseCls, thisCls_t) },
    #define  RSL_CHAIN_MEMBER(ClsName, member)                  { "Chain", -1, RSL_TAG_CHAIN, (TElemProc)&ClsName##::_Table, NULL, CHAIN_MEMBER_OFFS, 0, NULL, offsetof(thisCls_t, member) },
    #define  RSL_CHAIN_FUNC(ClsName)                            { "Chain", -1, RSL_TAG_CHAIN, (TElemProc)&ClsName##::_Table, (TElemProc)thunk_PtrTo##ClsName, CHAIN_FUNC, 0, NULL, 0 },

    // Can folow after previous entries
    #define  RSL_PROP_METH(name)                                {#name, -1, RSL_TAG_METHOD_PROP, thunk_get_##name, NULL, (unsigned char)V_UNDEF, 0, NULL },
    #define  RSL_PROP_METH2(name)                               {#name, -1, RSL_TAG_METHOD_PROP, thunk_get_##name, thunk_put_##name, (unsigned char)V_UNDEF, 0, NULL },
    #define  RSL_METH(name)                                     {#name, -1, RSL_TAG_METHOD, thunk_##name, NULL, (unsigned char)V_UNDEF, 0, NULL },

    #define  RSL_INIT                                           {"Init", RSL_DISP_CTRL, RSL_TAG_METHOD, thunk_Init, NULL, 0, 0, NULL },
    #define  RSL_FINAL                                          {"FinalRelease", RSL_DISP_FINAL, RSL_TAG_METHOD, thunk_Final, NULL, 0, 0, NULL },
    #define  RSL_POSTINIT                                       {"PostInit", RSL_DISP_POSTINIT, RSL_TAG_METHOD, thunk_PostInit, NULL, 0, 0, NULL },
                  

    #define  RSL_PROP(name, member)                             { #name, -1, RSL_TAG_PROPOFFS, NULL, NULL, V_UNDEF, offsetof(thisCls_t, member), NULL },

    #define  RSL_PROP_METH_EX(name, id, tp, attr, num, parms)   { #name, id, RSL_TAG_METHOD_PROP, thunk_get_##name, NULL, tp, num, parms, attr },
    #define  RSL_PROP_METH2_EX(name, id, tp, attr, num, parms)  { #name, id, RSL_TAG_METHOD_PROP, thunk_get_##name, thunk_put_##name, tp, num, parms, attr },
    #define  RSL_METH_EX(name, id, tp, attr, num, parms)        { #name, id, RSL_TAG_METHOD, thunk_##name, NULL, tp, num, parms, attr },
    #define  RSL_PROP_EX(name, member, id, tp, attr)            { #name, id, RSL_TAG_PROPOFFS, NULL, NULL, tp, offsetof(thisCls_t, member), NULL, attr },


    #define  RSL_ENTRY_TBL_END                                  { NULL, 0, 0, NULL, NULL, 0, 0, NULL }};


    #define  RSL_CTRDTR_IMPL                                                                                         \
                { "Constructor2", RSL_DISP_MAKE2, RSL_TAG_METHOD, (TElemProc)thunk_Constructor2, NULL, 0, 0, NULL }, \
                { "Constructor",  RSL_DISP_MAKE,  RSL_TAG_METHOD, thunk_Constructor, NULL, 0, 0, NULL },             \
                { "Destructor",   RSL_DISP_DTRL,  RSL_TAG_METHOD, thunk_Destructor, NULL, 0, 0, NULL },

    // -----------------------------------------------------------------------------
    template <class T>
    struct TThunk
    {
     public:
       // --------------------------------------------
       // --------------------------------------------
       T                    *obj;
       static  TMethodTable  _Table;
       static  TMethodEntry  _Entries[];

       // --------------------------------------------   
       static void *thunk_Constructor2(void *p, TGenObject *obj)
       {
        return ((TThunk<T> *)p)->obj = new T(obj);
       }

       // --------------------------------------------   
       static int thunk_Constructor(void *p, VALUE*)
       {
        ((TThunk<T> *)p)->obj = new T();
        
        return ((TThunk<T> *)p)->obj ? 0 : -1;
       }

       // --------------------------------------------   
       static int thunk_Destructor(void *p, VALUE *)
       {
        if(((TThunk<T> *)p)->obj)
          delete ((TThunk<T> *)p)->obj;
          
         return 0;
       }

       // --------------------------------------------   
       static void *thunk_PtrToImplClass(void *p)
       {
        return ((TThunk<T> *)p)->obj;
       }
    };

    // -----------------------------------------------------------------------------
    #define  THUNK_TABLE(clsName)                                                                                                                          \
                TMethodTable  clsName::thunk_t::_Table = { #clsName, sizeof(clsName::thunk_t), clsName::thunk_t::_Entries, 1, 1, 1 };                      \
                TMethodEntry  clsName::thunk_t::_Entries[] = {                                                                                             \
                   { "Chain", -1, RSL_TAG_CHAIN, (TElemProc)&clsName::_Table, (TElemProc)clsName::thunk_t::thunk_PtrToImplClass, CHAIN_FUNC, 0, NULL, 0 }, \
                   RSL_CTRDTR_IMPL                                                                                                                         \
                   RSL_ENTRY_TBL_END

    #define  RSL_IMPLEMENT_AUTOPROPS(clsName)  \
                RSL_MAKE_AUTOPROP(clsName##::_AutoPropTable, thunk_)

    #define  RSL_ENTRY_TBL_BEGIN(clsName)                                                                  \
                TMethodTable  clsName::_Table = { #clsName, sizeof(clsName), clsName::_Entries, 1, 1, 1 }; \
                TMethodEntry  clsName::_Entries[] = {

    #define  RSL_ENTRY_TBL_BEGIN_AUTOPROPS(clsName, number)                                                                                           \
                RSL_IMPLEMENT_AUTOPROPS(clsName)                                                                                                      \
                TMethodTable  clsName::_Table = { #clsName, sizeof(clsName), clsName::_Entries, 1, 1, 1, 0, &clsName##::_AutoPropTable, number, -1 }; \
                TMethodEntry  clsName::_Entries[] = {

    // -----------------------------------------------------------------------------
    #if _MSC_VER > 1100
        #define  PLACEMENTDELETE   void operator delete(void *, void *) {}
    #else 
        #define  PLACEMENTDELETE
    #endif

    // -----------------------------------------------------------------------------
    #define  RSL_CREATE_INPLACE(clsName)                                       \
                private:                                                       \
                   static  int thunk_Constructor(void *p, VALUE *obj)          \
                     { new(p) clsName(); return 0; }                           \
                   static  void *thunk_Constructor2(void *p, TGenObject *obj)  \
                     { return new(p) clsName(obj); }                           \
                   static  int thunk_Destructor(void *p, VALUE *)              \
                     { delete (clsName *)p; return 0;}                         \
                   void *operator new(size_t, void *p) { return p; }           \
                   PLACEMENTDELETE                                             \
                   void *operator new(size_t) { return NULL; }                 \
                   void operator delete(void *) {}


    #define  RSL_CREATE_EXTERN(clsName)     \
                public:                     \
                      typedef TThunk<clsName>      thunk_t;




    #define  RSL_CLASS_BEGIN_INPLACE(clsName) \
                TMethodTable  *clsName::TablePtr = &clsName::_Table;


    #define  RSL_CLASS_END_INPLACE          \
                RSL_CTRDTR_IMPL             \
                RSL_ENTRY_TBL_END

    #define  RSL_CLASS_BEGIN_EXTERN(clsName) \
                THUNK_TABLE(clsName)         \
                TMethodTable  *clsName::TablePtr = &clsName::thunk_t::_Table;

    #define  RSL_CLASS_END_EXTERN   RSL_ENTRY_TBL_END


    #define  RSL_DECLARE_CLASS(clsName)                 \
                public:                                 \
                    static  TMethodTable   *TablePtr;   \
                    static  TMethodTable    _Table;     \
                private:                                \
                    typedef clsName         thisCls_t;  \
                    static  TMethodEntry    _Entries[];


    #define RSL_CLASS_INPLACE(clsName)      \
               RSL_CREATE_INPLACE(clsName)  \
               RSL_DECLARE_CLASS(clsName)

    #define RSL_CLASS_EXTERN(clsName)       \
               RSL_CREATE_EXTERN(clsName)   \
               RSL_DECLARE_CLASS(clsName)


    #define RSL_DECLARE_AUTOPROPS(clsName)                                            \
               public:                                                                \
                  static  IRslAutoProps     _AutoPropTable;                           \
               private:                                                               \
                  static int thunk_EnumProc(void *p, int cmd, TMemberInfo *info)      \
                     { return ((thisCls_t *)p)->EnumProc(cmd, info); }                \
                  static int thunk_NProps(void *p)                                    \
                     { return ((thisCls_t *)p)->NAutoProps(); }                       \
                  static int thunk_GetIdOfName(void *p, const char *parm, long* id)   \
                     { return ((thisCls_t *)p)->GetIdOfName(parm, id); }              \
                  static int thunk_SetId(void *p,long id, VALUE *val)                 \
                     { return ((thisCls_t *)p)->SetAutoPropId(id, val); }             \
                  static int thunk_GetId(void *p,long id, VALUE *val)                 \
                     { return ((thisCls_t *)p)->GetAutoPropId(id, val); }             \
                                                                                      \
                  int EnumProc(int cmd, TMemberInfo *info);                           \
                  int NAutoProps();                                                   \
                  int GetIdOfName(const char *parm, long *id);                        \
                  int SetAutoPropId(long id, VALUE *val);                             \
                  int GetAutoPropId(long id, VALUE *val);


    #ifdef RSL_EXTERN_IMPL
        #define  RSL_CLASS(clsName)            RSL_CLASS_EXTERN(clsName)
        #define  RSL_CLASS_AUTOPROPS(clsName)  RSL_CLASS_EXTERN(clsName) RSL_DECLARE_AUTOPROPS(clsName)

        #define  RSL_CLASS_BEGIN(clsName)                                                          \
                                               RSL_CLASS_BEGIN_EXTERN(clsName)                     \
                                               RSL_ENTRY_TBL_BEGIN(clsName)

        #define  RSL_CLASS_BEGIN_AUTOPROPS(clsName, number)                                        \
                                               RSL_CLASS_BEGIN_EXTERN(clsName)                     \
                                               RSL_ENTRY_TBL_BEGIN_AUTOPROPS(clsName, number)

        #define  RSL_CLASS_END                 RSL_CLASS_END_EXTERN
    #else  // RSL_EXTERN_IMPL  (Inplace version)
        #define  RSL_CLASS(clsName)            RSL_CLASS_INPLACE(clsName)
        #define  RSL_CLASS_AUTOPROPS(clsName)  RSL_CLASS_INPLACE(clsName) RSL_DECLARE_AUTOPROPS(clsName)

        #define  RSL_CLASS_BEGIN(clsName)                                                          \
                                               RSL_CLASS_BEGIN_INPLACE(clsName)                    \
                                               RSL_ENTRY_TBL_BEGIN(clsName)

        #define  RSL_CLASS_BEGIN_AUTOPROPS(clsName, number)                                        \
                                               RSL_CLASS_BEGIN_INPLACE(clsName)                    \
                                               RSL_ENTRY_TBL_BEGIN_AUTOPROPS(clsName, number)

        #define  RSL_CLASS_END                 RSL_CLASS_END_INPLACE
    #endif  // RSL_EXTERN_IMPL


    #define  RSL_GETPROP_DECL(name)                                   \
                static int thunk_get_##name(void *p, VALUE *v)        \
                   { return ((thisCls_t *)p)->get_##name(v); }        \
                int get_##name(VALUE *retVal)

    #define  RSL_PUTPROP_DECL(name)                                   \
                static int thunk_put_##name(void *p, VALUE *v)        \
                   { return ((thisCls_t *)p)->put_##name(v); }        \
                int put_##name(VALUE *newVal)

    #define  RSL_METHOD_DECL(name)                                    \
                static int thunk_##name(void *p, VALUE *v)            \
                   { return ((thisCls_t *)p)->##name(v); }            \
                int name(VALUE *retVal)

    #define  RSL_INIT_DECL()                                          \
                static  int thunk_Init(void *p, VALUE *v)             \
                   { ((thisCls_t *)p)->Init((int *)v); return 0; }    \
                void Init(int *firstParmOffs)

    #define  RSL_FINAL_DECL()                                         \
                static  int thunk_Final(void *p, VALUE *v)            \
                   { ((thisCls_t *)p)->FinalRelease(); return 0; }    \
                void FinalRelease(void)

    #define  RSL_POSTINIT_DECL()                                      \
                static  int thunk_PostInit (void *p, VALUE *v)        \
                   { ((thisCls_t *)p)->PostInit(); return 0; }        \
                void PostInit(void)

    #define  RSL_GETPTRTO(ClsName)                                    \
                static void *thunk_PtrTo##ClsName(void *p)            \
                   { return ((thisCls_t *)p)->get_PtrTo##ClsName(); } \
                void *get_PtrTo##ClsName()


    // -----------------------------------------------------------------------------
    #define  GETPROP(p)   RSL_GETPROP_DECL(p)
    #define  PUTPROP(p)   RSL_PUTPROP_DECL(p)
    #define  METHOD(p)    RSL_METHOD_DECL(p)
    #define  GETPTRTO(p)  RSL_GETPTRTO(p)
    #define  INIT         RSL_INIT_DECL

    #define  RSL_GETPROP_IMP(cls, name)     int cls##::get_##name(VALUE *retVal)
    #define  RSL_PUTPROP_IMP(cls, name)     int cls##::put_##name(VALUE *newVal)
    #define  RSL_METHOD_IMP(cls, name)      int cls##::##name(VALUE *retVal)
    #define  RSL_INIT_IMP(cls)              void cls##::Init(int *firstParmOffs)
    #define  RSL_GETPTRTO_IMP(cls, parent)  void *cls##::get_PtrTo##parent ()

    #define  RSL_FINAL_IMP(cls)             void cls##::FinalRelease(void)

    #define  RSL_POSTINIT_IMP(cls)          void cls##::PostInit(void)
#else  // __cplusplus
    // -----------------------------------------------------------------------------
    #define  RSL_CLASS_BEGIN(clsName) \
                        TMethodEntry  clsName##Entries [] = {

    #define  RSL_PROP_METH(name, pref) \
                        { #name, -1, RSL_TAG_METHOD_PROP, get_##pref##name, NULL, V_UNDEF, 0, NULL },

    #define  RSL_PROP_METH_EX(name, id, tp, attr, num, parms, pref) \
                        { #name, id, RSL_TAG_METHOD_PROP, get_##pref##name, NULL, tp, num, parms, attr },

    #define  RSL_PROP_METH2(name, pref) \
                        { #name, -1, RSL_TAG_METHOD_PROP, get_##pref##name, put_##pref##name, V_UNDEF, 0, NULL },

    #define  RSL_PROP_METH2_EX(name, id, tp, attr, num, parms, pref) \
                        { #name, id, RSL_TAG_METHOD_PROP, get_##pref##name, put_##pref##name, tp, num, parms, attr },

    #define  RSL_METH(name, pref) \
                        { #name, -1, RSL_TAG_METHOD, pref##name, NULL, V_UNDEF, 0, NULL },

    #define  RSL_METH_EX(name, id, tp, attr, num, parms, pref) \
                        { #name, id, RSL_TAG_METHOD, pref##name, NULL, tp, num, parms, attr },


    #define  RSL_INIT(pref) \
                        { "Init",        RSL_DISP_CTRL, RSL_TAG_METHOD, pref##Init, NULL, 0, 0, NULL },

    #define  RSL_CTRL(pref) \
                        { "Constructor", RSL_DISP_MAKE, RSL_TAG_METHOD, pref##Constructor, NULL, 0, 0, NULL },
    
    #define  RSL_DSTR(pref) \
                        { "Destructor",  RSL_DISP_DTRL, RSL_TAG_METHOD, pref##Destructor, NULL, 0, 0, NULL },

    #define  RSL_CLASS_END(clsName)                         \
                        { NULL, 0, 0,NULL,NULL,0,0,NULL }}; \
                        TMethodTable  clsName##Table = { #clsName, sizeof(clsName), clsName##Entries, 1, 1, 1 };

    #define  RSL_CLASS_END_AUTOPROPS(clsName, number)                        \
                        {NULL, 0, 0, NULL, NULL, 0, 0, NULL }};              \
                        RSL_MAKE_AUTOPROP(clsName##_AutoPropTable, clsName)  \
                        TMethodTable  clsName##Table = { #clsName, sizeof(clsName), clsName##Entries, 1, 1, 1, 0, &clsName##_AutoPropTable, number, -1 };

    #define  RSL_CHAIN_MEMBER(ClsName, member, pref) \
                        {"Chain", -1, RSL_TAG_CHAIN, (TElemProc)&ClsName##_Table, NULL, CHAIN_MEMBER_OFFS, 0, NULL, offsetof(pref, member) },

    #define  RSL_CHAIN_FUNC(ClsName, pref)\
                        { "Chain", -1, RSL_TAG_CHAIN, (TElemProc)&ClsName##_Table, (TElemProc)get_##pref##PtrTo##ClsName, CHAIN_FUNC, 0, NULL, 0 },

    #define  RSL_PROP(name, member, ClsName) \
                        { #name, -1, RSL_TAG_PROPOFFS, NULL, NULL, V_UNDEF, offsetof(ClsName, member), NULL, 0 },

    #define  RSL_PROP_EX(name, member, id, tp, attr, ClsName) \
                        { #name, id, RSL_TAG_PROPOFFS, NULL, NULL, tp, offsetof(ClsName, member), NULL, attr },
#endif  // __cplusplus


#ifdef __cplusplus
extern "C" {
#endif
        void                  RslUnRegUniCls       (TGenClass *ptr);
        void                  RslRegUniCls         (TGenClass *ptr);
        void                  RslDelUniCls         (TGenClass *ptr);

        TGenClass            *RslMakeUniClass      (TMethodTable *clsInfo, bool visibleFromMacro);

        int                   RslGetNumObjectsOf   (TGenClass *cls);
                             
DLMLINK TGenClass  *RSLDLMAPI RslAddUniClass       (TMethodTable *clsInfo, bool visibleFromMacro);
DLMLINK TGenObject *RSLDLMAPI CreateObjectOfClass  (TGenClass *cls, TGenObject *outer);
DLMLINK void        RSLDLMAPI UniObjectInit        (TGenObject *obj, int firstParOffset);
DLMLINK TGenObject *RSLDLMAPI CreateObjectOfClassEx(TGenClass *cls, TGenObject *outer, void **userObj);
DLMLINK TGenClass  *RSLDLMAPI RslFindUniClass      (const char *typeName);
DLMLINK TGenClass  *RSLDLMAPI RslGetUniClass       (TGenObject *obj);
DLMLINK TGenObject *RSLDLMAPI RslUniCast           (const char *clsName, TGenObject *obj, void **userClass);

RSLLINK TGenClass  *RSLAPI    RslAddUniClassEx     (TMethodTable *clsInfo, bool creatable, bool canInherit);

RSLLINK void        RSLAPI    SetUpExtObject       (TGenObject *mainObj, TGenObject *obj);
       
#ifdef BUILD_DLM
    VALUE *DLMAPI RslUniPropPtr(int id, int *tp, int *stat);
#else
    VALUE        *RslUniPropPtr(int id, int *tp, int *stat);
#endif

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif  // __UNIOBJ_H

/* <-- EOF --> */