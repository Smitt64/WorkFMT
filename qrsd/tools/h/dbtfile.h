/*******************************************************************************
 FILE         :   DBTFILE.H

 COPYRIGHT    :   R-Style Software Lab, 1999

 DESCRIPTION  :   �����㬥��਩ ��� ࠡ��� � dbt-䠩����

 PROGRAMMED BY:   VECsoft 14.10.1992

 CREATION DATE:   19 Mar 1999

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/dbtfile.h $
                  $Revision: 3 $
                  $Date: 19.07.99 21:09 $
                  $Author: Markovitch $
                  $Modtime: 5.07.99 16:37 $
*******************************************************************************/

#ifndef __DBTFILE_H
#define __DBTFILE_H

#include <apptldef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ����஢��� dbt-䠩� � dbt-䠩� */
_APPTLFUNC int copydbt(
                       const char *source,       /* ��� ��室���� 䠩�� */
                       const char *target,       /* ��� �ਥ����� 䠩�� */
                       const char *spass,        /* ��஫� ��室���� 䠩�� */
                       const char *tpass,        /* ��஫� �ਥ����� 䠩�� */
                       int         sure          /* �ਧ��� ��易⥫쭮�� ����஢���� */
                      );

#ifdef __cplusplus
}
#endif

#endif /* __DBTFILE_H */

/* <-- EOF --> */