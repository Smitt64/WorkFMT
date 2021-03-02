#ifndef _PLAYREPH_
#define _PLAYREPH_

#ifndef __PLAYREP_H
#define __PLAYREP_H

#include "rsl/common.h"
#include "rsl/iexec.h"
#include "rsl/rslres.h"

#include "apptldef.h"


// -----------------------------------------------------------------------------
// ���⨯ �⠭���⭮�� ��ࠡ��稪� ᮮ�饭��
typedef  int (*TFMsgProc)(IMESSAGE mes, void *ptr, void *data);

// -----------------------------------------------------------------------------
//
//              �⠭����� �맮� ��������
//
typedef struct tagSTD_USERDATA  STD_USERDATA;

// -----------------------------------------------------------------------------
// �⠭���⭠� ���ଠ�� � �믮������
struct tagSTD_USERDATA
     {
      bool             stat;               // iTRUE - �ᯥ譮� �믮������
      int              exit_flag;          // EXIT_SHOWOUT,EXIT_NOTSHOW,EXIT_BREAK
      const char      *originalOutput;     // ��� �⠭���⭮�� ��室���� 䠩��
      const char      *output;             // ����饥 ��� �뢮����� 䠩��

      void            *addExecData;
      char            *cmdArgs;

      #ifdef RSL_NO_HOST
          const char  *szFileName;         // ��� 䠩�� � ����-��������
          FILE        *fo;
          PANEL       *pv;
          CNTLIST      stList;             // Status line stack
      #endif 

      #ifndef RSL_NO_HOST
          char         CurrentFileName[_MAX_PATH];
          const char  *addOutput;
      #endif // RSL_NO_HOST

      char             nameSpace[_MAX_NAMESPACE + 1];
     };

// ���祭�� ��⮢ showfile
#define  SF_OUTPUT         1   // �����뢠�� ��室��� 䠩�
#define  SF_EDIT           2   // ����᪠�� ।���� ��� ��ࠢ����� �訡��
#define  SF_ALL            3
#define  SF_PRINT          4
#define  SF_ALTPRINT       8
#define  SF_NOTCLRBREAK   16

#define  MODE_NOT_POPMODULE   1  // �� �⡨���� ����� ��᫥ �ᯮ������
#define  MODE_PRIVATE_MODULE  2  // ���������� ���饭���� �����
#define  MODE_ADDMODULE       4
#define  MODE_SPECIAL         (MODE_NOT_POPMODULE | MODE_PRIVATE_MODULE)


