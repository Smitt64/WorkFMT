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
// Режим работы трассировочных сообщений (вида IM_TRACE_CALL_)
// Представляет собой битовую маску
#define  dTraceMsgMode_Off               0    // Трассировка отключена (по-умолчанию)
#define  dTraceMsgMode_On           0xFFFF    // Трассировка включена (пока все в пределах 2-х байт)
#define  dTraceMsgMode_Trace             1    // Вывод сообщений через ToRSTrace
#define  dTraceMsgMode_Import            2    // Трассировка импорта модуля
#define  dTraceMsgMode_SpecVar           4    // Трассировка обращений к специальным переменным вида {}
#define  dTraceMsgMode_Var               8    // Трассировка обращений к переменным
#define  dTraceMsgMode_Proc             16    // Трассировка вызовов процедур
#define  dTraceMsgMode_Method           32    // Трассировка обращений к методам класса
#define  dTraceMsgMode_Prop             64    // Трассировка обращений к свойствам класса
#define  dTraceMsgMode_InitClass       128    // Трассировка вызовов инициализатора базового класса
#define  dTraceMsgMode_Macro           256    // Трассировка вызовов макрофайлов
#define  dTraceMsgMode_Macro2          512    // Трассировка вызовов макрофайлов (с выводом пути в трассу)

// Вспомогательный дефайн для трассировки всех операций с объектами
#define  dTraceMsgMode_Class         (dTraceMsgMode_Method | dTraceMsgMode_Prop | dTraceMsgMode_InitClass)


// -----------------------------------------------------------------------------
// Режим проверки наличия dbt-файлов
#define  dExistDbtFileMode_Get          -1    // Получить текущее состояние режима
#define  dExistDbtFileMode_access        0    // Проверка с помощью access
#define  dExistDbtFileMode_bfopen        1    // Проверка с помощью bfOpen

// -----------------------------------------------------------------------------
// Команды для получения системной информации (IM_GET_SYSTEMINFO)
#define  dGSI_MAC                       10
#define  dGSI_IP4                       11
#define  dGSI_CompName                  12
#define  dGSI_UserName                  13
#define  dGSI_CompName2                 14
#define  dGSI_PID                       15
#define  dGSI_TID                       16

