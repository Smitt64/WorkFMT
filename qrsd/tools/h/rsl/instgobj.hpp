///////////////////////////////////////////////////////////////////////////////
// Tokmashev (04/05/2001)
// Copyright R-Style SoftWare Lab. 2001
//
// Поставщик объектов RslInstance
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __INSTGOBJ_HPP
#define __INSTGOBJ_HPP

///////////////////////////////////////////////////////////////////////////////

#include "rsl/objintf.hpp"

///////////////////////////////////////////////////////////////////////////////
#define  INST_MTD_PUSHMODULE          1
#define  INST_MTD_POPMODULE           2
#define  INST_MTD_CALL                3
#define  INST_MTD_TESTEXISTMODULE     4
#define  INST_MTD_GETNAME             5
#define  INST_MTD_CALLHOST            6

#define  INST_MTD_LASTNUMBER          7
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// TRslInstObjHost (в дальнейшем нужно сделать потоко безопасным)
///////////////////////////////////////////////////////////////////////////////
class TRslInstObjHost : public TRSLHostBase
{
 public:
   // --------------------------------------------
   TRslInstObjHost(HRD owner) : m_instOwner(owner) {}
   virtual ~TRslInstObjHost() {}

   // --------------------------------------------
   virtual int message(HRSLINST inst, IMESSAGE mes, void *ptr);

   // --------------------------------------------   
   void *operator new(size_t size);
   void  operator delete(void *ptr);

 private:
   // --------------------------------------------
   // --------------------------------------------
   HRD  m_instOwner;
};

///////////////////////////////////////////////////////////////////////////////
// TRslInstObjNameSpace
///////////////////////////////////////////////////////////////////////////////
class TRslInstanceObj;

// -----------------------------------------------------------------------------
class TRslInstObjNameSpace : public TRSLNameSpace
{
 public:
   // --------------------------------------------
   TRslInstObjNameSpace(TRslInstanceObj *instObj)
                       : m_instObj(instObj), m_idOutput(-1), m_idErrOutput(-1), m_idError(-1), m_idRunError(-1) {}

   // --------------------------------------------
   virtual ~TRslInstObjNameSpace() {}

   // --------------------------------------------
   RSLMSG_TABLE_BEGIN(TRSLNameSpace)
      RSLMSG_OUTPUT
      RSLMSG_ERR_OUTPUT
      RSLMSG_ERROR
      RSLMSG_RUN_ERROR
   RSLMSG_TABLE_END

   // --------------------------------------------
   int mes_IM_OUTPUT    (HRSLINST inst, char *ptr);
   int mes_IM_ERR_OUTPUT(HRSLINST inst, char *ptr);
   int mes_IM_ERROR     (HRSLINST inst, ERRINFO *err);
   int mes_IM_RUN_ERROR (HRSLINST ist,  ERRINFO *err);

   // --------------------------------------------
   void *operator new (size_t size);
   void  operator delete (void *ptr);

   // --------------------------------------------
   static TRSLMsgHandler *CreateNameSpace(void *initData);

 private:
   // --------------------------------------------
   // --------------------------------------------
   TRslInstanceObj  *m_instObj;
   long              m_idOutput;
   long              m_idErrOutput;
   long              m_idError;
   long              m_idRunError;
};

///////////////////////////////////////////////////////////////////////////////
// TRslInstanceObj
///////////////////////////////////////////////////////////////////////////////
class TGenObjProxyStub;

// -----------------------------------------------------------------------------
class TRslInstanceObj : public TGenObject
{
 public:
   // --------------------------------------------
   LOBJ  m_elem;

   // --------------------------------------------
   TRslInstanceObj();
   virtual ~TRslInstanceObj();

   // --------------------------------------------
   void Init(int delta);
   bool InitFromString(char *outputName);
   bool InitFromGenObject(TGenObject *obj);

