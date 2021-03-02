/*-----------------------------------------------------------------------*-

 File Name   : appintf.hpp

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: $
               $Revision: $
               $Date: $
               $Author: $
               $Modtime: $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 December 19,2001  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __APPINTF_HPP
#define __APPINTF_HPP

#include "fs.h"

// -----------------------------------------------------------------------------
#ifndef __RSCOM_HPP
    #if defined(__cplusplus) /* && defined(_INC_WINDOWS) */
        #include "rscom/rscom.hpp"
    #else
        #define  IProxyStubMgr  void
        #define  IRsObj         void
        #define  IChanel        void
        #define  IParmInfo      void
        #define  IMetaInfo      void
        #define  IChanelHost    void
        #define  ISiteObj       void
        #define  ICallCtx       void

        typedef long  RSDISPID;
        typedef long  TRsStat;

    #endif
#endif

#include <packpshn.h>

// -----------------------------------------------------------------------------
#ifndef TFSEVENT_DEFINED
    #define TFSEVENT_DEFINED

    typedef struct
          {
           int  key;
           int  kbflags;
           int  scan;
           int  Xm;
           int  Ym;
           int  Butn;
          } TFSEvent;
#endif


#ifndef _INC_WINDOWS
    typedef int  (* TFilterPtr)(void *curMsg, void *data);
#else
    typedef BOOL (* TFilterPtr)(MSG *curMsg, void *data);
#endif

typedef void (* TIdlePtr)(void *data, unsigned level);

// -----------------------------------------------
typedef struct tagTChConInfo1
      {
       short int       reserv; // must be 0
       short int       protocol;
       const char     *serverName;
       const char     *keyPath;
       const char     *pipeName;
       const char     *ip;
       const char     *spx;
       const char     *NBpref;
       short int       lana;
       unsigned short  port;
       short int       termNumber;
       const char     *user;
       const char     *domain;
       const char     *password;
       const char     *hostApp;
       short int       useCmdPort;
       short int       promptForUser;
      } TChConInfo1;

// -----------------------------------------------
typedef struct tagTChConInfoEx1
      {
       TChConInfo1  info;
       char         buff[512];
      } TChConInfoEx1;

// -----------------------------------------------
typedef struct tagTChConInfo2
      {
       short int       version;     // must be 1
       short int       protocol;
       const char     *serverName;
       const char     *keyPath;
       const char     *pipeName;
       const char     *ip;
       const char     *spx;
       const char     *NBpref;
       short int       lana;
       unsigned short  port;
       short int       termNumber;
       const char     *user;
       const char     *domain;
       const char     *password;
       const char     *hostApp;
       short int       useCmdPort;
       short int       promptForUser;
       const char     *sbF1;
       const char     *sbF2; 
       const char     *sbF3; 
       const char     *sbPwd;
       short int       useTM;
      } TChConInfo2;

// -----------------------------------------------
typedef struct tagTChConInfoEx2
      {
       TChConInfo2  info;
       char         buff[1024];
      } TChConInfoEx2;

// -----------------------------------------------   
typedef struct tagTChConInfo3
      {
       short int       version;     // must be 2
       short int       protocol;
       const char     *serverName;
       const char     *keyPath;
       const char     *pipeName;
       const char     *ip;
       const char     *spx;
       const char     *NBpref;
       short int       lana;
       unsigned short  port;
       short int       termNumber;
       const char     *user;
       const char     *domain;
       const char     *password;
       const char     *hostApp;
       short int       useCmdPort;
       short int       promptForUser;

       // Added in version 1.
       const char     *sbF1;
       const char     *sbF2; 
       const char     *sbF3; 
       const char     *sbPwd;
       short int       useTM;

       // Added in version 2.
       const char     *masterHost;
       unsigned short  masterPort;
       const char     *skipList;
      } TChConInfo3;

// -----------------------------------------------
typedef struct tagTChConInfoEx3
   {
   TChConInfo3  info;
   char         buff[1024];
   } TChConInfoEx3;

// -----------------------------------------------   
typedef struct tagTChConInfo4
      {
       short int       version;     // must be 3
       short int       protocol;
       const char     *serverName;
       const char     *keyPath;
       const char     *pipeName;
       const char     *ip;
       const char     *spx;
       const char     *NBpref;
       short int       lana;
       unsigned short  port;
       short int       termNumber;
       const char     *user;
       const char     *domain;
       const char     *password;
       const char     *hostApp;
       short int       useCmdPort;
       short int       promptForUser;

       // Added in version 1.
       const char     *sbF1;
       const char     *sbF2; 
       const char     *sbF3; 
       const char     *sbPwd;
       short int       useTM;

       // Added in version 2.
       const char     *masterHost;
       unsigned short  masterPort;
       const char     *skipList;

       // Added in version 3.
       short int       checkServerVersion;
      } TChConInfo4;

