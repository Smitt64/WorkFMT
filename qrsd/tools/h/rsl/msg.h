/*******************************************************************************
 FILE         :   IMESSAGE.H

 COPYRIGHT    :   R-Style Software Lab, 1993, 2000
********************************************************************************/

#ifndef __IMESSAGE_H
#define __IMESSAGE_H

#ifndef __TIMER_H
#  include "timer.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENUMINTSIZE
#pragma option -b-
#endif


// -----------------------------------------------------------------------------
// ����� ࠡ��� ����஢���� ᮮ�饭�� (���� IM_TRACE_CALL_)
// �।�⠢��� ᮡ�� ��⮢�� ����
#define  dTraceMsgMode_Off               0    // ����஢�� �⪫�祭� (��-㬮�砭��)
#define  dTraceMsgMode_On           0xFFFF    // ����஢�� ����祭� (���� �� � �।���� 2-� ����)
#define  dTraceMsgMode_Trace             1    // �뢮� ᮮ�饭�� �१ ToRSTrace
#define  dTraceMsgMode_Import            2    // ����஢�� ������ �����
#define  dTraceMsgMode_SpecVar           4    // ����஢�� ���饭�� � ᯥ樠��� ��६���� ���� {}
#define  dTraceMsgMode_Var               8    // ����஢�� ���饭�� � ��६����
#define  dTraceMsgMode_Proc             16    // ����஢�� �맮��� ��楤��
#define  dTraceMsgMode_Method           32    // ����஢�� ���饭�� � ��⮤�� �����
#define  dTraceMsgMode_Prop             64    // ����஢�� ���饭�� � ᢮��⢠� �����
#define  dTraceMsgMode_InitClass       128    // ����஢�� �맮��� ���樠������ �������� �����
#define  dTraceMsgMode_Macro           256    // ����஢�� �맮��� ����䠩���
#define  dTraceMsgMode_Macro2          512    // ����஢�� �맮��� ����䠩��� (� �뢮��� ��� � �����)

// �ᯮ����⥫�� ��䠩� ��� ����஢�� ��� ����権 � ��ꥪ⠬�
#define  dTraceMsgMode_Class         (dTraceMsgMode_Method | dTraceMsgMode_Prop | dTraceMsgMode_InitClass)


// -----------------------------------------------------------------------------
// ����� �஢�ન ������ dbt-䠩���
#define  dExistDbtFileMode_Get          -1    // ������� ⥪�饥 ���ﭨ� ०���
#define  dExistDbtFileMode_access        0    // �஢�ઠ � ������� access
#define  dExistDbtFileMode_bfopen        1    // �஢�ઠ � ������� bfOpen

// -----------------------------------------------------------------------------
// ������� ��� ����祭�� ��⥬��� ���ଠ樨 (IM_GET_SYSTEMINFO)
#define  dGSI_MAC                       10
#define  dGSI_IP4                       11
#define  dGSI_CompName                  12
#define  dGSI_UserName                  13
#define  dGSI_CompName2                 14

// -----------------------------------------------------------------------------
// ����饭�� RSL
enum IMESSAGE_V
   {
    IM_BEGIN_PARSE,          // ��砫� ࠧ��� �����
    IM_END_PARSE,            // �����襭�� ࠧ��� �����
    IM_AGAIN_PARSE,          //
    IM_NEW_LINE,             //
    IM_ERROR,                //
    IM_BEGIN_EXEC,           // ��砫� �ᯮ������ �����
    IM_END_EXEC,             // �����襭�� �ᯮ������ �����
    IM_INIT_SCANER,          //
    IM_INIT_EXECUTOR,        //
    IM_DONE_EXECUTOR,        //
    IM_INPUT_DATA,           //
    IM_SET_STMODULE,         //
    IM_SET_STRUC,            //
    IM_REM_STRUC,            // [13]

    IM_RUN_ERROR,            // [14]
    IM_NOMEM,                // ����饭�� ��ࠢ����� �� ������������ �뤥���� ���� �����
    IM_EXEC_STATEMENT,       //
    IM_MODULE_OPEN,          //
    IM_MODULE_CLOSE,         //
    IM_EXIT_MES,             //
    IM_RUN_PROG,             // �믮������ ��楤��� Run
    IM_UNSERVED_ERROR,       //
    IM_RUNMACRO,             //
    IM_MEM_USAGE,            // �⫠��筮� ᮮ�饭�� �� ��窠� ����� � RSL
    IM_INIT_SCREEN,          //
    IM_DONE_SCREEN,          //
    IM_MESSAGE,              // �맮� ��楤��� �뢮�� � �����-��ப� Message
    IM_EXIT_CALLED,          //
    IM_MENU,                 // �맮� ��楤��� �ᯫ뢠�饣� ���� (Menu)
    IM_INIT_PROGRESS,        // �맮� ��楤��� InitProgress
    IM_USE_PROGRESS,         // �맮� ��楤��� UseProgress
    IM_REM_PROGRESS,         // �맮� ��楤��� RemProgress
    IM_INIT_TRN,             //
    IM_DONE_TRN,             //
    IM_PROCESS_TRN,          //
    IM_VIEW_FILE,            // �맮� ��ᬮ��騪� ⥪�⮢�� 䠩���
    IM_CHANGE_OUTPUT,        // ��७��ࠢ����� �뢮��
    IM_USERNUMBER,           // ����祭�� ����� ���짮��⥫�
    IM_ADD_MODULE,           // ���������� �����
    IM_SET_STMODULE_ADD,     //
    IM_GETUSERDATA,          //
    IM_ADD_PROVIDER,         //
    IM_SYSTEM_CALL,          //
    IM_FORMATSTR,            //
    IM_MSGBOX,               // �맮� ��楤��� MsgBox
    IM_ISSTANDALONE,         //
    IM_TESTEVENT,            //
    IM_ADD_STDPROC,          //
    IM_ISGUI,                //
    IM_ABORT_ON_PARSE_ERROR, //
    IM_CLEAR_INSTANCE,       //
    IM_ADD_NAMERESOLVER,     //
    IM_AX_ERROR,             //
    IM_SET_TIMER_PROC,       // ��⠭���� ⠩��୮� ��楤���
    IM_REM_TIMER_PROC,       // �������� ⠩��୮� ��楤���
    IM_EXTERN_SRC,           //
    IM_FILELOCK,             //
    IM_SET_TRN_FLAG,         //
    IM_CLR_TRN_FLAG,         //
    IM_POPSTATE_NOT_USED,    //
    IM_BREAKPOINT,           //
    IM_REM_EMPTY_MODULE,     //
    IM_ADD_MODULE2,          //
    IM_GET_CMD_ARG,          //
    IM_BEGIN_ADDEXEC,        //
    IM_END_ADDEXEC,          //
    IM_OPEN_RESFILE,         //
    IM_CLOSE_RESFILE,        //
    IM_GET_RESPANEL,         //
    IM_FREE_RESPANEL,        //
    IM_BTMES,                //
    IM_ADD_GLOBAL_DLMS,      //
    IM_MODE_PCODE,           //
    IM_PCODE_MESSAGE,        //
    IM_CONTINUERUN,          //
    IM_GETWND,               //
    IM_GETDBGMODE,           //
    IM_TRACE,                // ����஢�筮� ᮮ�饭�� (��楤�� Trace)
    IM_REM_MODULE,           //
    IM_GETTRACEFLAG,         //
    IM_SETTRACEFLAG,         //
    IM_REM_MODULE2,          //
    IM_OUTPUT,               //
    IM_ADD_C_INTF,           //
    IM_GET_C_INTF,           //
    IM_CACHE_LINKINST,       //
    IM_CACHE_UNLINKINST,     //
    IM_GETHOST,              //
    IM_DBG_ERROR,            //
    IM_ADD_COMMON,           //
    IM_INSPECT_SYM,          //
    IM_SET_EXITFLAG,         //
    IM_GET_NAMESPACE,        //
    IM_DEL_NAMESPACE,        //
    IM_GETCACHE,             //
    IM_CHANGE_ERRORS,        //
    IM_BEGIN_GEN,            //
    IM_END_GEN,              //
    IM_INIT_INSTANCE,        // ��ࢮ� ᮮ�饭�� ��᫥ ���樠����樨 RslInst
    IM_DONE_INSTANCE,        // ��᫥���� ᮮ�饭�� ��। �����樠����樥� RslInst
    IM_INIT_FS,              //
    IM_ERR_OUTPUT,           //
    IM_END_SPECEXP,          //
    IM_REWIND,               //
    IM_ADD_COMMONSYS,        //
    IM_SAVE_STATE,           // ���࠭��� ���ﭨ� ����� (��᫥ ������ �����)
    IM_RESTORE_STATE,        // ����⠭����� ���ﭨ� ����� (��। �����⨥� �����)
    IM_GETPRNINFO,           //
    IM_SETPRNINFO,           //
    IM_RUBTOSTR,             //
    IM_MONTOSTR,             //
    IM_GETDIRINFO,           //
    IM_TRNWRITE,             //
    IM_TRNBEGIN,             //
    IM_TRNEND,               //
    IM_TRNABORT,             //
    IM_BTR_CLIENT_MODE,      //
    // Add by Ushakov
    IM_INIT_INSTANCE_REC,    //
    IM_DONE_INSTANCE_REC,    //
    //
    IM_SET_DEBUG_MODULE,     //
    IM_NUMTOSTR,             //
    IM_GET_APPOBJ,           //
    IM_GET_RCWHOST,          //
    IM_DUMMY1,               //
    IM_GETHOSTINFO,          //
    IM_GETSTDSTRM,           //
    IM_GET_INSTANCE,         //
    IM_GETDIRINFOEX,         //
    IM_ADDEVENT,             //
    IM_RSCOM_ERR,            //
    IM_MONTOSTR_EX,          //
    IM_SET_DEBUG_FLAG,       //
    IM_BTRMACFILE,           //
    IM_DT_TRN,               //
    IM_DYNAMIC_MODULE,       //
    IM_ERRBOX,               //
    IM_GETBREAKYEAR,         //
    IM_GET_RSD_INFO,         //
    IM_ISSQLMODE,            //
    IM_RUN_ERROR_EX,         //
    IM_BTRVEM_HANDLE,        //
    IM_BTR_USE_EXT_CLIENT,   //
    IM_BEGACTION,            //
    IM_ENDACTION,            //
    IM_INITRSCOM,            //
    IM_ENTER_SCRIPT,         //
    IM_LEAVE_SCRIPT,         //
    IM_ENBL32DECEMBER,       //
    IM_LOGDIV100,            //
    IM_GETLOCALEINFORMT,     //
    IM_GETLANGIDRMT,         //
    IM_MONEY_MODE,           //
    IM_BEGACTIONEX,          //
    IM_PREMODAL,             //
    IM_POSTMODAL,            //
    IM_INPUT_DATAEX,         //
    IM_BREAKCALLBACK,        //
    IM_BRK2DBG,              //
    IM_NUMTOSTR2,            //
    IM_RUBTOSTR2,            //
    IM_GETINDIC,             //
    IM_FREEINDIC,            //
    IM_GETSTACKSIZE,         //
    IM_PROCESS_TRN_FINAL,    //
    IM_GETUIMODE,            //
    IM_RSD_TRN_INFO,         //
    IM_GET_RESPANEL2,        //
    IM_FREE_RESPANEL2,       //
    IM_CHACTION,             //
    IM_PROPERTY,             //
    IM_TIMER,                //
    IM_USER_ERROR,           //

    IM_PROXY_MESSAGE,        // ����饭�� ��� "�஡�᪨" ᮮ�饭�� RSL � �����㬥�⠫�� ��堭��� (171)

    IM_TRACE_CALL_IMPORT,    // ����஢�� ������ �����
    IM_TRACE_CALL_SPECVAR,   // ����஢�� ���饭�� � ᯥ樠��� ��६���� ���� {}
    IM_TRACE_CALL_VAR,       // ����஢�� ���饭�� � ��६����
    IM_TRACE_CALL_PROC,      // ����஢�� �맮��� ��楤��
    IM_TRACE_CALL_METHOD,    // ����஢�� ���饭�� � ��⮤�� �����
    IM_TRACE_CALL_PROP,      // ����஢�� ���饭�� � ᢮��⢠� �����
    IM_TRACE_CALL_INITCLASS, // ����஢�� �맮��� ���樠������ �������� �����

    IM_EXIST_DBT_FILE_MODE,  // ����� �஢�ન ������ dbt-䠩���
    IM_EXIST_DBT_FILE,       // �஢�ઠ ������ dbt-䠩��

    IM_TRACE_CALL_MACRO,     // ����஢�� �맮��� ����䠩���

    IM_TRACE_MODE,           // ����� ०��� ����஢��

    IM_DB_CONVERT_MODE,      // ����� ०��� �������樨 Numeric -> DBNumeric

    IM_MESSAGE2,             // �� ��, �� � IM_MESSAGE, ⮫쪮 ��� ��⨬���樨 �뢮��

    IM_SEND_NOTIFY,          // ��ࠢ��� 㢥��������

    IM_GET_SYSTEMINFO,       // ������� ��⥬��� ���ଠ��

    IM_GET_TRN_FLAG,         // ������ ���祭�� 䫠�� TrnFlag

    IM_GET_IMAGEOBJECT,      // ������ 㪠��⥫� �� ��ꥪ� CRSImage �� TGenObject (� �������)

    // -------------------------------------------
    IM_USER = 500,           //

    IM_OPENCONSOLE  = 1001,  //
    IM_CLOSECONSOLE = 1002   // For VCH in EasyWin support
   };

