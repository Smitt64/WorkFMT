/*@H*********************************************************************
*        File Name:   listobj.h            December 13,1993
*       Programmer:   S. Kubrin
*      Description:
*  Public routines:
* Private routines:
*
* VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/listobj.h $
*                  $Revision: 19 $
*                  $Date: 8.02.00 19:13 $
*                  $Author: Kubrin $
*                  $Modtime: 8.02.00 19:13 $
**********************************************************************@H*/
/*
   April 25,1996   Serg Kubrin (K78)
      Add  SortCntList, LobjInsertSort, LobjInsertAfter
   June 23,1997   Serg Kubrin (K78)
      Smart list support and MultiThread safe
*/

#if !defined(__LISTOBJ_H) /*&& !defined(__DLMINTF_H)*/
#define __LISTOBJ_H

#include <stddef.h>

#include "tooldef.h"

#include <packpshn.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
   LOBJ - ������ ����� ��� ᯨ᪮��� �������. �� ������ ���� ����祭 � ᮧ������� ���� 
   ������ ������, ����� ������ ��ꥤ������� � ᯨ᪨. �᫨ �ꥪ� ������ �室��� � ��� 
   ᯨ᪠ �����६����, � �� ������ ����� ��� �����ꥪ� LOBJ.

   CNTLIST - �� ��७� ��� ᯨ᪠ ��ꥪ⮢ ᮤ�ঠ�� LOBJ. ��� ������� ᯨ᪠ ������ 
   ����⢮���� ᢮� ��ꥪ� ⨯� CNTLIST. �㭪樨 ���樠����樨 CNTLIST ��᫥����
   ��ࠬ��஬ ��।����� ᬥ饭�� ᮮ⢥�����饣� �����ꥪ� LOBJ � ��।������� �����.

   delta == offsetof (MYLOBJ,lobj)

   �㭪�� �����樠����樨 㤠��� �� ������ �� ᯨ᪠. �� �⮬ �᫨ �� ��⠭����� 
   LST_NOTOWN 䫠� ��������� ��ꥪ⠬�, � ��� ������� ����� ��뢠���� �㭪�� ���⪨,
   㪠������ ���짮��⥫�� � ����⢥ ��ࠬ��� (�᫨ ��� �� NULL).
   ���������: �᫨ ����� �����६���� �室�� � ��᪮�쪮 ᯨ᪮�, ⮫쪮 ���� ᯨ᮪ �����
              �������� ��ꥪ⠬�

   ��� �ᯮ�짮����� 㬭�� ᯨ᪮� ����室��� ����� LOBJ �ᯮ�짮���� SOBJ � �� 
   ���樠����樨 ᯨ᪠ 㪠���� 䫠� LST_SMART.
*/

typedef struct tagLOBJ  LOBJ;

struct tagLOBJ
     {
      LOBJ *next;
     };


// Smart list link object
typedef struct tagSOBJ  SOBJ;
struct tagSOBJ
     {
      SOBJ               *next;
      struct tagCNTLIST  *owner;
     };


typedef struct tagCNTLIST
      {
       LOBJ           *last;
       int             delta;  // ���饭�� ��ꥪ� LOBJ � ����� ᯨ᪠
       unsigned char   flags;

       #ifdef RSL_MD_FLAT
           void       *sect;
       #endif
      } CNTLIST;
                                  
#ifndef BUILD_DLM
    #define  SobjFirstItem(sobj) _LobjGetObj(sobj->owner, _LobjFirst(sobj->owner))
    #define  SobjLastItem(sobj)  _LobjGetObj(sobj->owner, sobj->owner->last)

    #define  SobjNextItem(sobj)  _LobjGetObj(sobj->owner, _LobjNext(sobj->owner, (LOBJ *)sobj))
    #define  SobjPrevItem(sobj)  _LobjGetObj(sobj->owner, _LobjPrev(sobj->owner, (LOBJ *)sobj))
#endif

//------ ���祭�� flags
#define  LST_DEFAULT 0   // ���������� � ����� �������� ����⠬�
#define  LST_NOTOWN  1   // ���᮪ �� ������� ᢮��� ����⠬�
#define  LST_ADDBEG  2   // ���� ������ ����������� � ������ ᯨ᪠

#define  LST_SMART   4   // ���᮪ �࠭�� ��ꥪ�� SOBJ

#define  ADD_FLAGS(lst ,flg)  (lst)->flags |= (flg) 

