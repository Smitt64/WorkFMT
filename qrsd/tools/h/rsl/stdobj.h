/*-----------------------------------------------------------------------*-

 File Name   : stdobj.h  

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 March 16,1998  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __STDOBJ_H
#define __STDOBJ_H

#include "packpshn.h"

#ifdef __cplusplus
extern "C" {
#endif


// -----------------------------------------------------------------------------
// ����⠭�� ��� 䫠�� TArrayObj::cvtToBinArray
// ���� �����ন������ ⮫쪮 ��������� � BYTE (unsigned char) - VT_UI1
#define  dTARRAY_CVT_TO_BYTE  1

// -----------------------------------------------------------------------------
void AddRsComErrClass(void);

// TArray  class
void  RegisterTArray(void);
void  RslTArray(void);

#ifndef BUILD_DLM
    uintptr_t RslTArrayID(void);

    bool RslTArrayCnvType(TGenObject *obj, bool cnvToSafeAr);
    int  RslTArrayCnvBinType(TGenObject *obj, int cnvToBinAr);
    bool RslTArrayCnvCheckBounds(TGenObject *obj, bool checkBounds);
#else
    uintptr_t DLMAPI RslTArrayID(void);

    bool DLMAPI RslTArrayCnvType(TGenObject *obj, bool cnvToSafeAr);
    int  DLMAPI RslTArrayCnvBinType(TGenObject *obj, int cnvToBinAr);
    bool DLMAPI RslTArrayCnvCheckBounds(TGenObject *obj, bool checkBounds);
#endif          

uintptr_t TComObjWraperCLSID(void);
int GetRcwClassKind(TGenObject *ptr);

DLMLINK TGenObject *RSLDLMAPI RslTArrayCreate(int rootSize, int incSize);
DLMLINK void        RSLDLMAPI RslTArrayPut  (TGenObject *obj, long id, VALUE *val);
DLMLINK VALUE      *RSLDLMAPI RslTArrayGet  (TGenObject *obj, long id);
DLMLINK long        RSLDLMAPI RslTArraySize (TGenObject *obj);
DLMLINK TGenObject *RSLDLMAPI RslIsTArray   (TGenObject *obj);
DLMLINK void        RSLDLMAPI RslTArrayClear(TGenObject *obj);

RSLLINK int         RSLAPI    IsEqClasID    (TGenObject *obj, uintptr_t clsID);
RSLLINK TGenObject *RSLAPI    CastToCLSID   (TGenObject *obj, uintptr_t clsID);

typedef struct tagTWRef
      {
       LOBJ         objElem;  // Obect's list elem
       TGenObject  *weakRef; 
      } TWRef;

int ReleaseWeakRef(TWRef *ptr, void *data);

// -----------------------------------------------------------------------------

#ifndef NO_BTR
    // -------------------------------------------
    // TBfile
    enum TBMetId
       {
        BTR_ID_FLDNUM,      // RH part
        BTR_ID_FLDIND, 
        BTR_ID_FLDNAME,
        BTR_ID_FLDOFFS,
        BTR_ID_RECSZ,  
        BTR_ID_CLEAR,  
        BTR_ID_SETADDR,
        BTR_ID_FLDINFO,
        BTR_ID_KEYINFO,
        BTR_ID_FLDSIZE,

        BTR_ID_NEXT,       // Btr part
        BTR_ID_PREV,   
        BTR_ID_GETPOS, 
        BTR_ID_GETDIR, 
        BTR_ID_INSERT, 
        BTR_ID_UPDATE, 
        BTR_ID_DELETE, 
        BTR_ID_GETEQ,  
        BTR_ID_GETGE,  
        BTR_ID_GETLE,  
        BTR_ID_GETGT,  
        BTR_ID_GETLT,  
        BTR_ID_REWIND, 
        BTR_ID_NREC,   
        BTR_ID_VARSZ,  
        BTR_ID_PACK,   
        BTR_ID_UNPACK, 
        BTR_ID_WRBLOB, 
        BTR_ID_RDBLOB,
        BTR_ID_FLNAME,
        BTR_ID_ADDFLTR,
        BTR_ID_DROPFLTR,
        BTR_ID_CLEARBLOB,
        BTR_ID_REWINDBLOB
       };

    // -------------------------------------------
    enum TBPropId
       {
        BTR_ID_ITEM,       // RH part
        BTR_ID_REC,
        BTR_ID_TBLNAME,
        BTR_ID_DICNAME,
        BTR_ID_CNVMODE,

        BTR_ID_KEY,         // Btr part
        BTR_ID_OPENMODE
       };


    void RegisterTBFile(void);

    void InitWorkTBFile(void);
    void DoneWorkTBFile(void);

    void RslTBFile(void);
    uintptr_t RslTBFileID(void);

    void RslTRecHandler(void);
    RSLLINK uintptr_t RSLAPI RslTRecHandlerID(void);

    void AddDbErrClass(void);
    void SetDbErrObj(int stat, int op, const char *file, const char *mes, const char *extMessage);

    // -------------------------------------------
    typedef struct
          {
           TGenObject      derived;
           LOBJ            elem;
           unsigned short  refCount;

           char            dicname[25];  // ��� � ᫮���
           BTRSTRUCTURE   *str;          // ������� 䠩�� ���樠��������� �� ����室�����

           VALUE           altDic;

           TGenObject     *record;

           void           *buff;
           bool            alloced;
           // add A.Tokmashev (28.06.2000)
           void           *data; // pointer to PANEL
           // end add (28.06.2000)

           int             cnvMode;
          } TRecordHandler;

    // -------------------------------------------
    typedef struct
          {
           const char  *dicname;
           const char  *dic;
           void        *buff;
           int          size;
          } TRecHandlerInfo;

    // -------------------------------------------
    typedef struct
          {
           TGenObject      derived;
           LOBJ            elem;
           unsigned short  refCount;

           long            size;           // ����騩 ࠧ��� ���ᨢ� (���ਬ��, �� ��������� �१ Size)
           long            allocSize;      // ��᫮ �뤥������ ����⮢ ���ᨢ�

           int             rootSize;       // ��砫�� ࠧ��� ���ᨢ�
           int             incSize;        // ������ ��࠭��� ���ᨢ�

           CNTLIST         pageList;       // List of TArrayPage

           bool            cvtToSafeArray;

        #ifndef NO_GC
           unsigned short  generation;     // Used to collect cyclic objects
           unsigned short  marknumber;     // Used to report references
        #endif

           int             cvtToBinArray;  // #3441 (�. dTARRAY_CVT_TO_*)

           bool            checkBounds;    // ����஫� ��室� ������ �� �࠭��� ���ᨢ�
          } TArrayObj;

    // -------------------------------------------
    typedef struct
          {
           TRecordHandler  derived;

           // ----------------------------------------
           VALUE           fileName;

           BFILE          *bfile;

           bool            useBlob;
           int             mode;  // ����� ������
           int             path;  // ���� � ������
          } TBtrieveFile;

    // -------------------------------------------
    // TBfile and TRecord
    TGenObject *MakeTRecordObj(TRecordHandler *parent, BTRSTRUCTURE *str, char *buff);
    void  SetTRecBuff(TGenObject *o, char *buff);

    void DoneTRecordObj(TGenObject *obj);

    #ifdef BUILD_DLM
        TGenObject             *DLMAPI CreateRecordHandler(char *name, char *altDic, void *buff);
    #else
        RSLLINK TRecordHandler *RSLAPI CreateRecordHandler(char *name, char *altDic, void *buff);
    #endif

    // -------------------------------------------------------------------------
    // �������� ��ꥪ� TBFile �� �ਪ������� ���� (�� �������� � CreateRecordHandler).
    // ��ࠬ���� ��������� ��ࠬ��� ��������� TBFile:
    // [in]  structName - ������������ �������� � ᫮���
    // [in]  mode       - ०��
    // [in]  key        - ����� ����
    // [in]  fileName   - ������������ 䨧��᪮�� 䠩��
    // [in]  dicName    - ������������ ᫮����
    // [in]  cloneName  - ������������ १������饣� 䠩�� �� �����஢����
    // [out] stat       - - 1 - �맮� ��� RSL
    //                    - 2 - �訡�� ����祭�� �訡��
    //                    -45 - �訡�� ������祭�� � ᫮����
    //                    -46 - �� ������� ������� � ᫮���
    //                    ������⥫�� ���祭�� ��������� Status
    //
    RSLLINK TBtrieveFile *RSLAPI CreateBtrieveFile(const char *structName, const char *mode, int key, const char *fileName, const char *dicName, const char *cloneName, int *stat);

    RSLLINK int    RSLAPI     IsBtrieveFile       (TGenObject *obj);

    RSLLINK int    RSLAPI     IsRecHandler        (TGenObject *obj);
    DLMLINK int    RSLDLMAPIC GetRecordHandlerInfo(TGenObject *obj, TRecHandlerInfo *data);
    RSLLINK BFILE *RSLAPI     GetFileFromTBFile   (TGenObject *ob);

    void CloseTbfileObjects(void);

    bool RslTArrayMarhshalByVal(TGenObject *obj);

    // ������ getAtId ����� �������� NULL.
    // ���ਬ��, �᫨ ���祭�� id ��室�� �� �࠭��� ��������� ���ᨢ�.
    VALUE *getAtId(TArrayObj *obj, long id);

#endif // NO_BTR

void AddDosFileClass(void);

// -----------------------------------------------------------------------------
// ����⪠ ��� RcwGetIDisp
// ��㣮� ���������� ������� ��� RSL ��������� ����䥩� IDispObj ��� ��ꥪ�
// ���� �� 㤠����.
// ����� IDispObj �����頥� void, ���� �� ������� rscom.hpp.
/*IDispObj*/RSLLINK void *RslRcwGetIDisp(TGenObject *obj);

#ifdef __cplusplus
}
#endif

#include "packpop.h"

#endif  // __STDOBJ_H

/* <-- EOF --> */