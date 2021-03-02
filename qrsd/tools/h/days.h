//***********************************************************************
//                    �����㬥��਩ ��� ࠡ��� � ��⠬�
//                            VECsoft 13.11.1992
//
// VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/days.h $
//                  $Revision: 4 $
//                  $Date: 20.10.98 20:29 $
//                  $Author: Stepankov $
//                  $Modtime: 20.10.98 18:48 $
//***********************************************************************

#ifndef __DAYS_H
#define __DAYS_H

#include "rsdates.h"
#include "menu.h"
#include "apptldef.h"

#ifdef __cplusplus
 extern "C" {
#endif

// ����� ����� (����⠫) �� ᯨ᪠
_APPTLFUNC int list_date(
                         char  kind,                     // ���  ᯨ᪠  ( � / � )
                         int   item,                     // ⥪�騩 �롮� �� ᯨ᪠
                         char  *maneitem,                // �������� �롮�
                         coord  x,                       // ���न���� ᯨ᪠
                         coord  y
                        );

// ����� �������� ����� (����⠫�)
_APPTLFUNC char *name_date(
                           char kind,                    // ���  ᯨ᪠  ( � / � )
                           int  item                     // ⥪�騩 �롮� �� ᯨ᪠
                          );

// ��ॢ��� ���� � ��ப�
_APPTLFUNC void date_to_str(
                            int    kind,                 // ��� ����  0 - ��.�����.����
                                                         //           1 - ����.�����
                            bdate *dt,                   // ���
                            char  *str                   // ��ப� �������� (����.����� 40 ᨬ�����)
                           );

#ifdef __cplusplus
 } ;
#endif

#endif // __DAYS_H

/* <-- EOF --> */