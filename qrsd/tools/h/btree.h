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
// Построение иерархии для Btrieve-файлов, ресурс SCROL
//

#ifndef __BTREE_H
#define __BTREE_H

#include "btrv.h"
#include "menu.h"
#include "apptldef.h"

// Максимальная длиня строки с веткой и рисунком (некритично)
#define  CMAXBRANCHLENGTH    500

// Макс. уровень вложенности
#define  CMAXTREELEVEL     10000

// Сдвиг при нажатии Alt-<,>
#define  CTREESCROLSHIFT       3

// Типы фильтрации иерархического скроллинга:
// 1 - не пропускать все, что ниже уровнем не прошедшего через фильтр элемента
// 2 - пропускать все, что выше прошедшего через фильтр
// 3 - пропускать все, что выше и ниже прошедшего через фильтр
#define  TREE_FILTER_BLOCK_CHILD  1
#define  TREE_FILTER_PASS_PARENT  2
#define  TREE_FILTER_PASS_CHILD   3

// доп. признаки в TREEITEM->BitMask1
#define  TREEITEM_FILTERED  0x01
#define  TREEITEM_SELECTED  0x02
#define  TREEITEM_DISABLED  0x04

// Флаги для UserMakeBranch
// Построить иерархию с текущей записи
#define  MAKE_BRANCH_FROM_CURREC  0x01
// Проход по иерархии снизу-вверх
#define  MAKE_BRANCH_UPWARD       0x02

#include "packpshn.h"

// Элемент списка
typedef struct TREEITEM
      {
       int32       ParentNumInList;                // Порядковый номер родителя
       recpos_t    ItemRecPos;                     // RecPos в файле
       int         TreeLevel;                      // Уровень вложенности
       char        PosInGroup;                     // Позиция в группе одного родителя
       char        BranchOpened;                   // Признак распахнутости
       char        IsParent;                       // Является ли родителем
       char        BitMask1;                       // тут будут доп. признаки..
      } TREEITEM;

// Общая информация
typedef struct TREEINFO
      {
       int32       TreeNumRec;                     // Общее число записей в дереве
       int32       TreeNumRecVirt;                 // -"- с учетом распахнутости
       TREEITEM   *TreeItemBuf;                    // Указатель на массив TREEITEM
       BTRVFILE   *bfile;                          // Файл, с которым работаем
       SCROL      *scroll;                         // Скроллинг
       int         FieldForTree;                   // Номер поля, в котором рисуем дерево
       char       *BranchNamePtr;                  // Указатель на поле с названием ветви
       int       (*SetChildRecFltrs)               // Установка ключей
                 (void* recbuf , int ZeroLevel);  //
       int       (*SetChildRecFltrsEx)             // Установка ключей - используется вместо
                                                   // SetChildRecFltrs, если нужно передать ti
                 (void* recbuf , int ZeroLevel, void* ti); //
       int       (*UserFunDeleteTrn)(void* parm);  // Функция удаления в транзакции
       int       (*UserFunInput)(void* parm);      // Функция ввода записи
       int       (*UserFunEdit)(void* parm);       // Редактирование
       int       (*UserKeyProc)(PANEL* p, int key, void* ti );// Обработчик клавиатуры
       int       (*UserSynchFields)                // Установка полей
                 ( SCROL *S, void* t );
       int       (*UserFunIns)(void* ti);          // польз. ф-ция добавления записи в файл
       int         TreeCorrupt;                    // файл изменен
       int         ReadOnly;
       void       *UserParm;                       // Пользовательские параметры
       int         ShowMenu;                       // надо ли выдавать системное меню
       int         DontSaveF;                      // флаг сохранения области экрана (точнее, несохранения)
       int         Search_Running ;                // взводится при идущем поиске
       int       (*UserCheckDel)(void*);          // польз. ф-ция проверки перед удалением
       int         CollapseOnStart;                // схлопывать дерево при старте
       JBtrFltr    UserFilter;                     // фильтр записей
       int         HowToFilter;                    // как фильтровать (TREE_FILTER_*)
       int         ExitIfEmptyOnStart;             // немедленный выход из скроллинга при 0 записей
       int       (*UserFunDeleteTrn2)(void* parm); // Функция удаления в транзакции (после удаления)
       int         NoAskBeforeInsertRecord;        // Признак, что перед вводом записи не нужно спрашивать
                                                   // место вводимой записи (уровень, подуровень).
                                                   // Прикладной код сам все должен спросить
       int         NoAskBeforeDeleteRecord;        // Признак, что перед удалением записи не нужно
                                                   // задавать системный вопрос.
                                                   // Прикладной код сам все должен спросить
                                                   // Польз. ф-ция чтения дерева,
                                                   //  используется в SQL режиме
       int       (*UserMakeBranch)(void* ti, int (proc)(void* parm, int level, recpos_t pos), void* parm, int flags);

       int         DisableSelection;               // Заблокировать выделение записей
       int         NotEmpty;                       // Выход если нет записей

    // Tools.4.050.13
       int         Editable;                       // Режим редактирования записей в списке
       int       (*UserFunUpdate)(void* ti);       // польз. ф-ция обновления записи в файл
      } TREEINFO;

// Параметры ввода/редактир.
typedef struct TREEEDITPARM
      {
       SCROL     *S;                            // Скроллинг
       int        child;                        // Подуровень?
       int        FirstRec;                     // Самая первая запись?
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