// -----------------------------------------------
typedef struct tagTChConInfoEx4
      {
       TChConInfo4  info;
       char         buff[1024];
      } TChConInfoEx4;

// -----------------------------------------------   
typedef struct tagTChConInfo5
      {
       short int       version;     // must be 4
       short int       protocol;
       const char     *serverName;
       const char     *keyPath;
       const char     *pipeName;
       const char     *ip;
       const char     *spx;
       const char     *NBpref;
       short int       lana;
       unsigned short  port;
       short int       termNumber;
       const char     *user;
       const char     *domain;
       const char     *password;
       const char     *hostApp;
       short int       useCmdPort;
       short int       promptForUser;

       // Added in version 1.
       const char     *sbF1;
       const char     *sbF2; 
       const char     *sbF3; 
       const char     *sbPwd;
       short int       useTM;

       // Added in version 2.
       const char     *masterHost;
       unsigned short  masterPort;
       const char     *skipList;

       // Added in version 3.
       short int       checkServerVersion;

       // Added in version 4.
       int             receiveDelay;
      } TChConInfo5;

// -----------------------------------------------
typedef struct tagTChConInfoEx5
      {
       TChConInfo5  info;
       char         buff[1024];
      } TChConInfoEx5;

// -----------------------------------------------   
// [!!!] Порядок внесения изменений в структуры tagTChConInfo/tagTChConInfoEx
//       "Старые" структуры нумеруются по шаблону вида:
//
//       typedef struct tagTChConInfo[N]
//             {
//              short int  version;     // must be N-1
//              ...
//             } TChConInfo[N];
//
//       typedef struct tagTChConInfoEx[N]
//          {
//           TChConInfo[N]  info;
//           char           buff[1024];
//          } TChConInfoEx[N];
//
//       Здесь [N] - максимальный номер "старой структуры"
//
//       - Текущие структуры tagTChConInfo/tagTChConInfoEx сохраняются по вышеуказанному
//         шаблону, вместо [N] присваивается номер, на единицу больший текущего максимального
//         номера "старой" структуры.
//
typedef struct tagTChConInfo
      {
       short int       version;     // must be 5
       short int       protocol;
       const char     *serverName;
       const char     *keyPath;
       const char     *pipeName;
       const char     *ip;
       const char     *spx;
       const char     *NBpref;
       short int       lana;
       unsigned short  port;
       short int       termNumber;
       const char     *user;
       const char     *domain;
       const char     *password;
       const char     *hostApp;
       short int       useCmdPort;
       short int       promptForUser;

       // Added in version 1.
       const char     *sbF1;
       const char     *sbF2; 
       const char     *sbF3; 
       const char     *sbPwd;
       short int       useTM;

       // Added in version 2.
       const char     *masterHost;
       unsigned short  masterPort;
       const char     *skipList;

       // Added in version 3.
       short int       checkServerVersion;

       // Added in version 4.
       int             receiveDelay;

       // Added in version 5.
       int             useClntNetInfo; // Использовать заданные сетевые параметры клиента
       TClientNetInfo2 clntNetInfo;    // Сетевые параметры клиента
      } TChConInfo;

// -----------------------------------------------
typedef struct tagTChConInfoEx
   {
   TChConInfo  info;
   char        buff[1024];
   } TChConInfoEx;

// -----------------------------------------------
typedef struct TAppObj
      {
       struct AppIntf  *vtbl;
      } TAppObj;

// -----------------------------------------------
typedef struct AppIntf
      {
       int    (* version) (TAppObj*);

       char  *(* getSendBuff)         (TAppObj *, size_t sz, const char *dllName, int cmd);
       void   (* sendMessage)         (TAppObj *, const char *mes);
       char  *(* transactMessage)     (TAppObj *, char *mes, size_t *sz);

       void  (* addFilterProc)        (TAppObj *obj, TFilterPtr proc, void *data);
       void  (* remFilterProc)        (TAppObj *obj, TFilterPtr proc);
       void  (* addIdleProc)          (TAppObj *obj, TIdlePtr proc, void *data);
       void  (* remIdleProc)          (TAppObj *obj, TIdlePtr proc);

       int   (* getEvent)             (TAppObj *obj, TFSEvent *ev, int waitTime);
       int   (* addEvent)             (TAppObj *obj, TFSEvent *ev, int useCapture);

       int   (* stopSession)          (TAppObj *obj, int type);

    // In version 3
       IProxyStubMgr *(* getPSmgr)    (TAppObj *obj);

       int   (* createServerObject)   (TAppObj *obj, const char *dllName, const char *servType, const char *objName,
                                       int iid, void **objPtr);


       char *(* getReplyBuff)         (TAppObj *, size_t sz);
       void  (* sendMessageCh)        (TAppObj *, const char *mes, IChanel *cn);
       char *(* transactMessageCh)    (TAppObj *, const char *mes, size_t *sz, IChanel *cn);
       IChanel *(* getDefaultChanel)  (TAppObj *);

       int   (* getRemoteServerObject)(TAppObj *obj,const char *dllName, const char * servType,const char *objName,
                                       int iid, void** ptr, IChanel *chanel);

       int   (* getLocalServerObject) (TAppObj *obj,const char *dllName, const char * servType,const char *objName,
                                       int iid, void** ptr);

       int   (* getServerObject)      (TAppObj *obj,const char *dllName, const char *servType, const char *objName,
                                       int iid, void **ptr, int isLocal, IChanel *chanel, IParmInfo *prm, RSDISPID id);

       int   (* getMetaObject)        (TAppObj *obj, const char *dllName, const char *servType, unsigned mask,
                                       IMetaInfo **ptr, int isLocal, IChanel *chanel);

       int   (* createMetaObject)     (TAppObj *obj, const char *dllName, const char *servType, unsigned mask,
                                       IMetaInfo **ptr);

       int   (* getAppType)           (TAppObj *obj);

       int   (* makeChanel)           (TAppObj *obj, TChConInfo *data, IChanel **ret);

       const char *(* getStatDesc)    (TAppObj *obj, int stat, char *buff, int sz, int codePage);

       int   (* fillConnectInfo)      (TAppObj *obj, const char *iniFile, TChConInfoEx *data);

    // In version 4
       const char *(* getUserName)    (TAppObj *obj, char *buff);
       const char *(* getHomeDir)     (TAppObj *obj, char *buff);

       int   (* isServerLoaded)       (TAppObj *obj, const char *dllName);

    // In version 5
       void  (* addClearProc)         (TAppObj *obj, TIdlePtr proc, void *data);
       void  (* remClearProc)         (TAppObj *obj, TIdlePtr proc);

       unsigned (* setAppFlags)       (TAppObj *obj, unsigned flags);

       unsigned (* getClearLevel)     (TAppObj *obj);

    // In version 9
       int   (* setExtPath)           (TAppObj *obj, const char *path, int isAdd);


    // In version 10
       int   (* peekEvent)            (TAppObj *obj, TFSEvent *ev);
       void  (* flushEvents)          (TAppObj *obj);

       int   (* makeNamedChanel)      (TAppObj *obj, TChConInfo *data, const char *name, IChanel **ret);

    // In version 11
       int   (* setTopMsgLoop)        (TAppObj *obj, int enable, uintptr_t *id);
       int   (* setThisMsgLoop)       (TAppObj *obj, int enable, uintptr_t id);

    // In version 12
       void       (*setSiteObj)       (TAppObj *obj, ISiteObj *site);
       ISiteObj * (*getSiteObj)       (TAppObj *obj);

    // In version 13
       void  (* remClearProcEx)       (TAppObj *obj, TIdlePtr proc, void *data);


    // In version 14
       char *(* transactMessageChMt)  (TAppObj *, const char *mes, size_t *sz, IChanel *cn, ICallCtx *ctx);
       void  (* sendMessageChMt)      (TAppObj *, const char *mes, IChanel *cn, ICallCtx *ctx);

       long  (* interlockedIncrement) (TAppObj *, long volatile *);
       long  (* interlockedDecrement) (TAppObj *, long volatile *);

       bool  (* addDispTable2)        (TAppObj *, const char *name, void *ptr);
      } TAppIntf;


