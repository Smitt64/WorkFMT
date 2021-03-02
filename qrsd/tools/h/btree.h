//-*--------------------------------------------------------------------------*-
//
// File Name   : btree.c
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
//   Alexandrov S.A.  - Create file
//-*--------------------------------------------------------------------------*-

//
// ���������� �������� ��� Btrieve-������, ������ SCROL
//

#ifndef __BTREE_H
#define __BTREE_H

#include "btrv.h"
#include "menu.h"
#include "apptldef.h"

// ������������ ����� ������ � ������ � �������� (����������)
#define  CMAXBRANCHLENGTH    500

// ����. ������� �����������
#define  CMAXTREELEVEL     10000

// ����� ��� ������� Alt-<,>
#define  CTREESCROLSHIFT       3

// ���� ���������� �������������� ����������:
// 1 - �� ���������� ���, ��� ���� ������� �� ���������� ����� ������ ��������
// 2 - ���������� ���, ��� ���� ���������� ����� ������
// 3 - ���������� ���, ��� ���� � ���� ���������� ����� ������
#define  TREE_FILTER_BLOCK_CHILD  1
#define  TREE_FILTER_PASS_PARENT  2
#define  TREE_FILTER_PASS_CHILD   3

// ���. �������� � TREEITEM->BitMask1
#define  TREEITEM_FILTERED  0x01
#define  TREEITEM_SELECTED  0x02
#define  TREEITEM_DISABLED  0x04

// ����� ��� UserMakeBranch
// ��������� �������� � ������� ������
#define  MAKE_BRANCH_FROM_CURREC  0x01
// ������ �� �������� �����-�����
#define  MAKE_BRANCH_UPWARD       0x02

#include "packpshn.h"

// ������� ������
typedef struct TREEITEM
      {
       int32       ParentNumInList;                // ���������� ����� ��������
       recpos_t    ItemRecPos;                     // RecPos � �����
       int         TreeLevel;                      // ������� �����������
       char        PosInGroup;                     // ������� � ������ ������ ��������
       char        BranchOpened;                   // ������� �������������
       char        IsParent;                       // �������� �� ���������
       char        BitMask1;                       // ��� ����� ���. ��������..
      } TREEITEM;

// ����� ����������
typedef struct TREEINFO
      {
       int32       TreeNumRec;                     // ����� ����� ������� � ������
       int32       TreeNumRecVirt;                 // -"- � ������ �������������
       TREEITEM   *TreeItemBuf;                    // ��������� �� ������ TREEITEM
       BTRVFILE   *bfile;                          // ����, � ������� ��������
       SCROL      *scroll;                         // ���������
       int         FieldForTree;                   // ����� ����, � ������� ������ ������
       char       *BranchNamePtr;                  // ��������� �� ���� � ��������� �����
       int       (*SetChildRecFltrs)               // ��������� ������
                 (void* recbuf , int ZeroLevel);  //
       int       (*SetChildRecFltrsEx)             // ��������� ������ - ������������ ������
                                                   // SetChildRecFltrs, ���� ����� �������� ti
                 (void* recbuf , int ZeroLevel, void* ti); //
       int       (*UserFunDeleteTrn)(void* parm);  // ������� �������� � ����������
       int       (*UserFunInput)(void* parm);      // ������� ����� ������
       int       (*UserFunEdit)(void* parm);       // ��������������
       int       (*UserKeyProc)(PANEL* p, int key, void* ti );// ���������� ����������
       int       (*UserSynchFields)                // ��������� �����
                 ( SCROL *S, void* t );
       int       (*UserFunIns)(void* ti);          // �����. �-��� ���������� ������ � ����
       int         TreeCorrupt;                    // ���� �������
       int         ReadOnly;
       void       *UserParm;                       // ���������������� ���������
       int         ShowMenu;                       // ���� �� �������� ��������� ����
       int         DontSaveF;                      // ���� ���������� ������� ������ (������, ������������)
       int         Search_Running ;                // ��������� ��� ������ ������
       int       (*UserCheckDel)(void*);          // �����. �-��� �������� ����� ���������
       int         CollapseOnStart;                // ���������� ������ ��� ������
       JBtrFltr    UserFilter;                     // ������ �������
       int         HowToFilter;                    // ��� ����������� (TREE_FILTER_*)
       int         ExitIfEmptyOnStart;             // ����������� ����� �� ���������� ��� 0 �������
       int       (*UserFunDeleteTrn2)(void* parm); // ������� �������� � ���������� (����� ��������)
       int         NoAskBeforeInsertRecord;        // �������, ��� ����� ������ ������ �� ����� ����������
                                                   // ����� �������� ������ (�������, ����������).
                                                   // ���������� ��� ��� ��� ������ ��������
       int         NoAskBeforeDeleteRecord;        // �������, ��� ����� ��������� ������ �� �����
                                                   // �������� ��������� ������.
                                                   // ���������� ��� ��� ��� ������ ��������
                                                   // �����. �-��� ������ ������,
                                                   //  ������������ � SQL ������
       int       (*UserMakeBranch)(void* ti, int (proc)(void* parm, int level, recpos_t pos), void* parm, int flags);

       int         DisableSelection;               // ������������� ��������� �������
       int         NotEmpty;                       // ����� ���� ��� �������

    // Tools.4.050.13
       int         Editable;                       // ����� �������������� ������� � ������
       int       (*UserFunUpdate)(void* ti);       // �����. �-��� ���������� ������ � ����
      } TREEINFO;

