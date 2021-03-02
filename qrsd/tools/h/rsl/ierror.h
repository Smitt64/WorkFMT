/*******************************************************************************
 FILE         :   IERROR.H

 COPYRIGHT    :   R-Style Software Lab, 1993, 1998

 DESCRIPTION  :

 PROGRAMMED BY:   Serge Kubrin

 CREATION DATE:   26 Nov 1993

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsl/ierror.h $
                  $Revision: 90 $
                  $Date: 9/22/00 5:22p $
                  $Author: Tokmashev $
                  $Modtime: 9/22/00 3:55p $
*******************************************************************************/

#ifndef __IERROR_H
#define __IERROR_H

#define  MAX_ERRORS  20

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENUMINTSIZE
    #pragma option -b-
#endif

// -----------------------------------------------------------------------------
// Error codes
enum ERRCODE_V
   {
    IER_OK,
    IER_BAD,
    IER_COMMENT,
    IER_STRING,
    IER_NUMBER,
    IER_MONEY,
    IER_UNEXPEOF,
    IER_UNEXP,
    IER_MISS,
    IER_MISSMACNAME,
    IER_MISSBASENAME,
    IER_BADBASENAME,
    IER_MISSCLASSNAME,
    IER_UNDEF,
    IER_BADEXP,                  // 14
    IER_BADSTR,
    IER_REDEF,
    IER_USER_BREAK,
    IER_TOOMANY,
    IER_FORMAT,
    IER_IDSPEC,
    IER_MODCONST,
    IER_STOUT,                   // 22
    IER_MISFILEID,
    IER_MISDICNAME,
    IER_MISBADPATH,
    IER_BADFIELD,
    IER_OPENFILE,
    IER_IMPFILE,
    IER_BADELSE,                 // 29
    IER_BTRMACRO,
    IER_BTRCRC,
    IER_ASSIGN_VAL,
    IER_GETREF,

    IER_CONSTINIT,
    IER_STACK,
    IER_OPERANDUN,
    IER_OPERANDBIN,
    IER_ASSIGN,
    IER_NOFIELD,
    IER_ACCPROP,
    IER_EXECMET,
    IER_RUNTIME,
    IER_INDEX,
    IER_HASNOFIELD,
    IER_DICTIONARY,
    IER_STRUCTURE,
    IER_OPENDATA,
    IER_ZERODIV,
    IER_CREATE,
    IER_VARLENOVERLAP,
    IER_RESLIB,
    IER_INIT_TRN,
    IER_FILELOCK,
    IER_TRN,
    IER_BTR_OPER,
    IER_CREATEGENOBJECT,
    IER_SETPROP,
    IER_GETPROP,
    IER_RUNMETHOD,

    IER_OPENMODULE,
    IER_METHODASSIGN,

    IER_BAD_TYPE_NAME,
    IER_RETHROW,
    IER_FDECIMAL,

    IER_PCODE_INIOPEN,
    IER_PCODE_BADFILE,
    IER_PCODE_BADSIGNATURE, 
    IER_PCODE_NOMEM,
    IER_PCODE_BADINDEX,
    IER_PCODE_BADREF,            // IER_PCODE_MES,
    IER_PCODE_NOTDBF,
    IER_PCODE_NOTFILE,
    IER_PCODE_NOTRECORD,

    IER_CYCLEFILE,
    IER_STRING_TOOLONG,
    IER_RUNFILE,
    IER_HASNOFIELD2,
    IER_UNPLUS,
    IER_TRNABORT,
    IER_NESTEDCLASS,
    IER_BADELIF,

    IER_OUT_OF_BOUNDS            // Выход за границы (массива)
   };


// It's for constant parameter size support
//
typedef int  ERRCODE;

// -----------------------------------------------
typedef enum ERRTYPE
      {
       E_SIM1,
       E_SIM2,
       E_SIM3,
       E_FATAL,
       E_MES,
       E_NL,
       E_BUF,
       E_MISS,
       E_OPUN,
       E_OPBIN,
       E_RUNTIME,
       E_BUF_MES,
       E_RUNTIME1,
       E_PCODE,
       E_OPEN,
       E_USERBREAK,
       E_MES1
      } ERRTYPE;

#ifdef ENUMINTSIZE
#pragma option -b
#endif

#include "packpshn.h"

// -----------------------------------------------
typedef struct ERRINFO
      {
       ERRCODE  code;  // Код ошибки
       int      line;  // Номер строки или -1
       int      pos;   // Позиция в строке или -1
       char    *file;  // Имя файла или ""
       char    *mes;   // Дополнительная информация об ошибке или ""
       bool     isDos; // Error in DOS file
      } ERRINFO;

// -----------------------------------------------
typedef struct ERRLISTELEM
      {
       ERRINFO  info;
       LOBJ     er_elem;
      } ERRLISTELEM;

// -----------------------------------------------
// For handle IM_RUN_ERROR_EX
typedef struct tagExtErrErrInfo
      {
       int           version;
       bool          rscomError;
       bool          userError;
       bool          axError;

       TRsComErrMes  rdata;
       const char   *userData;
       TRslAxError  *axData; 

       ERRINFO      *erInfo;

       // Add in version 2
       VALUE        *extErr;
      } TExtErrErrInfo;

#include "packpop.h"

// -----------------------------------------------------------------------------
#ifndef RSL_DLM
   
    void DLMAPIC  RslError(char *fmt, ...);

    void iErrorEx(int level, ERRCODE code, ...);

    RSLLINK void          RSLAPI iError        (ERRCODE code, ...);
    RSLLINK void          RSLAPI ClearErrors   (void);
    RSLLINK int           RSLAPI IsErrors      (void);
    RSLLINK int           RSLAPI IsErrorList   (void);
    RSLLINK char        * RSLAPI GetErrMes     (ERRCODE code);
    RSLLINK char        * RSLAPI GetErrMesEx   (ERRCODE code, int isGui);

    RSLLINK ERRINFO     * RSLAPI ErrorFirst    (void);
    RSLLINK ERRINFO     * RSLAPI ErrorLast     (void);
    RSLLINK ERRINFO     * RSLAPI ErrorNext     (void);
    RSLLINK ERRINFO     * RSLAPI ErrorPrev     (void);
    RSLLINK void          RSLAPI RslLoopErrList(LITRFUNC func, void *data);
    RSLLINK ERRLISTELEM * RSLAPI RslGetCurErr  (void);
    RSLLINK void          RSLAPI RslSetCurErr  (ERRLISTELEM *er);
    RSLLINK void          RSLAPI InitErrors    (void);

    int      RslGetCountError(void);
    ERRINFO *RslGetErrorInfo(int index);

    RSLLINK bool RSLAPI RslGetLastErrorInfo(TExtErrErrInfo *data);

    #ifdef RSL_PL_WIN32
        RSLLINK int  RSLAPI RslSetAxInfo(TRslAxError *axErr);
    #endif

    void RslSaveError    (ERRINFO *erm);
    void SendErrorMessage(ERRINFO *er);

    void FillExtError(TExtErrErrInfo *data, ERRINFO *er);

    RSLLINK int RSLAPI MakeRSCOMError(TExtErrErrInfo *data);

#endif // RSL_DLM

#ifdef __cplusplus
}
#endif

#endif // __IERROR_H

/* <-- EOF --> */