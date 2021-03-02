/*************************************************************************
   File Name      : conintf.hpp           17.10.2000
   Programmer     : A.Tokmashev

   Subsystem      : Interpreter RSL
   Description    : Wrapper for working with RSL-instance
                    and cache RSL-instances.

   Copyright(C) 1991 - 2000 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __CONINTF_HPP
#define __CONINTF_HPP

#ifndef RSL_NO_HOST

    #include "rsl/common.h"
    #include "erlist.hpp"
    #include "rsl/objintf.hpp"
    #include "conedit.hpp"


    #if defined(_MSC_VER)
        // non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
        #pragma warning(disable : 4275)
    #endif

    // -----------------------------------------------------------------------------
    typedef struct tagTConDataInit
          {
           TRslMesProc     mesProc;
           void           *userData;
           TRSLErrorsList *errList;
          } TConDataInit;

    typedef struct tagTConFilesData
          {
           LOBJ            elem;
           FILE           *fo;      
           unsigned long   countRef;
           char            oldOutput [_MAX_PATH];
           char            originalOutput[_MAX_PATH];
          } TConFilesData;

    #include <packpshn.h>
    typedef struct tagTStatErrorsList
          {
           TRSLErrorsList  *newErrList;
           TRSLErrorsList  *oldErrList;
          } TStatErrorsList;
    #include <packpop.h>

    ///////////////////////////////////////////////////////////////////////////////
    //
    // class TRSLConObjMsgHandler
    //
    ///////////////////////////////////////////////////////////////////////////////
    class _APPTLEXP TRSLConObjMsgHandler : public TRSLNameSpace
        {
         private:
           // --------------------------------------------
           TRSLConObjMsgHandler(const TRSLConObjMsgHandler &) {}
        
           // --------------------------------------------           
           TRSLConObjMsgHandler &operator = (const TRSLConObjMsgHandler &)
           {
            return *this;
           }

         protected:
           TRSLConObjMsgHandler(TRslMesProc mesProc, void *uData, TRSLErrorsList *errList);
           virtual ~TRSLConObjMsgHandler();

           // --------------------------------------------
           static int conMsgProc(TRSLMsgHandler *handler, HRSLINST inst, IMESSAGE mes, void *ptr)
           {
            if(handler != NULL)
             {
              TRSLConObjMsgHandler  *_This   = STATIC_CAST(TRSLConObjMsgHandler *, handler);
              int                    nResult = _This->m_userMsgProc(mes, ptr, _This->m_userData);

              return nResult ? nResult : TRSLNameSpace::msgproc(handler, inst, mes, ptr);
             }

            return 0;
           }
          
           bool OpenFile(STD_USERDATA *data, FILE **fo, bool userFile);
           bool ChangeOutput(char *nativePath, const char *fileName, FILE **fo, bool userFile, bool flag, int *err_no);

           // --------------------------------------------
           RSLMSG_TABLE_BEGIN(TRSLNameSpace)
              RSLMSG_INIT_INSTANCE
              RSLMSG_DONE_INSTANCE
              RSLMSG_INIT_INSTANCE_REC
              RSLMSG_DONE_INSTANCE_REC
              RSLMSG_HANDLER_NAME(OnError, IM_RUN_ERROR)
              RSLMSG_HANDLER_NAME(OnError, IM_ERROR)
              RSLMSG_HANDLER_NAME(OnParse, IM_BEGIN_PARSE)
              RSLMSG_HANDLER_NAME(OnParse, IM_AGAIN_PARSE)
              RSLMSG_INIT_EXECUTOR
              RSLMSG_DONE_EXECUTOR
              RSLMSG_GETSTDSTRM
              RSLMSG_OUTPUT
              RSLMSG_ERR_OUTPUT
              RSLMSG_CHANGE_OUTPUT
              RSLMSG_EXIT_MES
              RSLMSG_EXIT_CALLED
              RSLMSG_GET_CMD_ARG
              RSLMSG_INSPECT_SYM
              RSLMSG_RUN_PROG
              RSLMSG_SET_EXITFLAG

           #ifndef RSL_NO_PCODE
              RSLMSG_PCODE_MESSAGE
           #endif // RSL_NO_PCODE

              RSLMSG_CHANGE_ERRORS
              RSLMSG_REWIND
              RSLMSG_BTRMACFILE

              RSLMSG_INIT_PROGRESS
              RSLMSG_USE_PROGRESS
              RSLMSG_REM_PROGRESS
              RSLMSG_HANDLER_NAME(OnBegAction,   IM_BEGACTION)
              RSLMSG_HANDLER_NAME(OnBegActionEx, IM_BEGACTIONEX)
              RSLMSG_HANDLER_NAME(OnEndAction,   IM_ENDACTION)

              RSLMSG_HANDLER_NAME(OnGetIndic,  IM_GETINDIC)
              RSLMSG_HANDLER_NAME(OnFreeIndic, IM_FREEINDIC)
           RSLMSG_TABLE_END

           void CloseActive();

           int mes_IM_INIT_INSTANCE    (HRSLINST inst, void *ptr);
           int mes_IM_DONE_INSTANCE    (HRSLINST inst, void *ptr);
           int mes_IM_INIT_INSTANCE_REC(HRSLINST inst, void *ptr);
           int mes_IM_DONE_INSTANCE_REC(HRSLINST inst, void *ptr);
           int OnParse                 (HRSLINST inst, IMESSAGE mes, void *ptr);
           int mes_IM_SET_EXITFLAG     (HRSLINST inst, void *ptr);
           int mes_IM_RUN_PROG         (HRSLINST inst, RUN_PARM *r);
           int mes_IM_INSPECT_SYM      (HRSLINST inst, InspectSymData *ptr);
           int mes_IM_GET_CMD_ARG      (HRSLINST inst, RSL_FORMAT_STR *parm);
           int mes_IM_EXIT_CALLED      (HRSLINST inst, int *ptr);
           int OnError                 (HRSLINST inst, IMESSAGE mes, void *ptr);
           int mes_IM_GETSTDSTRM       (HRSLINST inst, TRslStdStrm *ptr);
           int mes_IM_OUTPUT           (HRSLINST inst, char *ptr);
           int mes_IM_ERR_OUTPUT       (HRSLINST inst, char *ptr);

        #ifndef RSL_NO_PCODE
           int mes_IM_PCODE_MESSAGE    (HRSLINST inst, char *ptr);
        #endif // RSL_NO_PCODE

           int mes_IM_INIT_EXECUTOR    (HRSLINST inst, INIT_EXECUTOR *ptr);
           int mes_IM_DONE_EXECUTOR    (HRSLINST inst, bool *ptr);
           int mes_IM_CHANGE_OUTPUT    (HRSLINST inst, TChangeOutput *in);
           int mes_IM_EXIT_MES         (HRSLINST inst, char *ptr);
           int mes_IM_CHANGE_ERRORS    (HRSLINST inst, void *ptr);
           int mes_IM_REWIND           (HRSLINST inst, TRewindData *ptr);

           int mes_IM_BTRMACFILE       (HRSLINST inst, char *ptr);

           int mes_IM_INIT_PROGRESS    (HRSLINST inst, ProgressData *d);
           int mes_IM_USE_PROGRESS     (HRSLINST inst, long *ptr);
           int mes_IM_REM_PROGRESS     (HRSLINST inst, void *ptr);

           // --------------------------------------------
           int OnBegAction  (HRSLINST inst, IMESSAGE mes, void *ptr);
           int OnBegActionEx(HRSLINST inst, IMESSAGE mes, void *ptr);
           int OnEndAction  (HRSLINST inst, IMESSAGE mes, void *ptr);

           int OnGetIndic   (HRSLINST inst, IMESSAGE mes, void *ptr);
           int OnFreeIndic  (HRSLINST inst, IMESSAGE mes, void *ptr);

           int StdInitProc  (HRSLINST inst, void *ptr);
           int StdDoneProc  (HRSLINST inst, void *ptr);

         public:
           void *operator new(size_t size);
           void  operator delete(void *ptr);

           // --------------------------------------------
           static TRSLMsgHandler *CreateNameSpace(void *data);

           // --------------------------------------------
           void *GetUserData()
           {
            return m_userData;
           }

         protected:
           // --------------------------------------------
           // --------------------------------------------
           TRslMesProc       m_userMsgProc;
           void             *m_userData;
           TRSLErrorsList   *m_errList;
           CNTLIST           m_listFiles;
           FILE             *m_fo_sys;

           int  IndicOnScrean;
           int  activeCount;
        };

    ///////////////////////////////////////////////////////////////////////////////
    //
    // class TRSLConObjInstIntf
    //
    ///////////////////////////////////////////////////////////////////////////////
    class TRSLConObjInstIntf : public TRSLObjInstIntf
        {
         private:
           // --------------------------------------------
           TRSLConObjInstIntf(const TRSLConObjInstIntf &) {}

           // --------------------------------------------
           TRSLConObjInstIntf &operator = (const TRSLConObjInstIntf &)
           {
            return *this;
           }

         public:
           // --------------------------------------------           
           TRSLConObjInstIntf(HRSLINST inst = NULL) : TRSLObjInstIntf(inst) {}

           // --------------------------------------------        
           virtual ~TRSLConObjInstIntf() {}

           // --------------------------------------------           
           void RslSetCurrentPrintLine(int line)
           {
            HRD  oldInst = RslSetInstance(GetHRD());


            ::RslSetCurrentPrintLine(line);

            RslSetInstance(oldInst);
           }

           // --------------------------------------------
           int RslSendMes(IMESSAGE mes, void *ptr)
           {
            HRD  oldInst = RslSetInstance(GetHRD());
            int  stat    = ::RslSendMes(mes, ptr);


            RslSetInstance(oldInst);

            return stat;
           }

        #ifndef RSL_NO_PCODE
           // --------------------------------------------
           void SetPCodeData(int genPCode, bool forEach, bool isGenMode)
           {
            HRD  oldInst = RslSetInstance(GetHRD());


            ::RslSetPCodeData(genPCode, forEach, isGenMode);

            RslSetInstance(oldInst);   
           }

           // --------------------------------------------
           bool SavePCode()
           {
            HRD   oldInst = RslSetInstance(GetHRD());
            bool  stat    = ::SavePCode(RslGetHPCode());


            RslSetInstance(oldInst);

            return stat;
           }

           // --------------------------------------------
           bool IsHPCode()
           {
            HRD   oldInst = RslSetInstance(GetHRD());
            bool  stat    = ::RslGetHPCode() ? true : false;


            RslSetInstance(oldInst);

            return stat;
           }
        #endif // RSL_NO_PCODE

           // --------------------------------------------
           bool IsErrors() 
           {
            HRD   oldInst = RslSetInstance(GetHRD());
            bool  stat    = ::IsErrors() ? true : false;


            RslSetInstance(oldInst);

            return stat;
           }

           // --------------------------------------------
           bool SetPrivateMode(bool mode)
           {
            HRD   oldInst = RslSetInstance(GetHRD());
            bool  old     = ::RslSetPrivateMode(mode);


            RslSetInstance(oldInst);

            return old;
           }

           // --------------------------------------------
           const char *SetCurModuleName(const char *nameModule)
           {
            HRD          oldInst = RslSetInstance(GetHRD());
            const char  *oldName = ::RslSetCurModuleName(nameModule);


            RslSetInstance(oldInst);

            return oldName;
           }
        };


    ///////////////////////////////////////////////////////////////////////////////
    //
    // class TConInstPtr
    //
    ///////////////////////////////////////////////////////////////////////////////

    template <class TRSLInstance, class TMsgHandler>
    class TConInstPtr : public TInstPtr<TRSLInstance, TMsgHandler>
        {
         // --------------------------------------------
         TConInstPtr(const TConInstPtr<TRSLInstance, TMsgHandler> &) {}

         // --------------------------------------------         
         TConInstPtr <TRSLInstance, TMsgHandler> &operator = (const TConInstPtr<TRSLInstance, TMsgHandler> &)
         {
          return *this;
         }

         public:
           // --------------------------------------------
           TConInstPtr() {}

           // --------------------------------------------
           virtual ~TConInstPtr() {}
           
           // --------------------------------------------           
           bool Init(const char *szFileName, const char *output, const char *nameSpace, TRSLMsgHandler *host,
                     TRslMesProc msgProc, void *userData, HRSLCOM hCom = NULL, TRSLErrorsList *errList = NULL)
           {
            TConDataInit  data;


            data.mesProc  = msgProc;
            data.userData = userData;
            data.errList  = errList;

            InitUserData((STD_USERDATA *)userData, szFileName, output, nameSpace ? nameSpace : NAMESPACE_DEFAULT);
            
            if(!host)
              host = FindHost(msgProc, (STD_USERDATA *)userData);

            return TInstPtr<TRSLInstance, TMsgHandler>::Init(nameSpace ? nameSpace : NAMESPACE_DEFAULT, host, &data, hCom);
           }

           // --------------------------------------------
           void *GetUserData()
           {
            TMsgHandler  *msgHandler = GetNameSpace();


            if(msgHandler)
              return msgHandler->GetUserData();

            return NULL;
           }

         protected:
           // --------------------------------------------
           void InitUserData(STD_USERDATA *data, const char * /*szFileName*/, const char *output, const char *nameSpace)
           {
            data->output      = data->originalOutput = output;
            data->exit_flag   = 0;
            data->addExecData = NULL;
            data->addOutput   = NULL;
            data->stat        = 0;

            strncpy(data->nameSpace, nameSpace, _MAX_NAMESPACE);

            data->nameSpace[_MAX_NAMESPACE] = '\0';
           }
        };

    ///////////////////////////////////////////////////////////////////////////////

    typedef  TConInstPtr<TRSLConObjInstIntf, TRSLConObjMsgHandler>  TRSLConInstance;

    ///////////////////////////////////////////////////////////////////////////////
    // ---> new RunRSL
    _APPTLFUNC int RunRSLEx(const char *szFileName, const char *output, const char *nameSpace, TRslMesProc User_MsgProc, 
                            TRslActionProc ProcFun, void *udata, bool addExec, int fMode, bool *isErrors,
                            TRSLErrorsList *errList, char *realOutput);

    #ifndef RSL_NO_PCODE
        _APPTLFUNC int PlayGenerationImpl(const char *szFileName, const char *output, const char *nameSpace,
                                          TRslMesProc User_MsgProc, TRslActionProc ProcFun, void *userdata, int addExec,
                                          bool forEach, bool *isErrors, TRSLErrorsList *errList);
    #endif // RSL_NO_PCODE

#endif // RSL_NO_HOST
#endif // __CONINTF_HPP

/* <-- EOF --> */