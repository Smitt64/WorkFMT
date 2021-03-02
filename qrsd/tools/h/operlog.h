//***********************************************************************
//  Журнал операций.                                        JV R-Style  *
//  26.3.1994                                                           *
// 4.4.95 KA Исправлен прототип ViewOpLog                               *
//  VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/operlog.h $
//                   $Revision: 19 $
//                   $Date: 10.07.00 19:28 $
//                   $Author: Lebedev $
//                   $Modtime: 10.07.00 19:27 $
//***********************************************************************

#ifndef __OPERLOG_H
#define __OPERLOG_H
#define _OPERLOGH_

#include "rstypes.h"
#include "apptldef.h"
#include "menu.h"
#include "btrv.h"
#include "listobj.h"

// SHORT FORMAT: elements and attributes of xml-format for clob-records in operlog\fisclog
#define XML_ELEM_HEXVALUE   "v"
#define XML_ELEM_TABLE      "t"
#define XML_ATTR_TABLENAME  "n"
#define XML_ELEM_FIELD      "f"
#define XML_ATTR_FIELDNAME  "n"
#define XML_ELEM_OLDVALUE   "o"
#define XML_ELEM_NEWVALUE   "v"

// LONG FORMAT: elements and attributes of xml-format for clob-records in operlog\fisclog
#define XML_ELEM_HEXVALUE_LONG    "value"
#define XML_ELEM_TABLE_LONG       "table"
#define XML_ATTR_TABLENAME_LONG   "name"
#define XML_ELEM_FIELD_LONG       "field"
#define XML_ATTR_FIELDNAME_LONG   "name"
#define XML_ELEM_OLDVALUE_LONG    "oldVal"
#define XML_ELEM_NEWVALUE_LONG    "newVal"

// виды xml-форматов в которых журнализируются данные
// старый, длинный формат (value, table, name и т.д.)
#define    XMLLOG_FORMAT1  1  
// новый, краткий формат (v, t, n и т.д.)
#define    XMLLOG_FORMAT2  2  

enum EXmlLogFieldType
{
   e_XmlLogField_clob = 0,
   e_XmlLogField_XmlType
};


// Фиксируемые операции
enum OperLog_Operations
   {
    OLinsert  = 2,                 // Ввод записи
    OLupdate  = 3,                 // Обновление записи
    OLdelete  = 4,                 // Удаление записи

    OLentry   = 1000,              // Вход в программу
    OLexit    = 1001,              // Выход из программы

    OLstrproc = 1002,              // Начало выполнения процедуры
    OLfinproc = 1003,              // Окончание выполнения процедуры

    OLinstrn  = 1004,              // Транзакция ввода
    OLupdtrn  = 1005,              // Транзакция изменения
    OLdeltrn  = 1006,              // Транзакция удаления

    OLentfail = 1007,              // Неудачный вход в программу

    OLusrdef  = 1100               // Определяемые пользователем операции
   };



/******************************************************************************
   Буфер записи и буфер ключа журнала операций

   OPERLOG, OPERLOG_BUFKEY
******************************************************************************/

#define  sizeFN    13             // Длина имени файла
#define  sizeAppl  1024*16        // Длина информации приложения

#include <packpsh1.h>

// Журнал операций
typedef struct
      {
       db_bdate   bdDateRec;           // Дата записи
       db_btime   btTimeRec;           // Время записи
       db_int16   iOper;               // Номер исполнителя
       char       networkNumber[4];
       char       physicalNodeAddress[6];
       db_int16   iOperation;          // Операция
       db_int16   iNumberDir;          // Номер директория
       char       szFileName[sizeFN];  // Имя файла
       db_int16   iApplicationKind;    // Вид приложения
       char       Reserve[12];         // Резерв
       db_uint32  crc32;               // Контрольная сумма
       db_int32   recId;               // Идентификатор записи
       db_int16   iSizeVarLen;         // Длина переменной части
       db_int16   iSizeVarLenS;        // Длина буфера до выполнения
       char       szApplication[sizeAppl]; // Информация приложения
      } OPERLOG;