#define  fs_appVersionA(ptr)                             ((TAppObj *)ptr)->vtbl->version((TAppObj *)ptr)
#define  fs_getSendBuffA(ptr, sz, tblName, cmd)          ((TAppObj *)ptr)->vtbl->getSendBuff((TAppObj *)ptr, sz, tblName, cmd)
#define  fs_getReplyBuffA(ptr, sz)                       ((TAppObj *)ptr)->vtbl->getReplyBuff((TAppObj *)ptr, sz)
#define  fs_sendMessageA(ptr, mes)                       ((TAppObj *)ptr)->vtbl->sendMessage((TAppObj *)ptr, mes)
#define  fs_transactMessageA(ptr, mes, sz)               ((TAppObj *)ptr)->vtbl->transactMessage((TAppObj *)ptr, mes, sz)

#define  fs_sendMessageChA(ptr, mes, cn)                 ((TAppObj *)ptr)->vtbl->sendMessageCh((TAppObj *)ptr, mes, cn)
#define  fs_sendMessageChMtA(ptr, mes, cn, ctx)          ((TAppObj *)ptr)->vtbl->sendMessageChMt((TAppObj *)ptr, mes, cn, ctx)
#define  fs_transactMessageChA(ptr, mes, sz, cn)         ((TAppObj *)ptr)->vtbl->transactMessageCh((TAppObj *)ptr, mes, sz, cn)
#define  fs_transactMessageChMtA(ptr, mes, sz, cn, ctx)  ((TAppObj *)ptr)->vtbl->transactMessageChMt((TAppObj *)ptr, mes, sz, cn, ctx)
#define  fs_getDefaultChanelA(ptr)                       ((TAppObj *)ptr)->vtbl->getDefaultChanel((TAppObj *)ptr)

#define  fs_addFilterProcA(ptr, proc, data)              ((TAppObj *)ptr)->vtbl->addFilterProc((TAppObj *)ptr, proc, data)
#define  fs_remFilterProcA(ptr, proc)                    ((TAppObj *)ptr)->vtbl->remFilterProc((TAppObj *)ptr, proc)
#define  fs_addIdleProcA(ptr, proc, data)                ((TAppObj *)ptr)->vtbl->addIdleProc((TAppObj *)ptr, proc, data)
#define  fs_remIdleProcA(ptr, proc)                      ((TAppObj *)ptr)->vtbl->remIdleProc((TAppObj *)ptr, proc)
#define  fs_addClearProcA(ptr, proc, data)               ((TAppObj *)ptr)->vtbl->addClearProc((TAppObj *)ptr, proc, data)
#define  fs_remClearProcA(ptr, proc)                     ((TAppObj *)ptr)->vtbl->remClearProc((TAppObj *)ptr, proc)
#define  fs_remClearProcExA(ptr, proc, data)             ((TAppObj *)ptr)->vtbl->remClearProcEx((TAppObj *)ptr, proc, data)

#define  fs_getEventAppA(ptr, ev, tm)                    ((TAppObj *)ptr)->vtbl->getEvent((TAppObj *)ptr, ev, tm)
#define  fs_addEventAppA(ptr, ev, capture)               ((TAppObj *)ptr)->vtbl->addEvent((TAppObj *)ptr, ev, capture)
#define  fs_peekEventAppA(ptr, ev)                       ((TAppObj *)ptr)->vtbl->peekEvent((TAppObj *)ptr, ev)
#define  fs_flushEventsAppA(ptr)                         ((TAppObj *)ptr)->vtbl->flushEvents((TAppObj *)ptr)


#define  fs_stopSessionA(ptr, doAbort)                   ((TAppObj *)ptr)->vtbl->stopSession((TAppObj *)ptr, doAbort)
#define  fs_processConsoleA(ptr, doProcess)              ((TAppObj *)ptr)->vtbl->processConsole((TAppObj *)ptr, doProcess)

#define  fs_getPSmgrA(ptr)                               ((TAppObj *)ptr)->vtbl->getPSmgr((TAppObj *)ptr)

#define  fs_getServerObjectA(ptr, dllName, servType, objName, iid, newob, isLocal, chanel, param, id) \
                                                         ((TAppObj *)ptr)->vtbl->getServerObject((TAppObj *)ptr, dllName, servType, objName, iid, (void **)newob, isLocal, chanel, param, id)

#define  fs_getMetaObjectA(ptr, dllName, servType, mask, newob, isLocal, chanel) \
                                                         ((TAppObj *)ptr)->vtbl->getMetaObject((TAppObj *)ptr, dllName, servType, mask, newob, isLocal, chanel)

#define  fs_getAppTypeA(ptr)                             ((TAppObj *)ptr)->vtbl->getAppType((TAppObj *)ptr)

