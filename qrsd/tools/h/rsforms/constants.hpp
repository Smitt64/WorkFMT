/*-----------------------------------------------------------------------*-

 File Name   : rsforms\constants.hpp
 Programmer  : Leonid S. Pavlov

 Sybsystem   : rsforms library
 Description : 

 Source      : 

 Library(s)  : RSFSrv.d32

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Apr 12, 2002  Leonid S. Pavlov (LSP) - Create file
 Jun 21, 2002  Nechaev Pavel D. (NPD) - Modify file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSCONSTANTS_H__
#define __RSFORMSCONSTANTS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

const char* const runtime_module    = "rsfsrv";
const char* const terminal_module   = "rsfext";
const char* const report_module     = "rsrepsrv";

const int int_activeWnd = 1;
const int int_mainWnd = 2;

/*-----------------------------------------------------------------------*/

// уровни коммуникационных сообщений
typedef enum tag_invoke_level
{
    LEVEL_DEFAULT     = RSCOM_DEF_LEVEL,
    LEVEL_RSFRUNTIME  = 77,
    LEVEL_BROADCAST,
    LEVEL_INTRINSIC,
    LEVEL_COMMAND,
    LEVEL_BUBBLE
}
invoke_level;

/*-----------------------------------------------------------------------*/

// коды ошибок
typedef enum tag_errCodes
{
    ERR_GENERAL = 0,
    ERR_FSCREATE,           // Ошибка создания RSCOM объекта
    ERR_RTMCREATE,          // Ошибка создания объекта RSForms
    ERR_RTMINTF,            // Ошибка 
    ERR_RSL,                // Ошибка ядра RSL
    ERR_RSHOST,             // Ошибка RSHost
    ERR_LBR,                // Ошибка LBR
    ERR_RSFM,               // Ошибка RSFM
    ERR_RSREPORT,           // Ошибка RSReport
    ERR_HANDLER,            // Ошибка пользователя в обработчеке события
    ERR_LOAD                // Ошибка загрузки или связывания объекта
}
errCodes;

// категории ошибок
typedef enum tag_errCategory
{
   EC_BINDER = 1,           // Ошибки работы биндеров
   EC_SESSIONDATA = 2,      // Ошибки при работе с сессионными данными
   EC_GRID = 4,             // Ошибки грида
   EC_LOAD = 8,             // Ошибки (некритичные) при загрузке формы
   EC_EVENT = 16            // Ошибки, возникающие в обработчиках событий
}
errCategory;

/*-----------------------------------------------------------------------*/

// идентификаторы событий
typedef enum tag_events
{
    EV_ON_LOAD = 0x80018001,
    EV_ON_QUERYUNLOAD = 0x80018002,
    EV_ON_UNLOAD = 0x80018003,
    EV_ON_VIEW_ACTIVATE = 0x80018204,
    EV_ON_VIEW_DEACTIVATE = 0x80018205,
    EV_ON_LOAD_SESSIONDATA = 0x80018206,
    EV_ON_SAVE_SESSIONDATA = 0x80018209,
    EV_ON_CONTEXTMENU = 0x80018207,
    EV_ON_CREATEDATASOURCE = 0x80018208,

    EV_FINALIZE = 0,

    EV_ON_PUTELEMENT = 1,
    EV_ON_PUTHEADER = 2,
    EV_ON_PUTFOOTER = 3,
    EV_ON_GETCONTENTS = 4,
    EV_ON_PAGECOMPLETED = 6,

    EV_ON_CREATE = 10,
    EV_ON_DESTROY,
    EV_ON_SHOWWINDOW,
    EV_ON_ACTIVATE,
    EV_ON_ACTIVEVIEW_CHANGE,
    EV_ON_ENTER_FRAMEMODE,
    EV_ON_EXIT_FRAMEMODE,
    EV_ON_CLOSE,
    EV_ON_ERROR,
    EV_ON_ADDWND,
    EV_ON_REMWND,
    EV_ON_QUERY_EXIT
}
events;

