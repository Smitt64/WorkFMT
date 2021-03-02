#ifndef __MENUBTR_H
#define __MENUBTR_H

#ifdef __cplusplus
 extern "C" {
#endif

int  LoadMenuBtr(void **menu, int *n, int oper, int program);
void FreeMenuBtr(void *menu, int n);

int  BorrowMenuBtr(int Oper, int Type, int fromOper);
int  DeleteMenuBtr(int Oper, int Type);

#ifdef __cplusplus
 } ;
#endif

#endif //__MENUBTR_H

/* <-- EOF --> */