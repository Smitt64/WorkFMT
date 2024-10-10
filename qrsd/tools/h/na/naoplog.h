/*************************************************************************
   File Name      : naoplog.h
   Programmer     : Alexey Lebedev

   Creation       : 20 Feb 2002
   Description    : TOperLog, TOperLogTitle, TOperLogViewer.

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __NAOPLOG_H
#define __NAOPLOG_H

#include "bedit.h"
#include "btdict.h"
#include "na/naoper.h"
#include "eventlog.h"
#include "na/nadef.h"
#include "na/naoplog_type.h"


// non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#if defined(_MSC_VER)
    #pragma warning(disable : 4275)
#endif


//
// Получить указатель на интерфейс к Btrieve-файлу журнала.
// Номер версии:
//    1 - базовая версия
//    2 - добавена поддержка recID и crc32
//    3 - добавлены: номер филиала, вид объекта, код объекта (строка 41) и ключи
// Возвращает указатель на объект-интерфейс или NULL, если интерфейса с
// запрошенным номером не существует.
//
RSL_CFUNC TBtrvReceiver::TBFileIntf *GetTBFileIntf_OPERLOG(int vernum);


/******************************************************************************
   class TOperLog
******************************************************************************/
class _NALGEXP TOperLog : public TOperLog_type
    {
       friend class TOpLogViewer;
       friend class TRslOperLog;

     public:
       TOperLog(TEventLogTitle *titlePtr);
       ~TOperLog();

       using TOperLog_type::opType;

       // Открыть журнал. Файл журнала должен существовать.
       // Возвращает указатель на BTRVFILE или NULL, если произошла ошибка.
       BTRVFILE *open(const char *fname, const char *owner = NULL);
       BTRVFILE *open(const char *fname, const char *owner, bool no_xml, int xml_format = XMLLOG_FORMAT2 ); //AV

       // Открыть файл журнала по заданному указателю в R/O режиме в предположении,
       // что журнал в 3-м формате
       int openRO(BTRVFILE **bf, const char *fname, const char *owner = NULL);

       // Создать новый журнал и открыть его. Если файл журнала существует пересоздать его.
       // Возвращает указатель на BTRVFILE или NULL, если произошла ошибка.
       BTRVFILE *create(const char *fname);
       BTRVFILE *create(const char *fname, bool no_xml ); //AV 30.03.2011 add prototype with "no_xml"

       // Закрыть журнал.
       // Возвращает код ошибки или 0, если операция выполнена без ошибок.
       // Параметры:
       //    BTRVFILE *bf - указатель на открытый файл журнала или NULL, если
       //                   необходимо закрыть все открытые журналы
       int close(BTRVFILE *bf = NULL);

       // Закрыть файл журнала, открытый с помощью openRO (и только для нее !!!)
       int closeRO(BTRVFILE **bf);

       // Типы изображений записи для операций с типом BFile_type
       enum
          {
           PreImage    = 001,
           PostImage   = 002,
           BothImage   = PreImage | PostImage
          };

       // Добавить пользовательскую операцию с типом: Mesg_type или Program_type
       bool addUserOp(int opCode, int opType, const char *opName);

       // Добавить пользовательскую операцию с типом BFile_type
       bool addUserOp(int opCode, const char *opName, int imageType);

       // Добавить пользовательскую операцию нестандартного типа (UserDefined_type).
       bool addUserOp(int opCode, const char *opName, TEventLogEntry &entry);

       // Записать операцию в журнал.
       // Возвращает код ошибки или 0, если операция выполнена без ошибок.
       // Параметры:
       //    BTRVFILE *bf - указатель на открытый файл журнала или NULL, если
       //                   операция записывается во все открытые журналы
       //    opCode      - код операции
       //    description - текстовое описание операции
       //    bop         - код операции Btrieve
       //    opfile      - Btrieve-файл, с которым выполнялась операциЯ
       //    saveb       - буфер с записью до операции
       //    sizeb       - размер буфера saveb
       //    objType     - целочисленный вид объекта
       //    objID       - строка длиной до 40 символов - код объекта
       //
       //  Mesg_type
       int write(BTRVFILE *bf, int opCode, const char *description);
       int write(BTRVFILE *bf, int opCode, const char *description, int objType, const char *objID);

       //  BFile_type
       int write(BTRVFILE *bf, int bop, BTRVFILE *opfile, void *saveb, int sizeb);
       int write(BTRVFILE *bf, int bop, BTRVFILE *opfile, void *saveb, int sizeb, int objType, const char *objID);

       //  Program_type and UserDefined_type
       int write(BTRVFILE *bf, int opCode);

     protected:
       // Получить название операции по ее коду
       const char *getOpName(int opCode);

       // Найти вход для операции по ее коду и типу
       TEventLogEntry *getOpEntry(int opCode, int opType);

       // Получить тип операции по ее коду
       int getOpType(int OpCode);

       // Найти вход для операции по ее коду (без котроля типа)
       TEventLogEntry *getOpEntry(int opCode);

       // Получить массив кодов пользовательских операций
       // После использования массив надо удалить оператором delete.
       int *getUserOps(size_t &sz);

       // Найти приемник
       TBtrvReceiver *findBtrvRecv(BTRVFILE *bf);
       TBtrvReceiver *findBtrvRecv(const char *fname);

       // Добавить событие в журнал, связанный с файлом.
       // Если bf == NULL, во все подсоединенные приемники
       bool addEvent(BTRVFILE *bf, TEventLogEntry &entry);

       // Добавить событие в конкретный приемник
       bool addEvent(TBtrvReceiver *recv, TEventLogEntry &entry);

       // Получить указатель на журнал
       const TEventLog *getEventLog() const { return _btrvLog; }

       // Список пользовательских операций
       struct _NALGEXP TUserOpItem
            {
             int   _opCode;
             int   _opType;
             int   _imageType;

             const char      *_opName;
             TEventLogEntry  *_entry;

             TUserOpItem(int opCode)
             {
              _opCode = opCode;
             }

             TUserOpItem(int opCode, int opType, const char *opName)
             {
              _opCode = opCode;
              _opType = opType;
              _opName = opName;
             }

             bool operator == (const TUserOpItem &x) const
             {
              return _opCode == x._opCode;
             }
            };

       typedef RSListT<TUserOpItem>  TUserOpList;
       TUserOpList  _userOps;

       // Поддерживает ли операция с типом BFile_type изображение записи ДО операции
       virtual bool isPreImage(int opCode);

       // Тип изображения записи для операции с типом BFile_type
       int getImageType(int opCode);

     private:
       // Уведомление о начале, откате и подтверждении транзакции
       static void trnNotify(int bop, bclient_h clnt, void *parm);

       // Список приемников
       typedef RSListT<TBtrvReceiver *>  TBtrvRecvList;
       TBtrvRecvList  _receivers;

       // Журнал
       TBtrvLog          *_btrvLog;

       // Входы в журнал
       TMesgLogEntry     *_mesgEntry;
       TBFileLogEntry    *_bfileEntry;
       TProgramLogEntry  *_programEntry;

       // Резерв
       char  unused[660];
    };


/******************************************************************************
   class TOpLogTitle
******************************************************************************/
class _NALGEXP TOpLogTitle : public TEventLogTitle
    {
     public:
     // Overloaded
       // Вывести событие в поток
       //virtual void printEvent(RSL_STD::ostream &os);

       bdate          getDate()        const { return _date; }
       btime          getTime()        const { return _time; }
       char           getProdID()      const { return _progID; }
       int            getUserID()      const { return _userID; }
       char           getUserType()    const { return _userType; }
       unsigned long  getNetAddr()     const { return _netaddr; }
       const char    *getSAddr()       const { return _saddr; }
       const char    *getSysCompName() const { return _sysCompName; }
       const char    *getSysUserName() const { return _sysUserName; }

     protected:
       // Преобразование в RecordInfo и обратно
       virtual bool cvtTo(RecordInfo &recInfo) const;
       virtual bool cvtFrom(const RecordInfo &recInfo);

     private:
       bdate _date;
       btime _time;

       char  _progID;
       int   _userID;
       char  _userType;

       unsigned long  _netaddr;
       char           _saddr[6];
       char           _sysCompName[dNAME_LEN_COMP];  // Имя компьютера
       char           _sysUserName[dNAME_LEN_USER];  // Имя пользователя

    };


/******************************************************************************
   template <class Type> class TKeyProc
   Шаблонный класс для использования функции-члена некоторого класса
   в качестве клавиатурного обработчика.
******************************************************************************/

template <class Type>
class TKeyProc
    {
     public:
       // Указатель на функцию обработчик клавиатуры
       typedef int (Type::*procKey_t)(PANEL *pnl, int key);
       // Указатель на функцию синхронизатор записей
       typedef void (Type::*syncRow_t)(SCROL *scr);
       // Указатель на функцию
       typedef int (Type::*makeRow_t)(BSCROL *scr);

       // ----------------------------------------
       TKeyProc()
       {
        _procPtr = NULL;
        _syncPtr = NULL;
        _makePtr = NULL;
       }

       // ----------------------------------------
       procKey_t set(procKey_t procPtr)
       {
        procKey_t  curProc = _procPtr;
        _procPtr = procPtr;

        return curProc;
       }

       // ----------------------------------------
       syncRow_t set(syncRow_t syncPtr)
       {
        syncRow_t  curSync = _syncPtr;
        _syncPtr = syncPtr;

        return curSync;
       }

       // ----------------------------------------
       makeRow_t set(makeRow_t makePtr)
       {
        makeRow_t  curMake = _makePtr;
        _makePtr = makePtr;

        return curMake;
       }

       // ----------------------------------------
       static int BsProc(PANEL *pnl, int key)
       {
        BSCROL  *bs = (BSCROL *)pnl->Parm;

        Type  *object = (Type *)bs->Mp.Parm;
        procKey_t  procPtr = ((TKeyProc<Type> *)object)->_procPtr;

        if(procPtr)
          return (object->*procPtr)(pnl, key);

        return key == ESC ? -key : key;
       }

       // ----------------------------------------
       static int PanProc(PANEL *pnl, int key)
       {
        Type  *object = (Type *)pnl->Parm;
        procKey_t  procPtr = ((TKeyProc<Type> *)object)->_procPtr;

        if(procPtr)
          return (object->*procPtr)(pnl, key);

        return key == ESC ? -key : key;
       }

       // ----------------------------------------
       static int ScrolProc(PANEL *pnl, int key)
       {
        SCROL  *scr = (SCROL *)pnl->Parm;

        Type  *object = (Type *)scr->Mp.Parm;
        procKey_t  procPtr = ((TKeyProc<Type> *)object)->_procPtr;

        if(procPtr)
          return (object->*procPtr)(pnl, key);

        return key == ESC ? -key : key;
       }

       // ----------------------------------------
       static void SyncRow(SCROL *scr)
       {
        Type  *object = (Type *)scr->Mp.Parm;
        syncRow_t  syncPtr = ((TKeyProc<Type> *)object)->_syncPtr;

        if(syncPtr)
          (object->*syncPtr)(scr);
       }

       // ----------------------------------------
       static int MakeRow(BSCROL *bs)
       {
        Type  *object = (Type *)bs->Mp.Parm;
        makeRow_t  makePtr = ((TKeyProc<Type> *)object)->_makePtr;

        if(makePtr)
          return (object->*makePtr)(bs);

        return 0;
       }

     private:
       procKey_t _procPtr;
       syncRow_t _syncPtr;
       makeRow_t _makePtr;
    };


/******************************************************************************
   template <class Type> class TSetKeyProc
   Класс для изменения текущего обработчика клавиатуры
******************************************************************************/

template <class Type, class Proc>
class TSetKeyProc
    {
     public:
       TSetKeyProc(Type *obj, Proc proc)
       {
        _obj = obj;
        _proc = _obj->set(proc);
       }

      ~TSetKeyProc()
       {
        _obj->set(_proc);
       }

     private:
       Type  *_obj;
       Proc   _proc;
    };


/******************************************************************************
   class TSaveStatLine
******************************************************************************/

class TSaveStatLine
    {
     public:
       TSaveStatLine()
       {
        _save = new char[SizeStatLine()];

        SaveStatLine(_save);
       }

       TSaveStatLine(const char *status)
       {
        _save = new char[SizeStatLine()];

        SaveStatLine(_save);
        UseStatLine(status);
       }

      ~TSaveStatLine()
       {
        RestStatLine(_save);

        delete _save;
       }

     private:
       void  *_save;
    };


/******************************************************************************
   struct TOpLogFiltr
******************************************************************************/

struct TOpLogFiltr
     {
      struct FiltrData;
      struct FiltrParm;

      struct ExFiltrIntf;
      class  ExFiltrData;
      struct ExFiltrParm;

      class  BFileFiltr;

      // Структура данных для фильтра
   #include "packpshn.h"
      struct FiltrData
           {
            // Иннициализация фильтра значениями по умолчанию
            void   init() { memset(this, 0, sizeof *this); }

            // Установлен ли фильтр?
            bool   isFiltr() const
            {
             return setDprt || setDate || setTime || setTable || setCode ||
                    setUser || setUserType || setProg || setSAddr || setNetAddr || setTransComRecID || setRobot ||
                    setSysCompName || setSysUserName || setRecID || setObjectType;
            }

            size_t calcSize() const { return sizeof *this; }
            void   saveData(void *buf);
            void   restoreData(const void *buf);

            // Номер ключа для сортировки
            int      keyNum;

            // Текущая дата
            bdate    curdate;

            // Фильтр по филиалу
            char     setDprt;
            int16    numDprt;

            // Фильтр по идентификатору записи
            char     setRecID;
            int64_t  recID;

            // Фильтр по дате
            char     setDate;
            bdate    date1, date2;

            // Фильтр по времени
            char     setTime;
            btime    time1, time2;

            // Фильтр по имени файла
            char     setTable;
            char     tableName[TEventReceiver::tableName_size];

            // Фильтр по коду операции
            char     setCode;
            int16    opCode;

            // Фильтр по идентификатору пользователя
            char     setUser;
            int16    userID;

            // Фильтр по типу пользователя
            char     setUserType;
            char     userType;

            // Фильтр по идентификатору приложения
            char     setProg;
            char     progID;

            // Фильтр по сетевому адресу станции
            char     setSAddr;
            char     saddr[6];

            // Фильтр по IP-адресу станции
            char     setNetAddr;
            uint32   netaddr;

            // Фильтр по объекту
            char     setObject;
            char     objectID[TEventReceiver::objectID_size];

            // Фильтр по Id входящей записи транспортной компоненты (ТК)
            char     setTransComRecID;
            int32    transComRecID;

            // Фильтр по номеру робота-операционста, под которым запущен процесс на СП, для связи с внешней системой
            char     setRobot;
            int16    robot;

            // Фильтр по имени компьютера
            char     setSysCompName;
            char     sysCompName[dNAME_LEN_COMP];  // Имя компьютера

            // Фильтр по имени пользователя
            char     setSysUserName;
            char     sysUserName[dNAME_LEN_USER];  // Имя пользователя

            // Фильтр по PID
            char     setPID;
            int32    processID;

            // Фильтр по типу объекта
            char     setObjectType;
            int16    objectType;

           };
   #include "packpop.h"

      // Структура параметров фильтра
      struct _NALGEXP FiltrParm
           {
              FiltrParm(BSCROL *bs, long ommitFld, void *saveData);
             ~FiltrParm();

              const char *getFldName  (int num) { return _flds[num].name; }
              const void *getFldValue (int num) { return _flds[num].FValue; }
              coord       getFldLen   (int num) { return _flds[num].l; }
              int         getFldCount ()        { return _nflds; }

              BSCROL     *getBScrol   () const  { return _bs; }
              void       *getFiltrData() const  { return _saveData; }

            private:
              BSCROL  *_bs;
              FIELD   *_flds;
              int      _nflds;
              void    *_saveData;
           };

      // Интерфейс для элемента дополнительного фильтра
      struct ExFiltrIntf
           {
            virtual ~ExFiltrIntf() {};

            virtual int    getOpCode() const = 0;
            virtual char  *getHitStr() const = 0;
            virtual bool   checkRecord(const TEventLogEntry *entry) = 0;
            virtual char  *getFiltrSql( int xmlFormat ) = 0;

            // Сохранение/восстановление данных фильтра
            virtual size_t calcSize() = 0;
            virtual void   saveData(void *buf) = 0;
            virtual void   restoreData(const void *buf) = 0;
           };

      // Структура данных для дополнительного фильтра
      class _NALGEXP ExFiltrData : public RSListT<ExFiltrIntf*>
          {
             ExFiltrData(const ExFiltrData &) {}
             const ExFiltrData operator = (const ExFiltrData &) { return *this; }

           public:
             ExFiltrData() {}
            ~ExFiltrData() { clear(); }

             void clear();
          };

      // Структура параметров дополнительного фильтра
      struct ExFiltrParm
           {
            int    _opCode;
            void  *_filtrInft;
           };

      // Структура параметров фильтра для операций с Btrieve-файлом
      class _NALGEXP BFileFiltr : public ExFiltrIntf
          {
           public:
             BFileFiltr() {}
             BFileFiltr(int opCode, const char *tableName, bool no_xml);

             virtual ~BFileFiltr();

             bool           get_no_xml()   const { return _no_xml; }
             int            getOpCode()    const { return _opCode; }
             const char    *getTableName() const { return _tableName; }

             // Overloaded
             virtual char  *getHitStr() const;
             virtual bool   checkRecord(const TEventLogEntry *entry);
             virtual char  *getFiltrSql( int xmlFormat );

             virtual size_t calcSize();
             virtual void   saveData(void *buf);
             virtual void   restoreData(const void *buf);

             bool           addItem(short offset, uchar vtype, int vpoint);
             bool           removeItem(short offset);

             bool           inputTable(const char *title, const char *message, const char *status = NULL, int *do_key = NULL);

             const char    *makeStr(short offset, int image, char *buf, size_t sz);
             bool           editItem(short offset, coord fldlen, int image);

           private:
             int   _opCode;
             char  _tableName[TEventReceiver::tableName_size];
             bool  _no_xml;

             class _NALGEXP TFiltrItem
                 {
                    short    offset;
                    uchar    vtype;
                    int      vpoint;
                    int      condition;

                    size_t   fsize;   // размер поля
                    size_t   vsize;   // размер значения (двойной размер поля)
                    void    *value;
                    void    *value2;

                    enum Condition { Equal = 1, NotEqual, Less, Great };

                    bool     _no_xml;

                  public:
                    TFiltrItem();
                    TFiltrItem(const TFiltrItem &x);

                    TFiltrItem(short offset, uchar vtype = -1, int vpoint = 0, bool no_xml = false);
                   ~TFiltrItem();

                    bool operator == (TFiltrItem x) const
                    {
                     return offset == x.offset;
                    }

                    const char *make_str(int image, char *buf, size_t sz);
                    bool        edit_item(coord fldlen, int image);
                    bool        check_record(void *rbuf, int rlen, void *sbuf, int slen);
                    short       get_offset() { return offset; }

                           size_t calc_size();
                    static size_t save_data(const TFiltrItem *item, char *ptr);
                    static size_t rest_data(TFiltrItem *item, const char *ptr);

                    char *getFiltrSql(const char *tableName, const char *fieldName, int type, int opCode, int xmlFormat );
                    char *getFieldCond (const char* str_val, void* v, int type);

                    bool get_no_xml() { return _no_xml; }

                  private:
                    void        init();
                    void       *get_value();
                    const char *get_str(void *v, char *buf, size_t sz);
                    bool        edit_value(coord fldlen, void **pvalue);
                    bool        check_value(void *v, void *rv);
                    int         compare(void *rv, void *sv);

                    char *getFiltrSqlSingleFormat(const char *tableName, const char *fieldName, int type, int opCode, int xmlFormat, EXmlLogFieldType xmlFieldtype );

                    // Обработчик панели редактирования
                    static int BFFLTR_PROC(PANEL *pnl, int key);
                    // Структура параметров обработчика
                    struct BFFLTR_PARM;
                 };

             // Список фильтров по полям файла
             typedef RSListT<TFiltrItem>  TFiltrList;
             TFiltrList  _filtrs;
          };
     };


/******************************************************************************
   class TOpLogViewer
******************************************************************************/

class _NALGEXP TOpLogViewer : public TKeyProc<TOpLogViewer>
    {
       friend class TRslOperLog;

     public:
       typedef TEventReceiver::RecordInfo  RecordInfo;

       typedef TOpLogFiltr::FiltrData      FiltrData;
       typedef TOpLogFiltr::FiltrParm      FiltrParm;

       typedef TOpLogFiltr::ExFiltrIntf    ExFiltrIntf;
       typedef TOpLogFiltr::ExFiltrData    ExFiltrData;
       typedef TOpLogFiltr::ExFiltrParm    ExFiltrParm;

       typedef TOpLogFiltr::BFileFiltr     BFileFiltr;

       TOpLogViewer(TOperLog &oplog);
       TOpLogViewer(TOperLog &oplog, bool no_xml ); // AV
       virtual ~TOpLogViewer();

       void init();//AV 15.04.2011

       bool setDictionary(const char *dictFileList);

       bool setBFile(BTRVFILE *bfile);
       bool setBFile(const char *fname, const char *owner = NULL);

       // Разрешить показ примечания к полю взамен названия
       void useFldCommentAsName(bool enable)
       {
        _useFldComment = enable ? 1 : 0;
       }

       // Получить текущий режим показа  примечания
       bool getFldCommentAsName( void )
       {
          return _useFldComment ? true : false;
       }

       void setTitle(const char *title)
       {
        _title = title;
       }

       void setStatus(const char *status)
       {
        _status = status;
       }

       void setOpenProc(na_openoper_t pOpenO)
       {
        _pOpenO = pOpenO;
       }

       // Разрешить автоматическую установку фильтра по дате
       //    ndays - кол-во дней от текущей даты
       void enableAutoDateFiltr(bool enable, int ndays)
       {
        if(enable && ndays >= 0) _ndays = ndays;
        else                     _ndays = -1;
       }

       // Разрешить автоматическую установку фильтра по Id входящей записи транспортной компоненты (ТК)
       //    TransComRecID - Id входящей записи
       void enableAutoTransComFiltr(bool enable, int TransComRecID )
       {
          if (enable ) _transComRecID = TransComRecID;
          else         _transComRecID = 0;
       }

       // Разрешить автоматическую установку фильтра по objectID и objectType 
       void enableAutoObjectFiltr(bool enable, const char* objectID, int objectType)
       {
        if(enable) 
         {
          strncpy(_objectID, objectID, sizeof(_objectID) - 1)[sizeof(_objectID) - 1] = '\0';
          _objectType = objectType;
         }
        else
         {
          _objectID[0] = '\0';
          _objectType = 0;
         }
       }

       void setXmlOpen() { _xmlopen = true; }
       void unsetXmlOpen() { _xmlopen = false; }
       bool getXmlOpen() { return _xmlopen; }

       void set_no_xml()  { _no_xml = true; }
       void unset_no_xml() { _no_xml = false; }
       bool get_no_xml() { return _no_xml; }

       void setXmlFormat( int format ) {  if( _recv ) _recv->setXmlFormat( format ); }
       int getXmlFormat() {  return _recv ? _recv->getXmlFormat() : -1;  }

       void setViewFiltr() { _viewFiltr = true; }
       void unsetViewFiltr() { _viewFiltr = false; }
       bool getViewFiltr() { return _viewFiltr; }

    //Overloaded
       virtual bool view();
       virtual bool viewTitle();
       virtual bool viewEntry();

       virtual bool print();

    // Statics
       static bool        CheckDictName(const char *tableName);
       static BTFileInfo *GetFileInfo(const char *tableName, long flags = 0,
                                      const void *rbuf = NULL, int rlen = 0);
       static int         MakeDictFldVal(const void *rbuf, size_t offset, char *buf);

     protected:
       // Функции для скроллинга по журналу
       virtual int  make(BSCROL *bs);
       virtual int  proc(PANEL *pnl, int key);

       // Функции для скроллинга по TBFileLogEntry
       virtual void syncBFileEntry(SCROL *mn);
       virtual int  procBFileEntry(PANEL *pnl, int key);

       // Подкачка значения поля в скроллинг
       virtual void syncBFileRow(int nrow, FIELD *fld, void *value);
       // Подкачка значения поля из словаря
       virtual void syncBFileDictionary(int nrow, FIELD *fld, void *value);

       // Функции для скроллинга основного фильтра
       virtual void syncFiltr(SCROL *scr);
       virtual int  procFiltr(PANEL *pnl, int key);
       virtual int  procPanFiltr(PANEL *pnl, int key);

       // Функции для скроллинга расширенных фильтров
       virtual void syncExFiltr(SCROL *scr);
       virtual int  procExFiltr(PANEL *pnl, int key);

       // Функции для скроллинга к фильтру по Btrieve-файлу
       virtual void syncBFileFiltr(SCROL *scr);
       virtual int  procBFileFiltr(PANEL *pnl, int key);

       // Функции для дополнительного фильтра
       virtual bool exFiltr(const void *rbuf);
       static  bool ExFiltr(const void *rbuf, void *userData);
       char *exFiltrSql(const void *rbuf);


       // Функции для печати через Rsl
       virtual int  msgProc(int message, void *ptr, void *userData);
       static  int  MsgProc(int message, void *ptr, void *userData);

       // Подкачка полей из make'а
       virtual void getOpName     (int opCode, char *buf, size_t sz);
       virtual void getUserName   (int userID, char *buf, size_t sz);
       virtual void getUserType   (char userType, char *buf, size_t sz);
       virtual void getProgName   (char progID, char *buf, size_t sz);
       virtual void getDprtName   (int numDprt, char *buf, size_t sz);
       virtual void getTableDesc  (const char *tableName, char *buf, size_t sz);
       virtual void getObjDesc    (int objectType, char *buf, size_t sz);
       virtual void getSAddr      (const char *saddr, char *buf);
       virtual void getNetAddr    (unsigned long netaddr, char *buf);
       virtual void getSysCompName(const char *compName, char *buf, size_t sz);
       virtual void getSysUserName(const char *userName, char *buf, size_t sz);

       // Ввод значения для поиска/фильтра
       virtual bool inputDprt         (PANEL *pnl);
       virtual bool inputRecID        (PANEL *pnl);
       virtual bool inputDate         (PANEL *pnl);
       virtual bool inputTime         (PANEL *pnl);
       virtual bool inputTable        (PANEL *pnl);
       virtual bool inputTableDesc    (PANEL *pnl);
       virtual bool inputOpName       (PANEL *pnl);
       virtual bool inputUserID       (PANEL *pnl);
       virtual bool inputUserName     (PANEL *pnl);
       virtual bool inputUserType     (PANEL *pnl);
       virtual bool inputProgID       (PANEL *pnl);
       virtual bool inputSAddr        (PANEL *pnl);
       virtual bool inputNetAddr      (PANEL *pnl);
       virtual bool inputObjectID     (PANEL *pnl);
       virtual bool inputTransComRecID(PANEL *pnl);
       virtual bool inputRobot        (PANEL *pnl);
       virtual bool inputSysCompName  (PANEL *pnl);
       virtual bool inputSysUserName  (PANEL *pnl);
       virtual bool inputPID          (PANEL *pnl);

       // Поиск по полям постоянной части
       virtual int  findDprt         (int           numDprt);
       virtual int  findRecID        (int64_t       recID);
       virtual int  findDate         (bdate         date);
       virtual int  findTime         (btime         time);
       virtual int  findTable        (const char   *tableName);
       virtual int  findTableDesc    (const char   *tableDesc);
       virtual int  findOpCode       (int           opCode);
       virtual int  findUserID       (int           userID);
       virtual int  findUserName     (const char   *userName);
       virtual int  findUserType     (char          userType);
       virtual int  findProgID       (char          userID);
       virtual int  findSAddr        (const char   *saddr);
       virtual int  findNetAddr      (unsigned long netaddr);
       virtual int  findObjectID     (const char   *objectID);
       virtual int  findTransComRecID(int           transComRecID);
       virtual int  findRobot        (int           robot);
       virtual int  findSysCompName  (const char   *CompName);
       virtual int  findSysUserName  (const char   *UserName);
       virtual int  findPID          (int           pid);
       virtual int  findObjectType   (int           objectType);

       // Выбор значения из списка
       virtual bool listDprt      (int          *numDprt);
       virtual bool listOpName    (int          *opCode);
       virtual bool listUserName  (int          *userID);
       virtual bool listUserType  (char         *userType);
       virtual bool listProgName  (char         *progID);
       virtual bool listTableName (char         *tableName);

       // Проверка поля при поиске записи
       virtual bool checkTableDesc();
       virtual bool checkUserName ();

       // Подготовить скроллинг
       virtual BSCROL *createScroll();

       // Настроить поля скроллинга
       virtual bool setFlds (BSCROL *bs);

       // Настроить фильтр для скроллинга
       bool setFiltr (BSCROL *bs);

       // Удалить скроллинг
       virtual void deleteScroll (BSCROL *bs);


       // Просмотр входа
       virtual bool viewMesgEntry();
       virtual bool viewBFileEntry();
       virtual bool viewProgramEntry();

       // Поиск записи в скроллинге
       virtual int  doFind(PANEL *pnl);

       // Установка фильтра на скроллинг
       virtual bool doFiltr(BSCROL *bs);

       // Выбор сортировки
       virtual bool doSortList();

       // Установка параметров расширенного фильтра
       virtual bool doExFiltr();

       // Применить фильтр к скроллингу
       virtual int  doApplyFiltr(int key);

       // Печать записей из скроллинга
       virtual bool doPrint(BSCROL *bs);

       // Контроль целостности
       virtual int  doItegrityCheck(BSCROL *bs);

       // Уведомление о нарушении целостности
       virtual void onIntegrityError(void *rbuf, int rlen, recpos_t rpos);

       TOperLog      &_oplog;
       TBtrvReceiver *_recv;
       BTRVFILE      *_bfile;

       BTRVFILE     **_projFiles;

       // Структура динамически подкачиваемых полей
       struct ScrollFld
            {
             char  dprtName[41];     // название филиала
             char  tableDesc[41];    // описание файла в словаре
             char  opName[41];       // название операции
             char  userName[D_MAX_NAME_OPER]; // имя пользователя
             char  userType[21];     // тип пользователя (уровень доступа)
             char  progName[21];     // название программы
             char  saddr[13];        // адрес станции
             char  netaddr[16];      // IP-адрес станции
             char  sysCompName[dNAME_LEN_COMP];  // Имя компьютера
             char  sysUserName[dNAME_LEN_USER];  // Имя пользователя

             char  objDesc[41];      // описание типа объекта
            };

       // Структура полей для поиска
       struct FindFld
            {
             char  tableDesc[41];             // описание файла в словаре
             char  userName[D_MAX_NAME_OPER]; // имя пользователя
            };

       RecordInfo     _recInfo;
       ScrollFld      _scrFld;

       RecordInfo     _findInfo;
       FindFld        _findFld;

       int            _ndays;
       int32          _transComRecID; //для фильтра по Id входящей записи транспортной компоненты (ТК)

       bool           _xmlopen;

       bool           _no_xml;//AV 05.04.2011 признак работы журнала без xml-строк, в любых режимах XMLLOG

       bool           _viewFiltr;     // показать панель фильтра до запуска скролинга
       int            _objectType;    // для фильтра по типу и по идентификатору объекта
       char           _objectID[41];  

       // Структура параметров для скроллинга по TBFileLogEntry
       struct _NALGEXP BFileParm
            {
               BFileParm(TBFileLogEntry *bfileEntry);
               BFileParm(BFileFiltr     *bfileFiltr);
               ~BFileParm();

               BTFileInfo     *getFileInfo()    const { return _fileInfo; }
               TBFileLogEntry *getBFileEntry()  const { return _bfileEntry; }
               BFileFiltr     *getBFileFiltr()  const { return _bfileFiltr; }

               const char    *getFldName(int nfld)    const { return _fileInfo->Fields[nfld].FldName; }
               const char    *getFldDesc(int nfld)    const { return _fileInfo->Fields[nfld].FldDesc; }
               uchar          getFldType(int nfld)    const { return (uchar)_fileInfo->Fields[nfld].FldType; }
               int            getFldSize(int nfld)    const { return _fileInfo->Fields[nfld].FldSize; }
               int            getFldPoint(int nfld)   const { return _fileInfo->Fields[nfld].FldPoint; }
               coord          getFldLength(int nfld)  const { return _fileInfo->Fields[nfld].FldLength; }
               short          getFldOffset(int nfld)  const { return _fileInfo->Fields[nfld].FldOffset; }
               uchar          getFldHidden(int nfld)  const { return _fileInfo->Fields[nfld].Hidden; }
               int            getNumFields()          const { return _fileInfo->nFields; }

               int            getOpCode()             const
               {
                return _bfileEntry ? _bfileEntry->getOpCode() : (_bfileFiltr ? _bfileFiltr->getOpCode() : -1);
               }

             // Attributes
             private:
               BTFileInfo      *_fileInfo;

               TBFileLogEntry  *_bfileEntry;
               BFileFiltr      *_bfileFiltr;
            };

       BFileParm    *_bfileParm;

       // Просмотр содержимого записи
       virtual bool viewBFileRecord(BFileParm *entryParm, PANEL *pnl);

       // Данные для фильтра
       FiltrData    *_filtrData;

       // Данные для дополнительного фильтра
       ExFiltrData  *_exFiltrData;

       // Структура параметров фильтра
       FiltrParm    *_filtrParm;

       // Структура параметров дополнительного фильтра
       ExFiltrParm  *_exFiltrParm;

       // Применить фильтр к сроллингу
       virtual bool applyFiltr(BSCROL *bs);

       // Сбросить фильтр
       virtual bool resetFiltr();

       virtual void setAutoDateFiltr();

       // Автоматически установить фильтр по TransComRecID (Id Входящей записи транспортной компоненты ТК)
       virtual void setAutoTransComFiltr(); 
  
       // Автоматически установить фильтр по objectType и objectID 
       virtual void setAutoObjectFiltr();

       // Сохранить данные фильтра в динамически выделенном буфере
       void *saveFiltrData(size_t *size_ptr = NULL);

       // Восстановить данные фильтра из буфера
       void restoreFiltrData(const void *buf, size_t size = -1);

       // Добавить расширенный фильтр
       virtual bool addExFiltr();

       // Удалить расширенный фильтр
       virtual bool delExFiltr(size_t numItem);

       // Просмотр элемента дополнительного фильтра
       virtual bool viewExFiltr(size_t numItem);
       virtual bool viewExFiltr(size_t numItem, bool newFldCommentAsName);

       // Добавить расширенный фильтр по Btrieve-файлу
       virtual bool addBFileFiltr();

       // Показать расширенный фильтр по Btrieve-файлу
       virtual bool viewBFileFiltr(BFileParm *bfileParm);
       virtual bool viewBFileFiltr(BFileParm *bfileParm, bool newFldCommentAsName);

       // Структура параметров для печати
       struct PrintParm
            {
             PrintParm(BSCROL *bs) { _bs = bs; }

             BSCROL  *_bs;
            };

       PrintParm  *_printParm;

       // Установить оптимальный размер записи
       virtual void setRecordSize();

     private:
       const char    *_title;
       const char    *_status;
       na_openoper_t  _pOpenO;

       int            _useFldComment;
    };

#endif //__NAOPLOG_H

/* <-- EOF --> */