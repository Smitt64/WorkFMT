/**********************************************************************
*        File Name:   ibfile.h           March 11,1994            
*       Programmer:   S. Kubrin              
*      Description:   Функции переноса данных между Btrieve и
*                     переменными интерпретатора
*  Public routines:  
* Private routines:  
***********************************************************************/
#ifndef __IBFILE_H
#define __IBFILE_H

#ifndef __BT_H
#include "rsl/bt.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

int ValueToBuf(char *data, BTRSTRUCTURE *str, VALUE *val, int ind, int shower, int cnvMode);
int BufToValue(char *data, BTRSTRUCTURE *str, VALUE *val, int ind, int cnvMode);

TBtDataBase *MakeLoginBtrDic(char *altDic);

void BtrCloseBlob    (BF *bf, int doAbort);
bool RslSaveBlobValue(BF *bf, VALUE *val);
bool RslReadBlobValue(BF *bf, VALUE *val);

bool RslWriteBlobObject(BF *bf, TGenObject *gObj);
bool RslReadBlobObject (BF *bf, TGenObject *gObj);

void BtrMoveKeyValues(BFILE *bfile, int parId);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */