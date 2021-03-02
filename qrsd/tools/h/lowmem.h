/*-----------------------------------------------------------------------*-

 File Name   : lowmem.h

 Sybsystem   : TOOL
 Description : ����� � ������� � ��ࢮ� �������� ��� DPMI

 Source      : lowmem.c, copylow.c

 Library(s)  :

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/lowmem.h $
                  $Revision: 7 $
                  $Date: 28.08.00 18:26 $
                  $Author: Kireev $
                  $Modtime: 28.08.00 18:25 $

 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 March 30,1995  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __LOWMEM_H
#define __LOWMEM_H

#include "tooldef.h"

#include "packpsh1.h"

// -----------------------------------------------------------------------------
typedef union
      {
       struct
            {
             unsigned short  Selector;
             unsigned short  Segment;
            } u;

       unsigned long  SelSeg;
      } LowMemPtr;

// -----------------------------------------------------------------------------
typedef struct
      {
       unsigned long  EDI, ESI, EBP, RES, EBX, EDX, ECX, EAX;
       unsigned short  FLAGS, ES, DS, FS, GS, IP, CS, SP, SS;
      } DPMIREGS;

#define  INT31  { __emit__(0xCD); __emit__(0x31); }


#ifdef __cplusplus
 extern "C" {
#endif


/*  �����⥫� src � dst ������ ������ ���饭���� ०���
    sel � offs ������ ᥫ���� (�� ᥣ����!) � ᬥ饭�� ����� � 1 MB
*/
_TOOLEXP void copyToLow(unsigned short sel, unsigned short offs, void *src, int size);
_TOOLEXP void copyFromLow(void *dst, unsigned short sel, unsigned short offs, int size);

// � ��砥 �訡�� �����頥� 0 !
_TOOLEXP int  GetLowMem (LowMemPtr *MemBuf, unsigned size);
_TOOLEXP void FreeLowMem(LowMemPtr *MemBuf);

#ifdef __cplusplus
 }
#endif

#include "packpop.h"

#endif  // __LOWMEM_H

/* <-- EOF --> */