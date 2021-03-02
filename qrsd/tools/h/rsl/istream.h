/**********************************************************************
*        File Name:   istream.h          March 29,1994
*       Programmer:   S. Kubrin
*      Description:
*  Public routines:
* Private routines:
***********************************************************************/
#ifndef __RSL__ISTREAM_H
#define __RSL__ISTREAM_H


#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------
typedef struct
      {
       LOBJ   std_elem;
       char  *name;           // Имя стандартного модуля
       void  (*proc)(void);   // Процедура, добавляющая модуль
       void  (*init)(void);   // Процедура инициализации модуля
       void  (*done)(void);   // Процедура инициализации модуля
      } STDTABLE;

// -----------------------------------------------
enum
   {
    RSL_STREAM_NULL,
    RSL_STREAM_DOS,
    RSL_STREAM_BTR,
    RSL_STREAM_EXT,
    RSL_STREAM_RSM
   };

// -----------------------------------------------
typedef struct
      {
       unsigned char  mode;  //  RSL_STREAM_DOS, RSL_STREAM_BTR, RSL_STREAM_EXT, RSL_STREAM_RSM

       int            errcode;  // Значение для errno и т.п.

       union
           {
            TDosStream  dos;
            TBMac       btrmac;
           } u;

       char  name[_MAX_PATH];  // Имя обрабатываемого файла

       TRslGetChProc  next_ch;
       TRslCloseProc  closePtr;
       void          *data;  // for next_ch

       TRslReadProc   readPtr;

       int  (*setFileEncode)(int enc, void *data);
      } TOpenData;

//-@H------ BEGIN __ Module Add context________________________ --------@@-

typedef struct
      {
       TAddModContextBase  derived;

       TOpenData   openData;
             
       bool        needCloseParser;
            
       SCSTATE     scSt;

       int         save_CurLevel;
       int         save_ParserLevel;
       bool        save_OnlyDeclared;
      } TAddModContextMac;

// -----------------------------------------------
#ifndef RSL_NO_PCODE
    typedef struct
          {
           TAddModContextBase derived;
           TOpenData          openData;
          } TAddModContextRsm;
#endif // RSL_NO_PCODE

//-@@------ END __ ____________________________________________ --------@E-

typedef struct
      {
       LOBJ        elem;
       SYMMODULE  *addMod;
       bool        isLast;
      } TAddModule;

#ifdef DEBUG
    void CheckMem();
#endif 


#ifndef RSL_NO_PCODE
    RSLLINK void  RSLAPI SetPCodeDir(char *dirlist);
#endif // RSL_NO_PCODE

typedef void (*RslTableProc)(void);

RSLLINK void  RSLAPI AddStdTable  (ISTREAMDATA *data, char *name, RslTableProc proc, RslTableProc init);
RSLLINK void  RSLAPI AddStdTableEx(ISTREAMDATA *data, char *name, RslTableProc proc, RslTableProc init, RslTableProc done);

#ifndef NO_BTR
    bool RePositionInBtrMac(BF *fl, TOpenData *d, bool *exist);
    bool OpenBtrMacro(HRD inst, const char *name, bool *exist, TOpenData *d);

    RSLLINK bool RSLAPI BMacOpen(TBMac *ptr, HRD dt, const char *name, bool *exist);
    RSLLINK int  RSLAPI BMacRead(TBMac *ptr, char *buff, int sz);
    RSLLINK int  RSLAPI BMacSize(TBMac *ptr);

    bool BMacResetCurPosition(TBMac *ptr);
    RSLLINK bool BMacGetDate(HRD dt, const char *name, bdate *fldate);
#endif

int  GetBtrMacChar(void *data);

RSLLINK void RSLAPI InitAddExec   (TAddExecData *data);
RSLLINK void RSLAPI DoneAddExec   (TAddExecData *data, bool doDel);
RSLLINK void RSLAPI DoneAddExecEx (TAddExecData *data, bool doDel, bool isExec );

RSLLINK bool RSLAPI ExecAddModule (TAddExecData *data);
RSLLINK void RSLAPI StopAddModule (TAddExecData *data);

RSLLINK bool RSLAPI ParseUnit     (const char *file, int build);

RSLLINK bool RSLAPI ParseRslSource(ISTREAMDATA *data, const char *module);
RSLLINK void RSLAPI DoneIstream   (ISTREAMDATA *data);
RSLLINK bool RSLAPI InitIstream   (ISTREAMDATA *data);

RSLLINK bool RSLAPI ParseExternUnit     (const char *unitName, TExtSrcInfo *info);
RSLLINK bool RSLAPI ParseExternUnitMem  (const char *unitName, const char *mem);
RSLLINK bool RSLAPI ParseExternSourceMem(ISTREAMDATA *data, const char *unitName, const char *mem);
RSLLINK bool RSLAPI ParseExternUnitNew  (HRD inst, const char *unitName, TExtSrcInfo *info);

bool ParseExternSource(HRD inst, ISTREAMDATA *data, const char *unitName, TExtSrcInfo *info);

bool OpenSrcFile(ISTREAMDATA *data, const char *module, TOpenData *dt, bool pushSt);

TAddModContextBase *PushModState(int sz);
bool PopModState(void);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */