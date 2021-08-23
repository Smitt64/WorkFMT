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
   class TEventReceiver - получатель событий
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

       // Размеры полей в структурах фильтра и записи
       enum
          {
           tableName_size = 21,
           objectID_size  = 41
          };

       // Значения битовой маски в структурах фильтра и записи
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

       // Структура фильтра
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

       // Структура элемента переменной части
       struct _APPTLEXP VarItem
          {
          size_t  size;
          void   *buf;
          };

       // Структура записи
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
             int32       transComRecID;   //Id входящей записи ТК (транспортной компоненты)
             //AV 07.11.2011
             int16       robot;           //номер робота-операциониста, под которым запущен процесс для связи с внешней системой
             // 14.12.2017
             char        sysCompName[dNAME_LEN_COMP];  // Имя компьютера
             char        sysUserName[dNAME_LEN_USER];  // Имя пользователя

             int32       processID;       // Идентификатор процесса

             // Конструкто/деструктор
             RecordInfo();
            ~RecordInfo();

             // Поддержка копирования
             RecordInfo(const RecordInfo &);
             const RecordInfo &operator = (const RecordInfo &);

             // Добавить элемент в переменную часть записи
             bool addVarItem(const void *buf, size_t size);

             // Очистить переменную часть
             void clearVarPart();

             // Количество элементов в переменной части
             int getVarItems() const
             {
              return nItems;
             }

             // Доступ к элементу по номеру
             const struct VarItem &operator [] (int n) const
             {
              return varItem[n];
             }

             // Размер переменой части в байтах
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

       //   // Установить параметры фильтра
       //   bool setFiltr(const FiltrInfo *filtrPtr);
       //   // Получить текущие параметры фильтра
       //   bool getFiltr(FiltrInfo *filtrPtr);

       // Overloaded
       // Добавить запись
       virtual bool addRecord(RecordInfo &recInfo);

       // Получить запись
       virtual bool retrieveRecord(RecordInfo &recInfo, bool open_xml = false);

       // Найти запись
       virtual bool findRecord(const RecordInfo &recInfo);

       // Объект этого класса, используется для проверки записи
       class CheckRecord
           {
            public:
              virtual bool operator() () = 0;
           };

       // Перебор записей с проверкой
       virtual bool scanRecord(CheckRecord &checkObj);

       // Поддержка пакетной вставки записей
       virtual bool areBatchInTransaction();
       virtual bool canRetryBatchInsert();
       virtual void beginBatchInsert();
       virtual void endBatchInsert();
       virtual void abortBatchInsert();

     protected:
       // Список обратных ссылок приемника на журналы
       typedef TLinkList<TEventReceiver, TEventLog> TReceiverBackLinks;
       TReceiverBackLinks _backLinks;

     private:
       unsigned long  _recvID; // внутренний идентификатор приемника

       //Static
       static unsigned long  _globalID;
    };


/******************************************************************************
   class TReceiverList - список ссылок на приемники
******************************************************************************/

class _APPTLEXP TReceiverList : public TLinkList<TEventLog, TEventReceiver>
    {
     public:
       typedef TEventReceiver::RecordInfo RecordInfo;

       // Добавить запись во все приемники
       //bool addRecord(RecordInfo &recInfo);

       // Поместить запись в кэш
       //bool putCash(TReceiverCash &cash, const RecordInfo &recInfo);

       //template <class T>
       //bool addRecord(T &eventLog, RecordInfo &recInfo);

       bool addRecord(TEventLog &eventLog, RecordInfo &recInfo);
    };


/******************************************************************************
   class TReceiverLink - одиночная ссылка на приемник
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

       // Добавить событие в кэш
       bool addRecord(TEventReceiver &recv, RecordInfo &recInfo);

       // Сброс кэша
       void resetCash();

       // Сохранение данных из кэша
       void saveCash();

     protected:
       // Обнулить данные для определенного приемника
       void resetReceiver(TEventReceiver *recv);

       // Сохранить данные для определенного приемника
       void saveReceiver(TEventReceiver *recv);

     private:
       // Ячейка кэша
       struct TCashItem
            {
             TReceiverLink  _link;
             RecordInfo     _recInfo;
            };

       // Список ячеек кэша
       typedef RSListT<TCashItem *>  TEventList;
       TEventList  _events;
    };


/******************************************************************************
   class TEventLogTitle - заголовок сообщений в журнале
******************************************************************************/
class _APPTLEXP TEventLogTitle
    {
     friend class TEventLog;

     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       TEventLogTitle();
       ~TEventLogTitle();

     protected:
       // Преобразование в RecordInfo и обратно
       virtual bool cvtTo(RecordInfo &recInfo) const;
       virtual bool cvtFrom(const RecordInfo &recInfo);
    };


/******************************************************************************
   class TEventLogEntry - вход в журнал для записи сообщений
******************************************************************************/
class _APPTLEXP TEventLogEntry
    {
     friend class TEventLog;

     // Не подлежит копированию
     TEventLogEntry(const TEventLogEntry&) {}
     const TEventLogEntry &operator = (const TEventLogEntry&) { return *this; }

     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       TEventLogEntry();
       virtual ~TEventLogEntry();

       // Создать событие
       bool createEvent(int opCode);
       bool createEvent(int opCode, int objType, const char *objID);

       int         getOpCode()  const { return _opCode; }

       int         getObjType() const { return _objType; }
       const char *getObjID()   const { return _objID; }

     protected:
       // Преобразование в RecordInfo и обратно
       virtual bool cvtTo(RecordInfo &recInfo) const;
       virtual bool cvtFrom(const RecordInfo &recInfo);

     private:
       int   _opCode; // код события

       int   _objType;
       char  _objID[TEventReceiver::objectID_size];
    };


/******************************************************************************
   class TEventLog

   Выполняет запись событий во все подсоединенные приемники. Для извлечени
   события из приемника, он (приемник) должен быть задан явно.

   Для записи/извлечения информации независящей от типа события использется
   объект TEventLogTitle. А для информации, зависящей от типа события, -
   список объектов TEventLogEntry.
******************************************************************************/
class _APPTLEXP TEventLog
    {
     friend class TReceiverCash;
     friend class TReceiverList;

     // Не подлежит копированию
     TEventLog(const TEventLog &) {}
     const TEventLog &operator = (const TEventLog &) { return *this; }

     public:
       typedef TEventReceiver::RecordInfo RecordInfo;

       TEventLog();
       virtual ~TEventLog();

       // Подсоединить к журналу приемник
       bool attachReceiver(TEventReceiver &recv);

       // Отсоединить приемник от журнала
       bool detachReceiver(TEventReceiver &recv);

       // Добавить вход к журналу
       bool addEntry(int opCodeFirst, int opCodeLast, TEventLogEntry &entry);

       // Добавить событие в журнал
       virtual bool addEvent(int opCode);
       virtual bool addEvent(const TEventLogEntry &entry);
       virtual bool addEvent(TEventReceiver &recv, const TEventLogEntry &entry);

       //
       // Чтение событий из журнала
       //

       // Сформировать заголовок события на основе прочитанной записи
       // Возвращает указатель на вход, содержащий заголовок события
       const TEventLogTitle *retrieveTitle(const RecordInfo &recInfo);

       // Сформировать событие на основе прочитанной записи
       // Возвращает указатель на вход, содержащий событие
       const TEventLogEntry *retrieveEvent(const RecordInfo &recInfo) const;

       // Возвращает вход, содержащий последнее событие
       const TEventLogEntry *getLastEntry()
       {
        return _lastEntry;
       }

       // Установить/Получить указатель на объект-заголовок сообщений
       void            setEventTitle(TEventLogTitle *titlePtr) { _titlePtr = titlePtr; }
       TEventLogTitle *getEventTitle()                         { return _titlePtr;     }

     protected:
       // Список подсоединенных приемников
       TReceiverList  _receivers;

       // Добавить запись
       virtual bool addRecord(TEventReceiver &recv, RecordInfo &recInfo)
       {
        // Добавим запись в приемник
        return recv.addRecord(recInfo);
       }

       // Подготовить запись
       virtual bool formRecord(RecordInfo &recInfo, const TEventLogEntry &entry)
       {
        // После добавления вход может стать невалидным
        _lastEntry = NULL;

        // Зависимая от события часть
        if(!entry.cvtTo(recInfo))
          return false;

        // Независимая от события часть
        if(_titlePtr && !_titlePtr->cvtTo(recInfo))
          return false;

        return true;
       }

     private:
       // Элемент списка входов в журнал (TEventLogEntry)
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

       // Список входов в журнал
       typedef RSListT<TEntryListItem>  TEntryList;
       TEntryList  _entryes;

       // Вход, содержащий последнее событие
       TEventLogEntry  *_lastEntry;

       // Указатель на заголовок журнала
       TEventLogTitle  *_titlePtr;
    };


/******************************************************************************
   class TBtrvReceiver - получатель событий, помещающий их в Btrieve-файл
******************************************************************************/
class _APPTLEXP TBtrvReceiver : public TEventReceiver
    {
     TBtrvReceiver(const TBtrvReceiver &) {}
     const TBtrvReceiver &operator = (const TBtrvReceiver &) { return *this; }

     public:
       TBtrvReceiver(bool no_xml = false);
      ~TBtrvReceiver();

       // Интерфейс для контроля логической целостности данных
       struct TDataIntegrity
            {
            };

       // Указатель на функцию внешнего фильтра
       typedef bool (ExRecFiltr)(const void *rbuf, void *userData);

       // Интерфейс с Btrieve-файлом
       struct TBFileIntf
            {
             // внутренний номер версии Btrieve-файла
             virtual int       verNo() = 0;
             virtual bool      checkBFile(const FileSpecs *fs) = 0;

             virtual bool      createBFile(const char *fname, bool no_xml, int xml_format) = 0;
             virtual BTRVFILE *openBFile(const char *fname, const char *owner, bool no_xml, int xml_format) = 0;
             virtual bool      closeBFile(BTRVFILE *) = 0;

             virtual int       getVarLen(const void *rbuf) = 0;
             virtual int       getVarSize(const TEventReceiver::RecordInfo &rec) = 0;

             // Получить список возможных сортировок (ключей)
             virtual const char *getSortNames() = 0;
             // Получить маску поддерживаемых полей
             virtual int       getFldMask() = 0;

             virtual bool      cvtToRec(const void *rbuf, TEventReceiver::RecordInfo &rec) = 0;
             virtual bool      cvtFromRec(void *rbuf, TEventReceiver::RecordInfo &rec) = 0;

             virtual int       getRec(BTRVFILE *, void *rbuf, int rlen, bool open_xml, bool no_xml, int xml_format) = 0;
             virtual bool      insRec(BTRVFILE *, const void *rbuf, int rlen, TEventReceiver::RecordInfo &rec, bool no_xml, int xml_format) = 0;

             // Установить размер буфера записи в одно из двух значений:
             //    0 - только постоянная часть записи
             //   -1 - максимально возможный размер
             virtual bool      setRecBufSize(BTRVFILE *, int mode) = 0;

             // Найти запись
             // Возвращаемое значение:
             //    0 - не найдено
             //   -1 - недопустимое значение
             // addr - найдено, позиция записи
             virtual recpos_t  findRec(BTRVFILE *, const TEventReceiver::RecordInfo &rec) = 0;

             // Перебор записей
             virtual recpos_t  scanRec(BTRVFILE *, TEventReceiver::CheckRecord &check) = 0;

             // Установить фильтр
             virtual bool      setFiltr(BTRVFILE *, int knum, const TEventReceiver::RecordInfo &top,
                                                              const TEventReceiver::RecordInfo &bot) = 0;
             // Добавить внешний фильтр
             virtual bool      addExFiltr(BTRVFILE *, ExRecFiltr *exFiltr, void *userData) = 0;
             // Удалить внешний фильтр
             virtual bool      delExFiltr(BTRVFILE *, ExRecFiltr *exFiltr, void *userData) = 0;

             //
             // Контроль целостности данных (data integrity check)
             //
             virtual void     *dic_start(BTRVFILE *) = 0;
             virtual void      dic_end  (void *handle) = 0;
             virtual bool      dic_check(void *handle, const void *rbuf, int rlen, int xml_format) = 0;

             // Поддержка пакетной вставки записей
             virtual bool      areBatchInTransaction(BTRVFILE *) = 0;
             virtual bool      canRetryBatchInsert(BTRVFILE *) = 0;
             virtual void      beginBatchInsert(BTRVFILE *, bool no_xml) = 0;
             virtual void      endBatchInsert(BTRVFILE *, bool no_xml) = 0;
             virtual void      abortBatchInsert(BTRVFILE *, bool no_xml) = 0;
            };

       // Создать Btrieve-файл (пересоздать, если существует)
       bool createBFile(const char *fname);

       // Открыть Btrieve-файл
       bool openBFile(const char *fname, const char *owner = NULL);

       // Закрыть Btrieve-файл
       bool closeBFile();

       // Получить указатель на BTRVFILE
       BTRVFILE *getBFile()
       {
        return _bfile;
       }

       // Получить указатель на интерфейс к Btrieve-файлу
       TBFileIntf *getBFileIntf()
       {
        return _intf;
       }

       //AV 29.03.2011
       //получить признак принудительного выключения формирования xml, независимо от настроек (XMLLOG)
       bool get_no_xml()
       {
        return _no_xml;
       }

       int getXmlFormat() { return _xml_format; }
       void setXmlFormat( int xml_format ) {  _xml_format = xml_format; }
       void initXmlFormat() { _xml_format = XMLLOG_FORMAT2; } 

    // Overloaded
       // Добавить запись
       virtual bool addRecord(RecordInfo &recInfo);

       // Получить запись
       virtual bool retrieveRecord(RecordInfo &recInfo, bool open_xml = false);

       // Чтение записи по адресу
       virtual bool getRecord(recpos_t recPos, void **rbuf, int *rlen, bool force_no_xml = false);

       // Найти запись
       virtual bool findRecord(const RecordInfo &recInfo);

       // Перебор записей с проверкой
       virtual bool scanRecord(CheckRecord &checkObj);

       // Поддержка пакетной вставки записей
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
       // Добавить интерфейс в список
       static void addBFileIntf(TBFileIntf *intf, bool primary = false);
       // Удалить добавленный ранее интерфейс
       static void delBFileIntf(TBFileIntf *intf);
       // AV
       int  _xml_format;

     private:
       // Список поддерживаемых интерфейсов
       typedef RSListT<TBFileIntf*>  TIntfList;
       static TIntfList  *_interfaces;

       // Главный интерфейс
       static TBFileIntf  *_primIntf;

     public:
       // Иннициализировать статические члены
       static void initMembers();
    };


/******************************************************************************
   class TBtrvLog - поддерживает отложенную запись событий,
   посылаемых внутри транзакции.
******************************************************************************/
class _APPTLEXP TBtrvLog : public TEventLog
    {
     public:
       TBtrvLog();
      ~TBtrvLog();

       // Включить/Отключить журнализацию событий внутри транзакции
       bool loggingInsideTrn(bool enabled);
       bool loggingInsideTrn() const
       {
        return _loggingInsideTrn;
       }

       void setClientID(bclient_h clnt);

     protected:
       // Добавить запись
       virtual bool addRecord(TEventReceiver &recv, RecordInfo &recInfo)
       {
        // Если журнализация внутри транзакции разрешена...
        if(_loggingInsideTrn || !_insideTrn)
         {
          // Добавим запись в приемник
          return recv.addRecord(recInfo);
         }

        // Добавим запись в кэш
        return _cash.addRecord(recv, recInfo);
       }

       // Подготовить запись
       virtual bool formRecord(RecordInfo &recInfo, const TEventLogEntry &entry)
       {
        // Вызываем метод базового класса
        if(!TEventLog::formRecord(recInfo, entry))
          return false;

        if(_insideTrn)
         {
          // Внутри транзакции увеличиваем opCode
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
       // Уведомление о начале, откате и подтверждении транзакции
       static void trnNotify(int bop, bclient_h clnt, void *parm);

     private:
       // Список журналов. Используется для уведовлении о происходящих транзакциях
       typedef RSListT<TBtrvLog*>  TBtrvLogList;
       static TBtrvLogList  *_logs;

       // Список клиентов, находящихся внутри транзакции
       typedef RSListT<bclient_h>  TClntList;
       static TClntList  *_clients;

       // Добавить объект в список
       static bool addNotify(TBtrvLog &btrvLog);

       // Удалить объект из списка
       static bool delNotify(TBtrvLog &btrvLog);

     public:
       // Иннициализировать статические члены
       static void initMembers();
    };


/******************************************************************************
   class TMesgLogEntry - вход в журнал для записи текстовых сообщений
******************************************************************************/
class _APPTLEXP TMesgLogEntry : public TEventLogEntry
    {
     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       TMesgLogEntry();
      ~TMesgLogEntry();

       // Создать событие
       virtual bool createEvent(int opCode, const char *mesgText);
       virtual bool createEvent(int opCode, const char *mesgText, int objType, const char *objID);

       const char *getMesgText() const { return _mesgText; }

     protected:
       // Преобразование в RecordInfo и обратно
       virtual bool cvtTo(RecordInfo &recInfo) const;
       virtual bool cvtFrom(const RecordInfo &recInfo);

     private:
       char  *_mesgText;
    };

// For backward compatibility
typedef TMesgLogEntry  TTextMsgLogEntry;


/******************************************************************************
   class TBFileLogEntry - вход в журнал для записи операций с файлами БД
******************************************************************************/
class _APPTLEXP TBFileLogEntry : public TEventLogEntry
    {
     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       TBFileLogEntry();
      ~TBFileLogEntry();

       // Создать событие
       virtual bool createEvent(int opCode, const char *fileName, const void *recbuf, size_t reclen,
                                const void *saveb, size_t sizeb, int objType, const char *objID);

       const char *getTableName() const { return _tableName; }

       void       *getRecBuf()    const { return _recbuf; }
       size_t      getRecLen()    const { return _reclen; }
       void       *getSaveBuf()   const { return _saveb; }
       size_t      getSaveLen()   const { return _sizeb; }

     protected:
       // Преобразование в RecordInfo и обратно
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
   class TProgramLogEntry - вход в журнал для записи сообщений о входе в программу,
   выходе из программы и не удачном входе.
******************************************************************************/
class _APPTLEXP TProgramLogEntry : public TEventLogEntry
    {
     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       TProgramLogEntry();
      ~TProgramLogEntry();

       // Создать событие
       virtual bool createEvent(int opCode, int objType);

       bdate       getPgmDate()  const { return _progInfo->date; }
       btime       getPgmTime()  const { return _progInfo->time; }
       uint32      getPgmSize()  const { return _progInfo->size; }
       const char *getPgmPath()  const { return _progInfo->fullPath; }
       uint32      getVersion()  const { return _progInfo->version; }
       uint32      getBuild()    const { return _progInfo->build; }

     protected:
       // Преобразование в RecordInfo и обратно
       virtual bool cvtTo(RecordInfo &recInfo) const;
       virtual bool cvtFrom(const RecordInfo &recInfo);

     private:
       #include "packpsh1.h"
       struct ProgInfo
            {
             // Конструкторы
             static ProgInfo *createObject();
             static ProgInfo *createObject(const void *varPart, size_t varSize);

             enum { verInfoSize = 12 };

             // Сигнатура
             char        verInfo[verInfoSize];

             // ВерсиЯ
             db_uint32   version; //(verLo verHi)
             db_uint32   build;   //(build patch)

             // Размер, дата/времЯ
             db_uint32   size;
             db_bdate    date;
             db_btime    time;

             // Полный путь
             char        fullPath[1];
            };
       #include "packpop.h"

       ProgInfo   *_progInfo;

       static ProgInfo *_theProgInfo;

     public:
       // Иннициализировать статические члены
       static void initMembers();
    };


/******************************************************************************
   InitEventLog
******************************************************************************/

// Иннициализировать внутренние структуры
extern "C" void InitEventLog(void);

#endif // __EVENTLOG_H

/* <-- EOF --> */