typedef union
      {
       db_bdate      bdDateRec;        // Дата записи
       db_baseint32  recId;            // Идентификатор записи
      } OPERLOG_BUFKEY;

#include <packpop.h>


// -----------------------------------------------------------------------------
// Буфер "старой" записи OPERLOG_3 (без полей sysCompName/sysUserName)

#include <packpsh1.h>

typedef struct
      {
       db_bdate    date;             // дата-время операции
       db_btime    time;

       db_int16    numDprt;          // филиал
       char        progID;           // идентификатор программы
       db_int16    userID;           // идентификатор пользователя
       char        userType;         // тип пользователя
       db_int16    opCode;           // код операции

       db_uint32   netaddr;          // networkNumber
       char        saddr[6];         // physicalNodeAddress

       db_uint32   crc32;            // контрольная сумма
       db_int32    recID;            // идентификатор записи

       char        tableName[21];    // имя таблицы в БД
       db_int16    objectType;       // тип объекта
       char        objectID[41];     // идентификатор объекта

       char        reserv[46];       // резерв

       db_int32    transComRecID;    // Id входящего сообщения транспортной компоненты (ТК) (49-52 байты поля
                                     // t_Reserve в старых таблицах журнала (dfisclog_dbt, doperlog_dbt)) 

       db_int16    robot;            // Номер технологического пользователя-робота, от имени которого запущен
                                     // пользовательский процесс сервера приложений, обеспечивающий коммуникацию
                                     // с внешней системой

       db_uint32   varLenXML;        // длина переменной части в xml-записях журнала (53-56 байты поля t_Reserve
                                     // в старых таблицах журнала (dfisclog_dbt, doperlog_dbt))
       db_uint32   varLenBig;        // длина переменной части если > 64K (57-60 байты поля t_Reserve в старых
                                     // таблицах журнала (dfisclog_dbt, doperlog_dbt))
       db_uint16   varLen;           // длина переменной части < 64K. Если равно 1, то размер > 64K в varLenBig
      } OPERLOG_3;

typedef union
      {
       struct
            {
             db_bdate  date;
             db_btime  time;
            } stamp;

       struct
            {
             db_baseint16  numDprt;
             db_baseint32  recID;
            } id;

       struct
            {
             db_baseint16  numDprt;
             db_bdate      date;
             db_btime      time;
            } dprt;

       struct
            {
             char      progID;
             db_bdate  date;
             db_btime  time;
            } prog;

       struct
            {
             db_baseint16  userID;
             db_bdate      date;
             db_btime      time;
            } user;

       struct
            {
             char      tableName[21];
             db_bdate  date;
             db_btime  time;
            } table;

       struct
            {
             db_baseint16  objectType;
             db_bdate      date;
             db_btime      time;
            } type;

       struct
            {
             char      objectID[41];
             db_bdate  date;
             db_btime  time;
            } obj;

       struct
            {
             db_baseint32  transComRecID;               
            } transcom;

      } OPERLOG_3_BUFKEY;
#include <packpop.h>

/******************************************************************************
   Буфер записи и буфер ключа журнала операций

   OPERLOG_4, OPERLOG_4_BUFKEY
******************************************************************************/

#include <packpsh1.h>

