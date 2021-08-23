/*************************************************************************
   File Name      : eventlog.h
   Programmer     : Alexey Lebedev

   Creation       : 30 Jan 2002
   Description    : TEventReceiver,
                    TReceiverList, TReceiverCash,
                    TEventLogTitle, TEventLogEntry, TEventLog,
                    TBtrvReceiver,  TBtrvLog,
                    TMesgLogEntry, TBFileLogEntry, TProgramLogEntry

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __EVENTLOG_H
#define __EVENTLOG_H

#include <stdlib.h>

#include "btrv.h"
#include "bdate.h"
#include "objlink.h"
#include "rscdefs.h"
#include "rslist.h"
#include "apptldef.h"

#include "operlog.h"


#if defined(_MSC_VER)
    // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
    #pragma warning(disable : 4251)
    // non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
    #pragma warning(disable : 4275)
#endif


// Forward declaration
class _APPTLEXP TEventLog;
class _APPTLEXP TEventReceiver;
class _APPTLEXP TReceiverCash;


/******************************************************************************
   class TEventReceiver - ���������� �������
******************************************************************************/
class _APPTLEXP TEventReceiver
    {
     friend class TEventLog;
     friend class TReceiverCash;

     //   TEventReceiver(const TEventReceiver&) {}
     //   const TEventReceiver& operator = (const TEventReceiver&) { return *this; }

     public:
       TEventReceiver();
       virtual ~TEventReceiver();

       // ������� ����� � ���������� ������� � ������
       enum
          {
           tableName_size = 21,
           objectID_size  = 41
          };

       // �������� ������� ����� � ���������� ������� � ������
       enum
          {
           fm_Date           = 0x00000001,
           fm_Time           = 0x00000002,
           fm_NumDprt        = 0x00000004,
           fm_ProgID         = 0x00000008,
           fm_UserID         = 0x00000010,
           fm_UserType       = 0x00000020,
           fm_OpCode         = 0x00000040,
           fm_NetAddr        = 0x00000080,
           fm_SAddr          = 0x00000100,
           fm_Table          = 0x00000200,
           fm_ObjType        = 0x00000400,
           fm_ObjID          = 0x00000800,

           fm_recID           = 0x00001000,
           fm_TransComRecID   = 0x00002000,
           fm_Robot           = 0x00004000,
           fm_SysUserName     = 0x00010000,
           fm_SysCompName     = 0x00020000,

           fm_PID             = 0x00040000,

           fm_VarInfo         = 0x00100000,

           fm_FixedPart       = 0x000FFFFF,
           fm_VarPart         = 0xFFF00000
          };

       // ��������� �������
       #include "packpshn.h"
       //   struct FiltrInfo
       //   {
       //      int         mask;
       //      btimestamp  start;
       //      btimestamp  finish;
       //      int16       numDprt;
       //      int16       userID;
       //      int16       appKind;
       //      char        tableName[tableName_size];
       //      int16       objectType;
       //      char        objectID[objectID_size];
       //   };

       // ��������� �������� ���������� �����
       struct _APPTLEXP VarItem
          {
          size_t  size;
          void   *buf;
          };

       // ��������� ������
       struct _APPTLEXP RecordInfo
            {
             long        mask;
             bdate       date;
             btime       time;
             int16       numDprt;
             char        progID;
             int16       userID;
             char        userType;
             int16       opCode;
             uint32      netaddr;          // networkNumber
             char        saddr[6];         // physicalNodeAddress
             char        tableName[tableName_size];
             int16       objectType;
             char        objectID[objectID_size];

             // Kubrin's
             int64_t     recID;

             //AV 24.08.2011
             int32       transComRecID;   //Id �������� ������ �� (������������ ����������)
             //AV 07.11.2011
             int16       robot;           //����� ������-�������������, ��� ������� ������� ������� ��� ����� � ������� ��������
             // 14.12.2017
             char        sysCompName[dNAME_LEN_COMP];  // ��� ����������
             char        sysUserName[dNAME_LEN_USER];  // ��� ������������

             int32       processID;       // ������������� ��������

             // ����������/����������
             RecordInfo();
            ~RecordInfo();

             // ��������� �����������
             RecordInfo(const RecordInfo &);
             const RecordInfo &operator = (const RecordInfo &);

             // �������� ������� � ���������� ����� ������
             bool addVarItem(const void *buf, size_t size);

             // �������� ���������� �����
             void clearVarPart();

             // ���������� ��������� � ���������� �����
             int getVarItems() const
             {
              return nItems;
             }

             // ������ � �������� �� ������
             const struct VarItem &operator [] (int n) const
             {
              return varItem[n];
             }

             // ������ ��������� ����� � ������
             size_t getVarSize() const
             {
              return varSize;
             }

             //RecordInfo  dup() const;
             //void        free();

          private:
             int       nItems;
             size_t    varSize;
             VarItem  *varItem;
             bool      isOwned;
            };
       #include "packpop.h"

       //   // ���������� ��������� �������
       //   bool setFiltr(const FiltrInfo *filtrPtr);
       //   // �������� ������� ��������� �������
       //   bool getFiltr(FiltrInfo *filtrPtr);

       // Overloaded
       // �������� ������
       virtual bool addRecord(RecordInfo &recInfo);

       // �������� ������
       virtual bool retrieveRecord(RecordInfo &recInfo, bool open_xml = false);

       // ����� ������
       virtual bool findRecord(const RecordInfo &recInfo);

       // ������ ����� ������, ������������ ��� �������� ������
       class CheckRecord
           {
            public:
              virtual bool operator() () = 0;
           };

       // ������� ������� � ���������
       virtual bool scanRecord(CheckRecord &checkObj);

       // ��������� �������� ������� �������
       virtual bool areBatchInTransaction();
       virtual bool canRetryBatchInsert();
       virtual void beginBatchInsert();
       virtual void endBatchInsert();
       virtual void abortBatchInsert();

     protected:
       // ������ �������� ������ ��������� �� �������
       typedef TLinkList<TEventReceiver, TEventLog> TReceiverBackLinks;
       TReceiverBackLinks _backLinks;

     private:
       unsigned long  _recvID; // ���������� ������������� ���������

       //Static
       static unsigned long  _globalID;
    };


/******************************************************************************
   class TReceiverList - ������ ������ �� ���������
******************************************************************************/

class _APPTLEXP TReceiverList : public TLinkList<TEventLog, TEventReceiver>
    {
     public:
       typedef TEventReceiver::RecordInfo RecordInfo;

       // �������� ������ �� ��� ���������
       //bool addRecord(RecordInfo &recInfo);

       // ��������� ������ � ���
       //bool putCash(TReceiverCash &cash, const RecordInfo &recInfo);

       //template <class T>
       //bool addRecord(T &eventLog, RecordInfo &recInfo);

       bool addRecord(TEventLog &eventLog, RecordInfo &recInfo);
    };


/******************************************************************************
   class TReceiverLink - ��������� ������ �� ��������
******************************************************************************/

typedef TObjectLink<TEventLog, TEventReceiver> TReceiverLink;


/******************************************************************************
   class TReceiverCash
******************************************************************************/
class _APPTLEXP TReceiverCash
    {
     public:
       TReceiverCash();
      ~TReceiverCash();

    //   typedef TEventLog::TReceiverList    TReceiverList;
       typedef TEventReceiver::RecordInfo  RecordInfo;

       // �������� ������� � ���
       bool addRecord(TEventReceiver &recv, RecordInfo &recInfo);

       // ����� ����
       void resetCash();

       // ���������� ������ �� ����
       void saveCash();

     protected:
       // �������� ������ ��� ������������� ���������
       void resetReceiver(TEventReceiver *recv);

       // ��������� ������ ��� ������������� ���������
       void saveReceiver(TEventReceiver *recv);

     private:
       // ������ ����
       struct TCashItem
            {
             TReceiverLink  _link;
             RecordInfo     _recInfo;
            };

       // ������ ����� ����
       typedef RSListT<TCashItem *>  TEventList;
       TEventList  _events;
    };


/******************************************************************************
   class TEventLogTitle - ��������� ��������� � �������
******************************************************************************/
class _APPTLEXP TEventLogTitle
    {
     friend class TEventLog;

     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       TEventLogTitle();
       ~TEventLogTitle();

     protected:
       // �������������� � RecordInfo � �������
       virtual bool cvtTo(RecordInfo &recInfo) const;
       virtual bool cvtFrom(const RecordInfo &recInfo);
    };


