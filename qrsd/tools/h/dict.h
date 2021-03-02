/*******************************************************************************
 FILE         :   DICT.H

 COPYRIGHT    :   R-Style Software Lab, 1992, 1998

 DESCRIPTION  :   ������� 䠩���

 PROGRAMMED BY:   ���ᮢ  �.�.

 CREATION DATE:   12.04.94

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/dict.h $
                  $Revision: 3 $
                  $Date: 30.10.98 19:03 $
                  $Author: Lebedev $
                  $Modtime: 22.10.98 18:03 $
*******************************************************************************/

#ifndef __DICT_H
#define __DICT_H

#include "apptldef.h"

#include <rsl/bshstruc.h>


//**************************** ��������� *********************************

#ifdef __cplusplus
 extern "C" {
#endif

extern _APPTLDATA BTRVFILE *FilePrj;       // ���� �஥��

// ���� � 䨫���� �� �஥��
_APPTLFUNC extern void SKF_Prj(
                               BTRVFILE      *bf,              // ���� �஥��
                               enum BRECKEY   numkey,          // ����� ����
                               unsigned char  Group,           // �ਧ��� ��㯯�
                               long           code             // ��� 䠩��/����
                              );

#ifdef __cplusplus
 } ;
#endif

//**************************** ����� 䠩�� *******************************

#endif // __DICT_H

/* <-- EOF --> */