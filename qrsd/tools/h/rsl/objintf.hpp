/*************************************************************************
   File Name      : objintf.hpp           11.10.2000
   Programmer     : A.Tokmashev

   Subsystem      : Interpreter RSL
   Description    : Class wrapper for working with RSL-instance
                    and cache RSL-instances.

   Copyright(C) 1991 - 2000 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __OBJINTF_IMPL_HPP
#define __OBJINTF_IMPL_HPP

///////////////////////////////////////////////////////////////////////////////

#ifndef __RSEXCEPT_H
#include "rsexcept.h"
#endif // __RSEXCEPT_H


// -----------------------------------------------------------------------------
class RSLException : public RSException
{
 public:
   // --------------------------------------------
   RSLException(const char *str = "RSLException") : RSException(str)
   {
   }
};

///////////////////////////////////////////////////////////////////////////////
// TInstSwitch
///////////////////////////////////////////////////////////////////////////////
class TInstSwitch
{
 public:
   // --------------------------------------------
   TInstSwitch(HRD inst)
   {
    m_oldInst = RslSetInstance(inst);
   }

   // --------------------------------------------
  ~TInstSwitch()
   {
    RslSetInstance(m_oldInst);
   }

 private:
   // --------------------------------------------
   // --------------------------------------------
   HRD  m_oldInst;
};

// -----------------------------------------------------------------------------
class InstException : public RSLException
{
 public:
   // --------------------------------------------
   InstException(const char *str = "InstException") : RSLException(str)
   {
   }
};

///////////////////////////////////////////////////////////////////////////////
typedef void  (*rslTimerProc)(unsigned long, unsigned, void *);
///////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
#define  RSLMSG_TABLE_BEGIN(Base)                                                             \
                int Default(){ return Base::message(m_lastHRSLINST, m_lastMes, m_lastPtr ); } \
                virtual int message(HRSLINST inst, IMESSAGE mes, void* ptr )                  \
                { switch(mes) { default: return Default();

#define  RSLMSG_HANDLER(msg)               case msg:  return mes_##msg (inst, ptr );

#define  RSLMSG_HANDLER_NAME(name, msg)    case msg: return name (inst, mes, ptr );

#define  RSLMSG_TABLE_END       }}

///////////////////////////////////////////////////////////////////////////////
// Std Message
///////////////////////////////////////////////////////////////////////////////
#define  RSLMSG_BEGIN_PARSE             case IM_BEGIN_PARSE:            return mes_IM_BEGIN_PARSE(inst, (char *)ptr);
#define  RSLMSG_END_PARSE               case IM_END_PARSE:              return mes_IM_END_PARSE(inst, (bool *)ptr);
#define  RSLMSG_AGAIN_PARSE             case IM_AGAIN_PARSE:            return mes_IM_AGAIN_PARSE(inst, (char *)ptr);
#define  RSLMSG_NEW_LINE                case IM_NEW_LINE:               return mes_IM_NEW_LINE(inst, (unsigned *)ptr);
#define  RSLMSG_ERROR                   case IM_ERROR:                  return mes_IM_ERROR(inst, (ERRINFO *)ptr);
#define  RSLMSG_BEGIN_EXEC              case IM_BEGIN_EXEC:             return mes_IM_BEGIN_EXEC(inst, (char *)ptr);
#define  RSLMSG_END_EXEC                case IM_END_EXEC:               return mes_IM_END_EXEC(inst, (bool *)ptr);
#define  RSLMSG_INIT_SCANER             case IM_INIT_SCANER:            return mes_IM_INIT_SCANER(inst, (INIT_SCANER *)ptr);
#define  RSLMSG_INIT_EXECUTOR           case IM_INIT_EXECUTOR:          return mes_IM_INIT_EXECUTOR(inst, (INIT_EXECUTOR *)ptr);
#define  RSLMSG_DONE_EXECUTOR           case IM_DONE_EXECUTOR:          return mes_IM_DONE_EXECUTOR(inst, (bool *)ptr);
#define  RSLMSG_INPUT_DATA              case IM_INPUT_DATA:             return mes_IM_INPUT_DATA(inst, (INPUT_DATA *)ptr);
#define  RSLMSG_SET_STMODULE            case IM_SET_STMODULE:           return mes_IM_SET_STMODULE(inst, (ISTREAMDATA *)ptr);
#define  RSLMSG_SET_STRUC               case IM_SET_STRUC:              return mes_IM_SET_STRUC(inst, (SET_STRUC *)ptr);
#define  RSLMSG_REM_STRUC               case IM_REM_STRUC:              return mes_IM_REM_STRUC(inst, (SET_STRUC *)ptr);
#define  RSLMSG_RUN_ERROR               case IM_RUN_ERROR:              return mes_IM_RUN_ERROR(inst, (ERRINFO *)ptr);
#define  RSLMSG_NOMEM                   case IM_NOMEM:                  return mes_IM_NOMEM(inst, ptr); // ptr == NULL
#define  RSLMSG_EXEC_STATEMENT          case IM_EXEC_STATEMENT:         return mes_IM_EXEC_STATEMENT(inst, (STATEMENT *)ptr);
#define  RSLMSG_MODULE_OPEN             case IM_MODULE_OPEN:            return mes_IM_MODULE_OPEN(inst, ptr);  // ptr == NULL
#define  RSLMSG_MODULE_CLOSE            case IM_MODULE_CLOSE:           return mes_IM_MODULE_CLOSE(inst, ptr); // ptr == NULL
#define  RSLMSG_EXIT_MES                case IM_EXIT_MES:               return mes_IM_EXIT_MES(inst, (char *)ptr);
#define  RSLMSG_RUN_PROG                case IM_RUN_PROG:               return mes_IM_RUN_PROG(inst, (RUN_PARM *)ptr);
#define  RSLMSG_UNSERVED_ERROR          case IM_UNSERVED_ERROR:         return mes_IM_UNSERVED_ERROR(inst, (int *)ptr);
#define  RSLMSG_RUNMACRO                case IM_RUNMACRO:               return mes_IM_RUNMACRO(inst, ptr); // ptr == NULL
#define  RSLMSG_MEM_USAGE               case IM_MEM_USAGE:              return mes_IM_MEM_USAGE(inst, (MEM_USAGE *)ptr);
#define  RSLMSG_INIT_SCREEN             case IM_INIT_SCREEN:            return mes_IM_INIT_SCREEN(inst, ptr); // ptr == NULL
#define  RSLMSG_DONE_SCREEN             case IM_DONE_SCREEN:            return mes_IM_DONE_SCREEN(inst, ptr); // ptr == NULL
#define  RSLMSG_MESSAGE                 case IM_MESSAGE:                return mes_IM_MESSAGE(inst, (char *)ptr);
#define  RSLMSG_MESSAGE2                case IM_MESSAGE2:               return mes_IM_MESSAGE2(inst, (char *)ptr);
#define  RSLMSG_SEND_NOTIFY             case IM_SEND_NOTIFY:            return mes_IM_SEND_NOTIFY(inst, (char *)ptr);
#define  RSLMSG_EXIT_CALLED             case IM_EXIT_CALLED:            return mes_IM_EXIT_CALLED(inst, (int *)ptr);
#define  RSLMSG_MENU                    case IM_MENU:                   return mes_IM_MENU(inst, (MENU_DATA *)ptr);
#define  RSLMSG_INIT_PROGRESS           case IM_INIT_PROGRESS:          return mes_IM_INIT_PROGRESS(inst, (ProgressData *)ptr);
#define  RSLMSG_USE_PROGRESS            case IM_USE_PROGRESS:           return mes_IM_USE_PROGRESS(inst, (long *)ptr);
#define  RSLMSG_REM_PROGRESS            case IM_REM_PROGRESS:           return mes_IM_REM_PROGRESS(inst, ptr); // ptr == NULL
#define  RSLMSG_INIT_TRN                case IM_INIT_TRN:               return mes_IM_INIT_TRN(inst, (TRslProcessData *)ptr);
#define  RSLMSG_DONE_TRN                case IM_DONE_TRN:               return mes_IM_DONE_TRN(inst, (TRslProcessData *)ptr);
#define  RSLMSG_PROCESS_TRN             case IM_PROCESS_TRN:            return mes_IM_PROCESS_TRN(inst, (TRslProcessData *)ptr);
#define  RSLMSG_VIEW_FILE               case IM_VIEW_FILE:              return mes_IM_VIEW_FILE(inst, (TRslViewFile *)ptr);
#define  RSLMSG_CHANGE_OUTPUT           case IM_CHANGE_OUTPUT:          return mes_IM_CHANGE_OUTPUT(inst, (TChangeOutput *)ptr);
#define  RSLMSG_USERNUMBER              case IM_USERNUMBER:             return mes_IM_USERNUMBER(inst, ptr); // ptr == NULL
#define  RSLMSG_ADD_MODULE              case IM_ADD_MODULE:             return mes_IM_ADD_MODULE(inst, (SYMMODULE *)ptr);
#define  RSLMSG_SET_STMODULE_ADD        case IM_SET_STMODULE_ADD:       return mes_IM_SET_STMODULE_ADD(inst, (ISTREAMDATA *)ptr);
#define  RSLMSG_GETUSERDATA             case IM_GETUSERDATA:            return mes_IM_GETUSERDATA(inst, (TMesGetUserData *)ptr);
#define  RSLMSG_ADD_PROVIDER            case IM_ADD_PROVIDER:           return mes_IM_ADD_PROVIDER(inst, ptr); // ptr == NULL
#define  RSLMSG_SYSTEM_CALL             case IM_SYSTEM_CALL:            return mes_IM_SYSTEM_CALL(inst, (RSL_SYSTEM_CALL *)ptr);
#define  RSLMSG_FORMATSTR               case IM_FORMATSTR:              return mes_IM_FORMATSTR(inst, (RSL_FORMAT_STR *)ptr);
#define  RSLMSG_MSGBOX                  case IM_MSGBOX:                 return mes_IM_MSGBOX(inst, (char *)ptr);
#define  RSLMSG_ISSTANDALONE            case IM_ISSTANDALONE:           return mes_IM_ISSTANDALONE(inst, ptr); // ptr == NULL
#define  RSLMSG_TESTEVENT               case IM_TESTEVENT:              return mes_IM_TESTEVENT(inst, (RSL_EVENT_MES *)ptr);
#define  RSLMSG_ADDEVENT                case IM_ADDEVENT:               return mes_IM_ADDEVENT(inst, (RSL_EVENT_MES *)ptr);
#define  RSLMSG_ADD_STDPROC             case IM_ADD_STDPROC:            return mes_IM_ADD_STDPROC(inst, ptr); // ptr == NULL
#define  RSLMSG_ISGUI                   case IM_ISGUI:                  return mes_IM_ISGUI(inst, ptr); // ptr == NULL
#define  RSLMSG_ABORT_ON_PARSE_ERROR    case IM_ABORT_ON_PARSE_ERROR:   return mes_IM_ABORT_OM_PARSE_ERROR(inst, (ERRINFO *)ptr);
#define  RSLMSG_CLEAR_INSTANCE          case IM_CLEAR_INSTANCE:         return mes_IM_CLEAR_INSTANCE(inst, (HRD)ptr);
#define  RSLMSG_ADD_NAMERESOLVER        case IM_ADD_NAMERESOLVER:       return mes_IM_ADD_NAMERESOLVER(inst, (SYMMODULE *)ptr);
#define  RSLMSG_AX_ERROR                case IM_AX_ERROR:               return mes_IM_AX_ERROR(inst, (TRslAxError *)ptr);
#define  RSLMSG_SET_TIMER_PROC          case IM_SET_TIMER_PROC:         return mes_IM_SET_TIMER_PROC(inst, (rslTimerProc)ptr);
#define  RSLMSG_REM_TIMER_PROC          case IM_REM_TIMER_PROC:         return mes_IM_REM_TIMER_PROC(inst, (rslTimerProc)ptr);
#define  RSLMSG_EXTERN_SRC              case IM_EXTERN_SRC:             return mes_IM_EXTERN_SRC(inst, (TExtSrcInfo *)ptr);
#define  RSLMSG_FILELOCK                case IM_FILELOCK:               return mes_IM_FILELOCK(inst, ptr); // ptr == NULL
#define  RSLMSG_SET_TRN_FLAG            case IM_SET_TRN_FLAG:           return mes_IM_SET_TRN_FLAG(inst, (TRslProcessData *)ptr);
#define  RSLMSG_GET_TRN_FLAG            case IM_GET_TRN_FLAG:           return mes_IM_GET_TRN_FLAG(inst, (int *)ptr);
#define  RSLMSG_CLR_TRN_FLAG            case IM_CLR_TRN_FLAG:           return mes_IM_CLR_TRN_FLAG(inst, (TRslProcessData *)ptr);
#define  RSLMSG_POPSTATE_NOT_USED       case IM_POPSTATE_NOT_USED:      return mes_IM_POPSTATE_NOT_USED(inst, ptr); // ????
#define  RSLMSG_BREAKPOINT              case IM_BREAKPOINT:             return mes_IM_BREAKPOINT(inst, ptr); // ptr == void*
#define  RSLMSG_REM_EMPTY_MODULE        case IM_REM_EMPTY_MODULE:       return mes_IM_REM_EMPTY_MODULE(inst, (SYMMODULE *)ptr);
#define  RSLMSG_ADD_MODULE2             case IM_ADD_MODULE2:            return mes_IM_ADD_MODULE2(inst, (SYMMODULE *)ptr);
#define  RSLMSG_GET_CMD_ARG             case IM_GET_CMD_ARG:            return mes_IM_GET_CMD_ARG(inst, (RSL_FORMAT_STR *)ptr);
#define  RSLMSG_BEGIN_ADDEXEC           case IM_BEGIN_ADDEXEC:          return mes_IM_BEGIN_ADDEXEC(inst, ptr); // ptr == NULL
#define  RSLMSG_END_ADDEXEC             case IM_END_ADDEXEC:            return mes_IM_END_ADDEXEC(inst, ptr); // ptr == NULL
#define  RSLMSG_OPEN_RESFILE            case IM_OPEN_RESFILE:           return mes_IM_OPEN_RESFILE(inst, (TMesResData *)ptr);
#define  RSLMSG_CLOSE_RESFILE           case IM_CLOSE_RESFILE:          return mes_IM_CLOSE_RESFILE(inst, (TMesResData *)ptr);
#define  RSLMSG_GET_RESPANEL            case IM_GET_RESPANEL:           return mes_IM_GET_RESPANEL(inst, (TMesPanelData *)ptr);
#define  RSLMSG_FREE_RESPANEL           case IM_FREE_RESPANEL:          return mes_IM_FREE_RESPANEL(inst, (TMesPanelData *)ptr);
#define  RSLMSG_BTMES                   case IM_BTMES:                  return mes_IM_BTMES(inst, (TBtMesData *)ptr);
#define  RSLMSG_ADD_GLOBAL_DLMS         case IM_ADD_GLOBAL_DLMS:        return mes_IM_ADD_GLOBAL_DLMS(inst, (ISTREAMDATA *)ptr);
#define  RSLMSG_MODE_PCODE              case IM_MODE_PCODE:             return mes_IM_MODE_PCODE(inst, (int *)ptr);
#define  RSLMSG_PCODE_MESSAGE           case IM_PCODE_MESSAGE:          return mes_IM_PCODE_MESSAGE(inst, (char *)ptr);
#define  RSLMSG_CONTINUERUN             case IM_CONTINUERUN:            return mes_IM_CONTINUERUN(inst, (int *)ptr);
#define  RSLMSG_GETWND                  case IM_GETWND:                 return mes_IM_GETWND(inst, (HWND *)ptr);
#define  RSLMSG_GETDBGMODE              case IM_GETDBGMODE:             return mes_IM_GETDBGMODE(inst, ptr); // ptr == NULL
#define  RSLMSG_TRACE                   case IM_TRACE:                  return mes_IM_TRACE(inst, (char *)ptr);
#define  RSLMSG_REM_MODULE              case IM_REM_MODULE:             return mes_IM_REM_MODULE(inst, (SYMMODULE *)ptr);
#define  RSLMSG_GETTRACEFLAG            case IM_GETTRACEFLAG:           return mes_IM_GETTRACEFLAG(inst, ptr); // ptr == NULL
#define  RSLMSG_SETTRACEFLAG            case IM_SETTRACEFLAG:           return mes_IM_SETTRACEFLAG(inst, (short *)ptr);
#define  RSLMSG_REM_MODULE2             case IM_REM_MODULE2:            return mes_IM_REM_MODULE2(inst, (SYMMODULE *)ptr);

#define  RSLMSG_GETSTDSTRM              case IM_GETSTDSTRM:             return mes_IM_GETSTDSTRM(inst, (TRslStdStrm *)ptr);
#define  RSLMSG_OUTPUT                  case IM_OUTPUT:                 return mes_IM_OUTPUT(inst, (char *)ptr);
#define  RSLMSG_ERR_OUTPUT              case IM_ERR_OUTPUT:             return mes_IM_ERR_OUTPUT(inst, (char *)ptr);
#define  RSLMSG_BTRMACFILE              case IM_BTRMACFILE:             return mes_IM_BTRMACFILE(inst, (char *)ptr);

#define  RSLMSG_CACHE_LINKINST          case IM_CACHE_LINKINST:         return mes_IM_CACHE_LINKINST(inst, (HRD)ptr);
#define  RSLMSG_CACHE_UNLINKINST        case IM_CACHE_UNLINKINST:       return mes_IM_CACHE_UNLINKINST(inst, (HRD)ptr);
#define  RSLMSG_GETHOST                 case IM_GETHOST:                return mes_IM_GETHOST(inst, (TRSLHostData *)ptr);
#define  RSLMSG_INSPECT_SYM             case IM_INSPECT_SYM:            return mes_IM_INSPECT_SYM(inst, (InspectSymData *)ptr);
#define  RSLMSG_ADD_COMMON              case IM_ADD_COMMON:             return mes_IM_ADD_COMMON(inst, (TNameSpaceData *)ptr);
#define  RSLMSG_ADD_COMMONSYS           case IM_ADD_COMMONSYS:          return mes_IM_ADD_COMMONSYS(inst, (TNameSpaceData *)ptr);
#define  RSLMSG_DBG_ERROR               case IM_DBG_ERROR:              return mes_IM_DBG_ERROR(inst, (ERRINFO *)ptr);
#define  RSLMSG_SET_EXITFLAG            case IM_SET_EXITFLAG:           return mes_IM_SET_EXITFLAG(inst, ptr); // ptr == NULL
#define  RSLMSG_GETCACHE                case IM_GETCACHE:               return mes_IM_GETCACHE(inst, (TRSLCacheData *)ptr);
#define  RSLMSG_DEL_NAMESPACE           case IM_DEL_NAMESPACE:          return mes_IM_DEL_NAMESPACE(inst, ptr); // ptr == NULL
#define  RSLMSG_GET_NAMESPACE           case IM_GET_NAMESPACE:          return mes_IM_GET_NAMESPACE(inst, (TObjNameSpace *)ptr);
#define  RSLMSG_CHANGE_ERRORS           case IM_CHANGE_ERRORS:          return mes_IM_CHANGE_ERRORS(inst, ptr); // ptr == NULL
#define  RSLMSG_BEGIN_GEN               case IM_BEGIN_GEN:              return mes_IM_BEGIN_GEN(inst, (char*)ptr);
#define  RSLMSG_END_GEN                 case IM_END_GEN:                return mes_IM_END_GEN(inst, (bool *)ptr);
#define  RSLMSG_INIT_INSTANCE           case IM_INIT_INSTANCE:          return mes_IM_INIT_INSTANCE(inst, ptr); // ptr == NULL
#define  RSLMSG_DONE_INSTANCE           case IM_DONE_INSTANCE:          return mes_IM_DONE_INSTANCE(inst, ptr); // ptr == NULL

//Add by Ushakov
#define  RSLMSG_INIT_INSTANCE_REC       case IM_INIT_INSTANCE_REC:      return mes_IM_INIT_INSTANCE_REC(inst, ptr); // ptr == NULL
#define  RSLMSG_DONE_INSTANCE_REC       case IM_DONE_INSTANCE_REC:      return mes_IM_DONE_INSTANCE_REC(inst, ptr); // ptr == NULL
//

#define  RSLMSG_INIT_FS                 case IM_INIT_FS:                return mes_IM_INIT_FS(inst, (TFsInitData *)ptr);
#define  RSLMSG_END_SPECEXP             case IM_END_SPECEXP:            return mes_IM_END_SPECEXP(inst, ptr); // ptr == NULL
#define  RSLMSG_REWIND                  case IM_REWIND:                 return mes_IM_REWIND(inst, (TRewindData *)ptr);
#define  RSLMSG_SAVE_STATE              case IM_SAVE_STATE:             return mes_IM_SAVE_STATE(inst, ptr); // ptr = NULL
#define  RSLMSG_RESTORE_STATE           case IM_RESTORE_STATE:          return mes_IM_RESTORE_STATE(inst, ptr); // ptr = NULL
#define  RSLMSG_GET_INSTANCE            case IM_GET_INSTANCE:           return mes_IM_GET_INSTANCE(inst, (TRslInstData *)ptr);
#define  RSLMSG_PROXY_MESSAGE           case IM_PROXY_MESSAGE:          return mes_IM_PROXY_MESSAGE(inst, ptr);
#define  RSLMSG_EXIST_DBT_FILE_MODE     case IM_EXIST_DBT_FILE_MODE:    return mes_IM_EXIST_DBT_FILE_MODE(inst, ptr);
#define  RSLMSG_EXIST_DBT_FILE          case IM_EXIST_DBT_FILE:         return mes_IM_EXIST_DBT_FILE(inst, ptr);
#define  RSLMSG_GETSTACKSIZE            case IM_GETSTACKSIZE:           return mes_IM_GETSTACKSIZE(inst, ptr); // ptr == NULL
#define  RSLMSG_DB_CONVERT_MODE         case IM_DB_CONVERT_MODE:        return mes_IM_DB_CONVERT_MODE(inst, ptr);
#define  RSLMSG_GET_SYSTEMINFO          case IM_GET_SYSTEMINFO:         return mes_IM_GET_SYSTEMINFO(inst, (char *)ptr);

///////////////////////////////////////////////////////////////////////////////
//
// class TRSLObjMsgHandler
//
///////////////////////////////////////////////////////////////////////////////

class TRSLObjMsgHandler : public TRSLMsgHandler
{
 private:
   // --------------------------------------------
   TRSLObjMsgHandler(const TRSLObjMsgHandler &)
   {
   }

   // --------------------------------------------
   TRSLObjMsgHandler &operator=(const TRSLObjMsgHandler &);


 protected:
   // --------------------------------------------
   TRSLObjMsgHandler() : m_lastHRSLINST(NULL), m_lastMes(-1), m_lastPtr(NULL)
   {
    msgProc = msgproc;

    pNextHandler = NULL;
   }

   // --------------------------------------------
   virtual ~TRSLObjMsgHandler()
   {
    /*SobjUnlinkSys(&parent);*/
   }

 protected:
   // --------------------------------------------
   virtual int message(HRSLINST inst, IMESSAGE mes, void *ptr)
   {
    return Default();
   }

   // --------------------------------------------
   int Default()
   {
    return 0;
   }

   // --------------------------------------------
   virtual TRSLMsgHandler *GetNextMsgHandler()
   {
    return pNextHandler;
   }

   // --------------------------------------------
   static int msgproc(TRSLMsgHandler *handler, HRSLINST inst, IMESSAGE mes, void *ptr)
   {
    if(handler != NULL)
     {
      TRSLObjMsgHandler  *_This = STATIC_CAST(TRSLObjMsgHandler *, handler);
      int                 nResult;


      _This->m_lastHRSLINST = inst;
      _This->m_lastMes      = mes;
      _This->m_lastPtr      = ptr;

      nResult = _This->message(inst, mes, ptr);

      if(nResult)
        return nResult;

      TRSLMsgHandler  *next = _This->GetNextMsgHandler();

      if(next && next->msgProc)
        return next->msgProc(next, inst, mes, ptr);
     }

    return 0;
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   HRSLINST    m_lastHRSLINST;
   IMESSAGE    m_lastMes;
   void       *m_lastPtr;
};

