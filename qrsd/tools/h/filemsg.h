//***********************************************************************
//                        ����饭�� �� 䠩��
//                         VECsoft  2.12.92
//***********************************************************************
#ifndef __FILEMSG_H
#define __FILEMSG_H

#include "rstypes.h"
#include "apptldef.h"

#ifdef __cplusplus
 extern "C" {
#endif


#include <packpsh1.h>

typedef struct FILEMSG_OLD {                // ����饭�� �� 䠩��

       db_uint16  Number;           // ����� ᮮ�饭��
       db_uint16  Page;             // ����� ��࠭��� �����
       char       Contents[81];     // ����ঠ���
      } FILEMSG_OLD;


// ����饭�� �� 䠩��
typedef struct FILEMSG
      {
       db_uint16  Number;           // ����� ᮮ�饭��
       db_uint16  Page;             // ����� ��࠭��� �����
       char       Contents[256];    // ����ঠ���
      } FILEMSG;


typedef struct FILEMSG_ORA {    // ����饭�� �� 䠩�� ���짮��⥫�᪨� ᮮ�饭�� ��� �����३�⮢ (CONSTRAINT)  - dbank_ora_msg

 char       Error[10];           // ��� �訡��
 char       Object[31];          // ������������ ��ꥪ� Oracle (�����३��\������), max �����⨬�� � Oracle - 30 ᨬ�����.
 char       Message[256];        // ����饭�� �� �訡��
 db_uint16  Page;                // ����� ��࠭��� �����
 char       IsSystem;            // �ਧ��� ��⥬���� ᮮ�饭�� (����ࠥ��� ���३��஬)   

} FILEMSG_ORA;

//㭨����� ������, ⠡���� dbank_ora_msg
typedef struct FILEMSG_ORA_KEY { 
 char       Error[10];
 char       Object[31];
}FILEMSG_ORA_KEY;

#include <packpop.h>

// -----------------------------------------------------------------------------
// ������� ᮮ�饭�� �� 䠩��
_APPTLFUNC char *getfmsg(
                         const char   *NameF,           // ��� 䠩�� ᮮ�饭��
                         unsigned int  num              // ����� ᮮ�饭��
                        );

// ������� ᮮ�饭�� �� 䠩��
_APPTLFUNC char *getfmsgEx(
                           const char   *NameF,           // ��� 䠩�� ᮮ�饭��
                           unsigned int  num,             // ����� ᮮ�饭��
                           int          *Page             // ����� ��࠭��� �����
                          );

//AV ������� ᮮ�饭�� �� 䠩�� ���짮��⥫�᪨� ᮮ�饭�� ��� �����३�⮢\�����ᮢ - dbank_ora_msg
_APPTLFUNC extern char *getfmsg_ora
( 
   const char  *rsd_mes,         // ��ப� � �訡���� Oracle, ����� �� ��।����� (NULL),  �㤥� �ᯮ�짮������ ��������� szLastRsdError
   int         error,            // ��⥬�� ����� �訡��: BEdupkey, BEconstraint_parent_notfound, BEconstraint_child_found
   char        *ret_err_code,    // �����頥�� ��� �訡�� (ORA-00001,ORA-00291,ORA-00292)
   char        *ret_object,      // �����頥��� ������������ ��ꥪ� (�����३��\������), ��� ���ண� ��諨 ���짮��⥫�᪮� ᮮ�饭��
   int         *ret_page         // �����頥��� ���祭�� ����� ��࠭��� �����, ��� ����祭���� ���짮��⥫�᪮�� ᮮ�饭��
);

//AV ��ࠡ�⪠ �訡�� ᣥ���஢����� �����३�⠬�\�����ᠬ�, �����頥� true, �᫨ ⥪��� �訡�� � bfstat ����⢨⥫쭮 ᣥ���஢��� �����३�⠬�
_APPTLFUNC extern bool processOracleObjectError
( 
   const char  *rsd_mes,                  // ��ப� � �訡���� Oracle, ����� �� ��।����� (NULL),  �㤥� �ᯮ�짮������ ��������� szLastRsdError
   char        *outmes,                   //ᮮ�饭�� �� �訡�� - ��������� ���짮��⥫�᪮� ᮮ�饭��, �������� ��� �����३��, � �᫨ �� ��諨, � ���� ⥪�� �ࠪ������ ᮮ�饭�� 
   char        *ret_code_error   = NULL,  //�������� ��� �訡�� 
   char        *ret_object       = NULL,  //��������� ������������ ��ꥪ� (�����३��\������)
   int         *page_help        = NULL   //����� ��࠭��� ����� ��� ���������� ���짮��⥫�᪮�� ᮮ�饭��
);


// ���� ᮮ�饭�� �����頥� ��� ESC.
_APPTLFUNC int fmeswin(
                       const char   *NameF,           // ��� 䠩�� ᮮ�饭��
                       unsigned int  num              // ����� ᮮ�饭��
                      );

// ���� ᮮ�饭�� � ���� �����頥� ��� ESC.
_APPTLFUNC int ffldwin(
                       const char   *NameF,           // ��� 䠩�� ᮮ�饭��
                       unsigned int  num,             // ����� ᮮ�饭��
                       PANEL *p                       // ������ ������
                      );

// ���� ᮮ�饭�� � ��ࠬ��ࠬ� �����頥� ��� ESC
_APPTLFUNC int fformwin(
                        const char   *NameF,           // ��� 䠩�� ᮮ�饭��
                        unsigned int  num,             // ����� ᮮ�饭��
                        int           rf,              // ��᭮�
                        ...                            // ���᮪ ��६�����
                       );

/* ����� �� ��ப� ��譨� �஡���      */
_APPTLFUNC int ClearBlank(
                          char *line                     /* ��ப�                               */
                         );

// �����頥� ��� ESC
_APPTLFUNC int YesNoMsg(
                        const char   *NameF,    // ��� 䠩�� ᮮ�饭��
                        unsigned int  num,      // ����� ᮮ�饭��
                        int           ncf,      // ������ ������
                        ...                     // ���᮪ ��६�����
                       );

int iListFileMsg(const char* nameF, coord x, coord y, coord l, void *buff);

#ifdef __cplusplus
 } ;
#endif

#endif  // __FILEMSG_H

/* <-- EOF --> */