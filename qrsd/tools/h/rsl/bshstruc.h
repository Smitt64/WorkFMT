/*******************************************************************************
 FILE         :   BSHCTRUC.H

 COPYRIGHT    :   R-Style Software Lab, 1992, 1998

 DESCRIPTION  :   Струкура словаря базы данных

 PROGRAMMED BY:

 CREATION DATE:

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsl/bshstruc.h $
                  $Revision: 13 $
                  $Date: 26.06.00 19:31 $
                  $Author: Lebedev $
                  $Modtime: 26.06.00 19:24 $
*******************************************************************************/

#ifndef __BSHSTRUC_H
#define __BSHSTRUC_H

#ifndef __NWBTRV_H
#include "nwbtrv.h"
#endif

#ifndef __RSTYPES_H
#include "rstypes.h"
#endif

#include "packpsh1.h"

// -----------------------------------------------------------------------------
/* OLD_STRUCT DEF-FILE */
// Описание записи проекта
typedef union
      {
       // Описание файла Btrieve
       struct
            {
             db_baseint32   Code_File;      // Код файла
             unsigned char  Group;          // Признак группы
             char           Name[13];       // Название
             char           OWN[9];         // Владелец
             db_baseint16   TypeOWN;        // Тип владелец
             char           Contents[33];   // Содержание
             FileSpecs      FS;             // Спецификация файла
             db_baseint16   maxvarlen;      // Максимальная длина переменной части.
             char           UseBLOB;        // Использование VarLen as BLOB
             char           isTmp;          // GLOBAL TEMPORARY TABLE  for Oracle version.
            } bfile;

       // Описание поля файла Btrieve
       struct
            {
             db_baseint32   Code_Field;     // Код поля
             unsigned char  Group;          // Признак группы
             db_baseint32   Code_File;      // Код файла
             db_baseint16   Number;         // Номер поля
             char           Name[24];       // Название
             unsigned char  FVt;            // Тип значения поля
             db_baseint16   Field_Point;    // Знаков после точки, макс. длина строки
             db_baseint16   l;              // Длина строки вывода
             db_baseint16   fldlen;         // Длина буфера поля
             char           Contents[37];   // Содержание
             char           ViewS[2];       // Показ в скролинге
             char           ViewP[2];       // Показ в панели

             unsigned char  ViewE;          // Показ в экспорте
             unsigned char  decPoint;       // Количество знаков после точки для NUMERIC типа поля
             unsigned char  ViewI;          // Показ в импорте
             unsigned char  reserv;
            } bfield;

       // Описание ключа файла Btrieve
       struct
            {
             db_baseint32   Code_Key;       // Код ключа
             unsigned char  Group;          // Признак группы
             db_baseint32   Code_File;      // Код файла
             db_baseint16   Number;         // Номер ключа
             db_baseint32   Code_Field;     // Код поля
             db_baseint16   offset;         // Смещение относительно начала поля
             char           Contents[22];   // Содержание
             KeySpecs       KS;             // Спецификация ключа
             unsigned char  pKey;           // The number of Primary Key
            } bkey;

       struct
            {
             db_baseint32   Code_Vers;      // Код записи
             unsigned char  Group;          // Признак группы
             char           Name[13];       // Название
             db_baseint32   Version;        // Номер версии
            } version;
      } BUFREC_SIZE87;

// -----------------------------------------------------------------------------
// Буфер ключа файла проекта
typedef union
      {
       db_baseint32         Code_File;      // Код файла

       struct
            {
             unsigned char  Group;          // Признак группы
             char           Name[13];       // Название
            } GN;

       struct
            {
             db_baseint32   Code_File;      // Код файла
             unsigned char  Group;          // Признак группы
             db_baseint16   Number;         // Номер поля
            } GFN;
      } BUFKEY_SIZE87;

// -----------------------------------------------------------------------------
#ifndef _MSC_VER
    #ifndef __cplusplus
        #if (sizeof(BUFREC_SIZE87) != 87)
            #error "Incorrect sizeof(BUFREC_SIZE87) != 87"
        #endif

        #if (sizeof(BUFKEY_SIZE87) != 14)
            #error "Incorrect sizeof(BUFKEY_SIZE87) != 14"
        #endif
    #endif
#endif  // _MSC_VER

// -----------------------------------------------------------------------------
// Ключи записи проекта
enum BRECKEY
   {
    KEYAUTO = 0,                   // 0 - Code_File
    KEYGN,                         // 1 - Group, Name
    KEYGFN                         // 2 - Code_File, Group, Number
   };

// -----------------------------------------------------------------------------
// Группы записей проекта
enum BSHBGROUP
   {
    BFE = 'f',                     // Описание файла
    BFD = 'd',                     // Описание поля
    BKY = 'k',                     // Описание ключа
    BVR = '\0'                     // Описание версии
   };


// -----------------------------------------------------------------------------
#define  RSL_DEF_VERSION  "VERSION"

