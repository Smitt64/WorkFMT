/************************************************************************/
/*  �����窠 ࠡ��� � �-䠩����                           �ணࠬ����   */
/*  -���� ०����-                            ���ᮢ  �.�. */
/*                                24.10.92                              */
/************************************************************************/

#ifndef __MENUMODE_H
#define __MENUMODE_H

#include "apptldef.h"


// ����⥫� ���� ०����
typedef struct
      {
       const char **NameM;      // ���᮪ �������� ��������� ०����
       const char  *MaketM;     // ��ப�-����� ��������� ०����
       char        *StringM;    // ��ப� ०����
       int          insdel;     // ����� - ��⠢��/㤠�����     
       const char **RefN;       // ���᮪ ��뫮� �� NameM
       int         *NumbN;      // ���᮪ ����஢ ०����
       int          numm;       // ������⢮ ०����
      } MENUMODE;

#ifdef __cplusplus
 extern "C" {
#endif

// ���� ०����
_APPTLFUNC int MenuMode( 
                        const char **NameM,      // ���᮪ �������� ��������� ०����
                        const char  *MaketM,     // ��ப�-����� ��������� ०����
                        char        *StringM,    // ��ப� ०����
                        int          numm,       // ������⢮ ��������� ०����
                        const char  *head,       // ��������� ����
                        int          insdel,     // ����� - ��⠢��/㤠�����     
                        coord        x,          // ���न���� ���孥�� ������ 㣫� ����    
                        coord        y
                       );

// ������஢���� ०����
_APPTLFUNC int EditMode( 
                        const char  **NameM,     // ���᮪ �������� ��������� ०����
                        const char   *MaketM,    // ��ப�-����� ��������� ०����
                        char         *StringM,   // ��ப� ०����
                        int           numm,      // ������⢮ ��������� ०����
                        const char  **head,      // ��������� ����
                        coord         x,         // ���न���� ���孥�� ������ 㣫� ����    
                        coord         y
                       );

// ������� ��ப� � ०��
_APPTLFUNC void str_to_mode(
                            const char *str,     // ��ப�
                            short int  *mode     // �����
                           );

// ������� ०�� � ��ப�
_APPTLFUNC void mode_to_str( 
                            char       *str,     // ��ப�
                            const char *maket,   // �����
                            short int   mode     // �����
                           );

#ifdef __cplusplus
 } ;
#endif

#endif //__MENUMODE_H

/* <-- EOF --> */