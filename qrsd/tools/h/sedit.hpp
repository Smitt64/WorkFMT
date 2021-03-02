/*************************************************************************
   File Name      : sedit.hpp
   Programmer     : Alexey Lebedev
                    

   Creation       : 30 Jan 2003
   Description    : Графический редактор макро-файлов.

   Copyright(C) 2002-2003 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __SEDIT_HPP
#define __SEDIT_HPP

#include "rsl/common.h"


typedef int (TSEditCallback)(int message, void *data, void *pParam);

enum SEditMessageID
   {
    SED_CALLRSL = 1
   };

enum SEditCallRslID
   {
    SED_RSL_RUN         = 1,
    SED_RSL_DEBUG       = 2,
    SED_RSL_COMPILE     = 3,
    SED_RSL_RUN_NODEBUG = 4
   };

#include "packpshn.h"

typedef struct tagTCallRslData
      {
       int            nCmdID;     // значение из перечисления SEditCallRslID
       const char    *szCmdArgs;
       const char    *szFileName;
       const char    *szNameSpace;

       char          *szOutputFile;

       TRslMesProc    pMsgProc;
       TRslActionProc pRunProc;
       void          *pUserData;
      } TCallRslData;

#include "packpop.h"

#ifdef __cplusplus
    class TRSLWinEditor;
    class TRSLErrorsList;

    typedef       TRSLWinEditor  *TRSLEditorPtr;
    typedef const TRSLErrorsList *TErListConstPtr;
#else
    typedef       void *TRSLEditorPtr;
    typedef const void *TErListConstPtr;
#endif

#ifdef __cplusplus
extern "C" {
#endif

_APPTLFUNC TRSLEditorPtr CreateSourceEditor(void);

_APPTLFUNC bool  StartSourceEditor(const char *szFileName, TErListConstPtr pErrorList);
_APPTLFUNC void  StopSourceEditor(void);

_APPTLFUNC void  SetEditorCallback(TSEditCallback *pProc, void *pParam);
_APPTLFUNC void  SetEditorErrorList(TErListConstPtr pErrorList);

// Разрешить/Запретить вызов графического редактора
_APPTLFUNC bool  EnableSourceEditor(bool enabled);

#ifdef __cplusplus
}
#endif

#endif //__SEDIT_HPP

/* <-- EOF --> */