// -----------------------------------------------------------------------------
/* NEW_STRUCT DEF-FILE */
//RECORD DESCRIPTIONS
// Описание записи проекта
typedef struct
      {
       //COMMON & KEY
       struct BUFREC_common
            {
             long           Code;               // Код записи (файла, поля, ключа или версии)
             unsigned char  Group;              // Признак группы
             short int      Number;             // Номер поля или ключа
             char           Name[24];           // Название файла или поля
             long           Code_File;          // Код файла для поля или ключа
             char           Contents[40];       // Содержание
            } common;

       //VERSION
       struct
            {
             long           Version;            // Номер версии
            } version;

       //BFILE
       struct
            {
             char           OWN[9];             // Владелец
             short int      TypeOWN;            // Тип владелец
             FileSpecs      FS;                 // Спецификация файла
             short int      maxvarlen;          // Максимальная длина переменной части.
             char           UseBLOB;            // Использование VarLen as BLOB
            } bfile;

       //BFIELD
       struct
            {
             unsigned char  FVt;                // Тип значения поля
             short int      Field_Point;        // Знаков после точки, макс. длина строки
             short int      l;                  // Длина строки вывода
             short int      fldlen;             // Длина буфера поля
             char           ViewS[2];           // Показ в скролинге
             char           ViewP[2];           // Показ в панели
             short int      ViewE;              // Показ в экспорте
             short int      ViewI;              // Показ в импорте
            } bfield;

       //BKEY
       struct
            {
             long           Code_Field;         // Код поля
             short int      offset;             // Смещение относительно начала поля
             KeySpecs       KS;                 // Спецификация ключа
            } bkey;
      } BUFREC_NONOVERLAPPED_RECORD;

// -----------------------------------------------------------------------------
// Описание записи проекта
typedef union BUFREC_NONOVERLAPPED
      {
       //VERSION
       struct
            {
             long           Code_Vers;          // Код записи
             unsigned char  Group;              // Признак группы
             short int      Number;             // Номер поля или ключа
             char           Name[24];           // Название
             long           reserv2;
             char           Contents[40];       // Содержание
             long           Version;            // Номер версии
            } version;

       //FILE DESCRIPTIONS
       // Описание файла Btrieve
       struct
            {
             long           Code_File;          // Код файла
             unsigned char  Group;              // Признак группы
             short int      reserv1;
             char           Name[24];           // Название
             long           reserv2;
             char           Contents[40];       // Содержание

             char           version[sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->version)];

             char           OWN[9];             // Владелец
             short int      TypeOWN;            // Тип владелец
             FileSpecs      FS;                 // Спецификация файла
             short int      maxvarlen;          // Максимальная длина переменной части.
             char           UseBLOB;            // Использование VarLen as BLOB
            } bfile;

       //FIELDS DESCRIPTIONS
       // Описание поля файла Btrieve
       struct
            {
             long           Code_Field;         // Код поля
             unsigned char  Group;              // Признак группы
             short int      Number;             // Номер поля или ключа
             char           Name[24];           // Название
             long           Code_File;          // Код файла для поля или ключа
             char           Contents[40];       // Содержание

             char           version[sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->version)];
             char           bfile  [sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->bfile)];

             unsigned char  FVt;                // Тип значения поля
             short int      Field_Point;        // Знаков после точки, макс. длина строки
             short int      l;                  // Длина строки вывода
             short int      fldlen;             // Длина буфера поля
             char           ViewS[2];           // Показ в скролинге
             char           ViewP[2];           // Показ в панели
             short int      ViewE;              // Показ в экспорте
             short int      ViewI;              // Показ в импорте
            } bfield;

       //KEYS DESCRIPTIONS
       // Описание ключа файла Btrieve
       struct
            {
             long           Code_Key;           // Код ключа
             unsigned char  Group;              // Признак группы
             short int      Number;             // Номер поля или ключа
             char           reserv1[24];
             long           Code_File;          // Код файла для поля или ключа
             char           Contents[40];       // Содержание

             char           version[sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->version)];
             char           bfile  [sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->bfile)];

             unsigned char  pKey;               // The number of Primary Key
             char           bfield [sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->bfield) - 1];

             long           Code_Field;         // Код поля
             short int      offset;             // Смещение относительно начала поля
             KeySpecs       KS;                 // Спецификация ключа
            } bkey;
      } BUFREC_NONOVERLAPPED;

// -----------------------------------------------------------------------------
#ifndef _MSC_VER
    #ifndef __cplusplus
        #if sizeof(BUFREC_NONOVERLAPPED) != sizeof(BUFREC_NONOVERLAPPED_RECORD)
            #error "Invalid size 'BUFREC_NONOVERLAPPED'"
        #endif
    #endif
#endif // _MSC_VER

// -----------------------------------------------------------------------------
// Буфер ключа файла проекта
typedef union
      {
       long                 Code_File;      // Код файла

       struct
            {
             unsigned char  Group;          // Признак группы
             char           Name[24];       // Название
            } GN;

       struct
            {
             long           Code_File;      // Код файла
             unsigned char  Group;          // Признак группы
             short int      Number;         // Номер поля
            } GFN;
      } BUFKEY_NONOVERLAPPED;

#include "packpop.h"

// -----------------------------------------------------------------------------
/* SET DEFAULT STRUCT */
typedef BUFREC_SIZE87  BUFREC;
typedef BUFKEY_SIZE87  BUFKEY;

#endif // __BSHSTRUC_H

/* <-- EOF --> */