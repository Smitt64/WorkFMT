/*******************************************************************************
 FILE         :   RSFFIND.H

 COPYRIGHT    :   R-Style Software Lab, 2000

 DESCRIPTION  :   ������⢮ �㭪権 ��� ���᪠ 䠩��� �� ��᪥

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   16 Mar 2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsffind.h $
                  $Revision: 2 $
                  $Date: 25.04.00 11:08 $
                  $Author: Lebedev $
                  $Modtime: 22.04.00 19:06 $
*******************************************************************************/

#ifndef __RSFFIND_H
#define __RSFFIND_H

#include <stdlib.h>
#include <sys/stat.h>

#include "platform.h"
#include "rscdefs.h"
#include "tooldef.h"

#include "rstypes.h"


// -----------------------------------------------------------------------------
#ifndef S_IFDIR
    #define  S_IFDIR         0x4000  /* directory */
#endif

#ifndef S_ISDIR
    #define  S_ISDIR(mode)   ((mode) & S_IFDIR)
#endif


// -----------------------------------------------------------------------------
#ifdef __cplusplus
    struct RslFindBlock;
#endif

typedef struct RslFindData
      {
       #ifdef __cplusplus
           struct RslFindBlock  *fndblk;
       #else
           void                 *fndblk;
       #endif

       char                      fname[_MAX_FNAME + _MAX_EXT];
       char                      path[_MAX_PATH];

       unsigned short            mode;
       unsigned long             size;
       int64_t                   sizeFull;      // ����� ࠧ��� 䠩�� (64-��⭮� 楫�� ��������)

       time_t                    atime;
       time_t                    ctime;
       time_t                    mtime;
      } RslFindData;

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
//
// ����� ���᪠
//
enum
   {
    FF_Direc   = 01,    // ��⠫���, ᮮ⢥�����騥 ��᪥
    FF_File    = 02,    // 䠩��, ᮮ⢥�����騥 ��᪥

    FF_AnyDir  = 010,   // �� ��⠫���, �������� ����
    FF_AnyFile = 020,   // �� 䠩��, �������� ����

    FF_Default = (FF_AnyDir | FF_File)
   };

//
// �㭪��-䨫���
// �����頥� 0, 䠩� ���室��
//           -1, 䠩� �� ���室��
//           >0, ��१�ࢨ஢��� ��� �訡��
//
typedef int (* RslFindFiltr)(const RslFindData *);



/*******************************************************************************
 ��᫥����⥫�� ���� 䠩���, 㤮���⢮����� ��᪥
 
 RslFindFirst - ���� ���� 䠩�, 㤮���⢮���騩 �᫮��� ���᪠.
 ~~~~~~~~~~~~
   ��ࠬ����:
        fmask - ��᪠ 䠩���, ������ ����;
        flags - 䫠�� ���᪠, �� ����� ���� 0;
        filtr - �㭪��-䨫���, ����� ���� NULL;

   �����頥� 㪠��⥫� �� �������� RslFindData, �᫨ ���� 䠩� ������,
   ��� NULL, �᫨ 䠩��� � ������묨 �᫮��ﬨ ��� ��� �ந��諠 �訡��.
   ������쭠� ��६����� errno ᮤ�ন� ��� �訡��.

 RslFindNext  - ���� ᫥���騩 䠩�, 㤮���⢮���騩 �᫮��� ���᪠.
 ~~~~~~~~~~~
   ��ࠬ����:
        data  - 㪠��⥫� �� �������� RslFindData;

   �����頥� 0, �᫨ ᫥���騩 䠩� ������, ���� �����頥� ���㫥��� ���祭��
   � ��⠭�������� ��� �訡�� � ������쭮� ��६����� errno.

 RslFindClose - �᢮������ ������ � �ᯮ��㥬� ������.
 ~~~~~~~~~~~
   ��ࠬ����:
        data  - 㪠��⥫� �� �������� RslFindData;

   �����頥���� ���祭�� ���.

*******************************************************************************/

_TOOLEXP RslFindData *RslFindFirst(const char *fmask, long flags, RslFindFiltr filtr);
_TOOLEXP int          RslFindNext (RslFindData *data);
_TOOLEXP void         RslFindClose(RslFindData *data);



/*******************************************************************************
 ������⢮ �㭪権 ��� ���᪠ Btrieve-䠩���
*******************************************************************************/

#define  BtFindFirst(fmask, flags, filtr)  RslFindFirst(fmask, flags, filtr)
#define  BtFindNext(data)                  RslFindNext(data)
#define  BtFindClose(data)                 RslFindClose(data)

// Old names
#define  BtrvFindFirst  BtFindFirst
#define  BtrvFindNext   BtFindNext
#define  BtrvFindClose  BtFindClose

/*******************************************************************************

 RslMatchFile - �஢���� ᮮ⢥��⢨� ����� 䠩�� ��᪥
 ~~~~~~~~~~~~
   ��ࠬ����:
      pattern - ��᪠, ����� ᮤ�ঠ�� * � ?;
        fname - ��� 䠩��;

   �����頥� 0, �᫨ ��� 䠩�� ᮮ⢥����� ��᪥.

*******************************************************************************/

_TOOLEXP int RslMatchFile(const char *pattern, const char *fname);

#ifdef __cplusplus
};
#endif

#endif /* __RSFFIND_H */

/* <-- EOF --> */