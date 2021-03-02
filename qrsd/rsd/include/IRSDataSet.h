//-*--------------------------------------------------------------------------*-
//
// File Name   : irsdataset.h
//
// Copyright (c) 1991 - 2007 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// February 12, 2003  Derkach - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __IRSDATASET_H
#define __IRSDATASET_H

#define FLDNAME_LENGTH  256


typedef enum tagRSDataSetIID
{
   IID_UDATASETINIT = IID_RSUSER+1,
   IID_GROUPDATASET = IID_RSUSER+2,
   IID_RSDATASRC    = IID_RSUSER+3
}
RSDataSetIID;

typedef enum tagTColumnType
{
   ctNullable        = 0x01,
   ctSpecial         = 0x02,
   ctUncached        = 0x04,
   ctCaseSensitive   = 0x08,
   ctReadOnly        = 0x10,
   ctAggregate       = 0x20
} TColumnType;

// описание поля в GetFieldInfo
typedef struct tagColumnInfo 
{
   int      ciOrdinal;
   int      ciType;
   int      ciLength;
   int      ciColtype;
   int      ciPrecision;
   char     ciName [FLDNAME_LENGTH];
   char     ciTitle[FLDNAME_LENGTH];
} ColumnInfo;


enum tagBindDirection
{
   bdToDataset   = 1,
   bdFromDataset = 2
};

#define bdBoth (bdToDataset | bdFromDataset)

// структура биндинга в IRsDataSet
typedef struct tagBindInfo {
   long     biOrdinal;
   long     biType;
   long     biLength;
   long     biOffset;
   char*    biName;
   long     biStatus; 
   long     biDirection;
} TBindInfo;

typedef TBindInfo BindInfo;

// задание метаинформации в пользовательских источниках
typedef struct tagBindDesc{
   int      bdOrdinal;
   int      bdType;
   int      bdLength;
   int      bdColtype;
   void*    bdValue;
   char*    bdName;
} TBindDesc;

// получение данных в поставщике IRsDataSource
typedef struct tagRecordData{
   long     rdOrdinal;
   long     rdType;
   long     rdLength;
   void*    rdValue;
   long*    rdInd;
} TRecordData;

typedef enum tagDsType
{
   DT_UNKNOWN              = 0,
   DT_RSD_CLIENT_STATIC    = 1, // = DT_STATIC_DATASET
   DT_RSD_CLIENT_DYNAMIC   = 2, 
   DT_BTRIEVE              = 3, 
   DT_USER                 = 4, 
   DT_RSD_RECORDSET        = 5, 
   DT_STATIC_DATASET       = 6,
   DT_OFFLINE_DATASET      = 7,
   DT_DYNAMIC_DATASET      = 8,
   DT_FORWARDONLY_DATASET  = 9
} TDsType;

typedef enum tagTDsEvent{
   DS_EVENT_ONCHANGESELECTED   = 100,
   DS_EVENT_ONWILLCHANGERECORD = 101,
   DS_EVENT_ONCHANGERECORD     = 102,
   DS_EVENT_ONCHANGEDATA       = 103,
   DS_EVENT_ONSORTINGDATA      = 104,
   DS_EVENT_ONFILTERINGDATA    = 105,
   DS_EVENT_ONGROUPINGDATA     = 106,
   DS_EVENT_ONRELEASEGROUP     = 107
} TDsEvent;

typedef enum tagTDsMoveOrientation
{
   DS_MOVE_RELATIVE = 1,
   DS_MOVE_ABSOLUTE = 2
} TDsMoveOrientation;

typedef enum tagDsLoadFieldEnum
{
   DS_LOAD_SINGLE,
   DS_LOAD_ALL,
   DS_UNLOAD_SINGLE,
   DS_UNLOAD_ALL
} TDsLoadFieldEnum;

typedef enum tagDsUpdateDirectionEnum
{
   DS_TO_CACHE,
   DS_FROM_CACHE
} TDsUpdateDirection;

typedef enum tagDsRecordChangeType
{
   DS_RCT_UPDATE,
   DS_RCT_ADDNEW,
   DS_RCT_DELETE,
   DS_RCT_CANCEL
} TDsRecordChangeType;

typedef enum tagDsProperty
{
   DS_PROP_CURSOR_TYPE,
   DS_PROP_CURSOR_LOCATION,
   DS_PROP_DSTYPE,
   DS_PROP_ENCODING,
   DS_PROP_SET_CHANGEDATA_EVENT,
   DS_PROP_READONLY,
   DS_PROP_FILTER,
   DS_PROP_SORT,
   DS_PROP_MODE,
   DS_PROP_USERPROP,
   DS_PROP_SELECTED_FIELD, 
   DS_PROP_LEVEL_FIELD,
   DS_PROP_CHILDREN_FIELD,
   DS_PROP_RESET_CACHE,
   DS_PROP_BATCH_UPDATE,
   DS_PROP_RSL_DATASET,
   DS_PROP_DS_STRING_PARM,
   DS_PROP_TEXTBLOB_SIZE,
   DS_PROP_BLOB_MEMSIZE
} TDsProperty;

typedef enum tagDsCursorType
{   
   DS_FORWARD_ONLY = 0,
   DS_SCROLLABLE   = 1
} TDsCursorType;

typedef enum tagDsState
{
   DS_NAVGT_MODE,
   DS_EDIT_MODE,
   DS_INSERT_MODE,
   DS_DELETE_MODE,
   DS_CANCEL_MODE
} TDsState;

typedef enum tagDsFilterMode
{
   DS_FILTER_BLOCK_CHILD = 1,
   DS_FILTER_PASS_PARENT,
   DS_FILTER_PASS_CHILD,

   DS_FILTER_NOMODIFY = 0,
}
TDsFilterMode;

typedef enum tagDsFindMode
{
   DS_FIND_FIRST = 1,
   DS_FIND_LAST,

   DS_FIND_NEXT,
   DS_FIND_NEXT_TO_END,
   DS_FIND_NEXT_FROM_BEGIN,

   DS_FIND_PREV,
   DS_FIND_PREV_TO_BEGIN,
   DS_FIND_PREV_FROM_END
}
TDsFindMode;

typedef enum tagSortOrder
{
   soNone        = 0,
   soAsc         = 1,
   soDesc        = 2
} TSortOrder;

typedef struct tagTKeyInfo
{
   int  keyIndex;
   int  segIndex;
   int  colIndex;
   int  sortOrder; // TSortOrder
} TKeyInfo;

typedef enum DS_OFFCHANGES
{
   DS_OFFCHG_NONE   = 0,
   DS_OFFCHG_INSERT = 1,
   DS_OFFCHG_DELETE = 2,
   DS_OFFCHG_UPDATE = 3,
   DS_OFFCHG_ALL    = 4
} TDsOfflineChanges;

typedef enum tagTDatasetCaps
{
   DS_CAPS_SORTING   = 1,
   DS_CAPS_FILTERING = 2
} TDatasetCaps;

typedef void*   TBookmark;
typedef int     TBindId;

#define PURE_TRUE =0
#define PURE_FALSE 
#define DS_NOT_IMPLEMENTED { return RSL_STAT_NOTIMPL; } 

class IRsDataSource;

class IRsDataSet;

