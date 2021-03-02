/**********************************************************************
*        File Name:   xql.h              February 14,1994
*       Programmer:   S. Kubrin
*      Description:
*  Public routines:
* Private routines:
***********************************************************************/
#ifndef __BT_H
#define __BT_H

#if !defined(__STDLIB_H) && !defined(_INC_STDLIB)
#include <stdlib.h>
#endif

#include "listobj.h"
#include <nwbtrv.h>
#include "rsl/btintf.h"

#include <rscdefs.h>
#include <platform.h>

#include <packpshn.h>

#ifdef __cplusplus
extern "C" {
#endif


// -----------------------------------------------------------------------------
#ifdef   RSL_MD_FLAT
    #define  BTLOCAL
    #define  BTFARD
#else
    #define  BTLOCAL  _near _pascal

    #define  BTFARD   _far
#endif

// -----------------------------------------------------------------------------
#define  MAX_BT_ERRORS      200
#define  BT_ERR_ARRAY_SIZE  (MAX_BT_ERRORS / sizeof (unsigned))

// -----------------------------------------------------------------------------
typedef struct
      {
       FileSpecs  fl;
       KeySpecs   Key[119];
       AltCol     alt;
      } TBtSpec;

// -----------------------------------------------------------------------------
struct  tagBTRSTRUCTURE
     {
      TBtStructure           str;

      LOBJ                   str_elem;   // ������� ᯨ᪠ ������� 䠩��
      int                    count;      // ���稪 ��뫮�
      char                  *names;
      CNTLIST                bf_list;    // ���᮪ BFILE ������ ��������
      struct tagBtDataBase  *dataBase;
      char                  *comments;

      char                  *fileComment;
      char                  *keyComments;
      bool                   persistStr;
      bool                   shared;
     };

// -----------------------------------------------------------------------------
typedef struct tagBTRSTRUCTURE  BTRSTRUCTURE;
// � names ᮤ�ঠ��� ����� ����� ࠧ������� ᨬ����� '\0'

// -----------------------------------------------------------------------------
#define  BF_NUMBER_SCIP  10

// -----------------------------------------------------------------------------
#ifndef BKMHDEFINED
    #define BKMHDEFINED

    #ifdef SQLBUILD
        RSL_MAKE_HANDLE(BKMHANDLE);
    #endif
#endif

// -----------------------------------------------------------------------------
struct tagBF
     {
      char        name[_MAX_PATH];
/* [!!!:x64]
      #ifdef RSL_PL_WIN64
          void   *unused;     // Need to correct aligment of Pos_blk
      #endif
*/
      posblk_t    Pos_blk;

      int         path;       /* Curent key path   */
      char       *Buff;       /* Data   buffer                       */
      char       *KeyBuff;
      int         recsize;    // ������ �뤥������� ����
      int         fixsize;    // ������ 䨪�஢����� ���
      int         reclen;     // ����騩 ࠧ��� �����
      BKMHANDLE   lastpos;    // ������ ����祭��� ��᫥���� GetDirect
      int         keylen;

      struct tagTBlobControlData  *blob;
      struct tagBtDataBase        *dataBase;

      unsigned char openMode;
     };

// -----------------------------------------------------------------------------
typedef struct tagBF  BF;

// -----------------------------------------------------------------------------
struct tagBFILE
     {
      BF             derived;
      LOBJ           bf_elem;
      BTRSTRUCTURE  *str;
      CNTLIST        ref_list;    // ���᮪ REFERENCE
      char           next;        // ����� 1 �᫨ ����室��� �믮����� ������
                                  // BgetN ��� BgetP
      char           trnFlag;     // 1 = 䠩� ����� ���⢮���� � �࠭���樨

      CNTLIST        bindList;    // List of TBindElem
      // .... ���ଠ�� � 䨫���樨
     };

// -----------------------------------------------------------------------------
typedef struct tagBFILE  BFILE;

// -----------------------------------------------------------------------------
#define  P_BF(ptr)  ((BF *)(ptr))

// -----------------------------------------------------------------------------
typedef  void (*TFieldGetSetProc)(void *to, void *from, int len, struct tagTBindElem *bnd);

// -----------------------------------------------------------------------------
typedef struct tagTBindElem
      {
       LOBJ               listElem;
       BFILE             *bf;

       void              *fileData;
       unsigned short     fileSize;

       void              *userData;
       unsigned short     userSize;

       TFieldGetSetProc   getter;  // From file to user;
       TFieldGetSetProc   setter;  // From user to file;
       unsigned           flags;

       short int          id; // Field number
      } TBindElem;

// -----------------------------------------------------------------------------
typedef struct tagREFERENCE
      {
       BFILE   derived;
       LOBJ    ref_elem;
      } REFERENCE;

// -----------------------------------------------------------------------------
struct tagBtDataBase
     {
      TBtMesProc        BtMesProcPtr; // BtMesProc; �㭪�� ��ࠡ�⪨ �訡��
      TBtMemAllocProc   allocMem;
      TBtMemFreeProc    freeMem;
      unsigned          VarLenSize;   // 256;  ������ ��६����� ��� �� 㬮�砭��


      BF               *bf;   // BF ��� ����� �믮��﫠�� ��᫥���� ������
      int               stat;   // ����� ����樨
      int               op;   // ��� ����樨

      char             *DataDir;   // ��⠫�� � ���஬ �ᯮ�������� 䠩�� ������
      CNTLIST           str_list;   // ���᮪ BTRSTRUCTURE


      BF               *dic;  // ᫮����
      unsigned char     useBlob;
      unsigned char     noCurrencyChange;

      int               recLockFlag;   // Explicit lock flag
      unsigned char     noLockInTrn;   // If Trn is active, do not use trnLockFlag;

      unsigned          erScipArray[BT_ERR_ARRAY_SIZE];

      char              globalPassword[9];
      bool              reportError;

      TAltColProc       BtSetAltCol;
      void             *altColData;

   // August 17,1999
      TBtError          erInfo;

   // August 24,1999
      unsigned char     cnvType;   // RSL_BCNV_ ...

      short int         version;

      bool              isSQL;

      #ifdef RSL_MD_MT
          bool          clientAlloced;
      #endif

      bclient_h         clientID;
      BTClientID        extClientID;

      int               RslTrnFlag;
      int               trnLockFlag;

      int               trnActive;   // Now not used
     };

// -----------------------------------------------------------------------------
typedef struct tagBtDataBase  TBtDataBase;

// -----------------------------------------------------------------------------
typedef struct
      {
       TBtDataBase  *db;
       BtMesType     code;
       void         *mesparm;
      } TBtMesData;

// -----------------------------------------------------------------------------
typedef enum
      {
       BT_INFO_OPEN,     // �����㥬� 䠩� �� �����
       BT_INFO_RECLEN,   // ��ᮮ⢥��⢨� ������ ���� ������
       BT_INFO_NOSEG,    // � Btrieve 䠩�� ��� ᥣ����
       BT_INFO_SEGPOS,   // �� ᮮ⢥����� ������ ��� ࠧ��� ᥣ����
       BT_INFO_SEGTYPE,  // ��� ���� �� ᮮ⢥����� ⨯� ����
       BT_INFO_KEYLEN,   // ��ᮮ⢥��⢨� ������ keybuff
       BT_INFO_SEGATTR,
       BT_INFO_FLDSIZE,
       BT_INFO_FLDTYPE
      } BtInfoType;

// -----------------------------------------------------------------------------
typedef struct
      {
       BtInfoType  tp;
       BF         *bf;
       int         key;
       int         seg;

       int         posDic, posFile;
       int         sizeDic, sizeFile;
       int         typeDic, typeFile;

       int         flagsDic, flagsFile;
      } TBtInfoData;

//-------------------------------------------------------------------
_RSLLINK int            BtMesProc    (TBtDataBase *db, BtMesType code, void *data);
_RSLLINK int TUSERCALL  BtDefMesProc (TBtDataBase *db, BtMesType code, void *data);
         int TUSERCALL  BtNullMesProc(TBtDataBase *db, BtMesType code, void *data);

// -----------------------------------------------------------------------------
extern int  RslTrnFlag; // ����ন� RSL_TRN_... 䫠��

// -----------------------------------------------------------------------------
#define  RSL_TRN_ABORT   2    // 0 - �᫨ ����室��� �������� RSL �ண࠭��
                              // � ��砥 ��墠祭���� 䠩��
#define  RSL_TRN_ASC     4    // 1 - �᫨ ����室��� �뤠�� ����� �� ����७��
                              // ����樨 � ��砥 ��墠祭���� 䠩��

// -----------------------------------------------------------------------------
// By default No messages for BEeof, BEnorec, BEdupkey
bool BtScipError    (TBtDataBase *db, int er);
bool BtShowError    (TBtDataBase *db, int er);
void BtShowAllErrors(TBtDataBase *db);

// -----------------------------------------------------------------------------
TBtMesProc  BtSetMesProc(TBtDataBase *db, TBtMesProc newProc);

// -----------------------------------------------------------------------------
RSLLINK int RSLAPI BtGetLastStatus(TBtDataBase *db, BF **lastFile, int *lastOper);

int  BtStatus  (BF *bf, TBtSpec **sp); // sp dynamic alloceted
int  BtStatusEx(BF *bf, TBtSpec **sp, bool needNRec);

_RSLLINK int BtCreateFile(TBtDataBase *db, char *name, void *buf, int len);

// -----------------------------------------------------------------------------
#if defined(SQLBUILD)
    int BtCloneFile(TBtDataBase *db, const char *name, const char *clone, const char *own, bool permanent);
#endif

// -----------------------------------------------------------------------------
_RSLLINK int         BtGetBufSize         (BF *bf, int *keysize, int *nkeys, int *nseg, bool *varLen, TBtSpec **spec);
_RSLLINK BF         *BtOpen               (TBtDataBase *db, char *name, char *own, int mode);

         BF         *BtOpenFileEx         (TBtDataBase *db, size_t objSize, const char *name, const char *own,
                                           int mode, bool useBlob, TBtSpec **spec);
_RSLLINK int         BtOpenEx             (TBtDataBase *db, BF *bf, const char *name, const char *own, int mode);

_RSLLINK int         BtClose              (BF *bf);
_RSLLINK int         BtCloseEx            (BF *bf);
_RSLLINK int         BtSetOwner           (BF *bf, char *own);

         void        BtSetVarlen          (TBtDataBase *db, unsigned varlen);

         int         BtOpGetPos           (BF *bf, BKMHANDLE *position);
         int         BtOpGetDirect        (BF *bf, BKMHANDLE pos, int lock);

RSLLINK  int  RSLAPI BtOpGet              (BF *bf, int oper, int lock);
_RSLLINK int         BtOpUp               (BF *bf, int oper, int varlen);

         int         BtOpDel              (BF *bf);
         void        BtClearError         (BF *bf);
         int         BtBeginTrn           (TBtDataBase *db, int lock);
         int         BtEndTrn             (TBtDataBase *db);
         int         BtAbortTrn           (TBtDataBase *db);

         void        BtTrnParam           (int repCount, int delayTime, int isConcurent);

         int         BtUnlockEx           (BF *bf, int mode, unsigned long addr);


         int         BtPack               (BF *bf, int size);
         int         BtUnPack             (BF *bf);

// -----------------------------------------------------------------------------
BFILE      *BfileOpen             (TBtDataBase *db, char *name, int mode, int keypath);
BFILE      *BfileOpenEx           (TBtDataBase *db, char *name, int mode, int keypath, char *fileName);

void        BfileClose       (BFILE *bfile);
int         BfileGet         (BFILE *bfile, int oper);
int         BfileGetKey      (BFILE *bf, int oper);
int         BfileGetNP       (BFILE *bfile, int oper);
int         BfileDel         (BFILE *bfile);
int         BfileUp          (BFILE *bfile, int oper, int varlen);
int         BfileGetDirect   (BFILE *bfile, BKMHANDLE  pos);
int         BfileGetPos      (BFILE *bfile, BKMHANDLE *pos);

int         BreleasePos      (BFILE *bfile, BKMHANDLE pos);
int         BaddrefPos       (BFILE *bfile, BKMHANDLE pos);

void        BfileSetKeyValue (BFILE *bfile);
void        BfileClear       (BFILE *bfile);

int         BfileReadField   (BFILE *bf, int ind, void *aPtr, unsigned char dType, int bufLen);
int         BfileWriteField  (BFILE *bf, int ind, void *aPtr, unsigned char dType);
int         BfileGetFieldType(BFILE *bf, int ind, int *size, int *len);
int         BfileGetFieldID  (BFILE *bf, char *name);

void           BtDataBaseSetDataDir(TBtDataBase *db, char *dir);
void           ConnectToStrucList  (TBtDataBase *db, BTRSTRUCTURE *str);
BTRSTRUCTURE  *FindStructure       (TBtDataBase *db, char *name);

_RSLLINK BTRSTRUCTURE *StrucLoadEx (TBtDataBase *db, char *name, int rdComment, int persistStr);
_RSLLINK BTRSTRUCTURE *StrucLoad   (TBtDataBase *db, char *name);

/*
   ��� �������� � ᯨ᪥ ����㦥���� �������. �᫨ ��室�� 㢥��稢���
   ���稪 ��뫮�, ���� �������� ���� ����� � ᯨ᮪
*/
_RSLLINK void StrucFree(BTRSTRUCTURE *str);

/*
    �����蠥� ���稪 ��뫮�. �᫨ ���稪 == 0, 㤠��� �����
*/
BFILE      *StrucOpenFile        (TBtDataBase *db,BTRSTRUCTURE *str,int mode,int keypath,const char*name);

/*
   ���뢠�� BFILE � ����頥� ��� � ᯨ᮪ 䠩��� ������ ��������
*/
void        StrucCloseFile       (BFILE *bf);

/*
   ����뢠�� � 㤠��� �� ᯨ᪠ BFILE
*/
int         StrucFindField       (BTRSTRUCTURE *str, const char *field);
int         StrucFindFieldEx     (BTRSTRUCTURE *str, const char *field, int isAnsi);
char       *StrucGetField        (BTRSTRUCTURE *str, int nf);
char       *StrucGetComment      (BTRSTRUCTURE *str, int nf);
char       *StrucGetKeyComment   (BTRSTRUCTURE *str, int nf);

// -----------------------------------------------------------------------------
#ifndef RSL_RMT_PROXY
    RSLLINK int RSLAPI GetBuffSize   (BTRSTRUCTURE *str);
    RSLLINK int RSLAPI GetKeyBuffSize(BTRSTRUCTURE *str);
#endif

_RSLLINK BtrUserFilterType RslSetBtrFilter(BtrUserFilterType newFilter);

_RSLLINK BtrUserFilterStat RslSetBtrFilterStat(BtrUserFilterStat newFilter);

         int         FindFieldOffs        (CNTLIST *lst, long code, int *offset);

_RSLLINK int         BfileCheck           (BF *bf, BTRSTRUCTURE *str, TBtSpec *SP, TBtInfoData *data);

_RSLLINK BF *bshGetFile     (TBtDataBase *db);
TAltColProc bshSetAltColProc(TBtDataBase *db, TAltColProc newProc, void *data);

int BtMkPath(char *buff, const char *name, const char *DataDir, int curDir);

TBindElem *BfileBindField      (BFILE *bf, char *name, void *data, unsigned char dType, int len);
int        BfileUnbindField    (BFILE *bf, TBindElem *obj);
void       BfileUnbind         (BFILE *bf);
void       BfileBindGetUserData(BFILE *bf);
void       BfileBindPutUserData(BFILE *bf);

//-@H------ BEGIN __ BLOB data_________________________________ --------@@-
#ifndef BLOB_DATA_DEFINED
#define BLOB_DATA_DEFINED

    #ifdef _MSC_VER
        #pragma pack (1)
    #endif

    // This structure is written at the begining of BLOB field.
    typedef struct
          {
           unsigned char  state;
           unsigned long  size;            // Total lenght of Blob field
           unsigned char  blobType;        // 1 - if RSL values saved in BLOB
           char           reserved[8];
          } TBlobHeader;

    #ifdef _MSC_VER
        #pragma pack ()
    #endif

    // BLOB states
    enum TBlobState
       {
        BLOB_OK = 1,
        BLOB_DIRTY
       };

    typedef enum
          {
           BLOB_OPEN_READ,
           BLOB_OPEN_WRITE,
           BLOB_OPEN_RDWR,
           BLOB_CLOSED
          } TBlobOpenMode;

    typedef struct tagTBlobControlData
          {
           TBlobOpenMode   openMode;
           TBlobHeader     hd;
           unsigned        bufSize;   // Size of buffer
           char           *bufStart;
           char           *bufLast;   // Last used position in Buffer
           char           *bufPtr;
           unsigned long   offsInBlob;   // Offset in Blob of the first byte in buffer
           bool            isDirty;   // True if Blob was modified
           bool            isEmpty;   // If True the buffer is empty
           bool            newPos;   // If True new position in Blob was set
          } TBlobControlData;

#endif // BLOB_DATA_DEFINED

// -----------------------------------------------------------------------------
void BtInitBlob      (BF *bf);

int  BtOpenBlobEx    (BF *bf, TBlobOpenMode openMode);
int  BtCloseBlobEx   (BF *bf);
int  BtReadBlobEx    (BF *bf, char *buff, int size);
int  BtWriteBlobEx   (BF *bf, char *buff, int size);
int  BtGetBlobSizeEx (BF *bf, unsigned long *size);
int  BtSetBlobPosEx  (BF *bf, unsigned long pos);
int  BtGetBlobPosEx  (BF *bf, unsigned long *pos);
int  BtTruncateBlobEx(BF *bf);
int  BtSetBlobTypeEx (BF *bf, unsigned char tp);

// -----------------------------------------------------------------------------
int BtIPutChunk(
                BF            *bf,
                unsigned long  bias,  // May be BT_NEXT_BIAS or BT_APPEND_BIAS
                void RSL_FAR  *buff,  // Buffer to put chunk data. If NULL use data from TBlobControlData.
                unsigned long  offs,  // Offset of chunk in record
                unsigned long  len    // Lenght of chunk. if 0 than use data from TBlobControlData.
               );

// -----------------------------------------------------------------------------
int BtIGetChunk(
                BF            *bf,
                unsigned long  bias,      // May be BT_NEXT_BIAS
                void RSL_FAR  *buff,      // Buffer to store chunk data. If NULL use data from TBlobControlData.
                unsigned long  offs,      // Offset of chunk in record
                unsigned long  len,       // Lenght of chunk. if 0 then use data
                                          // from TBlobControlData.
                unsigned long *bytesRead  // If true we must read all bytes (len)
             );

// -----------------------------------------------------------------------------
int BtITruncateChunk(BF *bf, unsigned long offs);

//-@@------ END __ BLOB data___________________________________ --------@E-

void RslClearRecord(BTRSTRUCTURE *str, char *buff, int maxsize);

int KeyIterSet(KeyIter *ki, BTRSTRUCTURE *str, int key);

ResBtrKey *KeyIterGet(KeyIter *ki);

void FreePersistStr(TBtDataBase *db);

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif  // __BT_H

/* <-- EOF --> */