/******************************************************************************
   class TEventLogEntry - ���� � ������ ��� ������ ���������
******************************************************************************/
class _APPTLEXP TEventLogEntry
    {
     friend class TEventLog;

     // �� �������� �����������
     TEventLogEntry(const TEventLogEntry&) {}
     const TEventLogEntry &operator = (const TEventLogEntry&) { return *this; }

     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       TEventLogEntry();
       virtual ~TEventLogEntry();

       // ������� �������
       bool createEvent(int opCode);
       bool createEvent(int opCode, int objType, const char *objID);

       int         getOpCode()  const { return _opCode; }

       int         getObjType() const { return _objType; }
       const char *getObjID()   const { return _objID; }

     protected:
       // �������������� � RecordInfo � �������
       virtual bool cvtTo(RecordInfo &recInfo) const;
       virtual bool cvtFrom(const RecordInfo &recInfo);

     private:
       int   _opCode; // ��� �������

       int   _objType;
       char  _objID[TEventReceiver::objectID_size];
    };


/******************************************************************************
   class TEventLog

   ��������� ������ ������� �� ��� �������������� ���������. ��� ���������
   ������� �� ���������, �� (��������) ������ ���� ����� ����.

   ��� ������/���������� ���������� ����������� �� ���� ������� �����������
   ������ TEventLogTitle. � ��� ����������, ��������� �� ���� �������, -
   ������ �������� TEventLogEntry.
******************************************************************************/
class _APPTLEXP TEventLog
    {
     friend class TReceiverCash;
     friend class TReceiverList;

     // �� �������� �����������
     TEventLog(const TEventLog &) {}
     const TEventLog &operator = (const TEventLog &) { return *this; }

     public:
       typedef TEventReceiver::RecordInfo RecordInfo;

       TEventLog();
       virtual ~TEventLog();

       // ������������ � ������� ��������
       bool attachReceiver(TEventReceiver &recv);

       // ����������� �������� �� �������
       bool detachReceiver(TEventReceiver &recv);

       // �������� ���� � �������
       bool addEntry(int opCodeFirst, int opCodeLast, TEventLogEntry &entry);

       // �������� ������� � ������
       virtual bool addEvent(int opCode);
       virtual bool addEvent(const TEventLogEntry &entry);
       virtual bool addEvent(TEventReceiver &recv, const TEventLogEntry &entry);

       //
       // ������ ������� �� �������
       //

       // ������������ ��������� ������� �� ������ ����������� ������
       // ���������� ��������� �� ����, ���������� ��������� �������
       const TEventLogTitle *retrieveTitle(const RecordInfo &recInfo);

       // ������������ ������� �� ������ ����������� ������
       // ���������� ��������� �� ����, ���������� �������
       const TEventLogEntry *retrieveEvent(const RecordInfo &recInfo) const;

       // ���������� ����, ���������� ��������� �������
       const TEventLogEntry *getLastEntry()
       {
        return _lastEntry;
       }

       // ����������/�������� ��������� �� ������-��������� ���������
       void            setEventTitle(TEventLogTitle *titlePtr) { _titlePtr = titlePtr; }
       TEventLogTitle *getEventTitle()                         { return _titlePtr;     }

     protected:
       // ������ �������������� ����������
       TReceiverList  _receivers;

       // �������� ������
       virtual bool addRecord(TEventReceiver &recv, RecordInfo &recInfo)
       {
        // ������� ������ � ��������
        return recv.addRecord(recInfo);
       }

       // ����������� ������
       virtual bool formRecord(RecordInfo &recInfo, const TEventLogEntry &entry)
       {
        // ����� ���������� ���� ����� ����� ����������
        _lastEntry = NULL;

        // ��������� �� ������� �����
        if(!entry.cvtTo(recInfo))
          return false;

        // ����������� �� ������� �����
        if(_titlePtr && !_titlePtr->cvtTo(recInfo))
          return false;

        return true;
       }

     private:
       // ������� ������ ������ � ������ (TEventLogEntry)
       struct _APPTLEXP TEntryListItem
            {
             int  _first;
             int  _last;
             TEventLogEntry *_entry;

             TEntryListItem(int first, int last, TEventLogEntry &entry) : _first(first), _last(last), _entry(&entry) {}

             bool inRange(int opCode) const
             {
              return _first <= opCode && opCode <= _last;
             }

             bool operator == (const TEntryListItem &x) const
             {
              return x._first == _first && x._last == _last;
             }
            };

       // ������ ������ � ������
       typedef RSListT<TEntryListItem>  TEntryList;
       TEntryList  _entryes;

       // ����, ���������� ��������� �������
       TEventLogEntry  *_lastEntry;

       // ��������� �� ��������� �������
       TEventLogTitle  *_titlePtr;
    };


/******************************************************************************
   class TBtrvReceiver - ���������� �������, ���������� �� � Btrieve-����
******************************************************************************/
class _APPTLEXP TBtrvReceiver : public TEventReceiver
    {
     TBtrvReceiver(const TBtrvReceiver &) {}
     const TBtrvReceiver &operator = (const TBtrvReceiver &) { return *this; }

     public:
       TBtrvReceiver(bool no_xml = false);
      ~TBtrvReceiver();

       // ��������� ��� �������� ���������� ����������� ������
       struct TDataIntegrity
            {
            };

       // ��������� �� ������� �������� �������
       typedef bool (ExRecFiltr)(const void *rbuf, void *userData);

       // ��������� � Btrieve-������
       struct TBFileIntf
            {
             // ���������� ����� ������ Btrieve-�����
             virtual int       verNo() = 0;
             virtual bool      checkBFile(const FileSpecs *fs) = 0;

             virtual bool      createBFile(const char *fname, bool no_xml, int xml_format) = 0;
             virtual BTRVFILE *openBFile(const char *fname, const char *owner, bool no_xml, int xml_format) = 0;
             virtual bool      closeBFile(BTRVFILE *) = 0;

             virtual int       getVarLen(const void *rbuf) = 0;
             virtual int       getVarSize(const TEventReceiver::RecordInfo &rec) = 0;

             // �������� ������ ��������� ���������� (������)
             virtual const char *getSortNames() = 0;
             // �������� ����� �������������� �����
             virtual int       getFldMask() = 0;

             virtual bool      cvtToRec(const void *rbuf, TEventReceiver::RecordInfo &rec) = 0;
             virtual bool      cvtFromRec(void *rbuf, TEventReceiver::RecordInfo &rec) = 0;

             virtual int       getRec(BTRVFILE *, void *rbuf, int rlen, bool open_xml, bool no_xml, int xml_format) = 0;
             virtual bool      insRec(BTRVFILE *, const void *rbuf, int rlen, TEventReceiver::RecordInfo &rec, bool no_xml, int xml_format) = 0;

             // ���������� ������ ������ ������ � ���� �� ���� ��������:
             //    0 - ������ ���������� ����� ������
             //   -1 - ����������� ��������� ������
             virtual bool      setRecBufSize(BTRVFILE *, int mode) = 0;

             // ����� ������
             // ������������ ��������:
             //    0 - �� �������
             //   -1 - ������������ ��������
             // addr - �������, ������� ������
             virtual recpos_t  findRec(BTRVFILE *, const TEventReceiver::RecordInfo &rec) = 0;

             // ������� �������
             virtual recpos_t  scanRec(BTRVFILE *, TEventReceiver::CheckRecord &check) = 0;

             // ���������� ������
             virtual bool      setFiltr(BTRVFILE *, int knum, const TEventReceiver::RecordInfo &top,
                                                              const TEventReceiver::RecordInfo &bot) = 0;
             // �������� ������� ������
             virtual bool      addExFiltr(BTRVFILE *, ExRecFiltr *exFiltr, void *userData) = 0;
             // ������� ������� ������
             virtual bool      delExFiltr(BTRVFILE *, ExRecFiltr *exFiltr, void *userData) = 0;

             //
             // �������� ����������� ������ (data integrity check)
             //
             virtual void     *dic_start(BTRVFILE *) = 0;
             virtual void      dic_end  (void *handle) = 0;
             virtual bool      dic_check(void *handle, const void *rbuf, int rlen, int xml_format) = 0;

             // ��������� �������� ������� �������
             virtual bool      areBatchInTransaction(BTRVFILE *) = 0;
             virtual bool      canRetryBatchInsert(BTRVFILE *) = 0;
             virtual void      beginBatchInsert(BTRVFILE *, bool no_xml) = 0;
             virtual void      endBatchInsert(BTRVFILE *, bool no_xml) = 0;
             virtual void      abortBatchInsert(BTRVFILE *, bool no_xml) = 0;
            };

       // ������� Btrieve-���� (�����������, ���� ����������)
       bool createBFile(const char *fname);

       // ������� Btrieve-����
       bool openBFile(const char *fname, const char *owner = NULL);

       // ������� Btrieve-����
       bool closeBFile();

       // �������� ��������� �� BTRVFILE
       BTRVFILE *getBFile()
       {
        return _bfile;
       }

       // �������� ��������� �� ��������� � Btrieve-�����
       TBFileIntf *getBFileIntf()
       {
        return _intf;
       }

       //AV 29.03.2011
       //�������� ������� ��������������� ���������� ������������ xml, ���������� �� �������� (XMLLOG)
       bool get_no_xml()
       {
        return _no_xml;
       }

       int getXmlFormat() { return _xml_format; }
       void setXmlFormat( int xml_format ) {  _xml_format = xml_format; }
       void initXmlFormat() { _xml_format = XMLLOG_FORMAT2; } 

    // Overloaded
       // �������� ������
       virtual bool addRecord(RecordInfo &recInfo);

       // �������� ������
       virtual bool retrieveRecord(RecordInfo &recInfo, bool open_xml = false);

       // ������ ������ �� ������
       virtual bool getRecord(recpos_t recPos, void **rbuf, int *rlen, bool force_no_xml = false);

       // ����� ������
       virtual bool findRecord(const RecordInfo &recInfo);

       // ������� ������� � ���������
       virtual bool scanRecord(CheckRecord &checkObj);

       // ��������� �������� ������� �������
       virtual bool areBatchInTransaction();
       virtual bool canRetryBatchInsert();
       virtual void beginBatchInsert();
       virtual void endBatchInsert();
       virtual void abortBatchInsert();

     private:
       bool allocBuf(size_t sz);
       void freeBuf();

       TBFileIntf *_intf;
       BTRVFILE   *_bfile;

       void       *_rbuf;
       size_t      _rsize;

       bool        _no_xml;


     //Static members
     public:
       // �������� ��������� � ������
       static void addBFileIntf(TBFileIntf *intf, bool primary = false);
       // ������� ����������� ����� ���������
       static void delBFileIntf(TBFileIntf *intf);
       // AV
       int  _xml_format;

     private:
       // ������ �������������� �����������
       typedef RSListT<TBFileIntf*>  TIntfList;
       static TIntfList  *_interfaces;

       // ������� ���������
       static TBFileIntf  *_primIntf;

     public:
       // ����������������� ����������� �����
       static void initMembers();
    };


