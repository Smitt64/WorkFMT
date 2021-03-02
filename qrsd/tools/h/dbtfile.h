/*******************************************************************************
 FILE         :   DBTFILE.H

 COPYRIGHT    :   R-Style Software Lab, 1999

 DESCRIPTION  :   Инструментарий для работы с dbt-файлами

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

/* Копировать dbt-файл в dbt-файл */
_APPTLFUNC int copydbt(
                       const char *source,       /* Имя исходного файла */
                       const char *target,       /* Имя приемного файла */
                       const char *spass,        /* Пароль исходного файла */
                       const char *tpass,        /* Пароль приемного файла */
                       int         sure          /* Признак обязательного копирования */
                      );

#ifdef __cplusplus
}
#endif

#endif /* __DBTFILE_H */

/* <-- EOF --> */