/*-----------------------------------------------------------------------*/

// Тип фильтрации событий
typedef enum tag_evFilterType
{
    FT_ALL = 0,     // фильтрация отсутсвует
    FT_BY_DESIGNER, // фильтрация на основании информации, заданной для объекта в дизайнере
    FT_USER_DEF     // фильтрация на основании информации о добавленных обработчиках
}
evFilterType;

/*-----------------------------------------------------------------------*/

// Window Styles
#ifndef _INC_WINDOWS
typedef enum tag_WindowStyles
{
    WS_OVERLAPPED       = 0x00000000L,
//  WS_POPUP            = 0x80000000L,
    WS_CHILD            = 0x40000000L,
    WS_MINIMIZE         = 0x20000000L,
    WS_VISIBLE          = 0x10000000L,
    WS_DISABLED         = 0x08000000L,
//  WS_CLIPSIBLINGS     = 0x04000000L,
//  WS_CLIPCHILDREN     = 0x02000000L,
    WS_MAXIMIZE         = 0x01000000L,
    WS_CAPTION          = 0x00C00000L,    /* WS_BORDER | WS_DLGFRAME  */
    WS_BORDER           = 0x00800000L,
    WS_DLGFRAME         = 0x00400000L,
    WS_VSCROLL          = 0x00200000L,
    WS_HSCROLL          = 0x00100000L,
    WS_SYSMENU          = 0x00080000L,
    WS_THICKFRAME       = 0x00040000L,
//  WS_GROUP            = 0x00020000L,
//  WS_TABSTOP          = 0x00010000L,

    WS_MINIMIZEBOX      = 0x00020000L,
    WS_MAXIMIZEBOX      = 0x00010000L,

    WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
//  WS_POPUPWINDOW      = WS_POPUP | WS_BORDER | WS_SYSMENU,

    WS_TILED            = WS_OVERLAPPED,
    WS_ICONIC           = WS_MINIMIZE,
    WS_SIZEBOX          = WS_THICKFRAME,
    WS_TILEDWINDOW      = WS_OVERLAPPEDWINDOW,
    WS_CHILDWINDOW      = WS_CHILD
}
WindowStyles;
#else
typedef unsigned long WindowStyles;
#endif // _INC_WINDOWS

/*-----------------------------------------------------------------------*/

// ShowWindow flags
#ifndef _INC_WINDOWS
typedef enum tag_ShowWindowFlags
{
    SW_HIDE            = 0,
    SW_SHOWNORMAL      = 1,
    SW_NORMAL          = 1,
    SW_SHOWMINIMIZED   = 2,
    SW_SHOWMAXIMIZED   = 3,
    SW_MAXIMIZE        = 3,
    SW_SHOWNOACTIVATE  = 4,
    SW_SHOW            = 5,
    SW_MINIMIZE        = 6,
    SW_SHOWMINNOACTIVE = 7,
    SW_SHOWNA          = 8,
    SW_RESTORE         = 9,
    SW_SHOWDEFAULT     = 10,
    SW_FORCEMINIMIZE   = 11,
    SW_MAX             = 11
}
ShowWindowFlags;
#else
typedef int ShowWindowFlags;
#endif // _INC_WINDOWS

/*-----------------------------------------------------------------------*/

// Some Windows messages
#ifndef _INC_WINDOWS
typedef enum tag_WindowMessages
   {
   WM_LBUTTONDOWN                  = 0x0201,
   WM_LBUTTONUP                    = 0x0202,
   WM_LBUTTONDBLCLK                = 0x0203,
   WM_RBUTTONDOWN                  = 0x0204,
   WM_RBUTTONUP                    = 0x0205,
   WM_RBUTTONDBLCLK                = 0x0206,
   WM_MBUTTONDOWN                  = 0x0207,
   WM_MBUTTONUP                    = 0x0208,
   WM_MBUTTONDBLCLK                = 0x0209
   }
