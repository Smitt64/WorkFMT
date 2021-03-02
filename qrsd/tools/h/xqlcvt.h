/**********************************************************************
*        File Name:   xqlcvt.h           February 16,1994            
*       Programmer:   S. Kubrin              
*      Description:   
*  Public routines:  
* Private routines:  
***********************************************************************/

#ifndef _XQLCVT_H_
#define _XQLCVT_H_

#include <stdio.h>

#ifdef __cplusplus
 extern "C" {
#endif

// -----------------------------------------------------------------------------
// Конвертация словаря в формат DDF V1
// [in] bpr  - имя файла словаря в формате BSHELL
// [in] path - каталог в котором создать XQL словарь
// [in] Over - 1 - перезаписать существующий словарь
//             0 - добавить к существующему словарю
// [in] fout - файл для выдачи диагностических сообщений
int RunConvert(char *bpr, char *path, int Over, FILE *fout);

// -----------------------------------------------------------------------------
void  xCreate(int beg);

void  xBeginLoop(long nfile, char *file, char *head);
void  xNextLoop(long N, char *name);
void  xEndLoop(void);
int   xBreak(void);


// -----------------------------------------------------------------------------
// Конвертация словаря в формат DDF V2
// [in] bpr  - имя файла словаря в формате BSHELL
// [in] path - каталог в котором создать XQL словарь
// [in] Over - 1 - перезаписать существующий словарь
//             0 - добавить к существующему словарю
// [in] fout - файл для выдачи диагностических сообщений
int RunConvert2(char *bpr, char *path, int Over, FILE *fout);


// -----------------------------------------------------------------------------
void  x2Create(int beg);

void  x2BeginLoop(long nfile, char *file, char *head);
void  x2NextLoop(long N, char *name);
void  x2EndLoop(void);
int   x2Break(void);

#ifdef __cplusplus
 } ;
#endif

#endif _XQLCVT_H_

/* <-- EOF --> */