typedef struct
      {
       db_bdate    date;             // дата-время операции
       db_btime    time;

       db_int16    numDprt;          // филиал
       char        progID;           // идентификатор программы
       db_int16    userID;           // идентификатор пользователя
       char        userType;         // тип пользователя
       db_int16    opCode;           // код операции

       db_uint32   netaddr;          // networkNumber
       char        saddr[6];         // physicalNodeAddress

       db_uint32   crc32;            // контрольная сумма
       db_int32    recID;            // идентификатор записи

       char        tableName[21];    // имя таблицы в БД
       db_int16    objectType;       // тип объекта
       char        objectID[41];     // идентификатор объекта

       char        reserv[46];       // резерв

       db_int32    transComRecID;    // Id входящего сообщения транспортной компоненты (ТК) (49-52 байты поля
                                     // t_Reserve в старых таблицах журнала (dfisclog_dbt, doperlog_dbt)) 

       db_int16    robot;            // Номер технологического пользователя-робота, от имени которого запущен
                                     // пользовательский процесс сервера приложений, обеспечивающий коммуникацию
                                     // с внешней системой

       db_uint32   varLenXML;        // длина переменной части в xml-записях журнала (53-56 байты поля t_Reserve
                                     // в старых таблицах журнала (dfisclog_dbt, doperlog_dbt))
       db_uint32   varLenBig;        // длина переменной части если > 64K (57-60 байты поля t_Reserve в старых
                                     // таблицах журнала (dfisclog_dbt, doperlog_dbt))
       db_uint16   varLen;           // длина переменной части < 64K. Если равно 1, то размер > 64K в varLenBig

       char        sysCompName[dNAME_LEN_COMP];  // Имя компьютера
       char        sysUserName[dNAME_LEN_USER];  // Имя пользователя
      } OPERLOG_4;


typedef union
      {
       struct
            {
             db_bdate  date;
             db_btime  time;
            } stamp;

       struct
            {
             db_baseint16  numDprt;
             db_baseint32  recID;
            } id;

       struct
            {
             db_baseint16  numDprt;
             db_bdate      date;
             db_btime      time;
            } dprt;

       struct
            {
             char      progID;
             db_bdate  date;
             db_btime  time;
            } prog;

       struct
            {
             db_baseint16  userID;
             db_bdate      date;
             db_btime      time;
            } user;

       struct
            {
             char      tableName[21];
             db_bdate  date;
             db_btime  time;
            } table;

       struct
            {
             db_baseint16  objectType;
             db_bdate      date;
             db_btime      time;
            } type;

       struct
            {
             char      objectID[41];
             db_bdate  date;
             db_btime  time;
            } obj;

       struct
            {
             db_baseint32  transComRecID;               
            } transcom;

       struct
            {
             db_baseint32  recID;
            } recid;

      } OPERLOG_4_BUFKEY;

#include <packpop.h>


/******************************************************************************
   Буфер записи и буфер ключа журнала операций

   OPERLOG_5, OPERLOG_5_BUFKEY
******************************************************************************/

#include <packpsh1.h>

typedef struct
      {
       db_bdate    date;             // дата-время операции
       db_btime    time;

       db_int16    numDprt;          // филиал
       char        progID;           // идентификатор программы
       db_int16    userID;           // идентификатор пользователя
       char        userType;         // тип пользователя
       db_int16    opCode;           // код операции

       db_uint32   netaddr;          // networkNumber
       char        saddr[6];         // physicalNodeAddress

       db_uint32   crc32;            // контрольная сумма
       db_int32    recID;            // идентификатор записи

       char        tableName[21];    // имя таблицы в БД
       db_int16    objectType;       // тип объекта
       char        objectID[41];     // идентификатор объекта

       char        reserv[46];       // резерв

       db_int32    transComRecID;    // Id входящего сообщения транспортной компоненты (ТК) (49-52 байты поля
                                     // t_Reserve в старых таблицах журнала (dfisclog_dbt, doperlog_dbt)) 

       db_int16    robot;            // Номер технологического пользователя-робота, от имени которого запущен
                                     // пользовательский процесс сервера приложений, обеспечивающий коммуникацию
                                     // с внешней системой

       db_uint32   varLenXML;        // длина переменной части в xml-записях журнала (53-56 байты поля t_Reserve
                                     // в старых таблицах журнала (dfisclog_dbt, doperlog_dbt))
       db_uint32   varLenBig;        // длина переменной части если > 64K (57-60 байты поля t_Reserve в старых
                                     // таблицах журнала (dfisclog_dbt, doperlog_dbt))
       db_uint16   varLen;           // длина переменной части < 64K. Если равно 1, то размер > 64K в varLenBig

       char        sysCompName[dNAME_LEN_COMP];  // Имя компьютера
       char        sysUserName[dNAME_LEN_USER];  // Имя пользователя

       db_int32    processID;        // идентификатор процесса (или сессии - кто как хочет)
      } OPERLOG_5;


typedef union
      {
       struct
            {
             db_bdate  date;
             db_btime  time;
            } stamp;

       struct
            {
             db_baseint16  numDprt;
             db_baseint32  recID;
            } id;

       struct
            {
             db_baseint16  numDprt;
             db_bdate      date;
             db_btime      time;
            } dprt;

       struct
            {
             char      progID;
             db_bdate  date;
             db_btime  time;
            } prog;

       struct
            {
             db_baseint16  userID;
             db_bdate      date;
             db_btime      time;
            } user;

       struct
            {
             char      tableName[21];
             db_bdate  date;
             db_btime  time;
            } table;

       struct
            {
             db_baseint16  objectType;
             db_bdate      date;
             db_btime      time;
            } type;

       struct
            {
             char      objectID[41];
             db_bdate  date;
             db_btime  time;
            } obj;

       struct
            {
             db_baseint32  transComRecID;               
            } transcom;

       struct
            {
             db_baseint32  recID;
            } recid;

      } OPERLOG_5_BUFKEY;

#include <packpop.h>

// -----------------------------------------------------------------------------
enum                             // Номера ключей
   {
    OPERLOG_DATE = 0,
    OPERLOG_ID,
    OPERLOG_DPRT,
    OPERLOG_PROG,
    OPERLOG_USER,
    OPERLOG_TABLE,
    OPERLOG_TYPE,
    OPERLOG_OBJ,
    OPERLOG_TRANSCOMRECID,
    OPERLOG_RECID
   };



/******************************************************************************
   Буфер записи и буфер ключа вспомогательного файла журнала операций

   OPLOGSUP, OPLOGSUP_BUFKEY
******************************************************************************/

#include <packpsh1.h>

typedef struct
      {
       char       fileName[26];     // файл журнала
       db_int16   numDprt;          // филиал

       db_uint32  crc32;            // контрольная сумма
       db_int32   recID;            // идентификатор записи

       db_uint32  xml_crc32;        // контрольная сумма для xml-записей

       char       reserv[56];       // резерв
      } OPLOGSUP;

typedef union
      {
       struct
            {
             char          fileName[26];
             db_baseint16  numDprt;
            } dprt;
      } OPLOGSUP_BUFKEY;

#include <packpop.h>

enum                             // Номера ключей
   {
    OPLOGSUP_DPRT = 0,
   };



#ifdef __cplusplus
 extern "C" {
#endif

_APPTLDATA extern BTRVFILE  *FileOpLog;     // Файл журнала операций


#define  SKF_OpLog(KeyNum, beg, end)               SKF_OperLog(FileOpLog, KeyNum, beg, end)

#define  iOpenOpLog(iOpenMode, NameFile)           ((FileOpLog = Open_OperLog(iOpenMode, NameFile, NULL)), bfstat)
#define  iOpenOpLogEx(iOpenMode, NameFile, owner)  ((FileOpLog = Open_OperLog(iOpenMode, NameFile, owner)), bfstat)

#define  iCrOperLog(NameFile)                      Create_OperLog(NameFile)

#define  iWriteOpLog(iOperation, iOper, iApplicationKind, iNumberDir, bfile, saveb, iSizeVarLenS) \
         Write_OperLog(FileOpLog, iOperation, iOper, iApplicationKind, iNumberDir, bfile, saveb, iSizeVarLenS)

#define  ViewOpLog(NameDict, ViewAppl)     View_OperLog(FileOpLog, NameDict, ViewAppl)

//***********************************************************************

// Элемент списка операций
typedef struct
      {
       LOBJ  node;
       int   id;                     // Идентификатор операции
       int   categ;                  // Категория операции
       char  name[21];               // Наименование
      } OpLogOper;

// Категории журнализируемых операций
enum
   {
    OLinsrec = 1,                // Ввод записи
    OLdelrec = 2,                // Удаление записи
    OLchgrec = 3,                // Обновление записи
    OLproc   = 4                 // Процедура
   };

/*
 * InitOpLogOper
 *    Инициализация списка операций.
 *    Создание списка и внесение в него стандартных операций.
 *
 * AddOpLogOper
 *    Добавить операцию в список. id должен быть уникальным в списке.
 *
 * FindOpLogOper
 *    Найти операцию в списке. Возвращает указатель на найденный элемент списка.
 *    Или NULL, если операция не найдена.
 */

_APPTLFUNC int        InitOpLogOper(void);
_APPTLFUNC int        AddOpLogOper (int id, int categ, const char *name);
_APPTLFUNC OpLogOper *FindOpLogOper(int id);

//***********************************************************************

/*
 *    Семейство функций для поддержки журнала операций, с возможностью
 *          одновременного использования нескольких журналов.
 *
 *    Create_OperLog
 *    Open_OperLog
 *    SKF_OperLog
 *    Write_OperLog
 *    View_OperLog
 *
 * Функция Create_OperLog ничем не отличается от старой iCrOperLog.
 *
 * Вместо двух функций iOpenOpLog и iOpenOpLogEx сделана одна - Open_OperLog.
 * Она имеет три параметра (третий это имя владельца) как у iOpenOpLogEx,
 * разница между ними в том, что она возвращает не код ошибки, а указатель на
 * открытый файл или NULL, если файл не открыт.
 *
 * Остальные функции аналогичны старым функциям за исключением того, что
 * первый параметр это указатель на открытый файл журнала.
 * Пользователь должен (как и прежде) сам позаботиться о закрытии открытого
 * файла, вызвав bfClose.
 *
 */
_APPTLFUNC int        Create_OperLog(char *NameFile);
_APPTLFUNC BTRVFILE *Open_OperLog   (int OpenMode, const char *NameFile, const char *Owner);
_APPTLFUNC void       SKF_OperLog   (BTRVFILE *, int KeyNum, db_bdate* Beg, db_bdate* End);

_APPTLFUNC int        Write_OperLog (BTRVFILE *, int Operation, int oper, int ApplictionKind, int NumberDir,
                                     BTRVFILE *bfile, void *saveb, int size);

_APPTLFUNC int        View_OperLog  (BTRVFILE *, const char *NameDict, panel_keyproc_t proc);


// Функция ViewOpLogItem - просмотр записи из журнала операций
_APPTLFUNC void       ViewOpLogItem (OPERLOG *ol, BTRVFILE **ProjFiles);


//***********************************************************************

// Элемент списка
typedef struct
      {
       LOBJ  node;
       char *FileName;              // Имя файла - уникальный ключ в списке
       int   present_id;            // Признак наличия в файле поля recID
      } OpenOperLog;

void            SetFlagID_OpenOperLog(const char *FileName, int present_id);
_APPTLFUNC int  GetFlagID_OpenOperLog(const char *FileName);

/*
 * Проверить целостность записей в журнале операций.
 */
typedef int (* CheckCRCindic_t)(void*);

_APPTLFUNC int CheckCRC_OperLog(BTRVFILE *, CheckCRCindic_t indic, void *parm);

#ifdef __cplusplus
 } ;
#endif

#endif // __OPERLOG_H

//************************** Конец файла ********************************

/* <-- EOF --> */