#define IRSDATASETINTF(pure) \
   virtual TRsStat RSCOMCALL Init (TDsType dt, const char* connStr, void* val) pure ;\
   \
   virtual TRsStat RSCOMCALL SetRecordset       (void* pIRS, void* hRS) pure ;\
   virtual TRsStat RSCOMCALL RegisterBinding    (TBindInfo* pBinding, TBindId* id) pure ;\
   virtual TRsStat RSCOMCALL UnRegisterBinding  (TBindId id) pure ;\
   virtual TRsStat RSCOMCALL AddAutoAccessor    (void* pBase, TBindId id) pure ;\
   virtual TRsStat RSCOMCALL RemoveAutoAccessor (void* pBase) pure ;\
   \
   virtual TRsStat RSCOMCALL GetFieldCount  (int* count) pure ;\
   virtual TRsStat RSCOMCALL GetFieldInfo   (int fldNum, ColumnInfo* fldInfo) pure ;\
   virtual TRsStat RSCOMCALL GetRecCount    (int *RecCount) pure ;\
   virtual TRsStat RSCOMCALL GetRecIndex    (int *RecIndex) pure ;\
   virtual TRsStat RSCOMCALL GetFldIndex    (int* fldIndex, const char* name) pure;\
   virtual TRsStat RSCOMCALL GetDatasetType (TDsType *type) pure ;\
   \
   virtual TRsStat RSCOMCALL AddField     (ColumnInfo* fldInfo) pure ;\
   virtual TRsStat RSCOMCALL LoadField    (int fldNum, int opt) pure;\
   virtual TRsStat RSCOMCALL GetProperty  (int dwPropertyID, int tp, void* val) pure ;\
   virtual TRsStat RSCOMCALL SetProperty  (int dwPropertyID, int tp, void* val) pure ;\
   \
   virtual TRsStat RSCOMCALL GetFieldData (int fldNum, int tp, void* val, int len, long* valLen, int CodePage) pure ;\
   virtual TRsStat RSCOMCALL SetFieldData (int fldNum, int tp, void* val, int CodePage) pure ;\
   virtual TRsStat RSCOMCALL SetNull      (int fldNum) pure ;\
   virtual TRsStat RSCOMCALL GetBlobData  (int nOrdinal, void* pData, int* nCount) pure ;\
   virtual TRsStat RSCOMCALL SetBlobData  (int nOrdinal, void* pData, int* nCount) pure ;\
   \
   virtual TRsStat RSCOMCALL First        (bool* bOk ) pure ;\
   virtual TRsStat RSCOMCALL Last         (bool* bOk ) pure ;\
   virtual TRsStat RSCOMCALL Next         (bool* bEof) pure ;\
   virtual TRsStat RSCOMCALL Prev         (bool* bBof) pure ;\
   virtual TRsStat RSCOMCALL Move         (int MoveType, int Pos) pure ;\
   virtual TRsStat RSCOMCALL SetSelected  () pure ;\
   virtual TRsStat RSCOMCALL MoveToSelected (bool* bOk) pure ;\
   \
   virtual TRsStat RSCOMCALL AddNew       () pure ;\
   virtual TRsStat RSCOMCALL Delete       () pure ;\
   virtual TRsStat RSCOMCALL Edit         () pure ;\
   virtual TRsStat RSCOMCALL CancelEdit   () pure ;\
   virtual TRsStat RSCOMCALL Update       () pure ;\
   virtual TRsStat RSCOMCALL UpdateFrom   (void* pBase, TBindId id, TDsUpdateDirection updDirection) pure ;\
   \
   virtual TRsStat RSCOMCALL EndOfFile    (bool* bEof) pure ;\
   virtual TRsStat RSCOMCALL BeginOfFile  (bool* bBof) pure ;\
   virtual TRsStat RSCOMCALL IsNull       (int fldNum, bool* bOk) pure ;\
   \
   virtual TRsStat RSCOMCALL GetBookmark     (TBookmark* bmark) pure ;\
   virtual TRsStat RSCOMCALL AddRefBookmark  (TBookmark bmark) pure ;\
   virtual TRsStat RSCOMCALL ReleaseBookmark (TBookmark bmark) pure ;\
   virtual TRsStat RSCOMCALL MoveToBookmark  (TBookmark bmark) pure ; \
   virtual TRsStat RSCOMCALL Done       () pure ;\
   virtual TRsStat RSCOMCALL LockSync   () pure;\
   virtual TRsStat RSCOMCALL UnlockSync () pure;\
   virtual TRsStat RSCOMCALL Refresh    () pure;\
   \
   virtual TRsStat RSCOMCALL ShowSelectedOnly   (bool bShow) pure;\
   virtual TRsStat RSCOMCALL IsShowSelectedOnly (bool* bShow) pure;\
   \
   virtual TRsStat RSCOMCALL SetHierarchicalFilter (const char* filterStr, TDsFilterMode filterMode) pure;\
   virtual TRsStat RSCOMCALL GetHierarchicalFilter (const char** pfilterStr, TDsFilterMode* filterMode) pure;\
   \
   virtual TRsStat RSCOMCALL SetFilter (const char* sortStr) pure;\
   virtual TRsStat RSCOMCALL GetFilter (const char** pSortStr) pure;\
   \
   virtual TRsStat RSCOMCALL SetHierarchicalSort (const char* sortStr) pure;\
   virtual TRsStat RSCOMCALL GetHierarchicalSort (const char** pSortStr) pure;\
   \
   virtual TRsStat RSCOMCALL SetSort (const char* sortStr) pure;\
   virtual TRsStat RSCOMCALL GetSort (const char** pSortStr) pure;\
   \
   virtual TRsStat RSCOMCALL Find (const char* findCond, TDsFindMode findMode, bool* bRes) pure;\
   virtual TRsStat RSCOMCALL HierarchicalFind (const char* findCond, TDsFindMode findMode, IRsDataSet** pFindedDS, bool* bRes) pure;\
   \
   virtual TRsStat RSCOMCALL GroupByFields (const char* groupBy, IRsDataSet** ppGroupedDS) pure;\
   virtual TRsStat RSCOMCALL GetGroupedDS  (const char** pGroupByStr, IRsDataSet** ppGroupedDS) pure;\
   virtual TRsStat RSCOMCALL ReleaseGroupedDS () pure;\
   virtual TRsStat RSCOMCALL GetFieldDS    (int fldNum, IRsDataSet** ppUniqueDS) pure;\
   \
   virtual TRsStat RSCOMCALL SelectAll       () pure;\
   virtual TRsStat RSCOMCALL DeselectAll     () pure;\
   virtual TRsStat RSCOMCALL InvertSelection () pure;\
   \
   virtual TRsStat RSCOMCALL GetParent (IRsDataSet** ppParentDS) pure; \
   virtual TRsStat RSCOMCALL SetParent (IRsDataSet* pParentDS, TBookmark bmk) pure; \
   virtual TRsStat RSCOMCALL RefreshRecord () pure; \
   \
   virtual TRsStat RSCOMCALL Disconnect      () pure; \
   virtual TRsStat RSCOMCALL Connect         (IRsDataSource* src) pure; \
   virtual TRsStat RSCOMCALL SynchronizeData (bool* bAll) pure; \
   virtual TRsStat RSCOMCALL SetPendingRecFilter (int chgType) pure; \
   virtual TRsStat RSCOMCALL MoveToRecIndex  (int RecIndex, bool* bOk) pure; \
   \
   virtual TRsStat RSCOMCALL GetKeyInfo   (int* nKey, TKeyInfo* pKeyInfo) pure; \
   virtual TRsStat RSCOMCALL SetKey       (int  nKey) pure; \
   virtual TRsStat RSCOMCALL SetKeyFilter (TRecordData* topVal, TRecordData* botVal) pure;\
   \
   virtual TRsStat RSCOMCALL SetSelectedFromProvider () pure;\
   virtual TRsStat RSCOMCALL GetBlobSize  (int nOrdinal, int* size) pure ;\
   \
   virtual TRsStat RSCOMCALL GetDatasetCaps (TDatasetCaps capIndex, long* val) pure;\
   \
   virtual TRsStat RSCOMCALL GetAggregatedFieldsCount (long* count) pure;\
   virtual TRsStat RSCOMCALL GetAggregatedFieldsInfo (const char* name, ColumnInfo* colInfo) pure;

class IRsDataSet: public IRsObj
{
public: 
   IRSDATASETINTF ( PURE_TRUE )
};

typedef struct tagTDsSortInfo
{
   int colNum;
   int sortOrder; // TSortOrder
} TDsSortInfo;

typedef enum tagTCompareOp
{
   opEq = 0,
   opGT = 1,
   opLT = 2,
   opGE = 3,
   opLE = 4
} TCompareOp;

typedef struct tagTFilterInfo
{
   int   colIndex;
   int   compareOp; // TCompareOp
   void* value;
   long  len;
} TFilterInfo;

enum operation
{
   DS_OPERAND, DS_OR, DS_AND, DS_NOT
};

typedef struct tagParserValueDesc
{
   int         nodeType;      // OR, AND, NOT, operand 
   const char* fieldName;   // имя операнда
   void*       value;         // значение, если есть 
   int         valueType;     // тип значения   
   long        valueLen;      // длина значения 
   int         compareOp;     // операция сравнения
} TOperand;

class IParserTree : public IRsObj
{
public:
   virtual TRsStat RSCOMCALL GetParent (IParserTree** pParent) = 0;
   virtual TRsStat RSCOMCALL GetLeft   (IParserTree** pLeft)   = 0;
   virtual TRsStat RSCOMCALL GetRight  (IParserTree** pRight)  = 0;
   virtual TRsStat RSCOMCALL GetValue  (TOperand** op)         = 0;