   // --------------------------------------------
   // IRslObject -> TGenObject
   static unsigned short InstObjAddRef        (TGenObject *pObj);
   static unsigned short InstObjRelease       (TGenObject *pObj);
   static int            InstObjSet           (TGenObject *, const char *, VALUE *, long *);
   static int            InstObjSetId         (TGenObject *, long, VALUE *);
   static int            InstObjGet           (TGenObject *, const char *, VALUE *, long *);
   static int            InstObjGetId         (TGenObject *, long, VALUE *);
   static int            InstObjRun           (TGenObject *, const char *, long *);
   static int            InstObjRunId         (TGenObject *, long);
   static const char    *InstObjTypeName      (TGenObject *);
   static const char    *InstObjTypeNameCS    (TGenObject *);
   static uintptr_t      InstObjTypeId        (TGenObject *);
   static int            InstObjAttach        (TGenObject *, const char *, SYMPROC *);
   static long           InstObjGetUniqID     (TGenObject *pObj, const char *name);
   static int            InstObjMemberFromID  (TGenObject *pObj, long uniqID, long *id);
   static int            InstObjCvtToIDispatch(TGenObject *pObj);
   static int            InstObjGenProc       (TGenObject *pObj, int cmd, void *data);

   // --------------------------------------------
   static IRslObject   objTable;

   // --------------------------------------------
   void *operator new (size_t);
   void  operator delete (void *);

   // --------------------------------------------
   TInstPtr<TRSLObjInstIntf, TRslInstObjNameSpace> &GetInstance()
   {
    return m_instance;
   }

   // --------------------------------------------
   TGenObjProxyStub *TGenObjectToProxyStub(HRD instOwner, TGenObject *original, bool addRefInst = true);
   TGenObjProxyStub *FindProxyStub(TGenObject *obj);

   // --------------------------------------------
   void AddProxyStub   (TGenObjProxyStub *proxystub);
   void DetachProxyStub(TGenObjProxyStub *proxystub);

   // --------------------------------------------
   HRD GetOwnerInstance()
   {
    return m_instOwner;
   }

   // --------------------------------------------   
   FILE *GetFileOutput()
   {
    return m_file;
   }

   // --------------------------------------------   
   TGenObject *GetExtObj()
   {
    return m_extObj;
   }

   // --------------------------------------------   
   bool GetOwnerFlag()
   {
    return m_OwnerFlag;
   }

 protected:
   // --------------------------------------------
   bool CallInstObjMethod(long id);

   // --------------------------------------------
   bool InstObjPushModule(void);
   bool InstObjPopModule(void);
   bool InstObjCall(void);
   bool InstObjTestExistModule(void);
   bool InstObjGetName(void);
   bool InstObjCallHost(void);

   // --------------------------------------------
   static void DoneProxyStub(TGenObjProxyStub *ptr);

 private:
   // --------------------------------------------
   int FindMember(const char *name, long *id);
   const char *FindNameMember(long id);


   // --------------------------------------------
   // --------------------------------------------
   TInstPtr<TRSLObjInstIntf, TRslInstObjNameSpace>  m_instance;

   char              m_fileName[_MAX_PATH];
   FILE             *m_file;
   unsigned short    m_refCount;
   CNTLIST           m_listProxyStub;
   HRD               m_instOwner;
   TGenObject       *m_extObj;
   bool              m_OwnerFlag; // если флаг установлен, то обработчик пространства имен
                                  // перенаправляет все сообщения родительскому экземпляру RSL
};

///////////////////////////////////////////////////////////////////////////////
// TRslInstParams
///////////////////////////////////////////////////////////////////////////////
class TRslInstParams
{
 public:
   // --------------------------------------------
   TRslInstParams(TRslInstanceObj *inst, HRD instTo, HRD instFrom);
   virtual ~TRslInstParams();

   bool PushParams(int delta);
   bool PopParams (int delta);

   bool PushRetORSetParam(VALUE *val);
   bool PopRetORSetParam (VALUE *val, bool set);

   // --------------------------------------------
   VALUE *GetRetORSetParam()
   {
    return &m_retORset;
   }

   // --------------------------------------------
   VALUE *GetParams()
   {
    return m_ptrArrParams;
   }

   // --------------------------------------------   
   int GetNumParams()
   {
    return m_numParams;
   }

 private:
   // --------------------------------------------
   bool ValueSetFromValue(VALUE *from, VALUE *to, bool direct);
   bool SetParmFromValue(int index, VALUE *val);

   // --------------------------------------------
   // --------------------------------------------
   VALUE             m_retORset;
   VALUE            *m_ptrArrParams;
   int               m_numParams;
   HRD               m_instFrom;
   HRD               m_instTo;
   TRslInstanceObj  *m_inst;
};


///////////////////////////////////////////////////////////////////////////////
// TRslInstProxyStub
///////////////////////////////////////////////////////////////////////////////
class TGenObjProxyStub : public TGenObject
{
 public:
   // --------------------------------------------
   TGenObjProxyStub(TRslInstanceObj *inst, HRD instOwner, TGenObject *genObj, bool addRefInst = true);
   virtual ~TGenObjProxyStub();

   // --------------------------------------------
   // IRslObject -> TGenObject
   static unsigned short GenObjProxyStubAddRef      (TGenObject *pObj);
   static unsigned short GenObjProxyStubRelease     (TGenObject *pObj);
   static int            GenObjProxyStubSet         (TGenObject *, const char *, VALUE *, long *);
   static int            GenObjProxyStubSetId       (TGenObject *, long, VALUE *);
   static int            GenObjProxyStubGet         (TGenObject *, const char *, VALUE *, long *);
   static int            GenObjProxyStubGetId       (TGenObject *, long, VALUE *);
   static int            GenObjProxyStubRun         (TGenObject *, const char *, long *);
   static int            GenObjProxyStubRunId       (TGenObject *, long);
   static int            GenObjProxyStubIsParent    (TGenObject *, struct tagSYMCLASS *sym);
   static int            GenObjProxyStubCvtToIDisp  (TGenObject *pObj);
   static const char    *GenObjProxyStubTypeName    (TGenObject *);
   static uintptr_t      GenObjProxyStubTypeId      (TGenObject *);
   static int            GenObjProxyStubAttach      (TGenObject *, const char *, struct tagSYMP *);
   static long           GenObjProxyStubGetUniqID   (TGenObject *pObj, const char *name);
   static int            GenObjProxyStubMemberFromID(TGenObject *pObj, long uniqID, long *id);
   static int            GenObjProxyStubGenProc     (TGenObject *pObj, int cmd, void *data);
   static void          *GenObjProxyStubGetInterface(TGenObject *pObj, int kind);

   // --------------------------------------------
   // --------------------------------------------
   static IRslObject   objTable;

   // --------------------------------------------
   void *operator new(size_t);
   void  operator delete(void *);

   // --------------------------------------------
   TGenObject *GetTGenObject()
   {
    return m_genObj;
   }

   // --------------------------------------------
   TRslInstanceObj *GetRslInstanceObj()
   {
    return m_inst;
   }

   // --------------------------------------------   
   HRD GetInstOwner()
   {
    return m_instOwner;
   }

   // --------------------------------------------
   int ProxyStubObjInvoke(long id, int code, int nPar, VALUE *par, VALUE *propORret);
   int ProxyStubObjMemberFromName(const char *name, long *id);

 public:
   // --------------------------------------------
   // --------------------------------------------
   LOBJ  m_elem;

 private:
   // --------------------------------------------
   unsigned long AddRef();
   unsigned long Release();

   // --------------------------------------------
   TRslInstanceObj  *m_inst;
   HRD               m_instOwner;
   TGenObject       *m_genObj;
   unsigned long     m_refCount;
   bool              m_addRefInst;
};

///////////////////////////////////////////////////////////////////////////////

#endif // __INSTGOBJ_HPP

/* <-- EOF --> */