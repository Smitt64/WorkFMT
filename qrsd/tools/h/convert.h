// Комплекс:     RS-TOOLS 3.0 (c) VECsoft 1992-1993, (c) R-Style 1993.
// Модуль:       Преобразователей данных.
// Программист:  Эмиль Довидович.
// Информация:
// История:      14/01/94
//

#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <tooldef.h>

#ifdef __cplusplus
 extern "C" {
#endif


int iCnvDBtxtToBtype(
                     char         *sField,
                     char          cType,           // type of data  'C', 'N', 'D', 'L', 'M'.
                     unsigned int  uiWidth,         // field width
                     unsigned char ucDec,           // length of decimal places
                     void         *pV,              // Значение
                     unsigned int  uiBType,         // Тип значения
                     unsigned char ucP              // Знаков после точки или максимальная длина
                    );

_TOOLEXP char *szTermDir(char *Dir);

#ifdef __cplusplus
 } ;
#endif

#endif // _CONVERT_H_

/* <-- EOF --> */