#ifdef __cplusplus
extern "C" {
#endif


_APPTLFUNC void RslSetMoneyMode(unsigned char mode, bool cvt1to1);

// �⠭���⭠� ��楤�� ��ࠡ�⪨ ᮮ�饭��
_APPTLFUNC int Std_MsgProc(IMESSAGE mes, void *ptr, void *UserData);

_APPTLFUNC const char *SetDefInstName(const char *name);

// �믮������ ����-�������
_APPTLFUNC int PlayRep(
                       const char    *szFileName,    // ��� 䠩�� � ����-��������
                       const char    *output,        // ��� �뢮����� 䠩��
                       TRslMesProc    User_MsgProc,  // ���짮��⥫�᪠� ��楤��
                       TRslActionProc ProcFun,       // ��楤�� ����㯠 � ����ᠬ (����易⥫쭮)
                       int            showfile,      // ��������� SF_... 䫠���
                       void          *userdata       // ���짮��⥫�᪨� ����� ��᫥����� ⨯� STD_USERDATA
                      );

#ifndef RSL_NO_PCODE
    #ifndef RSL_NO_HOST
        _APPTLFUNC int PlayGeneration(const char *szFileName, const char *output, const char *nameSpace,
                                      TRslMesProc User_MsgProc, TRslActionProc ProcFun, // NULL
                                      int showfile, void *userdata, bool forEach);

    #else
        _APPTLFUNC int PlayGeneration(const char *szFileName, const char *output, TRslMesProc User_MsgProc,
                                      TRslActionProc ProcFun, // NULL
                                      int showfile, void *userdata, bool forEach);

        int PlayGeneration2(const char *szFileName,  // Macro file name
                            const char *output, TRslMesProc User_MsgProc, TRslActionProc ProcFun,   // NULL
                            void *userdata, int addExec, bool forEach);
    #endif // RSL_HO_HOST

#endif // RSL_NO_PCODE

//              �ਬ�� �맮�� ��������
/*
 BfileSetDataDir ( FileDir );   // ��⠭���� ���� ��४�ਥ� btrieve-䠩���
 SetIncDir (MacroDir);          // ��⠭���� ���� ��४�ਥ� 䠩��� � ���஧�����ﬨ
 BfileSetDictionary ( PrjName );// ��⠭���� ������� ����� ᫮����

 PlayRep( FileMac, FileOut, NULL, NULL, ON, &data );
*/

// �������/������� �맮� ����᪮�� ।����
_APPTLFUNC bool EnableSourceEditor(bool enabled);

_APPTLFUNC int EditCmpFV(const char *Fname, const char *Primary, const char *output,
                         TRslMesProc User_MsgProc, TRslActionProc ProcFun,
                         void *userdata, int create, int mode);

_APPTLFUNC int RunRSL(const char *szFileName,  // Macro file name
                      const char *output, TRslMesProc User_MsgProc, TRslActionProc ProcFun,
                      void *userdata, bool addExec);

_APPTLFUNC const char *MakeSysFileNameRsl(char *nativePath);


_APPTLFUNC int  SwitchDebugMacroFlag(void);
_APPTLFUNC int  SetDebugMacroFlag(int flag);
_APPTLFUNC int  GetDebugMacroFlag(void);
_APPTLFUNC int  SetDebugGUIFlag(int flag);
_APPTLFUNC int  SetDbgOnErrorFlag(int flag);
_APPTLFUNC int  SetRSCOMFlag(int enable);
_APPTLFUNC int  SetUseEditorOnRun(int flag);
_APPTLFUNC int  GetUseEditorOnRun(void);

// ��⠭�����/������� ���� ०�� ࠡ��� ��४���⥫� ०���� �⫠���
// [in]  flag   - 0 - ���� ०�� ࠡ��� ��४���⥫� (��⠭����� ��-㬮�砭��)
//                1 - ���� ०�� ࠡ��� ��४���⥫�
_APPTLFUNC int  SetNewSwitchDebugMode(int flag);
_APPTLFUNC int  GetNewSwitchDebugMode(void);

_APPTLFUNC void RslStep(void);
_APPTLFUNC int  RslSetStepFlag(int stepFlag);
_APPTLFUNC int  ToolsFindMacroFile(char *Buff, const char *file);
                     
#define  FindMacroFile(a, b)  ToolsFindMacroFile(a, b)


void RunRslDialog(void);
void SetFocusToField(void);
void UpdateRslFields(void);
void DisableRslFields(void);
void RslSelectFile(void);       
void SetRslTimer(void);
void RslRunMenu(void);
void AddDlgSupport(void);

void MesGetResPanel (TMesPanelData *data);
void MesGetResPanel2(TMesPanelData *data);
void MesFreeResPanel(TMesPanelData *data);
void MesOpenResFile (TMesResData   *data);
void MesCloseResFile(TMesResData   *data);

_APPTLFUNC int RslOnBtMes(TBtDataBase *db, BtMesType code, void *data);
int RslDefOutProc(const char *outBuff);

// For Backward compatibility

#define  iTRUE   true
#define  iFALSE  false

////////////////////////////////////////////////////////////////////////////
_APPTLFUNC int  SetPCodeFlag(int flag);
_APPTLFUNC int  GetPCodeFlag(void);

////////////////////////////////////////////////////////////////////////////
// ������ ��� �ࠢ����� ࠡ�⮩ PCode
////////////////////////////////////////////////////////////////////////////
#define  RSL_DISABLE_PCODE      0  // ����頥��� �����஢��� � ����㦠�� PCode.
#define  RSL_ONLYLOAD_PCODE     1  // ����頥��� �����஢��� PCode, ⮫쪮 ����㦠�� 
#define  RSL_GENLOAD_PCODE      2  // ࠧ�蠥��� �����஢��� � ����㦠�� PCode.
#define  RSL_AUTOGENLOAD_PCODE  3  // ࠧ�蠥��� ����㦠�� PCode � ��⮬���᪨ �����஢��� PCode.
#define  RSL_NOMAC_PCODE        4  // ࠧ�蠥��� ����㦠�� PCode � �ᯮ�짮����� mac 䠩��� ����������.

///////////////////////////////////////////////////////////////////////////////
#ifndef RSL_NO_HOST ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
   
    #define NAMESPACE_DEFAULT "Default"

    // �����頥� 㪠��⥫� �� ������ �������� ���
    _APPTLFUNC TRSLMsgHandler *RslGetMainHost(void);
    _APPTLFUNC HRSLINST RslSetCurrentInst(HRSLINST inst);
    _APPTLFUNC HRSLINST RslGetCurrentInst(void);
    _APPTLFUNC void *RslGetUserData(void);
    _APPTLFUNC void *RslSetUserData(void *data);
    _APPTLFUNC void *RslGetUserDataEx(const char *nameSpace);
    _APPTLFUNC bool  RslInExec(void);
    _APPTLFUNC bool  RslInExecEx(const char *nameSpace);
    _APPTLFUNC void  CloseMainHost(void);
    _APPTLFUNC void  RslClearCache(void);

    // new --- PlayRep
    _APPTLFUNC int PlayRepEx(const char *szFileName,  // Macro file name
                             const char *output, const char *nameSpace,   // defualt string == "Default"
                             TRslMesProc User_MsgProc, TRslActionProc ProcFun,
                             int showfile, int fMode, void *userdata);

    _APPTLFUNC void RslSetModuleMode(int fMode);
    _APPTLFUNC int  RslGetModuleMode(void);
    _APPTLFUNC void RslInstPopModules(char *nameSpace);


    _APPTLFUNC TRSLMsgHandler *FindHost(TRslMesProc User_MsgProc, STD_USERDATA *userData);
    int  ShowError(ERRINFO *er, FILE *fo);


    typedef int (*TRslDynModProc)(TRslDynModule *data);

    _APPTLFUNC int RslRegDynModProc  (TRslDynModProc ptr);
    _APPTLFUNC int RslUnRegDynModProc(TRslDynModProc ptr);

    _APPTLFUNC int RslClearBreakFlag(int newVal);

    _APPTLFUNC BtrUserFilterType ToolsSetBtrFilter(BtrUserFilterType newFilter);

    _APPTLFUNC BtrUserFilterStat ToolsSetBtrFilterStat(BtrUserFilterStat newFilter);

    // ��⠭����� ���짮��⥫�᪨� ��ࠡ��稪 ����஢���� ᮮ�饭�� RSL.
    // �����頥� 㪠��⥫� �� �।��饥 ���祭�� ��ࠡ��稪� (�.�. NULL).
    _APPTLFUNC TFMsgProc ToolsSetRslTraceHandler(TFMsgProc proc);

    // ��⠭���� ०��� ��ࠢ�� ����஢���� ᮮ�饭��
    // �����頥� �।��饥 ���祭�� ०���
    _APPTLFUNC int ToolsSetRslTraceMode(int mode);

///////////////////////////////////////////////////////////////////////////////
#endif // RSL_NO_HOST /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif __PLAYREP_H
#endif // _PLAYREPH_

/* <-- EOF --> */