// -----------------------------------------------------------------------------
typedef int  IMESSAGE;
typedef int (*TRslMesProc)(IMESSAGE, void *, void *);
typedef int (*TRslOutProc)(const char *outBuff);

#ifdef ENUMINTSIZE
#pragma option -b
#endif

#include "packpshn.h"


// -----------------------------------------------------------------------------
// ��� ���㦨����� ᮮ�饭�� IM_PROPERTY
typedef struct
      {
       int           code; // 0 - get, 1 set, 2 - enum
       const char   *key;
       const char   *value;
       void         *data;
      } TToolsPropertyMsg;

// -----------------------------------------------------------------------------
// ��� ���㦨����� ᮮ�饭�� IM_RSD_TRN_INFO
//  oper one of:
//   BBT     = 19,     /* Begin transaction. No parametrs     */
//   BET     = 20,     /* End transaction. No parametrs    */
//   BAT     = 21,     /* Abort transaction. No parametrs     */
typedef struct
      {
       int         oper;
       short int   btrvEm;
      } TRsdTrnInfo;

// -----------------------------------------------------------------------------
// ��� ���㦨����� ᮮ�饭�� IM_TIMER
typedef struct
      {
       unsigned long  timeOut;
       TRslTimerProc  proc;
       void          *data;
       unsigned       id;
       int            result;
       bool           doInstall;
      } TTimerData;

