// -------------------------------------------------------------------------------------------------
// 2013-11-11  Новый механизм справочной системы

#ifndef _RSHELP_H_
#define _RSHELP_H_

#include "menu.h"

// -----------------------------------------------------------------------------
// Установить наименование текущего msg-файла. Возвращается предыдущее значение.
const char *SetCurrentMsgFile(const char *name, int num = 0);

// -----------------------------------------------------------------------------
// Получить наименование текущего msg-файла
const char *GetCurrentMsgFile(int *num = NULL);


#ifdef __cplusplus
 extern "C" {
#endif

// -----------------------------------------------------------------------------
_TOOLEXP PANEL *GetCurrentPanel(void);

// -----------------------------------------------------------------------------
_TOOLEXP PANEL *SetCurrentPanel(PANEL *p);

#ifdef __cplusplus
 } ;
#endif

// -----------------------------------------------------------------------------
PANEL *GetCurrentMenuPanel(void);

// -----------------------------------------------------------------------------
PANEL *SetCurrentMenuPanel(PANEL *p);


// -----------------------------------------------------------------------------
class CCurrentPanel
{
 public:
   CCurrentPanel(PANEL *p = NULL, bool isPanel = true);
  ~CCurrentPanel();

 private :
   PANEL  *m_pv_CurrentPanel;
   bool    m_pv_isPanel;
};

// -----------------------------------------------------------------------------
class CCurrentMsgFile
{
 public:
   CCurrentMsgFile(const char *name = NULL, int num = 0, bool restore = true);
  ~CCurrentMsgFile();

 private :
   const char  *m_pv_CurrentMsgFile;
   int          m_pv_NumberMsg;
   bool         m_pv_Restore;
};


// -----------------------------------------------------------------------------
// Собственно, функция для определения объекта при вызове F1
int GetHelpForObject(int key, char *helpfile, int *page);

#endif // _RSHELP_H_

/* <-- EOF --> */