/******************************************************************************
   class TBtrvLog - ������������ ���������� ������ �������,
   ���������� ������ ����������.
******************************************************************************/
class _APPTLEXP TBtrvLog : public TEventLog
    {
     public:
       TBtrvLog();
      ~TBtrvLog();

       // ��������/��������� ������������ ������� ������ ����������
       bool loggingInsideTrn(bool enabled);
       bool loggingInsideTrn() const
       {
        return _loggingInsideTrn;
       }

       void setClientID(bclient_h clnt);

     protected:
       // �������� ������
       virtual bool addRecord(TEventReceiver &recv, RecordInfo &recInfo)
       {
        // ���� ������������ ������ ���������� ���������...
        if(_loggingInsideTrn || !_insideTrn)
         {
          // ������� ������ � ��������
          return recv.addRecord(recInfo);
         }

        // ������� ������ � ���
        return _cash.addRecord(recv, recInfo);
       }

       // ����������� ������
       virtual bool formRecord(RecordInfo &recInfo, const TEventLogEntry &entry)
       {
        // �������� ����� �������� ������
        if(!TEventLog::formRecord(recInfo, entry))
          return false;

        if(_insideTrn)
         {
          // ������ ���������� ����������� opCode
          switch(recInfo.opCode)
               {
                case Binsert :
                case Bupdate :
                case Bdelete :
                     recInfo.opCode = (int16)(recInfo.opCode + 1002);
                     break;
               }
         }

        return true;
       }

     private:
       TReceiverCash  _cash;

       bool  _loggingInsideTrn;
       bool  _insideTrn;
       bclient_h  _clnt;

     //Static members
     public:
       // ����������� � ������, ������ � ������������� ����������
       static void trnNotify(int bop, bclient_h clnt, void *parm);

     private:
       // ������ ��������. ������������ ��� ����������� � ������������ �����������
       typedef RSListT<TBtrvLog*>  TBtrvLogList;
       static TBtrvLogList  *_logs;

       // ������ ��������, ����������� ������ ����������
       typedef RSListT<bclient_h>  TClntList;
       static TClntList  *_clients;

       // �������� ������ � ������
       static bool addNotify(TBtrvLog &btrvLog);

       // ������� ������ �� ������
       static bool delNotify(TBtrvLog &btrvLog);

     public:
       // ����������������� ����������� �����
       static void initMembers();
    };


