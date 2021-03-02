// Комплекс:    RS-TOOLS 3.0 (c) VECsoft Ltd. 1992-1994, (c)JV R-Style 1993-1994.
// Модуль:      Для работы с dbf-файлами.
// Программист: Эмиль Довидович.
// Информация:
// История:     06/01/94
//

#ifndef __CDBF_H
#define __CDBF_H

#ifdef __cplusplus
 extern "C" {
#endif

int CDbf(char *dbfn, int (*pr)(char *rb));

char *dbern(int rc);

void dberr(char *mes, int rc);

#ifdef __cplusplus
 }
#endif

#endif //__CDBF_H

/* <-- EOF --> */