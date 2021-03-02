/*******************************************************************************
 FILE         :   REGIST.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   ��������� � �������쭮� �ᯮ�짮����� ��⥬�

 PROGRAMMED BY:   ���॥� �.�.

 CREATION DATE:   2 Oct 1995

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/regist.h $
                  $Revision: 4 $
                  $Date: 8.02.00 16:38 $
                  $Author: Lebedev $
                  $Modtime: 8.02.00 16:37 $
*******************************************************************************/

#ifndef __REGIST_H
#define __REGIST_H

#include "rstypes.h"
#include "apptldef.h"
#include "bedit.h"

#include <packpsh1.h>

// ��ୠ� ॣ����権
typedef struct REGISTRY
      {
       db_int16   ConnectionNumber;       // �������樮��� �����
       db_bdate   RegDate;                // ��� ॣ����樨
       db_btime   RegTime;                // �६� ॣ����樨
       char       ShortNameUser[16];      // ���⪮� ��� ���짮��⥫� (����� ����樮����)
       char       FullNameUser[101];      // ������ ��� ���짮��⥫�
       char       PhysicalNodeAddress[6]; // ���� �⠭樨 (��⮪�� NetBios)

       db_int32   Serial;
       db_int32   AudSid;
       db_int32   Mode;

       db_int32   TaskNumber;             // �����䨪��� ����� (#2990)
       db_int32   ThreadNumber;           // �����䨪��� ��⮪�
       db_int32   RefCount;               // ����稪 ��뫮� (#3091)

       char       Reserv[843];            // �����
      } REGISTRY;

typedef union
      {
       struct
            {
             db_baseint16  ConnectionNumber;
            } key0;

       struct
            {
             db_baseint32  AudSid;
            } key1;
      } REGISTRY_KEY;

#include <packpop.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef int (* FillRegInfoType)(REGISTRY *reg);

// Network group initialisation.
_APPTLFUNC void initNetwGrEx(
                             char            *grn,              // Group name. Truncated to 8 characters!
                             FillRegInfoType  userCallBack     
                            );

_APPTLFUNC unsigned int AlternateRegistrationEx(FillRegInfoType userCallBack);

_APPTLDATA extern BTRVFILE *FileReg;

_APPTLFUNC int          iOpenReg(int iOpenMode);
_APPTLFUNC unsigned int AlternateRegistration(void);
_APPTLFUNC void         CloseConnection(int Number);
_APPTLFUNC unsigned int ExclusiveRegistration(int Number);
_APPTLFUNC void         EndOfExclusive(void);

_APPTLFUNC void         ClearRegistry(int CNumAlt);
_APPTLFUNC void         ShowRegistry(int CNumAlt);

#ifdef __cplusplus
 }
#endif

#endif // __REGIST_H

/* <-- EOF --> */