   virtual bool    RSCOMCALL Execute   (TOperand* ops, int nOp) = 0;
};

#define IRSDATASOURCEINTF(pure) \
   virtual TRsStat RSCOMCALL open  () pure ;\
   virtual TRsStat RSCOMCALL close () pure ;\
   \
   virtual TRsStat RSCOMCALL getFieldInfo  (int fldNum, ColumnInfo* fldInfo) pure ;\
   virtual TRsStat RSCOMCALL getFieldCount (int* count) pure ;\
   virtual TRsStat RSCOMCALL getRecCount   (int *RecCount) pure ;\
   virtual TRsStat RSCOMCALL getRecIndex   (int *RecIndex) pure ;\
   \
   virtual TRsStat RSCOMCALL getProperty   (int dwPropertyID, int tp, void* val) pure ;\
   virtual TRsStat RSCOMCALL setProperty   (int dwPropertyID, int tp, void* val) pure ;\
   \
   virtual TRsStat RSCOMCALL getFieldData  (int fldNum, int tp, void* val, int len, long* valLen, int CodePage) pure ;\
   \
   virtual TRsStat RSCOMCALL first (bool* bOk) pure ;\
   virtual TRsStat RSCOMCALL last  (bool* bOk)  pure ;\
   virtual TRsStat RSCOMCALL next  (bool* bOk)  pure ;\
   virtual TRsStat RSCOMCALL prev  (bool* bOk)  pure ;\
   \
   virtual TRsStat RSCOMCALL endOfFile   (bool* bEof) pure ;\
   virtual TRsStat RSCOMCALL beginOfFile (bool* bEof) pure ;\
   \
   virtual TRsStat RSCOMCALL getBookmark     (TBookmark* bmark) pure ;\
   virtual TRsStat RSCOMCALL addRefBookmark  (TBookmark  bmark) pure ;\
   virtual TRsStat RSCOMCALL releaseBookmark (TBookmark  bmark) pure ;\
   virtual TRsStat RSCOMCALL moveToBookmark  (TBookmark  bmark) pure ;\
   \
   virtual TRsStat RSCOMCALL setFieldData (int fldNum, int tp, void* val, int CodePage) pure ;\
   virtual TRsStat RSCOMCALL setNull (int fldNum) pure ;\
   \
   virtual TRsStat RSCOMCALL addNew     () pure ;\
   virtual TRsStat RSCOMCALL deleteRec  () pure ;\
   virtual TRsStat RSCOMCALL edit       () pure ;\
   virtual TRsStat RSCOMCALL cancelEdit () pure ;\
   virtual TRsStat RSCOMCALL update     () pure ;\
   \
   virtual TRsStat RSCOMCALL getBlobData (int nOrdinal, void* pData, int* nCount) pure ;\
   virtual TRsStat RSCOMCALL setBlobData (int nOrdinal, void* pData, int* nCount) pure ;\
   \
   virtual TRsStat RSCOMCALL getPrimaryKeyInfo (int* keySeg) pure;\
   virtual TRsStat RSCOMCALL getErrorInfo (const char** Info) pure;\
   \
   virtual TRsStat RSCOMCALL getRecord        (TRecordData* Data) pure ;\
   virtual TRsStat RSCOMCALL newRecord        (TRecordData* newData) pure ;\
   virtual TRsStat RSCOMCALL deleteRecord     (TRecordData* deleteData) pure ;\
   virtual TRsStat RSCOMCALL updateRecord     (TRecordData* newData, TRecordData* oldData) pure ;\
   virtual TRsStat RSCOMCALL resolve          (IMarker* marker, IRsObj** obj) pure ;\
   virtual TRsStat RSCOMCALL refresh          () pure ;\
   virtual TRsStat RSCOMCALL addField         (ColumnInfo* fldInfo) pure ;\
   virtual TRsStat RSCOMCALL setSort          (const char* sortStr, bool bHierarhical) pure;\
   virtual TRsStat RSCOMCALL setFilter        (const char* filterStr, int filterMode, bool bHierarhical) pure;\
   virtual TRsStat RSCOMCALL getFilterMode    (int* filterMode) pure;\
   virtual TRsStat RSCOMCALL showSelectedOnly (bool bShow) pure;\
   virtual TRsStat RSCOMCALL refreshRecord    () pure ;\
   virtual TRsStat RSCOMCALL getDefValue      (int fldNum, int tp, void* val, int len, long* valLen) pure ;\
   virtual TRsStat RSCOMCALL moveToRecIndex   (int RecIndex, bool* bOk) pure ;\
   \
   virtual TRsStat RSCOMCALL setSortEx        (TDsSortInfo* sortInfo, int n) pure ;\
   virtual TRsStat RSCOMCALL getSortEx        (TDsSortInfo* sortInfo, int* n) pure ;\
   virtual TRsStat RSCOMCALL setFilterEx      (IParserTree* filterInfo) pure ;\
   virtual TRsStat RSCOMCALL getFilterEx      (IParserTree** filterInfo) pure ;\
   virtual TRsStat RSCOMCALL find             (const char* findStr, TDsFindMode mode, bool* bOk) pure ;\
   virtual TRsStat RSCOMCALL findEx           (IParserTree* findInfo, TDsFindMode mode, bool* bOk) pure ;\
   \
   virtual TRsStat RSCOMCALL getKeyInfo   (int* nKey, TKeyInfo* pKeyInfo) pure; \
   virtual TRsStat RSCOMCALL setKeyFilter (TRecordData* topVal, TRecordData* botVal) pure;\
   virtual TRsStat RSCOMCALL getBlobSize  (int nOrdinal, int* size) pure ;\
   \
   virtual TRsStat RSCOMCALL getProviderCaps (TDatasetCaps capIndex, long* val) pure;\
   \
   virtual TRsStat RSCOMCALL calculate (const char* expression, TRsData* value) pure;\
   virtual TRsStat RSCOMCALL getExpressionInfo (const char* expression, ColumnInfo* ci) pure;\
   virtual TRsStat RSCOMCALL setKey (int key) pure;

class IRsDataSource : public IRsObj
{
public:
   IRSDATASOURCEINTF (PURE_TRUE)
};

class IDataSetInit : public IRsObj
{
public:
   virtual void   RSCOMCALL init (IRsDataSource* pHandler) = 0;
   virtual size_t RSCOMCALL unMarshMessageMt (int cmdId, const char *inBuff, 
      size_t inSize, IChanel *cn, ICallCtx* ctx) = 0;
      
};

#define IUserDataSetInit IDataSetInit

#endif // __IRSDATASET_H