WindowMessages;
#else
typedef unsigned int WindowMessages;
#endif // _INC_WINDOWS

/*-----------------------------------------------------------------------*/

// идентификаторы интерфейсов
typedef enum tag_RSFSrvIID
{
    IID_DYN_DISP = 50,
    IID_HOSTOBJECT,
    IID_HOSTOBJECT2,
    IID_EVENTHANDLER,
    IID_EVENTHANDLER2,
    IID_AXOBJ,
    IID_FRAME_WND,
    IID_FRAME_WND2,
    IID_FRAME_VIEW,
    IID_FRAME_VIEW2,
    IID_CONTROL,
    IID_CONTROL2,
    IID_CONTAINER,
    IID_FORM,

    IID_REPORTSGROUP,
    IID_REPORTSGROUP2,
    IID_REPORT,
    IID_REPORT2,

    IID_CMDSET,
    IID_CMDSETCONT,
    IID_CMDTARGET,
    IID_DESIGNEDOBJ,
    IID_TABCTL,
    IID_CLONE,
    IID_SLOTCONT,
    IID_ROLLFORM,
    IID_ROLLSECTION,
    IID_WINDOW,
    IID_CONTROL_LIGHT,
    IID_TRAYICON
}
RSFSrvIID;

/*-----------------------------------------------------------------------*/

// типы объектов для создания на терминале 
const char* const RSFORM_CTRL = "1";
const char* const RSTAB_CTRL  = "2";
const char* const RSROLLSECTION_CTRL = "3";
const char* const RSROLLFORM_CTRL = "5";

const char* const RSREPORTOBJ_CTRL = "3";
const char* const RSREPTABLE_CTRL = "RsCmnCtl.dll:{4BF30560-3BF5-11D6-A95A-0060085C9480}";

/*-----------------------------------------------------------------------*/