///////////////////////////////////////////////////////////////////////////////
// For Message Handler NameSpace
///////////////////////////////////////////////////////////////////////////////
class TRSLNameSpace : public TRSLObjMsgHandler
{
   // --------------------------------------------
   TRSLNameSpace(const TRSLNameSpace &)
   {
   }

   // --------------------------------------------
   TRSLNameSpace &operator=(const TRSLNameSpace &)
   {
    return *this;
   }

 protected:
   // --------------------------------------------
   TRSLNameSpace()
   {
   }

   // --------------------------------------------
   virtual ~TRSLNameSpace()
   {
   }

   // --------------------------------------------
   RSLMSG_TABLE_BEGIN(TRSLObjMsgHandler)
      RSLMSG_DEL_NAMESPACE
      RSLMSG_GET_NAMESPACE
   RSLMSG_TABLE_END

   // --------------------------------------------
   int mes_IM_DEL_NAMESPACE(HRSLINST inst, void *ptr)
   {
    delete this;

    return 1;
   }

   // --------------------------------------------
   int mes_IM_GET_NAMESPACE(HRSLINST inst, TObjNameSpace *ptr)
   {
    if(ptr)
      ptr->nameSpace = STATIC_CAST(TRSLMsgHandler *, this);

    return 1;
   }