typedef int  (DLMAPIC *LCMPFUNC)(void *, void *);
typedef int  (DLMAPIC *LITRFUNC)(void *, void *);
typedef void (DLMAPIC *LCLRFUNC)(void *);

typedef int  (DLMAPIC *LSRTFUNC)(void *left, void *right, void *data);

#ifdef BUILD_DLM       
    void DLMAPI LobjInitList(CNTLIST *lst, int delta);
    void DLMAPI LobjDoneList(CNTLIST *lst, LCLRFUNC func);
#endif
               
#ifdef RSL_MD_FLAT
    _TOOLEXP void InitSafeList (CNTLIST *lst, unsigned char flag, int delta, void *sect);
    _TOOLEXP void DoneSafeList (CNTLIST *lst, LCLRFUNC func);
    _TOOLEXP void InitCntListEx(CNTLIST *lst, unsigned char flag, int delta, void *sect);
#endif

_TOOLEXP void InitCntList(CNTLIST *lst, unsigned char flag, int delta);
_TOOLEXP void DoneCntList(CNTLIST *lst, LCLRFUNC func);

TOOLDLMEXP void  TOOLDLMC LobjRemove   (CNTLIST *lst, void *obj, LCLRFUNC func);
TOOLDLMEXP int   TOOLDLMC LobjForEach  (CNTLIST *lst, LITRFUNC func, void *args);
TOOLDLMEXP int   TOOLDLMC LobjForEachL (CNTLIST *lst, LITRFUNC func, void *args);
TOOLDLMEXP void *TOOLDLMC LobjFirstThat(CNTLIST *lst, LCMPFUNC func, void *args);
TOOLDLMEXP void *TOOLDLMC LobjLastThat (CNTLIST *lst, LCMPFUNC func, void *args);
TOOLDLMEXP void *TOOLDLMC LobjAt       (CNTLIST *lst, int index);
TOOLDLMEXP int   TOOLDLMC LobjNitems   (CNTLIST *lst);
TOOLDLMEXP void  TOOLDLMC LobjInsert   (CNTLIST *lst, void *obj);
TOOLDLMEXP void *TOOLDLMC LobjPrevItem (CNTLIST *lst, void *obj);
TOOLDLMEXP void *TOOLDLMC LobjNextItem (CNTLIST *lst, void *obj);
TOOLDLMEXP void *TOOLDLMC LobjFirstItem(CNTLIST *lst);
TOOLDLMEXP void *TOOLDLMC LobjLastItem (CNTLIST *lst);

// June 23, 1997
TOOLDLMEXP void  TOOLDLMC SobjInit  (SOBJ *obj);
TOOLDLMEXP void  TOOLDLMC SobjUnlink(SOBJ *obj);

_TOOLEXP void  LobjDetach      (CNTLIST *lst, void *obj);
_TOOLEXP void *LobjForEachEx   (CNTLIST *lst, LITRFUNC func, void *args, int *retval);
_TOOLEXP void *LobjForEachLEx  (CNTLIST *lst, LITRFUNC func, void *args, int *retval);
_TOOLEXP int   LobjIndexOf     (CNTLIST *lst, void *obj);
_TOOLEXP void  LobjInsertAt    (CNTLIST *lst, void *obj, int at);
_TOOLEXP void *LobjSetLast     (CNTLIST *lst, void *obj);
_TOOLEXP void  LobjInsertBefore(CNTLIST *lst, void *ptr, void *Target);

// April 25,1996
_TOOLEXP void  LobjInsertAfter(CNTLIST *lst, void *ptr, void *Targ);
_TOOLEXP void  LobjInsertSort (CNTLIST *lst, void *obj, LSRTFUNC func, void *sortData);
_TOOLEXP void  SortCntList    (CNTLIST *lst, LSRTFUNC func, void *sortData);


// October 14, 1997
_TOOLEXP void *LobjExtractFirst(CNTLIST *lst);
_TOOLEXP void *LobjExtractAfter(CNTLIST *lst, void *obj);


_TOOLEXP LOBJ *_LobjFirst (CNTLIST *lst);
_TOOLEXP LOBJ *_LobjNext  (CNTLIST *lst, LOBJ *ob);
_TOOLEXP LOBJ *_LobjPrev  (CNTLIST *lst, LOBJ *ob);
_TOOLEXP void *_LobjGetObj(CNTLIST *lst, LOBJ *ob);

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif  //listobj_h

/* <-- EOF --> */