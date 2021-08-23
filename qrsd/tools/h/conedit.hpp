/*************************************************************************
   File Name      : conedit.hpp
   Programmer     : Alexey Lebedev

   Creation       : 10 Apr 2003
   Description    : TRSLEditor, TRSLConEditor, TRSLWinEditor classes.

   Copyright(C) 2003 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __CONEDIT_HPP
#define __CONEDIT_HPP

#include "rsl/common.h"

#include "erlist.hpp"
#include "apptldef.h"
#include "edit.h"


///////////////////////////////////////////////////////////////////////////////
// TRSLEditor class
class _APPTLEXP TRSLEditor
    {
     TRSLEditor(const TRSLEditor &) {} // нельзя
     TRSLEditor &operator = (const TRSLEditor &); // нельзя

     protected:
       const char     *Fname;
       const char     *Title;
       int             create;
       int             mode;
       int             canEdit;

       const char     *Primary;
       const char     *output;
       const char     *nameSpace;
       TRslMesProc     msgProc;
       TRslActionProc  runProc;
       void           *userdata;
       int             fMode;

       TRSLErrorsList *listErr;
       bool            isErrors;

       char            Prim[_MAX_PATH];
       char            m_realOutput[_MAX_PATH];

       MENU           *m_pt_save_vG;

       virtual int OnGeneratePCode();
       virtual int OnCompileAndRun(bool noDebug);

       virtual int run() = 0;
       virtual const char *CurrentFileName() = 0;

     public:
       TRSLEditor();
       virtual ~TRSLEditor();

       // Запуск редактора в режиме отладки макрофайла
       int process(const char *Fname, const char *Primary, const char *output, const char *nameSpace,
                   TRslMesProc User_MsgProc, TRslActionProc ProcFun, void *userdata, int create, int mode,
                   int fMode, TRSLErrorsList *listErr);

       // Запуск редактора в режиме просмотра отчетов
       int process(const char *Fname, const char *Title, int create, int mode);

       int initRsl(const char *szNameSpace, TRslMesProc pMsgProc, TRslActionProc pRunProc, void *pUserData,
                   TRSLErrorsList *pErrorList);

       int callRsl(bool pCode, const char *szFileName, const char *szOutput, bool noDebug);
    };

///////////////////////////////////////////////////////////////////////////////
// TRSLConEditor class
class _APPTLEXP TRSLConEditor : public TRSLEditor
    {
     TRSLConEditor(const TRSLConEditor &) {} // нельзя
     TRSLConEditor &operator = (const TRSLConEditor &); // нельзя

     private:
       char  buff[_MAX_PATH];

     protected:
       virtual int  run();
       virtual void processKey(int &event);
       virtual const char *CurrentFileName();

       void HandleTag();
       void HandleBookMark(bool useList);
       void ProcessForms();
       void PrepareMenu(PANEL *pMenu);

     public:
       TRSLConEditor();

       ViewData  vdata;
       bool      tabToSpace;
       int       tabSize;
       bool      indentMode;

       char      extPath[_MAX_PATH];
    };

///////////////////////////////////////////////////////////////////////////////
// TRSLImageEditor class
class _APPTLEXP TRSLImageViewer : public TRSLEditor
    {
     TRSLImageViewer(const TRSLImageViewer &) {} // нельзя
     TRSLImageViewer&operator = (const TRSLImageViewer &); // нельзя

     private:
       char   buff[_MAX_PATH];
       void  *m_pv_Image;       // Указатель на объект класса CRSImage
       bool   m_pv_HasImage;    // Флаг наличия картинки (непустой)

     protected:
       virtual int  run();
       virtual void processKey(int &event);
       virtual const char *CurrentFileName();

       void HandleTag();
       void PrepareMenu(PANEL *pMenu);

     public:
       TRSLImageViewer();

       void SetImageObject(void *data);

       ViewData  vdata;
    };

///////////////////////////////////////////////////////////////////////////////
// TRSLWinEditor class
class TRSLWinEditor : public TRSLEditor
    {
     private:
       // Обработчик KbdIdle
       static int KbdIdle(void);
       // и его параметр
       static TRSLWinEditor  *KbdIdleParam;

       // Старый обработчик
       fs_idle_proc    saveKbdIdle;
       // и его параметр
       TRSLWinEditor  *saveKbdIdleParam;

       // Установить KbdIdle-процедуру
       void SetKbdIdle();
       // Восстановить KbdIdle-процедуру
       void RestKbdIdle();
       // Ждать завершения редактора
       void WaitExit();

       // Перевести редактор в модальное состояние
       virtual bool BeginModal() = 0;
       // Вывести редактор из модального состояния
       virtual bool EndModal() = 0;

       // Загрузить файл
       virtual bool LoadFile(const char *szFileName, bool bReadOnly = false) = 0;
       // Сохранить все файлы
       virtual bool SaveAll() = 0;
       // Перезагрузить все файлы
       virtual bool ReloadAll() = 0;

       // Обработать ошибки компиляции
       virtual bool ProcessError() = 0;

     protected:
       bool  isValid;
       bool  doAbort;

       enum Commands
          {
           CmdRslRun        = 301,
           CmdRslDebug,
           CmdRslCompile,

           CmdStartDesigner = 401,
          };

       // Обработчик меню
       static unsigned int MenuHookProc(unsigned int cmdId, void *obj);

       // Обработчики меню
       virtual void OnRslRun(bool noDebug);
       virtual void OnRslDebug();
       virtual void OnRslCompile();

       // Обработчик меню
       virtual void OnStartDesigner();

       // Обработчик KbdIdle
       virtual int ProcessIdle();
       // Обработчик клавиатуры
       virtual int ProcessKey(int key);

       virtual int run();

     public:
       TRSLWinEditor();

       inline operator bool() const
       {
        return isValid;
       }
    };

// -----------------------------------------------------------------------------
_APPTLFUNC int EditCmpFVEx(const char *Fname, const char *Primary, const char *output, const char *nameSpace,
                           TRslMesProc User_MsgProc, TRslActionProc ProcFun, void *userdata, int create,
                           int mode, int fMode, TRSLErrorsList *listErr);

#endif //__CONEDIT_HPP

/* <-- EOF --> */