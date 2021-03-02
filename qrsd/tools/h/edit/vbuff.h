/*@H*********************************************************************
*        File Name:   vbuff.h            January 27,1994
*       Programmer:   S. Kubrin
*      Description:
*  Public routines:
* Private routines:

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/EDIT/vbuff.h $
                  $Revision: 12 $
                  $Date: 14.10.98 15:55 $
                  $Author: Markovitch $
                  $Modtime: 14.10.98 0:21 $
**********************************************************************@H*/
#ifndef __EDIT_VBUFF_H
#define __EDIT_VBUFF_H
#define __VBUFF_H

#include <stdlib.h>
#include "pexpdef.h"

#ifndef __LISTOBJ_H
#include "listobj.h"
#endif

#ifndef __RSCDEFS_H
#include <rscdefs.h>
#endif // RSCDEFS.H

#ifndef __RSTYPES_H
#include <rstypes.h>
#endif // RSTYPES.H

// -----------------------------------------------------------------------------
#ifdef RSL_MD_FLAT
    #define  EDLOCAL
#else
    #define  EDLOCAL  _near _pascal
#endif

#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <packpsh1.h>

// -----------------------------------------------------------------------------
typedef struct
      {
       db_int16  size;  // ��।���� ⨯ ����
       // ����� ���� ������஢���� �����
      } ZONE;
/* size 0,1,2 .... ��।���� �������㥬�� ���� � ������ ࠧ��� �����
   ������ ZNEW,
   -1 - ���������㥬�� ����  ZORIG
   -2 - ����� 䠩�� ZEOF
*/

// ��ଠ� ��������஢����� ����
typedef struct
      {
       db_int16   size;  // ��।���� ⨯ ���� -1
       db_uint32  offs;  // ᬥ饭�� ���� � 䠩��
       db_uint32  flen;  //
      } FZONE;

#include <packpop.h>

// -----------------------------------------------------------------------------
#define  ZEOF(zon)   ((zon)->size == -2)
#define  ZORIG(zon)  ((zon)->size == -1)
#define  ZNEW(zon)   ((zon)->size >= 0)

// -----------------------------------------------------------------------------
#define  ZONESIZE(zon)  ((zon)->size  < 0 ? ((FZONE*)(zon))->flen : \
                        (unsigned long)((zon)->size))

// ������ ���� ZEOF
#define  ZEOFSIZE   sizeof(ZONE)

#define  ZMIN(a, b)  ((a) > (b) ? (b) : (a))
#define  ZMAX(a, b)  ((a) < (b) ? (b) : (a))

#include <packpshn.h>

// -----------------------------------------------------------------------------
// ����� ��� ZONE
typedef struct
      {
       FZONE  *pZ;
      } ZITER;

// -----------------------------------------------------------------------------
typedef struct
      {
       FZONE          *pZone;  // �����⥫� �� ����
       unsigned long   offs;   // ���饭�� � ����
      } FPOS;

// -----------------------------------------------------------------------------
#define  ZoneGetPtr(pos)  ((char *)(pos)->pZone + sizeof(ZONE) + (int)(pos)->offs)

// -----------------------------------------------------------------------------
typedef enum
      {
       VB_RESIZE,VB_BEGSAVE,VB_SAVE,VB_ENDSAVE,VB_CHANGEINFO,

       VB_COMMAND_SAVE, VB_COMMAND_TESTEXIT,VB_COMMAND_RELOAD,VB_COMMAND_DEACT_VIEW
      } VBMES;

// -----------------------------------------------------------------------------
typedef struct
      {
       int  global;
       int  erCount;
      } VBCMDDATA;

// -----------------------------------------------------------------------------
typedef struct
      {
       long  size;      // ��騩 ࠧ��� �����뢠���� ���ଠ樨
       long  saved;     // ��� ����ᠭ�� ࠧ��� ���ଠ樨 � �����
      } SAVEMES;

// -----------------------------------------------------------------------------
typedef struct tagVBUFF
      {
       LOBJ     vbuf_elem;              // ���᮪ ����㠫��� ���஢

       //  �㭪�� ��뢠���� ��� ���㦨����� ᮮ�饭��
       int      (*mesfun)(struct tagVBUFF *pB, VBMES mes, void *ptr);

       char     filename[_MAX_PATH];    // ��� ࠡ�祣� 䠩��
       int      hd;                     // ���ਯ�� 䠩��
       int      number;                 // ����� ������� ����
       int      flags;                  // ZB_ ����� ���ﭨ� � ��ਡ���
       FPOS     CurPos;                 // ������ ������ � VBUFF
       char    *CH_beg;                 // �����⥫� ��砫� ����
       char    *CH_end;                 // �����⥫� �� ����� ᢮������ ������
       int      bsize;                  // ����騩 ࠧ��� ����
       long     flen;                   // ������ ������������� 䠩��

       int      hdSwap;

       int      origLen;                // Original file size,
       time_t   st_mtime;               // modification and
       time_t   st_ctime;               // creationt time

       unsigned char  notActual;

       int      fileEncode;
       FILE    *uniFp;
       char     uniTmpName[_MAX_PATH];
       int      err_uni;                // �訡�� �� errno �� ����⨨ uniFp
       char     target[_MAX_PATH];      // ��� ������ 䠩�� �� �믮������ "���࠭��� ���..."
      } VBUFF;