// идентификаторы команд для диспетчерских вызовов 
// по уровню LEVEL_RSFRUNTIME
typedef enum tag_dispcommands
{
    RTM_REMALLFRAMES = 0,
    RTM_APPRUN,
    RTM_ONCOMMAND_FORM,
    RTM_ONCOMMAND_WND,
    RTM_SETUSEROBJ,
    RTM_GETUSEROBJ,
    RTM_GETMAINFRAME,
    RTM_CLEAR_UNUSEDOBJ,
    RTM_ENTERMODAL,
    RTM_LEAVEMODAL,
    RTM_ATTACH = 10,
    RTM_DETACH,
    RTM_GETCONTROLOBJ,
    RTM_GETSINK,
    RTM_RAISEEVENT,
    RTM_ADDHANDLER,
    RTM_REMHANDLER,
    RTM_CREATEAX,
    RTM_UNSUBSCRIBE,
    RTM_CREATE,
    RTM_CREATE_EX,
    RTM_DESTROY,
    RTM_SHOW,
    RTM_SHOWMODAL,
    RTM_CLOSE,
    RTM_OPENVIEW,
    RTM_CLOSEVIEW,
    RTM_CONTROLINIT_1,
    RTM_CONTROLINIT_2,
    RTM_GETFULLNAME,
    RTM_GETCONTROLTYPE,
    RTM_GETEVFILTERTYPE,
    RTM_GETFRAME,
    RTM_GETCONTAINER,
    RTM_DOMODAL,
    RTM_SHOWPOPUP,
    RTM_COUNT,
    RTM_ADDITEM,
    RTM_REMITEM,
    RTM_ITEM,
    RTM_FINDITEM,
    RTM_SETTEMPLATE,
    RTM_GETLIBNAME,
    RTM_GETRESNAME,
    RTM_ADDCONTROL,
    RTM_ADDFORM,
    RTM_DEINIT,
//    RTM_REFUSEROBJ,
    RTM_SETPREVPAGLIMIT,
    RTM_PRINT,
    RTM_PUTELEMENTBYNAME,
    RTM_PUTELEMENT,
    RTM_PUTFORMBYRECORDSETBYNAME,
    RTM_PUTFORMBYRECORDSET,
    RTM_SHOWVIEW,
    RTM_GETACTIVEVIEW,
    RTM_GETVIEW,
    RTM_GETNAME,
    RTM_ACTIVATE,
    RTM_ISCREATED,
    RTM_SETGROUPSTATE,
    RTM_HANDLE_ERROR,
    RTM_SETAPPNAME,
    RTM_SETOWNER,
    RTM_SETNAME,
    RTM_SETPARENT,
    RTM_GETPARENT,
    RTM_GETCMDHANDLER,
    RTM_CMDSETCOUNT,
    RTM_GETCMDSET,
    RTM_ADDCMDSET,
    RTM_REMCMDSET,
    RTM_GETCONTAINERNAME,
    RTM_LOADCMDSET,
    RTM_SYSMENU,
    RTM_USERMENU,
    RTM_ADMINMENU,
    RTM_SETCMDFILTER,
    RTM_FINDCMDITEM,
    RTM_CENTERWND,
    RTM_GETSTATUSBAR,
    RTM_TESTELEMENT,
    RTM_TESTELEMENTBYNAME,
    RTM_MOVE,
    RTM_EXITCODE,
    //RTM_SWITCHFRMMODE
    RTM_SETHELPINFO,
    RTM_ISVISIBLE,
    RTM_BINDSLOT,
    RTM_ISLOADED,
    RTM_NAME,
    RTM_FULLNAME, //unused!
    RTM_GETHANDLE,
    RTM_SETREPORTCOPIES,
    RTM_ALTERBASE,
    RTM_GETCMDHANDLER2,
    RTM_SAVE,
    RTM_OPENDOCKVIEW,
    RTM_THIS,
    RTM_SETMAINFRAME,
    RTM_REMCONTROL,
    RTM_SETCONTEXTMENU,
    RTM_JOB_BEGIN,
    RTM_JOB_END,
    //RTM_JOB_PRINT,
    RTM_JOB_ABORT,
    RTM_SHOWSTATUSBAR,
    RTM_SETASYNCMODE,
    RTM_SETICON,
    RTM_SETTEXT
} 
dispcommands;

/*-----------------------------------------------------------------------*/

// режимы печати для отчетов
typedef enum tag_printmode
{
    PM_NORMAL = 0,
    PM_NOCHOOSEPRINTER,
    PM_NOCHOOSEPRINTPRMS
}
printmode;

/*-----------------------------------------------------------------------*/

// типы модальности для окон
typedef enum tag_ModalType
{
    MT_FRAME = 0,
    MT_THREAD = MT_FRAME,
    MT_APPLICATION = 1,
    MT_NONE = 2
} 
ModalType;

/*-----------------------------------------------------------------------*/

// типы состояний для команд меню
typedef enum tag_CommandState
{
    CS_UNDEFINED = 0,
    CS_DISABLED  = 1,
    CS_CHECKED   = 2,
    CS_HIDED     = 4
}
CommandState;

/*-----------------------------------------------------------------------*/

// типы вызова контекстной справки
typedef enum tag_HelpType
{
    HN_TOPIC = 0,
    HN_TOPICID = 1,
    HN_KEYWORDINDEX = 2
} 
HelpType;

/*-----------------------------------------------------------------------*/

// типы создания объекта
typedef enum tag_ConstructType
{
    CT_ATTACH = 0,
    CT_CLONE
}
ConstructType;

/*-----------------------------------------------------------------------*/

// Поведение при установке контекстного меню для контрола
typedef enum tag_SetContextMenuBehaviour
{
    SCM_MERGE = 1,
    SCM_REPLACE = 2
}
SetContextMenuBehaviour;

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSCONSTANTS_H__