 public:
   // --------------------------------------------
   static TRSLMsgHandler *CreateNameSpace(void * /*initData*/)
   {
    return NULL;
   }
};

///////////////////////////////////////////////////////////////////////////////
// For Host
// class TRSLHostBase
///////////////////////////////////////////////////////////////////////////////
class TRSLHostBase : public TRSLObjMsgHandler
{
 private:
   // -------------------------------------------- 
   TRSLHostBase(const TRSLHostBase &)
   {
   }

   // --------------------------------------------   
   TRSLHostBase &operator=(const TRSLHostBase &)
   {
    return *this;
   }

 protected:
   // --------------------------------------------
   TRSLHostBase(TRSLMsgHandler *pParent = NULL)/* : m_parentHost(parent)*/
   {
    pNextHandler = pParent;
   }

   // --------------------------------------------
   virtual ~TRSLHostBase()
   {
   }
};

///////////////////////////////////////////////////////////////////////////////
//
// class TRSLObjInstIntf
//
///////////////////////////////////////////////////////////////////////////////

class TRSLObjInstIntf
{
 private:
   // --------------------------------------------
   TRSLObjInstIntf(const TRSLObjInstIntf &)
   {
   }

   // --------------------------------------------
   TRSLObjInstIntf &operator=(const TRSLObjInstIntf &)
   {
    return *this;
   }

 protected:
   // --------------------------------------------
   TRSLObjInstIntf(HRSLINST inst = NULL)
   {
    m_hInst = inst;
   }

   // --------------------------------------------   
   virtual ~TRSLObjInstIntf()
   {
   }

 public:
   // --------------------------------------------
   bool AddModule(const char *name)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->AddModule(m_hInst, name) : false);
   }

   // --------------------------------------------
   bool PushModule(const char *name, bool privateMode = false, bool notExecute = false)
   {
    if(m_hInst)
     {
      if(!notExecute)
        return HRSLINST_FUN(m_hInst)->PushModule(m_hInst, name, privateMode);
      else
        return HRSLINST_FUN(m_hInst)->PushModuleNotExec(m_hInst, name, privateMode);
     }

    return false;
   }

   // --------------------------------------------   
   bool PopModule()
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->PopModule(m_hInst) : false);
   }

   // --------------------------------------------
   bool Execute()
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->Execute(m_hInst) : false);
   }

   // --------------------------------------------   
   void Stop()
   {
    if(m_hInst)
      HRSLINST_FUN(m_hInst)->Stop(m_hInst);
   }

   // --------------------------------------------   
   RSLINST_STATE GetState()
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->GetState(m_hInst) : STATE_UNKNOWN);
   }

   // --------------------------------------------
   int GetCountError()
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->GetCountError(m_hInst) : 0);
   }

   // --------------------------------------------   
   ERRINFO *GetErrorInfo(int index)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->GetErrorInfo(m_hInst, index) : NULL);
   }

   // --------------------------------------------
   HRSLSYM RslGetInstSymbol(const char *name, const char *moduleName = NULL)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->RslGetInstSymbol(m_hInst, name, moduleName) : NULL);
   }

   // --------------------------------------------
   bool RslCallInstSymbol(HRSLSYM hSym, int code, int nPar, VALUE *par, VALUE *propORret)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->RslCallInstSymbol(m_hInst, hSym, code, nPar, par, propORret) : false);
   }

   // --------------------------------------------
   bool CallSymGetOrRun(HRSLSYM hSym, int nPar, VALUE *par, VALUE *propORret)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->RslCallInstSymbol(m_hInst, hSym, RSL_DISP_GET_OR_RUN,
                                                               nPar, par, propORret) : false);
   }

   // --------------------------------------------   
   bool CallSymRun(HRSLSYM hSym, int nPar, VALUE *par, VALUE *propORret)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->RslCallInstSymbol(m_hInst, hSym, RSL_DISP_RUN, nPar, par, propORret) : false);
   }

   // --------------------------------------------   
   bool CallSymGet(HRSLSYM hSym, VALUE *val)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->RslCallInstSymbol(m_hInst, hSym, RSL_DISP_GET, 0, NULL, val) : false);
   }

   // --------------------------------------------
   bool CallSymSet(HRSLSYM hSym, VALUE *val)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->RslCallInstSymbol(m_hInst, hSym, RSL_DISP_SET, 0, NULL, val) : false);
   }

   // --------------------------------------------
   bool TestExistModule(const char *modName)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->TestExistModule(m_hInst, modName) : false);
   }

   // --------------------------------------------
   bool AttachMsgHandler(TRSLMsgHandler *handler)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->AttachMsgHandler(m_hInst, handler) : false);
   }

   // --------------------------------------------
   bool DetachMsgHandler(TRSLMsgHandler *handler)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->DetachMsgHandler(m_hInst, handler) : false);
   }

   // --------------------------------------------
   bool CheckActualFiles()
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->CheckActualFiles(m_hInst) : false);
   }

   // --------------------------------------------
   int RslSendMes(IMESSAGE mes, void *ptr)
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->RslSendMes (m_hInst, mes, ptr) : 0);
   }

   // --------------------------------------------
   const char* GetName()
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->GetName(m_hInst) : NULL);
   }

   // --------------------------------------------
   HRD GetHRD()
   {
    return (m_hInst ? HRSLINST_FUN(m_hInst)->GetHRD(m_hInst) : NULL);
   }

   // --------------------------------------------
   bool SetDbgModule(const char *name)
   {
    return m_hInst ? HRSLINST_FUN(m_hInst)->SetDbgModule(m_hInst, name) : false;
   }

   // --------------------------------------------
   bool ExecuteEx()
   {
    return m_hInst ? HRSLINST_FUN(m_hInst)->ExecuteEx(m_hInst) : false;
   }

   // --------------------------------------------
   short int GetVersion(short int *verLo)
   {
    if(m_hInst)
      return HRSLINST_FUN(m_hInst)->GetVersion(m_hInst, verLo);
    else if(verLo)
      *verLo = 0;

    return 0;
   }

   // --------------------------------------------
   bool UninitModule(const char *name)
   {
    return m_hInst ? HRSLINST_FUN(m_hInst)->UninitModule(m_hInst, name) : false;
   }

   // --------------------------------------------
   bool SetModuleNotExecState(const char *name)
   {
    return m_hInst ? HRSLINST_FUN(m_hInst)->SetModuleNotExecState(m_hInst, name) : false;
   }

   // --------------------------------------------
   int GetRSCOMerror(void)
   {
    return m_hInst ? HRSLINST_FUN(m_hInst)->GetRSCOMerror(m_hInst) : -1;
   }

   // --------------------------------------------
   HRSLINST GetInst()
   {
    return m_hInst;
   }

   // --------------------------------------------   
   operator HRSLINST()
   {
    return m_hInst;
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   HRSLINST        m_hInst;
};

///////////////////////////////////////////////////////////////////////////////
//
// class TInstPtr
//
///////////////////////////////////////////////////////////////////////////////
template <class TRSLInterface, class TMsgHandler>
class TInstPtr : public TRSLInterface
{
 private:
   // --------------------------------------------
   TInstPtr(const TInstPtr<TRSLInterface, TMsgHandler> &)
   {
   }

   // --------------------------------------------   
   TInstPtr<TRSLInterface, TMsgHandler> &operator=(const TInstPtr<TRSLInterface, TMsgHandler> &)
   {
    return *this;
   }

 public:
   // --------------------------------------------
   TInstPtr()
   {
    TRSLInterface::m_hInst = NULL;
   }

   // --------------------------------------------
   virtual ~TInstPtr()
   {
    if(TRSLInterface::m_hInst)
     {
      TInterpreterIntf  *rsl = GetInterpreterInterface();

      rsl->DeleteRSLInstanceEx(TRSLInterface::m_hInst);
     }
   }

   // --------------------------------------------
   bool Init(const char *nameSpace, TRSLMsgHandler *host, void *initData = NULL, HRSLCOM hCom = NULL)
   {
    TInterpreterIntf  *rsl      = GetInterpreterInterface();
    TRSLMsgHandler    *pHandler = TMsgHandler::CreateNameSpace(initData);


    TRSLInterface::m_hInst = rsl->CreateRSLInstanceEx(nameSpace, host, pHandler, hCom);


    if(TRSLInterface::m_hInst)
      return true;

    return false;
   }

   // --------------------------------------------
   TMsgHandler *GetNameSpace()
   {
    TObjNameSpace  data;

    
    data.nameSpace = NULL;

    if(TRSLInterface::m_hInst)
     {
      TRSLInterface::RslSendMes(IM_GET_NAMESPACE, &data);

      if(data.nameSpace != NULL)
        return STATIC_CAST(TMsgHandler *, data.nameSpace);
     }

    return NULL;
   }
};

///////////////////////////////////////////////////////////////////////////////
//
// class TRSLUniInstance
//
///////////////////////////////////////////////////////////////////////////////
class TRSLUniInstance : public TRSLObjInstIntf, public TRSLObjMsgHandler
{
 private:
   // --------------------------------------------
   TRSLUniInstance(const TRSLUniInstance &);
   TRSLUniInstance &operator=(const TRSLUniInstance &);

 public:
   // --------------------------------------------
   TRSLUniInstance()
   {
    m_hInst = NULL;
   }

   // --------------------------------------------
   virtual ~TRSLUniInstance()
   {
   }

   // --------------------------------------------
   bool Init(const char *pszNameSpace, TRSLMsgHandler *pHost = NULL, HRSLCOM hCom = NULL)
   {
    Release();

    m_hInst = (::GetInterpreterInterface())->CreateRSLInstanceEx(pszNameSpace, pHost,
                                                                 STATIC_CAST(TRSLMsgHandler *, this), hCom);

    return m_hInst ? true : false;
   }

   // --------------------------------------------
   void Release()
   {
    if(m_hInst)
     {
      (::GetInterpreterInterface())->DeleteRSLInstanceEx(m_hInst);
      
      m_hInst = NULL;
     }
   }

   // --------------------------------------------
   TRSLObjInstIntf *GetInstance()
   {
    return STATIC_CAST(TRSLObjInstIntf *, this);
   }

   // --------------------------------------------
   TRSLMsgHandler *GetMsgHandler()
   {
    return STATIC_CAST(TRSLMsgHandler *, this);
   }
};

///////////////////////////////////////////////////////////////////////////////
//
// class TRSLUniPtr
//
///////////////////////////////////////////////////////////////////////////////
template <class TUniInstance>
class TRSLUniPtr
{
 public:
   //Constructors & destructor

   // --------------------------------------------
   TRSLUniPtr()
   {
    Attach(NULL);
   }

   // --------------------------------------------   
   TRSLUniPtr(TUniInstance *pInst)
   {
    Attach(pInst);
   }

   // --------------------------------------------
   virtual ~TRSLUniPtr()
   {
    Release();
   }

   // --------------------------------------------
   TUniInstance *operator->() const
   {
    return m_pInst;
   }

   // --------------------------------------------
   //Attach and detach to pointer
   void Attach(TUniInstance *pInst)
   {
    m_pInst       = pInst;
    m_bNeedDelete = false;
   }

   // --------------------------------------------
   void Detach()
   {
    m_pInst       = NULL;
    m_bNeedDelete = false;
   }

   // --------------------------------------------
   //Init and release methods
   bool Init(const char *pszNameSpace, TRSLMsgHandler *pHost = NULL, HRSLCOM hCom = NULL)
   {
    m_pInst = new TUniInstance;

    return m_pInst ? (m_bNeedDelete = true, m_pInst->Init(pszNameSpace, pHost, hCom)) : false;
   }

   // --------------------------------------------   
   void Release()
   {
    if(m_pInst)
     {
      m_pInst->Release();

      if(m_bNeedDelete)
        delete m_pInst;

      Detach();
     }
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   TUniInstance  *m_pInst;

 private:
   // --------------------------------------------
   // --------------------------------------------
   bool  m_bNeedDelete;
};

///////////////////////////////////////////////////////////////////////////////
//
// class TRSLObjCache
//
///////////////////////////////////////////////////////////////////////////////
class CacheException : public RSLException
{
 public:
   // --------------------------------------------
   CacheException(const char *str = "CacheException") : RSLException(str)
   {
   }
};

// -----------------------------------------------------------------------------
class TRSLObjCache
{
   // --------------------------------------------
   TRSLObjCache(const TRSLObjCache &)
   {
   }

   // --------------------------------------------   
   TRSLObjCache & operator=(const TRSLObjCache &)
   {
    return *this;
   }

 public:
   // --------------------------------------------
   TRSLObjCache() : m_hCache(NULL)
   {
   }

   // --------------------------------------------   
   explicit TRSLObjCache(int size) throw (CacheException)
   {
    if(!Create(size))
      throw CacheException();
   }

   // --------------------------------------------
   virtual ~TRSLObjCache()
   {
    /*if(m_hCache) Delete(); Use explicit call to Delete */
   }

   // --------------------------------------------
   bool Create(int size = 10)
   {
    TInterpreterIntf  *rsl = GetInterpreterInterface();


    m_hCache = rsl->CreateRSLCache(size);

    if(m_hCache)
      return true;

    return false;
   }

   // --------------------------------------------
   void Delete()
   {
    if(m_hCache)
     {
      TInterpreterIntf  *rsl = GetInterpreterInterface();


      rsl->DeleteRSLCache(m_hCache);

      m_hCache = NULL;
     }
   }

   // --------------------------------------------
   bool Enable(bool fEnable)
   {
    return (m_hCache ? HRSLCACHE_FUN(m_hCache)->Enable(m_hCache, fEnable) : false);
   }

   // --------------------------------------------   
   bool IsEnable()
   {
    return (m_hCache ? HRSLCACHE_FUN(m_hCache)->IsEnable(m_hCache) : false);
   }
   
   // --------------------------------------------   
   void Clear()
   {
    if(m_hCache)
      HRSLCACHE_FUN(m_hCache)->Clear(m_hCache);
   }
   
   // --------------------------------------------   
   HRSLINST FindInstance(const char *nameSpace, bool *flag)
   {
    return (m_hCache ? HRSLCACHE_FUN(m_hCache)->FindInstance(m_hCache, nameSpace, flag) : NULL);
   }
   
   // --------------------------------------------   
   bool PutInstance(HRSLINST inst)
   {
    return (m_hCache ? HRSLCACHE_FUN(m_hCache)->PutInstance(m_hCache, inst) : false);
   }

   // --------------------------------------------
   bool HasFreeInstance(void)
   {
    if(m_hCache)
      return HRSLCACHE_FUN(m_hCache)->HasFreeInstance(m_hCache);

    return false;
   }

   // --------------------------------------------   
   void DoneInstance(HRSLINST inst)
   {
    if(m_hCache)
      HRSLCACHE_FUN(m_hCache)->DoneInstance(m_hCache, inst);
   }
   
   // --------------------------------------------   
   void DeleteInstance(HRSLINST inst)
   {
    if(m_hCache)
      HRSLCACHE_FUN(m_hCache)->DeleteInstance(m_hCache, inst);
   }
   
   // --------------------------------------------   
   short int GetVersion(short int *verLo)
   {
    if(m_hCache)
      return HRSLCACHE_FUN(m_hCache)->GetVersion(m_hCache, verLo);
    else if(verLo)
      *verLo = 0;

    return 0;
   }

   // --------------------------------------------
   bool GetSize(int *maxSize, int *curSize)
   {
    if(m_hCache)
      return HRSLCACHE_FUN(m_hCache)->GetSize(m_hCache, maxSize, curSize);

    return false;
   }

   // --------------------------------------------
   HRSLCACHE GetHCache()
   {
    return m_hCache;
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   HRSLCACHE  m_hCache;
};

///////////////////////////////////////////////////////////////////////////////
//
// Begin class TRSLValue
//
///////////////////////////////////////////////////////////////////////////////

class TRSLValue
{
 public:
   // --------------------------------------------
   TRSLValue(HRSLINST inst)
   {
    makeObj(inst);
   }

   // --------------------------------------------
   TRSLValue(const TRSLValue &val)
   {
    makeObj(val.m_inst);

    m_intf->Copy(m_inst, (VALUE *)&val.m_value, &m_value);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const VALUE *val)
   {
    makeObj(inst);

    m_intf->Copy(m_inst, (VALUE *)val, &m_value);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, VALTYPE v_type, const void *ptr)
   {
    makeObj(inst);

    Set(v_type, ptr);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const int val) // V_INTEGER
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const long val) // V_INTEGER
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const int64_t val) // V_BIGINT
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const double val) // V_DOUBLE
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const long double val) // V_DOUBLEL
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const char *val) // V_STRING
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const bool val) // V_BOOL
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const bdate val) // V_DATE
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const btime val) // V_TIME
   {
    makeObj(inst);

    Set(val);
   }

#ifdef USE_NUMERIC
   // --------------------------------------------
   TRSLValue(HRSLINST inst, const Numeric &val)
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const Numeric_t &val)
   {
    makeObj(inst);

    Set(val);
   }
#endif

#ifdef USE_FDECIMAL
   // --------------------------------------------
   TRSLValue(HRSLINST inst, const FDecimal_t &val)
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const FMoney &val)
   {
    makeObj(inst);

    Set(val);
   }

   // --------------------------------------------
   TRSLValue(HRSLINST inst, const FDecimal &val)
   {
    makeObj(inst);

    Set(val);
   }
#endif

   // --------------------------------------------
   virtual ~TRSLValue()
   {
    m_intf->Clear(m_inst, &m_value);
   }

   // --------------------------------------------
   void Clear()
   {
    m_intf->Clear(m_inst, &m_value);
    m_intf->Make(m_inst, &m_value);
   }

   // --------------------------------------------
   void Set(VALTYPE v_type, const void *ptr)
   {
    m_intf->Clear(m_inst, &m_value);
    m_intf->Set(m_inst, &m_value, v_type, (void *)ptr);
   }

   // --------------------------------------------
   void Set(const int          val) { Set((const long)val); }
   void Set(const int64_t      val) { Set(V_BIGINT,  &val); }
   void Set(const long         val) { Set(V_INTEGER, &val); }
   void Set(const double       val) { Set(V_DOUBLE,  &val); }
   void Set(const long double  val) { Set(V_DOUBLEL, &val); }
   void Set(const char        *val) { Set(V_STRING,   val); }
   void Set(const bool         val) { Set(V_BOOL,    &val); }
   void Set(const bdate        val) { Set(V_DATE,    &val); }
   void Set(const btime        val) { Set(V_TIME,    &val); }

#ifdef USE_NUMERIC
   void Set(const Numeric     &val) { Set(V_NUMERIC, &val); }
   void Set(const Numeric_t   &val) { Set(V_NUMERIC, &val); }
#endif

#ifdef USE_FDECIMAL
   // --------------------------------------------
   void Set(const FDecimal_t &val)
   {
    Set(V_DECIMAL, &val);
   }

   // --------------------------------------------
   void Set(const FMoney &val)
   {
    FDecimal_t  fdec;


    val.get(&fdec);

    Set(V_MONEY, &val);
   }

   // --------------------------------------------
   void Set(const FDecimal &val)
   {
    FDecimal_t  fdec;


    val.get(&fdec);

    Set(V_DECIMAL, &fdec);
   }
#endif

   // --------------------------------------------
   void Copy(TRSLValue &val)
   {
    if(this != &val)
      m_intf->Copy(m_inst, &val.m_value, &m_value);
   }

   // --------------------------------------------
   void Copy(VALUE *val)
   {
    m_intf->Copy(m_inst, val, &m_value);
   }

   // --------------------------------------------
   void Move(TRSLValue &val)
   {
    if(this != &val)
      m_intf->Move(m_inst, &val.m_value, &m_value);
   }

   // --------------------------------------------   
   void Move(VALUE *val)
   {
    m_intf->Move(m_inst, val, &m_value);
   }

   // --------------------------------------------
   TRSLValue &operator=(const TRSLValue &val)
   {
    if(this != &val)
      m_intf->Copy(m_inst, (VALUE *)&val.m_value, &m_value);

    return *this;
   }

   // --------------------------------------------   
   TRSLValue &operator=(const VALUE *val)
   {
    if(&m_value != val)
      m_intf->Copy(m_inst, (VALUE *)val, &m_value);

    return *this;
   }

   // --------------------------------------------
   TRSLValue &operator= (const int          val) { Set(val); return *this; }
   TRSLValue &operator= (const int64_t      val) { Set(val); return *this; }
   TRSLValue &operator= (const long         val) { Set(val); return *this; }
   TRSLValue &operator= (const double       val) { Set(val); return *this; }
   TRSLValue &operator= (const long double  val) { Set(val); return *this; }
   TRSLValue &operator= (const char        *val) { Set(val); return *this; }
   TRSLValue &operator= (const bool         val) { Set(val); return *this; }
   TRSLValue &operator= (const bdate        val) { Set(val); return *this; }
   TRSLValue &operator= (const btime        val) { Set(val); return *this; }

#ifdef USE_NUMERIC
   TRSLValue &operator= (const Numeric     &val) { Set(val); return *this; }
   TRSLValue &operator= (const Numeric_t   &val) { Set(val); return *this; }
#endif

#ifdef USE_FDECIMAL
   TRSLValue &operator= (const FDecimal_t  &val) { Set(val); return *this; }
   TRSLValue &operator= (const FMoney      &val) { Set(val); return *this; }
   TRSLValue &operator= (const FDecimal    &val) { Set(val); return *this; }
#endif

   // --------------------------------------------
   bool IsEqual(const TRSLValue &val)
   {
    if(this != &val )
      return m_intf->IsEqual(m_inst, &m_value, (VALUE *)&val.m_value);

    return true;
   }

   // --------------------------------------------   
   bool IsEqual(const VALUE *val)
   {
    return m_intf->IsEqual(m_inst, &m_value, (VALUE *)val);
   }

   // --------------------------------------------
   bool operator== (const TRSLValue  &val)  { return IsEqual(val); }
   bool operator== (const VALUE      *val)  { return IsEqual(val); }
   bool operator== (const int         val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const int64_t     val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const long        val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const double      val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const long double val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const char       *val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const bool        val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const bdate       val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const btime       val)  { return IsEqual(TRSLValue(m_inst, val)); }

#ifdef USE_NUMERIC
   bool operator== (const Numeric    &val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const Numeric_t  &val)  { return IsEqual(TRSLValue(m_inst, val)); }
#endif

#ifdef USE_FDECIMAL
   bool operator== (const FDecimal_t &val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const FMoney     &val)  { return IsEqual(TRSLValue(m_inst, val)); }
   bool operator== (const FDecimal   &val)  { return IsEqual(TRSLValue(m_inst, val)); }
#endif

   // --------------------------------------------
   VALUE *GetValue()
   {
    return &m_value;
   }

   // --------------------------------------------
   const VALUE *GetValue() const
   {
    return &m_value;
   }

   // --------------------------------------------
   HRSLINST GetHRSLINST() const
   {
    return m_inst;
   }

   // --------------------------------------------
   operator VALUE *()
   {
    return &m_value;
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   VALUE           m_value;
   HRSLINST        m_inst;
   TRSLValueIntf  *m_intf;

 private:
   // --------------------------------------------
   void makeObj(HRSLINST inst)
   {
    m_intf = GetInterpreterInterface()->GetValueInterface();
    m_inst = inst;

    m_intf->Make(m_inst, &m_value);
   }
};

///////////////////////////////////////////////////////////////////////////////
//
// The End class TRSLValue
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// Begin class TRSLValueArray
//
///////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
class NoMemException : public RSLException
{
 public:
   // --------------------------------------------
   NoMemException(const char *str = "NoMemException") : RSLException(str)
   {
   }
};

// -----------------------------------------------------------------------------
class RslIndexException : public RSLException
{
 public:
   // --------------------------------------------
   RslIndexException(const char *str = "RslIndexException") : RSLException(str)
   {
   }
};

// -----------------------------------------------------------------------------
class RslConvertException : public RSLException
{
 public:
   // --------------------------------------------
   RslConvertException(const char *str = "RslConvertException") : RSLException(str)
   {
   }
};

// -----------------------------------------------------------------------------
class TRSLValueArrayBase
{
   // --------------------------------------------
   TRSLValueArrayBase(const TRSLValueArrayBase &)
   {
   }

   // --------------------------------------------   
   TRSLValueArrayBase &operator= (const TRSLValueArrayBase &)
   {
    return *this;
   }

 protected:
   // --------------------------------------------
   TRSLValueArrayBase(HRSLINST inst, size_t size)
   {
    m_intf = GetInterpreterInterface()->GetValueInterface();
    m_inst = inst;
    m_size = size;
   }

   // --------------------------------------------
   virtual ~TRSLValueArrayBase()
   {
   }

 public:
   // --------------------------------------------
   void Clear()
   {
    for(size_t  i = 0; i < m_size; ++i)
      {
       m_intf->Clear(m_inst, &m_ptrArray[i]);
       m_intf->Make(m_inst, &m_ptrArray[i]);
      }
   }

   // --------------------------------------------
   VALUE *operator[] (int index)
   {
    return GetValue(index);
   }

   // --------------------------------------------
   void Set(size_t index, VALTYPE v_type, const void *ptr)
   {
    VALUE  *val = GetValue(index);


    m_intf->Clear(m_inst, val);
    m_intf->Set(m_inst, val, v_type, (void *)ptr);
   }

   // --------------------------------------------
   void Set(size_t index, const int          val) { Set(index, (const long)val); }
   void Set(size_t index, const long         val) { Set(index, V_INTEGER, &val); }
   void Set(size_t index, const int64_t      val) { Set(index, V_BIGINT,  &val); }
   void Set(size_t index, const double       val) { Set(index, V_DOUBLE,  &val); }
   void Set(size_t index, const long double  val) { Set(index, V_DOUBLEL, &val); }
   void Set(size_t index, const char        *val) { Set(index, V_STRING,   val); }
   void Set(size_t index, const bool         val) { Set(index, V_BOOL,    &val); }
   void Set(size_t index, const bdate        val) { Set(index, V_DATE,    &val); }
   void Set(size_t index, const btime        val) { Set(index, V_TIME,    &val); }

   // --------------------------------------------
   void Copy(size_t index, TRSLValue &objVal)
   {
    VALUE  *val = GetValue(index);

    m_intf->Copy(m_inst, objVal.GetValue(), val);
   }

   // --------------------------------------------
   void Copy(size_t index, VALUE *pVal)
   {
    VALUE  *val = GetValue(index);

    m_intf->Copy(m_inst, pVal, val);
   }

   // --------------------------------------------
   void Move(size_t index, TRSLValue &objVal)
   {
    VALUE *val = GetValue(index);

    m_intf->Move(m_inst, objVal.GetValue(), val);
   }

   // --------------------------------------------
   void Move(size_t index, VALUE *pVal)
   {
    VALUE *val = GetValue(index);

    m_intf->Move(m_inst, pVal, val);
   }

   // --------------------------------------------
   bool IsEqual(size_t index1, size_t index2)
   {
    VALUE *val1 = GetValue(index1);
    VALUE *val2 = GetValue(index2);

    return m_intf->IsEqual(m_inst, val1, val2);
   }

   // --------------------------------------------
   bool IsEqual(size_t index, const TRSLValue &objVal)
   {
    VALUE *val = GetValue(index);

    return m_intf->IsEqual(m_inst, val, (VALUE *)objVal.GetValue());
   }

   // --------------------------------------------
   bool IsEqual(size_t index, const VALUE *pVal)
   {
    VALUE *val = GetValue(index);

    return m_intf->IsEqual(m_inst, val, (VALUE *)pVal);
   }

   // --------------------------------------------
   VALUE *GetValue(size_t index)
   {
    VALUE  *val;


    if(index < m_size)
      val = &m_ptrArray[index];
    else
      val = &m_ptrArray[index%m_size];

    return val;
   }

   // --------------------------------------------
   const VALUE *GetValue(size_t index) const
   {
    VALUE  *val;


    if(index < m_size)
      val = &m_ptrArray[index];
    else
      val = &m_ptrArray[index%m_size];

    return val;
   }

   // --------------------------------------------
   operator VALUE *()
   {
    return m_ptrArray;
   }

   // --------------------------------------------
   size_t GetSize() const
   {
    return m_size;
   }

   // --------------------------------------------
   HRSLINST GetHRSLINST() const
   {
    return m_inst;
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   VALUE          *m_ptrArray;
   HRSLINST        m_inst;
   TRSLValueIntf  *m_intf;
   size_t          m_size;
};

// -----------------------------------------------------------------------------
class TRSLValueArray5 : public TRSLValueArrayBase
{
   // --------------------------------------------
   enum
      {
       SIZE_ARRAY = 5
      };

 public:
   // --------------------------------------------
   TRSLValueArray5(HRSLINST inst) : TRSLValueArrayBase(inst, SIZE_ARRAY)
   {
    m_ptrArray = m_arrayVal;

    for(size_t  i = 0; i < m_size; ++i)
       m_intf->Make(m_inst, &m_ptrArray[i]);
   }

   // --------------------------------------------
   TRSLValueArray5(const TRSLValueArray5 &array) : TRSLValueArrayBase(array.GetHRSLINST(), SIZE_ARRAY)
   {
    Init(array);
   }

   // --------------------------------------------
   TRSLValueArray5(const TRSLValueArrayBase &array) : TRSLValueArrayBase(array.GetHRSLINST(), SIZE_ARRAY)
   {
    Init(array);
   }

   // --------------------------------------------
   TRSLValueArray5(HRSLINST inst, size_t size, VALUE *valArr) : TRSLValueArrayBase(inst, SIZE_ARRAY)
   {
    m_ptrArray = m_arrayVal;

    for(size_t  i = 0; i < m_size; ++i)
      {
       m_intf->Make(inst, &m_ptrArray[i]);

       if(i < size)
         m_intf->Copy(m_inst, &valArr[i], &m_ptrArray[i]);
      }
   }

   // --------------------------------------------
   virtual ~TRSLValueArray5()
   {
    for(size_t  i = 0; i < m_size; ++i)
       m_intf->Clear(m_inst, &m_ptrArray[i]);
   }

   // --------------------------------------------
   TRSLValueArrayBase &operator= (const TRSLValueArray5 &array)
   {
    return operator= (STATIC_CAST(const TRSLValueArrayBase&, array));
   }

   // --------------------------------------------
   TRSLValueArrayBase &operator= (const TRSLValueArrayBase &array)
   {
    if(this != &array)
     {
      size_t  i;


      for(i = 0; i < m_size; ++i)
         m_intf->Clear(m_inst, &m_ptrArray[i]);

      for(i = 0; i < m_size; ++i)
         if(i < array.GetSize())
           m_intf->Copy(m_inst, (VALUE *)array.GetValue(i), &m_ptrArray[i]);
     }

    return *this;
   }

 private:
   // --------------------------------------------
   void Init(const TRSLValueArrayBase &array)
   {
    m_ptrArray = m_arrayVal;

    for(size_t i = 0; i < m_size; ++i)
      {
       m_intf->Make(m_inst, &m_ptrArray[i]);

       if(i < array.GetSize())
         m_intf->Copy(m_inst, (VALUE *)array.GetValue(i), &m_ptrArray[i]);
      }
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   VALUE  m_arrayVal[SIZE_ARRAY];
};

// -----------------------------------------------------------------------------
class TRSLValueArray10 : public TRSLValueArrayBase
{
   // --------------------------------------------
   enum
      {
       SIZE_ARRAY = 10
      };

 public:
   // --------------------------------------------
   TRSLValueArray10(HRSLINST inst) : TRSLValueArrayBase(inst, SIZE_ARRAY)
   {
    m_ptrArray = m_arrayVal;

    for(size_t  i = 0; i < m_size; ++i)
       m_intf->Make(m_inst, &m_ptrArray[i]);
   }

   // --------------------------------------------
   TRSLValueArray10(const TRSLValueArray10 &array) : TRSLValueArrayBase(array.GetHRSLINST(), SIZE_ARRAY)
   {
    Init(array);
   }

   // --------------------------------------------
   TRSLValueArray10(const TRSLValueArrayBase &array) : TRSLValueArrayBase(array.GetHRSLINST(), SIZE_ARRAY)
   {
    Init(array);
   }

   // --------------------------------------------
   TRSLValueArray10(HRSLINST inst, size_t size, VALUE *valArr) : TRSLValueArrayBase(inst, SIZE_ARRAY)
   {
    m_ptrArray = m_arrayVal;

    for(size_t  i = 0; i < m_size; ++i)
      {
       m_intf->Make(inst, &m_ptrArray[i]);

       if(i < size)
         m_intf->Copy(m_inst, &valArr[i], &m_ptrArray[i]);
      }
   }

   // --------------------------------------------
   virtual ~TRSLValueArray10()
   {
    for(size_t  i = 0; i < m_size; ++i)
       m_intf->Clear(m_inst, &m_ptrArray[i]);
   }

