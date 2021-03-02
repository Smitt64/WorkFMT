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
// ��������� ᫮���� � �ଠ� DDF V1
// [in] bpr  - ��� 䠩�� ᫮���� � �ଠ� BSHELL
// [in] path - ��⠫�� � ���஬ ᮧ���� XQL ᫮����
// [in] Over - 1 - ��१������ �������騩 ᫮����
//             0 - �������� � �������饬� ᫮����
// [in] fout - 䠩� ��� �뤠� ���������᪨� ᮮ�饭��
int RunConvert(char *bpr, char *path, int Over, FILE *fout);

// -----------------------------------------------------------------------------
void  xCreate(int beg);

void  xBeginLoop(long nfile, char *file, char *head);
void  xNextLoop(long N, char *name);
void  xEndLoop(void);
int   xBreak(void);


// -----------------------------------------------------------------------------
// ��������� ᫮���� � �ଠ� DDF V2
// [in] bpr  - ��� 䠩�� ᫮���� � �ଠ� BSHELL
// [in] path - ��⠫�� � ���஬ ᮧ���� XQL ᫮����
// [in] Over - 1 - ��१������ �������騩 ᫮����
//             0 - �������� � �������饬� ᫮����
// [in] fout - 䠩� ��� �뤠� ���������᪨� ᮮ�饭��
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