#include <packpop.h>

// -----------------------------------------------------------------------------
_RSPNLEXP int DefMesFun(VBUFF *pB, VBMES mes, void *ptr);

// -----------------------------------------------------------------------------
// ������ ᢮������� ����࠭�⢠ � VBUFF
#define  AVAIL(pB)  (int)((pB)->CH_beg + (pB)->bsize - (pB)->CH_end)

// �����⥫� �� ����� ����
#define  ZoneFirst(pB)  ((FZONE *)(pB)->CH_beg)

// �����⥫� �� ��᫥���� ���� ZEOF
#define  ZoneLast(pB)   ((FZONE *)((pB)->CH_end - ZEOFSIZE))

#define  ZBMINSIZE      (sizeof(FZONE) + sizeof(ZONE))  // ��������� ࠧ��� VBUFF

// -----------------------------------------------------------------------------
// ���祭�� ��⮢ flags
#define  ZB_EDIT          0
#define  ZB_RDONLY        1   // ���� ����� ������஢���
#define  ZB_TEMP          2   // ���� �� �६����� 䠩��
#define  ZB_CHANGED       4   // ���� ������஢��
#define  ZB_SUSPEND       8   // ���� ����⨢�� VbSuspend
#define  ZB_SWAPED       16   // ����騩 䠩� ।���஢���� - 䠩� ᢮�����
#define  ZB_OPENED       32   // ���� �����


#define  ZB_DRAWLN       64   // ����ᮢ�� �ॡ�� ⥪��� ��ப�
#define  ZB_DRAWALL     128   // ����ᮢ�� �ॡ�� �� ����
#define  ZB_NONAME      256   // The user must enters the new file name
#define  ZB_UNICODE     512   // Source file is Unicode
#define  ZB_NEWBUFF    1024   // ����� ���� �� ᮧ���
#define  ZB_NOFILE     2048   // No assoc. file
#define  ZB_UNIXMODE   4096
#define  ZB_HIDETAG    8192   // Hide tags <!-- -->
#define  ZB_BINARY    16384   // FBUFF in binary mode.

// -----------------------------------------------------------------------------
_RSPNLEXP int  VbOpen       (VBUFF *pZB, char *fname, int flags, char *buf, int bsize);
          int  VbOpenEncode (VBUFF *pZB, char *fname, int flags, char *buf, int bsize, unsigned char encode);
_RSPNLEXP void VbClose      (VBUFF *pZB);
_RSPNLEXP int  VbDataInsert (VBUFF *pB, char *buf, int size);
_RSPNLEXP void VbSetPos     (VBUFF *pB, long offset);
_RSPNLEXP void VbGetPos     (VBUFF *pB, long *offset);
_RSPNLEXP int  VbRead       (VBUFF *pB, char *buf, int size);
_RSPNLEXP int  VbSave       (VBUFF *pB, int sv_swap);
_RSPNLEXP int  VbSuspend    (VBUFF *pB);
_RSPNLEXP int  VbResume     (VBUFF *pB, char *buf, int bsize);

          int  VbReset      (VBUFF *pB);

_RSPNLEXP int  VbDelete     (VBUFF *pB, unsigned long size, unsigned long *realSizePtr);
_RSPNLEXP void VbShowZone   (VBUFF *pB);

_RSPNLEXP void   ZoneInitIter(ZITER *zI, VBUFF *pB);
_RSPNLEXP FZONE *ZoneIter    (ZITER *zI);

_RSPNLEXP int  VbNumber     (void);
_RSPNLEXP int  VbModified   (VBUFF *pB);
_RSPNLEXP void VbGotoEof    (VBUFF *pB);
_RSPNLEXP long VbSize       (VBUFF *pB);

_RSPNLEXP VBUFF *VbFind       (char *filename);
_RSPNLEXP int  VbLoopBuff   (LITRFUNC func, void *arg);

_RSPNLEXP int  VbSetNewFileName (VBUFF *pB, char *newname, int encode);

// -----------------------------------------------------------------------------
int CheckActialFile (VBUFF *pB);
int VbReOpenForWrite(VBUFF *pZB);
int VbMakeUnchanged (VBUFF *pB);

#ifdef __cplusplus
}
#endif

#endif   // __EDIT_VBUFF_H

/* <-- EOF --> */