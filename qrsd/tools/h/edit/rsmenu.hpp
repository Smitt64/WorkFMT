#ifndef __RSMENU_HPP
#define __RSMENU_HPP
/*************************************************************************
   File Name      : rsmenu.hpp
   Programmer     : Alexey Lebedev


   Creation       : 19 Oct 2002
   Description    : RSMenu class.

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __PLATFORM_H
#include "platform.h"
#endif // __PLATFORM_H

#ifdef RSL_PL_WIN32

#ifndef _INC_WINDOWS
#define STRICT
#include <windows.h>
#endif

#ifndef __PLATFORM_H
#include "platform.h"
#endif // __PLATFORM_H

#ifndef RSL_RSMENU_DEFS
#define RSL_RSMENU_DEFS

// -----------------------------------------------------------------------------
RSL_MAKE_HANDLE(HRSMenu);

// -----------------------------------------------------------------------------
enum RSMenuFlags
     {
      RSMenuDisable   = 1,     //
      RSMenuChecked   = 2,     //
      RSMenuUnchecked = 4,     //
      RSMenuInactive  = 8      //
     };

// -----------------------------------------------------------------------------
enum RSAccelFlags
   {
    RSAltKey     = 1,          //
    RSShiftKey   = 2,          //
    RSControlKey = 4           //
   };

// -----------------------------------------------------------------------------
#include "packpshn.h"

typedef struct tagRSMenuItem
      {
       const char    *itemName;
       unsigned int   itemFlags;
       unsigned int   accelFlags;
       unsigned int   accelKey;
       unsigned int   commandId;
       int            imageId;
       HRSMenu        hSubMenu;
      } RSMenuItem;

#include "packpop.h"

// -----------------------------------------------------------------------------
typedef unsigned int (RSMenuHookProc)(unsigned int commandId, void *);

#endif //RSL_RSMENU_DEFS


///////////////////////////////////////////////////////////////////////////////
// RSMenu class

class RSMenu
    {
     // Construction
     public:
       inline RSMenu();

     // Operations
     public:
       inline bool Create();
       inline bool CreateIndirect(const RSMenuItem *MenuItems);
       inline void Destroy();

       inline bool Attach(HANDLE hMenu);
       inline void Detach();

       inline static RSMenu *FromHandle(HANDLE hMenu);
       inline HANDLE GetHandle();

       inline int  GetNumItems();
       inline bool GetItem(int nIndex, RSMenuItem *MenuItem);

       inline bool AddMenuItem(int nPosition, bool bCommand, const RSMenuItem *MenuItem);
       inline bool AddSubMenu(int nPosition, bool bCommand, const char *szMenuName, RSMenu &SubMenu);

     // Implementation
     public:
       inline ~RSMenu();

     private:
       inline bool GetProcAddr(const char *szNameProc, FARPROC *pProcAddr);

     // Attributes
     private:
       HINSTANCE m_hInst;
       HANDLE m_hMenu;
       bool   m_owned;

       bool (*m_pCreate)(HANDLE*);
       bool (*m_pCreateIndirect)(HANDLE*, const RSMenuItem *MenuItems);
       void (*m_pDestroy)(HANDLE);

       int  (*m_pGetNumItems)(HANDLE);
       bool (*m_pGetItem)(HANDLE, int nIndex, RSMenuItem *MenuItem);

       bool (*m_pAddMenuItem)(HANDLE, int nPosition, bool bCommand, const RSMenuItem *MenuItem);
       bool (*m_pAddSubMenu)(HANDLE, int nPosition, bool bCommand, const char *szMenuName, HANDLE hSubMenu);
    };

///////////////////////////////////////////////////////////////////////////////

inline RSMenu::RSMenu()
{
 m_hMenu = NULL;
 m_owned = true;

 // Загружаем DLL
 m_hInst = ::GetModuleHandle("SrcEdit.dll");

 if(!m_hInst)
   return;

 // Получаем адреса экспортируемых процедур
 if(!GetProcAddr("RSMenu_Create",         (FARPROC *)&m_pCreate)         ||
    !GetProcAddr("RSMenu_CreateIndirect", (FARPROC *)&m_pCreateIndirect) ||
    !GetProcAddr("RSMenu_Destroy",        (FARPROC *)&m_pDestroy)        ||

    !GetProcAddr("RSMenu_GetNumItems",    (FARPROC *)&m_pGetNumItems)    ||
    !GetProcAddr("RSMenu_GetItem",        (FARPROC *)&m_pGetItem)        ||

    !GetProcAddr("RSMenu_AddMenuItem",    (FARPROC *)&m_pAddMenuItem)    ||
    !GetProcAddr("RSMenu_AddSubMenu",     (FARPROC *)&m_pAddSubMenu))
  {
   m_hInst = NULL;
  }
}

// -----------------------------------------------------------------------------
inline RSMenu::~RSMenu()
{
 if(m_owned && m_hMenu)
   m_pDestroy(m_hMenu);
}

// -----------------------------------------------------------------------------
inline bool RSMenu::GetProcAddr(const char *szNameProc, FARPROC *pProcAddr)
{
 FARPROC  ProcAddr = ::GetProcAddress(m_hInst, szNameProc);

 if(!ProcAddr)
   return false;

 *pProcAddr = ProcAddr;

 return true;
}

///////////////////////////////////////////////////////////////////////////////

inline bool RSMenu::Create()
{
 if(!m_hInst)
   return false;

 return m_pCreate(&m_hMenu);
}

// -----------------------------------------------------------------------------
inline bool RSMenu::CreateIndirect(const RSMenuItem *MenuItems)
{
 if(!m_hInst)
   return false;

 return m_pCreateIndirect(&m_hMenu, MenuItems);
}

// -----------------------------------------------------------------------------
inline void RSMenu::Destroy()
{
 if(!m_hInst)
   return;

 if(m_owned && m_hMenu)
   m_pDestroy(m_hMenu);

 m_hMenu = NULL;
}

///////////////////////////////////////////////////////////////////////////////

inline bool RSMenu::Attach(HANDLE hMenu)
{
 if(m_owned && m_hMenu)
   m_pDestroy(m_hMenu);

 m_hMenu = hMenu;
 m_owned = false;

 return hMenu ? true : false;
}

// -----------------------------------------------------------------------------
inline void RSMenu::Detach()
{
 m_hMenu = NULL;
 m_owned = false;
}

///////////////////////////////////////////////////////////////////////////////

inline RSMenu *RSMenu::FromHandle(HANDLE hMenu)
{
 RSMenu  *pMenu = new RSMenu;

 if(!pMenu->m_hInst)
   return NULL;

 pMenu->m_hMenu = hMenu;
 pMenu->m_owned = false;

 return pMenu;
}

// -----------------------------------------------------------------------------
inline HANDLE RSMenu::GetHandle()
{
 return m_hMenu;
}

///////////////////////////////////////////////////////////////////////////////

inline int RSMenu::GetNumItems()
{
 if(!m_hInst)
   return -1;

 return m_pGetNumItems(m_hMenu);
}

// -----------------------------------------------------------------------------
inline bool RSMenu::GetItem(int nIndex, RSMenuItem *MenuItem)
{
 if(!m_hInst)
   return false;

 return m_pGetItem(m_hMenu, nIndex, MenuItem);
}

///////////////////////////////////////////////////////////////////////////////

inline bool RSMenu::AddMenuItem(int nPosition, bool bCommand, const RSMenuItem *MenuItem)
{
 if(!m_hInst)
   return false;

 return m_pAddMenuItem(m_hMenu, nPosition, bCommand, MenuItem);
}

// -----------------------------------------------------------------------------
inline bool RSMenu::AddSubMenu(int nPosition, bool bCommand, const char *szMenuName, RSMenu &SubMenu)
{
 if(!m_hInst)
   return false;

 if(!m_pAddSubMenu(m_hMenu, nPosition, bCommand, szMenuName, SubMenu.m_hMenu))
   return false;

 SubMenu.m_owned = false;

 return true;
}

#endif // RSL_PL_WIN32

#endif // __RSMENU_HPP

/* <-- EOF --> */