/******************************************************************************
   class TMesgLogEntry - ���� � ������ ��� ������ ��������� ���������
******************************************************************************/
class _APPTLEXP TMesgLogEntry : public TEventLogEntry
    {
     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       TMesgLogEntry();
      ~TMesgLogEntry();

       // ������� �������
       virtual bool createEvent(int opCode, const char *mesgText);
       virtual bool createEvent(int opCode, const char *mesgText, int objType, const char *objID);

       const char *getMesgText() const { return _mesgText; }

     protected:
       // �������������� � RecordInfo � �������
       virtual bool cvtTo(RecordInfo &recInfo) const;
       virtual bool cvtFrom(const RecordInfo &recInfo);

     private:
       char  *_mesgText;
    };

// For backward compatibility
typedef TMesgLogEntry  TTextMsgLogEntry;


/******************************************************************************
   class TBFileLogEntry - ���� � ������ ��� ������ �������� � ������� ��
******************************************************************************/
class _APPTLEXP TBFileLogEntry : public TEventLogEntry
    {
     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       TBFileLogEntry();
      ~TBFileLogEntry();

       // ������� �������
       virtual bool createEvent(int opCode, const char *fileName, const void *recbuf, size_t reclen,
                                const void *saveb, size_t sizeb, int objType, const char *objID);

       const char *getTableName() const { return _tableName; }

       void       *getRecBuf()    const { return _recbuf; }
       size_t      getRecLen()    const { return _reclen; }
       void       *getSaveBuf()   const { return _saveb; }
       size_t      getSaveLen()   const { return _sizeb; }

     protected:
       // �������������� � RecordInfo � �������
       virtual bool cvtTo(RecordInfo &recInfo) const;
       virtual bool cvtFrom(const RecordInfo &recInfo);

     private:
       char  _tableName[_MAX_FNAME + _MAX_EXT];

       void    *_recbuf;
       size_t   _reclen;
       void    *_saveb;
       size_t   _sizeb;
    };


/******************************************************************************
   class TProgramLogEntry - ���� � ������ ��� ������ ��������� � ����� � ���������,
   ������ �� ��������� � �� ������� �����.
******************************************************************************/
class _APPTLEXP TProgramLogEntry : public TEventLogEntry
    {
     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       TProgramLogEntry();
      ~TProgramLogEntry();

       // ������� �������
       virtual bool createEvent(int opCode, int objType);

       bdate       getPgmDate()  const { return _progInfo->date; }
       btime       getPgmTime()  const { return _progInfo->time; }
       uint32      getPgmSize()  const { return _progInfo->size; }
       const char *getPgmPath()  const { return _progInfo->fullPath; }
       uint32      getVersion()  const { return _progInfo->version; }
       uint32      getBuild()    const { return _progInfo->build; }

     protected:
       // �������������� � RecordInfo � �������
       virtual bool cvtTo(RecordInfo &recInfo) const;
       virtual bool cvtFrom(const RecordInfo &recInfo);

     private:
       #include "packpsh1.h"
       struct ProgInfo
            {
             // ������������
             static ProgInfo *createObject();
             static ProgInfo *createObject(const void *varPart, size_t varSize);

             enum { verInfoSize = 12 };

             // ���������
             char        verInfo[verInfoSize];

             // ������
             db_uint32   version; //(verLo verHi)
             db_uint32   build;   //(build patch)

             // ������, ����/�����
             db_uint32   size;
             db_bdate    date;
             db_btime    time;

             // ������ ����
             char        fullPath[1];
            };
       #include "packpop.h"

       ProgInfo   *_progInfo;

       static ProgInfo *_theProgInfo;

     public:
       // ����������������� ����������� �����
       static void initMembers();
    };


/******************************************************************************
   InitEventLog
******************************************************************************/

// ����������������� ���������� ���������
extern "C" void InitEventLog(void);

#endif // __EVENTLOG_H

/* <-- EOF --> */