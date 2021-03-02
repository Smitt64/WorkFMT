/*******************************************************************************
 FILE         :   OPENFILE.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :

 PROGRAMMED BY:

 CREATION DATE:   20 Oct 1998

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/openfile.h $
                  $Revision: 14 $
                  $Date: 27.01.00 18:35 $
                  $Author: Lebedev $
                  $Modtime: 24.01.00 16:10 $
*******************************************************************************/

#ifndef __OPENFILE_H
#define __OPENFILE_H

#include <stdlib.h>

#include "platform.h"

#include "rscdefs.h"

#include "pexpdef.h"

#ifdef RSL_MD_FLAT
    #define  RSL_FILELEN  _MAX_PATH // Было 80 #4148
#else
    #define  RSL_FILELEN  13
#endif

#ifndef RSL_FTIME_DEFINED
    #define RSL_FTIME_DEFINED

    struct ftime
         {
          unsigned  ft_tsec  : 5;   /* Two second interval */
          unsigned  ft_min   : 6;   /* Minutes */
          unsigned  ft_hour  : 5;   /* Hours */
          unsigned  ft_day   : 5;   /* Days */
          unsigned  ft_month : 4;   /* Months */
          unsigned  ft_year  : 7;   /* Year */
         };
#endif


typedef struct
      {
       char  drv[_MAX_DRIVE];
       char  dir[_MAX_DIR];
       char  file[_MAX_FNAME];
       char  ext[_MAX_EXT];
       char  path[_MAX_PATH];
      } CURDR;

// дерево каталогов
typedef struct
      {
       int   level;       // номер уровня
       int   t_level;     // ссылка наверх
       int   n_level;     // номер в уровне
       char  d_name[13];
       char  st[2];
      } ListDir;

#ifdef RSL_PL_MS

    //список файлов и каталогов в текущей директории
    typedef struct
          {
           unsigned char attr;                  // атрибут
           char          f_name[RSL_FILELEN];   // имя файла/директории
           struct        ftime tm;
           long          f_size;
           char          mark;
          } ListFile;

#endif

typedef struct
      {
       unsigned char  drnum;

       struct
            {
             unsigned  logic : 1;   // 1 = logic
             unsigned  remot : 1;   // 1 = remote, 0 - local */
            } flag;
      } Driver;

typedef struct
      {
       unsigned int  kol;
       long          checksum;
      } DirRoot;

#define  DN        20  // шаг прирашения при формировании списка
#define  MAXPATHS  20  // максимальная длина строки в скролинге дерева каталогов
#define  NMHIST    10  // mask - history


// September 19,1997

#define  OPFL_NORESTRICT  0
#define  OPFL_CURRENT     1
#define  OPFL_ROOT        2


#ifdef __cplusplus
 extern "C" {
#endif

_RSPNLEXP unsigned char SetOpenFileMode(unsigned char newMode);
_RSPNLEXP void SetOpenFileRoot(const char *path);


RSL_MAKE_HANDLE(RMTFIND);

RMTFIND  RmtFindFirst(const char *mask, bool dirs, bool files, ListFile *elem);
bool     RmtFindNext(RMTFIND hd, ListFile *elem);
void     RmtFindClose(RMTFIND hd);

#ifdef __cplusplus
 } ;
#endif

#endif // __OPENFILE_H

/* <-- EOF --> */