// -----------------------------------------------------------------------------
// ��� ���㦨����� ᮮ�饭�� IM_BREAKCALLBACK
typedef struct
      {
       TRslTimerProc  proc;
       void          *data;
       bool           doInstall;
      } TBrakCallBack;

// -----------------------------------------------------------------------------
// ��� ���㦨����� ᮮ�饭�� IM_MONEY_MODE
typedef struct
      {
       unsigned char  ver;
       unsigned char  mixMode;
       bool           cvtMoney1to1;
      } TRslMoneyMode;

// -----------------------------------------------------------------------------
// ��� ���㦨����� ᮮ�饭�� IM_GETLOCALEINFO
typedef struct
      {
       int    id;
       int    code;
       char  *buff;
       int    szBuff;
      } TRslLocaleData;

// -----------------------------------------------------------------------------
// ��� ���㦨����� ᮮ�饭�� IM_BEGACTION,IM_ENDACTION
typedef struct TActionDlgData
      {
       int          tm;
       const char  *mes;
       int          flags;
       const char  *header;
      } TActionDlgData;

// -----------------------------------------------------------------------------
// ��� ���㦨����� ᮮ�饭�� IM_AX_ERROR
typedef struct TRslAxError
      {
       char          *desc;
       unsigned long  wCode;
       unsigned long  sCode;
      } TRslAxError;

// -----------------------------------------------------------------------------
// For handle IM_RSCOM_ERR
typedef struct tagTRsComErrMes
      {
       void  *app;
       int    stat;
      } TRsComErrMes;

// -----------------------------------------------------------------------------
typedef struct tagGetIntf
      {
       const char  *intfName;
       void        *intf;
      } TGetCIntf;

// -----------------------------------------------------------------------------
typedef struct
      {
       int     mode;
       int     numrows;
       int     numcols;
       int     vmode;
       int     mouse;
       int     argc;
       char  **argv;
      } TFsInitData;

// -----------------------------------------------------------------------------
#define  FS_NORM_MODE  0
#define  FS_COMM_MODE  1
#define  FS_NOFS_MODE  2

// -----------------------------------------------------------------------------
// For handle IM_GETPRNINFO and IM_SETPRNINFO message
typedef struct
      {
       char  *prnName;
       char  *escSeq;
       char  *banner;
       bool   frmFeed;
      } TGetSetPrnInfo;

