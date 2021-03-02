// Комплекс:     RS-TOOLS 3.0 (c) VECsoft 1992-1993, (c) R-Style 1993.
// Модуль:       Заголовок для распределения памяти.
// Программист:  Эмиль Довидович.
// Информация:
// История:      09/11/93
// VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/talloc.h $
//                  $Revision: 4 $
//                  $Date: 8.09.98 18:12 $
//                  $Author: Markovitch $
//                  $Modtime: 3.09.98 21:57 $
//

#ifndef _TALLOC_H_
#define _TALLOC_H_

#include "pexpdef.h"

#ifdef __cplusplus
extern "C" {
#endif

_RSPNLEXP void *Allocate(size_t size);
_RSPNLEXP void  FreeMem(void **ptr);

#ifdef __cplusplus
}
#endif

#endif // _TALLOC_H_

/* <-- EOF --> */
