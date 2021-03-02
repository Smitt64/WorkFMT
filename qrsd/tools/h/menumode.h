/************************************************************************/
/*  Оболочка работы с В-файлами                           Программист   */
/*  -меню режимов-                            Чаусов  В.И. */
/*                                24.10.92                              */
/************************************************************************/

#ifndef __MENUMODE_H
#define __MENUMODE_H

#include "apptldef.h"


// описатель меню режимов
typedef struct
      {
       const char **NameM;      // Список названий возможных режимов
       const char  *MaketM;     // Строка-макет возможных режимов
       char        *StringM;    // Строка режимов
       int          insdel;     // Режим - вставка/удаление     
       const char **RefN;       // Список ссылок на NameM
       int         *NumbN;      // Список номеров режимов
       int          numm;       // Количество режимов
      } MENUMODE;

#ifdef __cplusplus
 extern "C" {
#endif

// Меню режимов
_APPTLFUNC int MenuMode( 
                        const char **NameM,      // Список названий возможных режимов
                        const char  *MaketM,     // Строка-макет возможных режимов
                        char        *StringM,    // Строка режимов
                        int          numm,       // Количество возможных режимов
                        const char  *head,       // Заголовок меню
                        int          insdel,     // Режим - вставка/удаление     
                        coord        x,          // Координаты верхнего левого угла меню    
                        coord        y
                       );

// Редактирование режимов
_APPTLFUNC int EditMode( 
                        const char  **NameM,     // Список названий возможных режимов
                        const char   *MaketM,    // Строка-макет возможных режимов
                        char         *StringM,   // Строка режимов
                        int           numm,      // Количество возможных режимов
                        const char  **head,      // Заголовки меню
                        coord         x,         // Координаты верхнего левого угла меню    
                        coord         y
                       );

// Записать строку в режим
_APPTLFUNC void str_to_mode(
                            const char *str,     // Строка
                            short int  *mode     // Режим
                           );

// Записать режим в строку
_APPTLFUNC void mode_to_str( 
                            char       *str,     // Строка
                            const char *maket,   // Макет
                            short int   mode     // Режим
                           );

#ifdef __cplusplus
 } ;
#endif

#endif //__MENUMODE_H

/* <-- EOF --> */