#define  fs_getNamedChanelA(ptr, data, name, retCn)      ((TAppObj *)ptr)->vtbl->makeNamedChanel((TAppObj *)ptr, data, name, retCn)
#define  fs_makeChanelA(ptr, data, retCn)                ((TAppObj *)ptr)->vtbl->makeChanel((TAppObj *)ptr, data, retCn)
#define  fs_fillConnectInfoA(ptr, iniFile, data)         ((TAppObj *)ptr)->vtbl->fillConnectInfo((TAppObj *)ptr, iniFile, data)

#define  fs_getStatDescA(ptr, stat, buff, sz, codePage)  ((TAppObj *)ptr)->vtbl->getStatDesc((TAppObj *)ptr, stat, buff, sz, codePage)

#define  fs_getUserNameA(ptr, buff)                      ((TAppObj *)ptr)->vtbl->getUserName((TAppObj*)ptr, buff)
#define  fs_getHomeDirA(ptr, buff)                       ((TAppObj *)ptr)->vtbl->getHomeDir((TAppObj *)ptr, buff)

#define  fs_setAppFlagsA(ptr, flags)                     ((TAppObj *)ptr)->vtbl->setAppFlags((TAppObj *)ptr, flags)

#define  fs_addDispTable2A(ptr, name, p)                 ((TAppObj *)ptr)->vtbl->addDispTable2((TAppObj *)ptr, name, p)

// -----------------------------------------------
#define  RSCOM_DISABLE_WIN   1
#define  RSCOM_DISABLE_KBD   2
#define  RSCOM_DISABLE_MES   4
#define  RSCOM_USEMSGLOOP    8  // Use MsgLoop in transactMessage
#define  RSCOM_INDIC_ON     16  // Use MsgLoop in transactMessage
#define  RSCOM_INDIC_OFF    32  // Use MsgLoop in transactMessage
#define  RSCOM_NOBLOCKCALL  64

#define fs_getClearLevelA(ptr)               ((TAppObj *)ptr)->vtbl->getClearLevel((TAppObj *)ptr)

#define fs_setExtPath(ptr, path, isAdd)      ((TAppObj *)ptr)->vtbl->setExtPath((TAppObj *)ptr, path, isAdd)

#define fs_setTopMsgLoopA(ptr, enable, id)   ((TAppObj *)ptr)->vtbl->setTopMsgLoop((TAppObj *)ptr, enable, id)
#define fs_setThisMsgLoopA(ptr, enable, id)  ((TAppObj *)ptr)->vtbl->setThisMsgLoop((TAppObj *)ptr, enable, id)


#define fs_setSiteObjA(ptr, site)            ((TAppObj *)ptr)->vtbl->setSiteObj((TAppObj *)ptr, site)
#define fs_getSiteObjA(ptr)                  ((TAppObj *)ptr)->vtbl->getSiteObj((TAppObj *)ptr)


#define fs_interlockedIncrement(ptr, val)    ((TAppObj *)ptr)->vtbl->interlockedIncrement((TAppObj *)ptr, val)
#define fs_interlockedDecrement(ptr, val)    ((TAppObj *)ptr)->vtbl->interlockedDecrement((TAppObj *)ptr, val)

// -----------------------------------------------
#define  RSCOM_LOCAL    1
#define  RSCOM_REMOTE   0

#define  RSL_TERM_APP   0
#define  RSL_SERV_APP   1
#define  RSL_LOCAL_APP  2

#define  RSCOM_UNLOAD_EXT  1000

// -----------------------------------------------
#ifdef RSL_PL_MS
    #ifdef BUILDDLL
        #define  RSCOM_EXP  __declspec(dllexport)
    #elif defined (RS_INTERNAL)
        #define  RSCOM_EXP
    #else
        #define  RSCOM_EXP  __declspec(dllimport)
    #endif
#else
    #define RSCOM_EXP
#endif


#ifdef __cplusplus
extern "C" {
#endif

RSCOM_EXP int      InitRsCom(void);
RSCOM_EXP int      DoneRsCom(void);
RSCOM_EXP TAppObj *GetRsComAppIntf(void);
RSCOM_EXP int      HandleRsComCalls(int delayTime);

RSCOM_EXP TRsStat RSComCreateChanel     (TAppObj *app, TChConInfo *conInfo, IChanelHost *host, IChanel **ptr);
RSCOM_EXP TRsStat RSComCreateNamedChanel(TAppObj *app, TChConInfo *conInfo, IChanelHost *host, const char *name,
                                         IChanel **ptr);

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif 

/* <-- EOF --> */