// -----------------------------------------------------------------------------
// For handle IM_RUBTOSTR and IM_RUBTOSTR2
typedef struct
      {
       VALUE  *val;
       char   *kop;
       char   *sum;
       char   *full;
       int     code;
       bool    fullText;
       int     prec;
      } TRubToStrData;

// -----------------------------------------------------------------------------
// For handle IM_NUMTOSTR
typedef struct
      {
       double  val;
       char   *n1;
       char   *n2;
       char   *n3;
       int     rod;
       int     prec;
       char   *buff;
      } TNumToStrData;

// -----------------------------------------------------------------------------
#ifdef USE_NUMERIC
    // For handle IM_NUMTOSTR2
    typedef struct
          {
           Numeric_t  *val;
           char       *n1;
           char       *n2;
           char       *n3;
           int         rod;
           int         prec;
           char       *buff;
          } TNumToStrDataEx;
#endif

// -----------------------------------------------------------------------------
typedef struct
      {
       VALUE  *val;
       char   *buff;
       char   *year;
      } TMonToStrData;

// -----------------------------------------------------------------------------
// IM_GETDIRINFO
typedef struct
      {
       const char  *ToolsDef;
       const char  *ToolsDbt;
       const char  *ToolsMac;
       const char  *ToolsTxt;
       const char  *ToolsDbf;
       const char  *ToolsLbr;
       const char  *ToolsRsm;
      } TRslDirInfo;

// -----------------------------------------------------------------------------
// IM_GETDIRINFOEX
typedef struct
      {
       const char  *ToolsDef;
       const char  *ToolsDbt;
       const char  *ToolsMac;
       const char  *ToolsTxt;
       const char  *ToolsDbf;
       const char  *ToolsLbr;
       const char  *ToolsRsm;

       int          version;
       const char  *ToolsLbrDir;  // In version 1
      } TRslDirInfoEx;

// -----------------------------------------------------------------------------
// IM_BTR_CLIENT_MODE
typedef struct
      {
       bool  useOwnBtrClient;
       bool  keepDBopen;
       bool  keepBrtMacopen;
       bool  keepBtrStruct;
       bool  useBtrMacDDF;
      } TBtrClientMode;

// -----------------------------------------------------------------------------
// IM_GET_APPOBJ, IM_GET_RCWHOST
typedef struct
      {
       void  *obj;
       void  *instObj;
      } TAppObjData;

// -----------------------------------------------------------------------------
// IM_GETHOSTINFO

typedef struct
      {
       int       version;

    // version 0 fields

       bool      useFdecimal;
       bool      isGUI;
       unsigned  toolsVersion;
      } THostInfoData;

// -----------------------------------------------------------------------------
// IM_GETSTDSTRM
typedef struct
      {
       FILE  *stdOut;
       FILE  *stdErr;
      } TRslStdStrm;

// -----------------------------------------------------------------------------
// IM_DT_TRN
typedef struct
      {
       int  code;
       int  lock;
       int  stat;
      } RslDtTrn;

// -----------------------------------------------------------------------------
// IM_DYNAMIC_MODULE
typedef struct
      {
       const char  *name;     // ��� �����
       void  (*proc)(void);   // ��楤��, ���������� �����
       void  (*init)(void);   // ��楤�� ���樠����樨 �����
       void  (*done)(void);   // ��楤�� ���樠����樨 �����
      } TRslDynModule;

// -----------------------------------------------------------------------------
// IM_ERRBOX
typedef struct
      {
       void          *obj;   // TArray
       const char    *caption;
       unsigned long  flags;
       bool           useAnsi;
      } TErrBoxData;

// -----------------------------------------------------------------------------
// IM_GET_RSD_INFO
typedef struct
      {
       const char  *conStr;
       void        *handle;
       void        *intf;
      } TRsdInfo;

// -----------------------------------------------------------------------------
// ������� ��� IM_PROXY_MESSAGE
typedef struct
      {
       int    msgId;             // �����䨪��� �ਣ����쭮�� ᮮ�饭��
       void  *data,              // ����� ᮮ�饭��
             *add;               // �������⥫�� ����� ᮮ�饭��
      } TProxyMessage;

// -----------------------------------------------------------------------------
// �������-����⥫� �᭮���� ��ࠬ��஢ �����䨪���
typedef struct
      {
       const char  *name,        // ������������ �����䨪���
                   *file;        // �����, ᮤ�ঠ騩 ����� �����䨪���
       int          type,        // ��� �����䨪��� (�. SYMTYPE �� rsl\isymbol.h)
                    attr;        // ��ਡ�� �����䨪��� (�. vaXXXXXXX �� rsl\isymbol.h)
       int          line;        // ��ப� � ���㫥, ᮤ�ঠ�� ����� �����䨪��� (���� �� �ᯮ������)
      } TIdentData;

