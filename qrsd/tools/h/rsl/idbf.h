/**********************************************************************
*        File Name:   idbf.h             July 7,1994            
*       Programmer:   S. Kubrin              
*      Description:   �����প� 䠩��� dBase. ����㯭� �᫨ ��
*                     �������樨 ��।���� ����� DBASE
*  Public routines:  
* Private routines:  
***********************************************************************/

#ifndef __IDBF_H
#define __IDBF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <dbcdef.h>
#include <dbc_h.h>

// -----------------------------------------------------------------------------
typedef struct
      {
       DBFFILE       *file;     // ��ꥪ� DBC ������⥪�
       RECNUM         current;  // ����� ⥪�饩 �����, 0 �᫨ ��� ⥪�饩 �����
       char          *Buff;     // ���� �����
       int            stat;     // ����� ��᫥���� ����樨
       char           write;    // 1 - 䠩� ����� ������஢���
       RECNUM         lastpos;  // ����� �����, ��� ���ன �뫠 �믮����� ������
       unsigned long  key;
      } dBaseFile;              // getpos

//--- @H --  dBaseFile functions --------------------------------------- 
int   dBaseGetFieldNum(dBaseFile *db, char *name);
long  dBaseGetPos     (dBaseFile *db);
int   dBaseGetDirect  (dBaseFile *db, long rec);
void  dBaseClearRecord(dBaseFile *db);

void  ClearDbfInst(DINST *r);
int   InitSymDbf  (SYMDBFFILE *sym);
void  DoneSymDbf  (SYMDBFFILE *sym, DINST *r);
void  DbfRewind   (DINST *r);

_RSLLINK void ConnectDbfFile(SYMDBFFILE *sym, DINST *r);

bool  ConnectDbfFile_x(SYMDBFFILE *sym, DINST *r, char *name, int doAbort);
void  DbfReadValue    (SYMDBFFILE *sym, DINST *r, VALUE *v, int Id);
void  DbfWriteValue   (SYMDBFFILE *sym, DINST *r, VALUE *v, int Id, int shower);
int   GetDbfRecord    (SYMDBFFILE *sym, DINST *r, int prev);
int   PutDbfRecord    (SYMDBFFILE *sym, DINST *r, int insert);
int   DelDbfRecord    (SYMDBFFILE *sym, DINST *r);
int   CloneDbfFile    (SYMDBFFILE *sym, DINST *r, char *name, int doAbort);
int   CreateDbfFile   (struct tagBtDataBase *db, char *strName, char *file, int doAbort);

void  CopyDbfFields(VALUE *left, VALUE *right);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */