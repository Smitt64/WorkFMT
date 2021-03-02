//***********************************************************************
//                        Сообщения из файла
//                         VECsoft  2.12.92
//***********************************************************************
#ifndef __FILEMSG_H
#define __FILEMSG_H

#include "rstypes.h"
#include "apptldef.h"

#ifdef __cplusplus
 extern "C" {
#endif


#include <packpsh1.h>

typedef struct FILEMSG_OLD {                // Сообщение из файла

       db_uint16  Number;           // Номер сообщения
       db_uint16  Page;             // Номер страницы помощи
       char       Contents[81];     // Содержание
      } FILEMSG_OLD;


// Сообщение из файла
typedef struct FILEMSG
      {
       db_uint16  Number;           // Номер сообщения
       db_uint16  Page;             // Номер страницы помощи
       char       Contents[256];    // Содержание
      } FILEMSG;


typedef struct FILEMSG_ORA {    // Сообщение из файла пользовательских сообщения для констрейнтов (CONSTRAINT)  - dbank_ora_msg

 char       Error[10];           // Код ошибки
 char       Object[31];          // Наименование объекта Oracle (констрейнта\индекса), max допустимое в Oracle - 30 символов.
 char       Message[256];        // Сообщение об ошибке
 db_uint16  Page;                // Номер страницы помощи
 char       IsSystem;            // Признак системного сообщения (перетирается апгрейдером)   

} FILEMSG_ORA;

//уникальный индекс, таблицы dbank_ora_msg
typedef struct FILEMSG_ORA_KEY { 
 char       Error[10];
 char       Object[31];
}FILEMSG_ORA_KEY;

#include <packpop.h>

// -----------------------------------------------------------------------------
// Получить сообщение из файла
_APPTLFUNC char *getfmsg(
                         const char   *NameF,           // Имя файла сообщений
                         unsigned int  num              // Номер сообщения
                        );

// Получить сообщение из файла
_APPTLFUNC char *getfmsgEx(
                           const char   *NameF,           // Имя файла сообщений
                           unsigned int  num,             // Номер сообщения
                           int          *Page             // Номер страницы помощи
                          );

//AV Получить сообщение из файла пользовательских сообщений для констрейнтов\индексов - dbank_ora_msg
_APPTLFUNC extern char *getfmsg_ora
( 
   const char  *rsd_mes,         // строка с ошибками Oracle, можно не передавать (NULL),  будет использоваться глобализм szLastRsdError
   int         error,            // системный номер ошибки: BEdupkey, BEconstraint_parent_notfound, BEconstraint_child_found
   char        *ret_err_code,    // Возвращаемый код ошибки (ORA-00001,ORA-00291,ORA-00292)
   char        *ret_object,      // Возвращаемое наименование объекта (констрейнта\индекса), для которого нашли пользовательское сообщение
   int         *ret_page         // Возвращаемое значение номера страницы помощи, ддя полученного пользовательского сообщения
);

//AV обработка ошибок сгенерированных констрейнтами\индексами, возвращает true, если текущая ошибка в bfstat действительно сгенерирована констрейнтами
_APPTLFUNC extern bool processOracleObjectError
( 
   const char  *rsd_mes,                  // строка с ошибками Oracle, можно не передавать (NULL),  будет использоваться глобализм szLastRsdError
   char        *outmes,                   //сообщение об ошибке - найденное пользовательское сообщение, заданное для констрейнта, а если не нашли, то просто текст ораклового сообщения 
   char        *ret_code_error   = NULL,  //найденный код ошибки 
   char        *ret_object       = NULL,  //найденное наименование объекта (констрейнта\индекса)
   int         *page_help        = NULL   //номер страницы помощи для найденного пользовательского сообщения
);


// Окно сообщения возвращает код ESC.
_APPTLFUNC int fmeswin(
                       const char   *NameF,           // Имя файла сообщений
                       unsigned int  num              // Номер сообщения
                      );

// Окно сообщения в поле возвращает код ESC.
_APPTLFUNC int ffldwin(
                       const char   *NameF,           // Имя файла сообщений
                       unsigned int  num,             // Номер сообщения
                       PANEL *p                       // Текущая панель
                      );

// Окно сообщения с параметрами возвращает код ESC
_APPTLFUNC int fformwin(
                        const char   *NameF,           // Имя файла сообщений
                        unsigned int  num,             // Номер сообщения
                        int           rf,              // Краснота
                        ...                            // Список переменных
                       );

/* Убрать из строки лишние пробелы      */
_APPTLFUNC int ClearBlank(
                          char *line                     /* Строка                               */
                         );

// возвращает код ESC
_APPTLFUNC int YesNoMsg(
                        const char   *NameF,    // Имя файла сообщений
                        unsigned int  num,      // Номер сообщения
                        int           ncf,      // Текущая кнопка
                        ...                     // Список переменных
                       );

int iListFileMsg(const char* nameF, coord x, coord y, coord l, void *buff);

#ifdef __cplusplus
 } ;
#endif

#endif  // __FILEMSG_H

/* <-- EOF --> */