// -----------------------------------------------------------------------------
// ������� ������� ��� ���㦨����� ᮮ�饭�� ���� IM_TRACE_CALL_
typedef struct
      {
       int          version;     // ����� �������� (�� ���� 1)

       TIdentData   data,        // ����� �����䨪���
                    parent;      // ����� த�⥫�᪮�� �����䨪���

       const char  *file;        // �����, �� ���ண� ��뢠���� �����䨪��� (��� ����஢��)
       int          line,        // ��ப� � ���㫥 �맮�� (��� ����஢��)
                    type,        // ��� ����� (�. RSL_STREAM_* �� rsl\istream.h)
                    modeTrace;   // ����� ����஢�� ����ᮢ (TRD::modeTraceMessage)

       // -----
       char         reserv[32];  // �����
      } TBaseTraceCall;

// -----------------------------------------------------------------------------
// ������� ��� IM_EXIST_DBT_FILE
typedef struct
      {
       const char  *name;        // �����⥫� �� ��� 䠩��
       int          mode;        // ����� �஢�ન (dExistDbtFileMode_)
      } TExistDbtFile;

// -----------------------------------------------------------------------------
// ������� ��� IM_SEND_NOTIFY
typedef struct
      {
       const char  *head;        // ��������� 㢥��������
       const char  *text;        // ����� 㢥��������
       int          timeOut;     // ����-���
       int          iconID;      // �����䨪��� ������
       int          secID;       // �����䨪��� ᥪ樨 �����-��ப�
       int          colorText;   // ���� ⥪��
       int          colorBack;   // ���� 䮭�
      } TSendNotify;

// -----------------------------------------------------------------------------
// ������� ��� IM_GET_SYSTEMINFO
typedef struct
      {
       int          cmd;        // �����䨪��� ����訢����� ���ଠ樨 (dGSI_*)
       int          remote;     // �ਧ��� 㤠�񭭮�� ����祭�� ���ଠ樨

       void        *buff;       // ���� ������
      } TSystemInfo;

// -----------------------------------------------------------------------------
// ������� ��� ���� cmd ᮮ�饭�� TGetImageObject

#define  dIM_GIO_CMD_GETOBJECT     0    // ������� ��ꥪ� CRSImage ��� CRSImageList �� TGenObject
#define  dIM_GIO_CMD_STORE         1    // ���࠭��� ��ꥪ� CRSImage ��� CRSImageList � ���� data
#define  dIM_GIO_CMD_RESTORE       2    // ����⠭����� ��ꥪ� CRSImage ��� CRSImageList �� ���� data
#define  dIM_GIO_CMD_FREEDATA      3    // �᢮������ ������
#define  dIM_GIO_CMD_HEADERSIZE    4    // ������� ࠧ��� ��������� ��ꥪ�
#define  dIM_GIO_CMD_DATASIZE      5    // ������� ࠧ��� ������ ��ꥪ� (�� ���������)


// �����䨪���� ��ꥪ⮢ ��� ����� reqType/objType ᮮ�饭�� TGetImageObject
#define  dIM_GIO_TYPE_IMAGE      100    // ���⢥����� dCLASSID_IMAGE
#define  dIM_GIO_TYPE_IMAGELIST  200    // ���⢥����� dCLASSID_IMAGELIST

// -----------------------------------------------------------------------------
// ������� ��� IM_GET_IMAGE_OBJECT
typedef struct
      {
       int          cmd;        // �����䨪��� ����訢����� ���ଠ樨 ()

       int          reqType,    // ��� ����訢������ ��ꥪ� (� gObj)
                    objType;    // ��� ॠ�쭮�� ��ꥪ�, ��室�饣��� � ������ data

       bool         ret;        // ������� ����樨
       int          stat;       // ����� ����樨

       void        *gObj;       // TGenObject
       void        *cObj;       // ���� ������ (CRSImage ��� CRSImageList)
       int          size;       // ������ ������ � data
       void        *data;       // ���� ������ (������ ��� ����⠭������� ��ꥪ�)
      } TGetImageObject;

#include "packpop.h"

#ifdef __cplusplus
}
#endif

#endif // __IMESSAGE_H

/* <-- EOF --> */