// -----------------------------------------------------------------------------
// Сообщения RSL
enum IMESSAGE_V
   {
    IM_BEGIN_PARSE,          // Начало разбора модуля
    IM_END_PARSE,            // Завершение разбора модуля
    IM_AGAIN_PARSE,          //
    IM_NEW_LINE,             //
    IM_ERROR,                //
    IM_BEGIN_EXEC,           // Начало исполнения модуля
    IM_END_EXEC,             // Завершение исполнения модуля
    IM_INIT_SCANER,          //
    IM_INIT_EXECUTOR,        //
    IM_DONE_EXECUTOR,        //
    IM_INPUT_DATA,           //
    IM_SET_STMODULE,         //
    IM_SET_STRUC,            //
    IM_REM_STRUC,            // [13]

    IM_RUN_ERROR,            // [14]
    IM_NOMEM,                // Сообщение отправляется при невозможности выделить блок памяти
    IM_EXEC_STATEMENT,       //
    IM_MODULE_OPEN,          //
    IM_MODULE_CLOSE,         //
    IM_EXIT_MES,             //
    IM_RUN_PROG,             // Выполнение процедуры Run
    IM_UNSERVED_ERROR,       //
    IM_RUNMACRO,             //
    IM_MEM_USAGE,            // Отладочное сообщение при утечках памяти в RSL
    IM_INIT_SCREEN,          //
    IM_DONE_SCREEN,          //
    IM_MESSAGE,              // Вызов процедуры вывода в статус-строку Message
    IM_EXIT_CALLED,          //
    IM_MENU,                 // Вызов процедуры всплывающего меню (Menu)
    IM_INIT_PROGRESS,        // Вызов процедуры InitProgress
    IM_USE_PROGRESS,         // Вызов процедуры UseProgress
    IM_REM_PROGRESS,         // Вызов процедуры RemProgress
    IM_INIT_TRN,             //
    IM_DONE_TRN,             //
    IM_PROCESS_TRN,          //
    IM_VIEW_FILE,            // Вызов просмотрщика текстовых файлов
    IM_CHANGE_OUTPUT,        // Перенаправление вывода
    IM_USERNUMBER,           // Получение номера пользователя
    IM_ADD_MODULE,           // Добавление модуля
    IM_SET_STMODULE_ADD,     //
    IM_GETUSERDATA,          //
    IM_ADD_PROVIDER,         //
    IM_SYSTEM_CALL,          //
    IM_FORMATSTR,            //
    IM_MSGBOX,               // Вызов процедуры MsgBox
    IM_ISSTANDALONE,         //
    IM_TESTEVENT,            //
    IM_ADD_STDPROC,          //
    IM_ISGUI,                //
    IM_ABORT_ON_PARSE_ERROR, //
    IM_CLEAR_INSTANCE,       //
    IM_ADD_NAMERESOLVER,     //
    IM_AX_ERROR,             //
    IM_SET_TIMER_PROC,       // Установка таймерной процедуры
    IM_REM_TIMER_PROC,       // Удаление таймерной процедуры
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
    IM_TRACE,                // Трассировочное сообщения (процедура Trace)
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
    IM_INIT_INSTANCE,        // Первое сообщение после инициализации RslInst
    IM_DONE_INSTANCE,        // Последнее сообщение перед деинициализацией RslInst
    IM_INIT_FS,              //
    IM_ERR_OUTPUT,           //
    IM_END_SPECEXP,          //
    IM_REWIND,               //
    IM_ADD_COMMONSYS,        //
    IM_SAVE_STATE,           // Сохранить состояние парсера (после открытия парсера)
    IM_RESTORE_STATE,        // Восстановить состояние парсера (перед закрытием парсера)
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

    IM_PROXY_MESSAGE,        // Сообщение для "проброски" сообщений RSL в инструментальный механизм (171)

    IM_TRACE_CALL_IMPORT,    // Трассировка импорта модуля
    IM_TRACE_CALL_SPECVAR,   // Трассировка обращений к специальным переменным вида {}
    IM_TRACE_CALL_VAR,       // Трассировка обращений к переменным
    IM_TRACE_CALL_PROC,      // Трассировка вызовов процедур
    IM_TRACE_CALL_METHOD,    // Трассировка обращений к методам класса
    IM_TRACE_CALL_PROP,      // Трассировка обращений к свойствам класса
    IM_TRACE_CALL_INITCLASS, // Трассировка вызовов инициализатора базового класса

    IM_EXIST_DBT_FILE_MODE,  // Режим проверки наличия dbt-файлов
    IM_EXIST_DBT_FILE,       // Проверка наличия dbt-файла

    IM_TRACE_CALL_MACRO,     // Трассировка вызовов макрофайлов

    IM_TRACE_MODE,           // Запрос режима трассировки

    IM_DB_CONVERT_MODE,      // Запрос режима конвертации Numeric -> DBNumeric

    IM_MESSAGE2,             // То же, что и IM_MESSAGE, только без оптимизации вывода

    IM_SEND_NOTIFY,          // Отправить уведомление

    IM_GET_SYSTEMINFO,       // Запросить системную информацию

    IM_GET_TRN_FLAG,         // Вернуть значение флага TrnFlag

    IM_GET_IMAGEOBJECT,      // Вернуть указатель на объект CRSImage по TGenObject (и наоборот)

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
// Для обслуживания сообщения IM_PROPERTY
typedef struct
      {
       int           code; // 0 - get, 1 set, 2 - enum
       const char   *key;
       const char   *value;
       void         *data;
      } TToolsPropertyMsg;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_RSD_TRN_INFO
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
// Для обслуживания сообщения IM_TIMER
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
// Для обслуживания сообщения IM_BREAKCALLBACK
typedef struct
      {
       TRslTimerProc  proc;
       void          *data;
       bool           doInstall;
      } TBrakCallBack;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_MONEY_MODE
typedef struct
      {
       unsigned char  ver;
       unsigned char  mixMode;
       bool           cvtMoney1to1;
      } TRslMoneyMode;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_GETLOCALEINFO
typedef struct
      {
       int    id;
       int    code;
       char  *buff;
       int    szBuff;
      } TRslLocaleData;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_BEGACTION,IM_ENDACTION
typedef struct TActionDlgData
      {
       int          tm;
       const char  *mes;
       int          flags;
       const char  *header;
      } TActionDlgData;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_AX_ERROR
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
       const char  *name;     // Имя модуля
       void  (*proc)(void);   // Процедура, добавляющая модуль
       void  (*init)(void);   // Процедура инициализации модуля
       void  (*done)(void);   // Процедура инициализации модуля
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
// Структура для IM_PROXY_MESSAGE
typedef struct
      {
       int    msgId;             // Идентификатор оригинального сообщения
       void  *data,              // Данные сообщения
             *add;               // Дополнительные данные сообщения
      } TProxyMessage;

// -----------------------------------------------------------------------------
// Структура-описатель основных параметров идентификатора
typedef struct
      {
       const char  *name,        // Наименование идентификатора
                   *file;        // Модуль, содержащий данный идентификатор
       int          type,        // Тип идентификатора (см. SYMTYPE из rsl\isymbol.h)
                    attr;        // Атрибут идентификатора (см. vaXXXXXXX из rsl\isymbol.h)
       int          line;        // Строка в модуле, содержащая данный идентификатор (пока не используется)
      } TIdentData;

// -----------------------------------------------------------------------------
// Базовая структура для обслуживания сообщений вида IM_TRACE_CALL_
typedef struct
      {
       int          version;     // Версия структуры (не ниже 1)

       TIdentData   data,        // Данные идентификатора
                    parent;      // Данные родительского идентификатора

       const char  *file;        // Модуль, из которого вызывается идентификатор (для трассировки)
       int          line,        // Строка в модуле вызова (для трассировки)
                    type,        // Тип модуля (см. RSL_STREAM_* из rsl\istream.h)
                    modeTrace;   // Режим трассировки макросов (TRD::modeTraceMessage)

       // -----
       char         reserv[32];  // Резерв
      } TBaseTraceCall;

// -----------------------------------------------------------------------------
// Структура для IM_EXIST_DBT_FILE
typedef struct
      {
       const char  *name;        // Указатель на имя файла
       int          mode;        // Режим проверки (dExistDbtFileMode_)
      } TExistDbtFile;

// -----------------------------------------------------------------------------
// Структура для IM_SEND_NOTIFY
typedef struct
      {
       const char  *head;        // Заголовок уведомления
       const char  *text;        // Текст уведомления
       int          timeOut;     // Тайм-аут
       int          iconID;      // Идентификатор иконки
       int          secID;       // Идентификатор секции статус-строки
       int          colorText;   // Цвет текста
       int          colorBack;   // Цвет фона
      } TSendNotify;

// -----------------------------------------------------------------------------
// Структура для IM_GET_SYSTEMINFO
typedef struct
      {
       int          cmd;        // Идентификатор запрашиваемой информации (dGSI_*)
       int          remote;     // Признак удалённого получения информации

       void        *buff;       // Буфер данных
      } TSystemInfo;

// -----------------------------------------------------------------------------
// Команды для поля cmd сообщения TGetImageObject

#define  dIM_GIO_CMD_GETOBJECT     0    // Получить объект CRSImage или CRSImageList по TGenObject
#define  dIM_GIO_CMD_STORE         1    // Сохранить объект CRSImage или CRSImageList в буфере data
#define  dIM_GIO_CMD_RESTORE       2    // Восстановить объект CRSImage или CRSImageList из буфера data
#define  dIM_GIO_CMD_FREEDATA      3    // Освободить память
#define  dIM_GIO_CMD_HEADERSIZE    4    // Получить размер заголовка объекта
#define  dIM_GIO_CMD_DATASIZE      5    // Получить размер данных объекта (из заголовка)


// Идентификаторы объектов для полей reqType/objType сообщения TGetImageObject
#define  dIM_GIO_TYPE_IMAGE      100    // Соответствует dCLASSID_IMAGE
#define  dIM_GIO_TYPE_IMAGELIST  200    // Соответствует dCLASSID_IMAGELIST

// -----------------------------------------------------------------------------
// Структура для IM_GET_IMAGE_OBJECT
typedef struct
      {
       int          cmd;        // Идентификатор запрашиваемой информации ()

       int          reqType,    // Тип запрашиваемого объекта (в gObj)
                    objType;    // Тип реального объекта, находящегося в данных data

       bool         ret;        // Результат операции
       int          stat;       // Статус операции

       void        *gObj;       // TGenObject
       void        *cObj;       // Буфер данных (CRSImage или CRSImageList)
       int          size;       // Размер данных в data
       void        *data;       // Буфер данных (память для восстановления объекта)
      } TGetImageObject;

#include "packpop.h"

#ifdef __cplusplus
}
#endif

#endif // __IMESSAGE_H

/* <-- EOF --> */