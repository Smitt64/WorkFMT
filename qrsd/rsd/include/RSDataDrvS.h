//-*--------------------------------------------------------------------------*-
//
// File Name   : RSDataDrvS.h
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// May 18, 2000  Alex Fedosov, Sergey Khorev - Create file
//-*--------------------------------------------------------------------------*-

// Интерфейс драйверов баз данных и серверов приложений
// (специфичный для универсального набора данных)

#ifndef __RSDATADRVS_H
#define __RSDATADRVS_H

#include "RSDataDrv.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
    // ************************* enumerators *************************
    
    enum RSDPropId_RSet_t {
        // pCreateRecordset
         RSDPID_CMD_IPTR   = 100,  // required, RSDBINARY (IRSCmd), указатель на интерфейс команд
         RSDPID_CMD_HANDLE = 101,  // required, RSDBINARY (RSDHCM), хендл обьекта-команды
         RSDPID_PAGE_SIZE = 102, // размер страницы кэша (в записях)
         RSDPID_MAX_PAGES = 103, // максимальное количество страниц
         RSDPID_CMD_RSNUM  = 104,   // optional, RSDLONG, номер набора данных (Кубрин. И как оно должно было бы использоваться?)
         RSDPID_AUTOTIMESTAMP = 105,
         RSDPID_NAME_BINDING = 106,  // adv. mode only
         RSDPID_CON_HANDLE = 107,      // adv. mode only
         RSDPID_CON_IPTR = 109,    // adv. mode only
         RSDPID_TABLE_NAME = 110,
         RSDPID_PRIMARY_KEY = 111,
         RSDPID_KEYDEFS = 112,
         RSDPID_UNIQUE_BINDING = 113,  // adv. mode only
         RSDPID_STATUS_BASE = 114,
         RSDPID_DEPTABLES = 115,
         RSDPID_MASTERCOLS = 116,
         RSDPID_JOINCOND = 117,
         RSDPID_DEPFIELDS = 118,
         RSDPID_WHERECOND = 119,
         RSDPID_MAKETMPJMP = 120,
         RSDPID_ADVHINTS = 121,
         RSDPID_CURPOS = 122,
         //RSDPID_USEROWNUM = 123,
         RSDPID_KEYNUM = 124,
         RSDPID_UPDATE_ALL_FIELDS = 125,
         RSDPID_EMPTY_QUERY = 126,
         RSDPID_TOOLS_WHERECOND = 127,
         RSDPID_FIELDS_LIST = 128,
         RSDPID_BLOB_FIELD = 129,
         RSDPID_BLOB_SIZE = 130,
         RSDPID_CACHE_BLOBS = 131,
         RSDPID_ROWNUM_LOCK = 132,
         RSDPID_ALLRECS_SQL = 133,
         RSDPID_FULL_SHIFT = 134,
         RSDPID_PASSIVE_CHECK = 135,
         RSDPID_UNIQUE_KEY = 136,
         RSDPID_BATCH_UPDATES = 137,
         RSDPID_SORT_BINDING = 138,  // adv. mode only
         RSDPID_PARAMETERS = 139,
         RSDPID_RESTORE_ON_CANCEL = 140,
         RSDPID_PARAMETERS_COUNT = 141,
         RSDPID_CLEAR_PARAMETERS = 142,
         RSDPID_BACKGROUND_FETCH = 143,
         RSDPID_USENULLS = 144,
         RSDPID_USER_FILTR = 145,
         RSDPID_USER_SORT = 146,
         RSDPID_FIELD_SEL = 147,
         RSDPID_AUTO_REFRESH = 148 // обновление записи из БД после Update
    };
    
    enum RSDPropValue_RSet_t {
    /*   // pCreateRecordset, RSDPID_
    RSDVAL_VERSION_2    = 0
        */
    };
#define USERCMDSN 5

    enum RSDCmdType_RSet_t {
       RSDVAL_INSERT_CMD,
       RSDVAL_UPDATE_CMD,
       RSDVAL_DELETE_CMD,
       RSDVAL_INSUPD_CMD,
       RSDVAL_REFRESH_CMD
    };

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __RSDATADRVS_H