   // --------------------------------------------
   TRSLValueArrayBase &operator= (const TRSLValueArray10 &array)
   {
    return operator= (STATIC_CAST(const TRSLValueArrayBase &, array));
   }

   // --------------------------------------------
   TRSLValueArrayBase &operator= (const TRSLValueArrayBase &array)
   {
    if(this != &array )
     {
      size_t  i;
      

      for(i = 0; i < m_size; ++i)
         m_intf->Clear(m_inst, &m_ptrArray[i]);

      for(i = 0; i < m_size; ++i)
         if(i < array.GetSize())
           m_intf->Copy(m_inst, (VALUE *)array.GetValue(i), &m_ptrArray[i]);
     }

    return *this;
   }

 private:
   // --------------------------------------------
   void Init(const TRSLValueArrayBase &array)
   {
    m_ptrArray = m_arrayVal;

    for(size_t  i = 0; i < m_size; ++i)
      {
       m_intf->Make(m_inst, &m_ptrArray[i]);

       if(i < array.GetSize())
         m_intf->Copy(m_inst, (VALUE *)array.GetValue(i), &m_ptrArray[i]);
      }
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   VALUE  m_arrayVal[SIZE_ARRAY];
};

// -----------------------------------------------------------------------------
class TRSLValueArray : public TRSLValueArrayBase
{
 public:
   // --------------------------------------------
   TRSLValueArray(HRSLINST inst, size_t size) throw (NoMemException) : TRSLValueArrayBase(inst, size)
   {
    m_ptrArray = (VALUE *)malloc(sizeof(VALUE)*m_size);

    if(!m_ptrArray)
      throw NoMemException();

    for(size_t  i = 0; i < m_size; ++i )
       m_intf->Make(m_inst, &m_ptrArray[i]);
   }

   // --------------------------------------------
   TRSLValueArray(const TRSLValueArray &array) throw (NoMemException) : TRSLValueArrayBase(array.GetHRSLINST(), array.GetSize())
   {
    Init(array);
   }

   // --------------------------------------------
   TRSLValueArray(const TRSLValueArrayBase &array) throw (NoMemException) : TRSLValueArrayBase(array.GetHRSLINST(), array.GetSize())
   {
    Init(array);
   }

   // --------------------------------------------
   TRSLValueArray(HRSLINST inst, size_t size, VALUE *valArr) throw (NoMemException) : TRSLValueArrayBase(inst, size)
   {
    m_ptrArray = (VALUE *)malloc(sizeof(VALUE)*m_size);

    if(!m_ptrArray)
      throw NoMemException();

    for(size_t  i = 0; i < m_size; ++i )
      {
       m_intf->Make(inst, &m_ptrArray[i]);
       m_intf->Copy(m_inst, &valArr[i], &m_ptrArray[i]);
      }
   }

   // --------------------------------------------
   virtual ~TRSLValueArray()
   {
    if(m_ptrArray != NULL)
     {
      for(size_t  i = 0; i < m_size; ++i)
         m_intf->Clear(m_inst, &m_ptrArray[i]);

      free(m_ptrArray);
     }
   }

   // --------------------------------------------
   TRSLValueArrayBase &operator= (const TRSLValueArray &array) throw (NoMemException)
   {
    return operator= (STATIC_CAST(const TRSLValueArrayBase &, array));
   }

   // --------------------------------------------
   TRSLValueArrayBase &operator= (const TRSLValueArrayBase &array) throw (NoMemException)
   {
    if(this != &array)
     {
      size_t  i;


      for(i = 0; i < m_size; ++i)
         m_intf->Clear(m_inst, &m_ptrArray[i]);

      free(m_ptrArray);

      m_size = array.GetSize();

      m_ptrArray = (VALUE *)malloc(sizeof(VALUE)*m_size);

      if(!m_ptrArray)
        throw NoMemException();

      for(i = 0; i < m_size; ++i)
        {
         m_intf->Make(m_inst, &m_ptrArray[i]);
         m_intf->Copy(m_inst, (VALUE *)array.GetValue(i), &m_ptrArray[i]);
        }
     }

    return *this;
   }

 private:
   // --------------------------------------------
   void Init(const TRSLValueArrayBase &array) throw(NoMemException)
   {
    m_ptrArray = (VALUE *)malloc(sizeof(VALUE)*m_size);

    if(!m_ptrArray)
      throw NoMemException();

    for(size_t  i = 0; i < m_size; ++i)
      {
       m_intf->Make(m_inst, &m_ptrArray[i]);
       m_intf->Copy(m_inst, (VALUE *)array.GetValue(i), &m_ptrArray[i]);
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
//
// The End class TRSLValueArray
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// The Begin class TRSLGenObj
//
///////////////////////////////////////////////////////////////////////////////

class GenObjException : public RSLException
{
 public:
   // --------------------------------------------
   GenObjException(const char *str = "GenObjException") : RSLException(str)
   {
   }
};

// -----------------------------------------------------------------------------
class TRSLGenObj
{
 protected:
   // --------------------------------------------
   TRSLGenObj(const TRSLGenObj &) {}

   // --------------------------------------------   
   TRSLGenObj &operator= (const TRSLGenObj &)
   {
    return *this;
   }

 public:
   // --------------------------------------------
   TRSLGenObj(HRSLINST inst, VALUE *val) throw (GenObjException)
   {
    if(val->v_type != V_GENOBJ)
      throw GenObjException();

    m_ptrIntf = GetInterpreterInterface()->GetGenObjInterface();
    m_instRsl = inst;
    m_genObj  = P_GOBJ(val->value.obj);

    AddRef();
   }

   // --------------------------------------------
   TRSLGenObj(HRSLINST inst, TGenObject *obj)
   {
    m_ptrIntf = GetInterpreterInterface()->GetGenObjInterface();
    m_instRsl = inst;
    m_genObj  = obj;

    AddRef();
   }

   // --------------------------------------------
   virtual ~TRSLGenObj()
   {
    Release();
   }

   // --------------------------------------------
   unsigned short AddRef()
   {
    return (m_ptrIntf ? m_ptrIntf->AddRef(m_instRsl, m_genObj) : short(0));
   }

   // --------------------------------------------
   unsigned short Release()
   {
    return (m_ptrIntf ? m_ptrIntf->Release(m_instRsl, m_genObj) : short(0));
   }

   // --------------------------------------------
   int RslObjMemberFromName(const char *name, long *id)
   {
    return (m_ptrIntf ? m_ptrIntf->RslObjMemberFromName(m_instRsl, m_genObj, name, id) : -1);
   }

   // --------------------------------------------
   int RslObjInvoke(long id, int code, int nPar, VALUE *par, VALUE *propORret)
   {
    return (m_ptrIntf ? m_ptrIntf->RslObjInvoke(m_instRsl, m_genObj, id, code, nPar, par, propORret) : -1);
   }

   // --------------------------------------------
   int ObjInvokeGetOrRun(long id, int nPar, VALUE *par, VALUE *propORret)
   {
    return (m_ptrIntf ? m_ptrIntf->RslObjInvoke(m_instRsl, m_genObj, id, RSL_DISP_GET_OR_RUN, nPar, par, propORret) : -1);
   }

   // --------------------------------------------
   int ObjInvokeRun(long id, int nPar, VALUE *par, VALUE *propORret)
   {
    return (m_ptrIntf ? m_ptrIntf->RslObjInvoke(m_instRsl, m_genObj, id, RSL_DISP_RUN, nPar, par, propORret) : -1);
   }

   // --------------------------------------------
   int ObjInvokeGet(long id, VALUE *val)
   {
    return (m_ptrIntf ? m_ptrIntf->RslObjInvoke(m_instRsl, m_genObj, id, RSL_DISP_GET, 0, NULL, val) : -1);
   }

   // --------------------------------------------
   int ObjInvokeSet(long id, VALUE *val)
   {
    return (m_ptrIntf ? m_ptrIntf->RslObjInvoke(m_instRsl, m_genObj, id, RSL_DISP_SET, 0, NULL, val) : -1);
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   TRSLGenObjIntf  *m_ptrIntf;
   HRSLINST         m_instRsl;
   TGenObject      *m_genObj;
};
///////////////////////////////////////////////////////////////////////////////
//
// The End class TRSLGenObj
//
///////////////////////////////////////////////////////////////////////////////


#include "rsl/iparms.hpp"

///////////////////////////////////////////////////////////////////////////////
#endif // __OBJINTF_IMPL_HPP

/* <-- EOF --> */