// ��������� �����/��������.
typedef struct TREEEDITPARM
      {
       SCROL     *S;                            // ���������
       int        child;                        // ����������?
       int        FirstRec;                     // ����� ������ ������?
       TREEINFO  *info;
       int        bop;
      } TREEEDITPARM;

#include "packpop.h"

typedef int (*TREE_USERFUN)(TREEINFO*, void*, int);

_APPTLDATA extern MENU vTreeMenu[16];
_APPTLDATA extern MENU vTreeHorMenu[1];

#ifdef __cplusplus
extern "C" {
#endif

_APPTLFUNC int  PutTreeScrol(TREEINFO *ti);
_APPTLFUNC void RemTreeScrol(TREEINFO *ti);
_APPTLFUNC int  UseTree(TREEINFO *ti);
_APPTLFUNC int  TreeOverlook(TREEINFO *ti, TREE_USERFUN UserFun, void *UserParm);
_APPTLFUNC int  iTestTreeItemForChilds(TREEINFO *ti);
_APPTLFUNC int  TreeGoTo(recpos_t RecPos, int unfold, SCROL *mn);
_APPTLFUNC void SetUpdateTreeMode(TREEINFO *ti);
_APPTLFUNC int  CollapseTree(TREEINFO *ti);
_APPTLFUNC int  RefreshTree(TREEINFO *ti);

#ifdef __cplusplus
_APPTLFUNC void RefreshTreeEx(TREEINFO *ti, bool bNewRec);
#else 
_APPTLFUNC void RefreshTreeEx(TREEINFO *ti, bool bNewRec);
#endif 

_APPTLFUNC int  OpenTree(TREEINFO *ti);
_APPTLFUNC int  TreeOverlook2(TREEINFO *ti, TREE_USERFUN UserFun, void *UserParm, recpos_t recpos);
_APPTLFUNC int  TreeDeleteBranch(TREEINFO *ti);
_APPTLFUNC int  TreeInsertRec(TREEEDITPARM *tep);
_APPTLFUNC int  TreeUpdateRec(TREEINFO *ti);

_APPTLFUNC int  TreeHasSelection(TREEINFO *ti);
_APPTLFUNC int  TreeFirstSelected(TREEINFO *ti);
_APPTLFUNC int  TreeNextSelected(TREEINFO *ti, int item);

_APPTLFUNC void SetQuickSearchMode(TREEINFO *ti, int on);
_APPTLFUNC void ScrollTree(SCROL *pSCROL, int iLine);
_APPTLFUNC int  TreeItemInvertSelection(SCROL *S, TREEITEM *item);

#ifdef __cplusplus
};
#endif

#endif // __BTREE_H

/* <-- EOF --> */