//-*--------------------------------------------------------------------------*-
//
// File Name   : unifile.hpp
//
// Copyright (c) 1991 - 2007 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// December 19,2007  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __UNIFILE_HPP
#define __UNIFILE_HPP


// -----------------------------------------------------------------------------
#define  dUF_BUFF_SIZE  2048    // Размер буфера по-умолчанию

// -----------------------------------------------------------------------------
enum UFOpenMode
   {
    UF_READ        = 0,
    UF_WRITE       = 1,
    UF_APPEND      = 2,
    UF_READ_PLUS   = 3   // READ and WRITE after Rewrite.
   };


// -----------------------------------------------------------------------------
enum UFShareMode
   {
    UF_DENYNO   = 0,
    UF_DENYWR   = 1
   };

// -----------------------------------------------------------------------------
enum UFEndOfLineMode
   {
    UF_EOL_DOSWIN   = 0,
    UF_EOL_UNIX     = 1,
    UF_EOL_MAC      = 2,
   };

// -----------------------------------------------------------------------------
enum UFEncodeType
   {
    UF_RSOEM    = 0,
    UF_RSANSI   = 1,
    UF_LCOEM    = 2,
    UF_LCANSI   = 3,
    UF_UTF8     = 4,
    UF_UTF16LE  = 5,
    UF_UTF16BE  = 6,

    UF_USEDEFAULT = 100
   };


#ifdef USE_UFILE

    #ifdef __cplusplus

    // -----------------------------------------------------------------------------
    template<class CharType>
    class _TOOLEXP DynBuffer
        {
         public:
           DynBuffer() : bufSize(0), buffer(0) {}

          ~DynBuffer()
           {
            if(buffer)
              free(buffer);
           }

           CharType *getBuffer(size_t newSz = dUF_BUFF_SIZE) const
           {
            CharType *newBlock = buffer;

            if(newSz > bufSize)
             {
              newBlock = (CharType *)realloc(buffer, newSz*sizeof(CharType));

              bufSize = newSz;

              if(newBlock)
                buffer = newBlock;
             }

            return newBlock;
           }

           size_t getBufferSize() const
           {
            return bufSize;
           }

         private:
           mutable CharType  *buffer;
           mutable size_t     bufSize;
        };

    // -----------------------------------------------------------------------------
    class _TOOLEXP TUniFileBase
        {
         public:

           TUniFileBase(UFEncodeType fileEncode_ = UF_RSOEM, UFEncodeType progEncode_ = UF_RSOEM, UFEndOfLineMode eolMode_ = UF_EOL_DOSWIN);
          ~TUniFileBase();

           bool flush();
           bool commit();
           bool close();
           bool rewind();
           bool reWrite(UFEncodeType encode);

           bool getChar(char *ptr);
           bool putChar(char ch);
           void unGetChar(char ch);

           bool print  (const char *fmt, ...);
           bool println(const char *fmt, ...);
           bool vprint (const char *fmt, va_list args);

           char *readLine();
           bool  writeLine(const char *str);

           int read (char *buff, int num);
           int write(const char *buff, int num);

           virtual bool isOpen() { return openFlag; }

           bool isEof()   { return eofFlag; }
           bool isError() { return errorFlag; }

           UFEncodeType    getFileEncode()    { return fileEncode; }
           UFEncodeType    getProgEncode()    { return progEncode; }
           UFEndOfLineMode getEndOfLineMode() { return eolMode; }

           void setFileEncode   (UFEncodeType enc)     { fileEncode = enc; }
           void setProgEncode   (UFEncodeType enc)     { progEncode = enc; }
           void setEndOfLineMode(UFEndOfLineMode mode) { eolMode = mode; }

           // Do not pass this buffer to print or println functions!
           char *getTmpBuffer(size_t sz = 0)
           {
            if(!sz)
              sz = 256;

            return lineBuffer.getBuffer(sz);
           }

           // ------------------------------------
           bool setAsIsFlag(bool newFlag = true)
           {
            bool  retVal = flagReadAsIs;


            flagReadAsIs = newFlag;

            return retVal;
           }

         protected:
           bool checkReadMode();
           bool checkWriteMode();

           virtual int  readImpl  (unsigned char *buff, int sz) = 0;
           virtual int  writeImpl (const unsigned char *buff, int sz) = 0;
           virtual void closeImpl () = 0;
           virtual bool seekImpl  (int pos, int from) = 0;
           virtual bool truncImpl (long) = 0;
           virtual bool commitImpl() = 0;

           bool openBase(UFOpenMode mode, UFShareMode share, size_t buffSize);

           bool getWChar  (wchar_t *ptr);
           bool putWChar  (wchar_t ch);
           void unGetWChar(wchar_t ch);

           bool getWCharImpl(wchar_t *ptr);
           bool putWCharImpl(wchar_t wch);


           bool getCharImpl(char *ptr);
           bool putCharImpl(char ch);

           bool writeNewLine();

           bool getByte(unsigned char *ptr);
           bool putByte(unsigned char byte);

           bool underflow(unsigned char *ptr);
           bool overflow(unsigned char byte);

           bool readBOM(bool *emptyFile = 0);
           bool writeBOM();

           DynBuffer<unsigned char>   fileBuffer;
           unsigned char  *endPtr;
           unsigned char  *curPtr;

           DynBuffer<char>            lineBuffer;

           UFEncodeType      fileEncode;
           UFEncodeType      progEncode;

           wchar_t           unGetWCharacter;
           char              unGetCharacter;

           bool              eofFlag;
           bool              errorFlag;
           bool              openFlag;

           unsigned          pendigNewLines;
           int               lastNewLineChar;
           UFOpenMode        oMode;
           UFEndOfLineMode   eolMode;

           bool              flagReadAsIs;         // Читать данные "как есть", байт в байт (#2782)
        };

    // -----------------------------------------------------------------------------
    class _TOOLEXP TUniFile : public TUniFileBase
        {
         public:
           TUniFile(UFEncodeType fileEncode = UF_RSOEM, UFEncodeType progEncode = UF_RSOEM, UFEndOfLineMode eolMode_ = UF_EOL_DOSWIN);
          ~TUniFile();

        #ifdef DEBUG
           unsigned long  magic;
        #endif

           bool open(const char *fileName, UFOpenMode mode = UF_READ, UFShareMode share = UF_DENYWR,
                     size_t buffSize = dUF_BUFF_SIZE, int *errcode = NULL);

           int getFileno()
           {
            return fd;
           }

         protected:
           int   fd;

           virtual void closeImpl ();
           virtual int  readImpl  (unsigned char *buff, int sz);
           virtual int  writeImpl (const unsigned char *buff, int sz);
           virtual bool seekImpl  (int pos, int from);
           virtual bool truncImpl (long);
           virtual bool commitImpl();
        };

    #endif // __cplusplus

#endif // UNI_FILE

// -----------------------------------------------------------------------------
#define  TXT_ENCODE_OUT  "TXT_ENCODE_OUT"
#define  TXT_ENCODE_IN   "TXT_ENCODE_IN"
#define  TXT_ENCODE_PRN  "TXT_ENCODE_PRN"

#define  RSANSI_TXT   "rsansi"
#define  LCANSI_TXT   "lcansi"
#define  RSOEM_TXT    "rsoem"
#define  LCOEM_TXT    "lcoem"
#define  UTF8_TXT     "utf8"
#define  UTF16LE_TXT  "utf16le"
#define  UTF16BE_TXT  "utf16be"


#ifdef __cplusplus
extern "C" {
#endif

_TOOLEXP FILE *ufOpen   (const char *fileName, int openMode, int fileEncode, int progEncode, int shareMode);
_TOOLEXP FILE *ufOpenEx (const char *fileName, int openMode, int fileEncode, int progEncode, int shareMode, int eol);
_TOOLEXP FILE *ufOpen2  (const char *fileName, int openMode, int fileEncode, int progEncode, int shareMode, int *errcode);
_TOOLEXP FILE *ufOpenEx2(const char *fileName, int openMode, int fileEncode, int progEncode, int shareMode, int eol, int *errcode);
_TOOLEXP void  ufClose       (FILE *fl);
_TOOLEXP int   ufGetch       (FILE *fl);
_TOOLEXP void  ufUnGetch     (FILE *fl, char ch);
_TOOLEXP bool  ufPutch       (FILE *fl, char ch);
_TOOLEXP void  ufFlush       (FILE *fl);
_TOOLEXP int   ufWrite       (FILE *fl, const char *buff, int sz);
_TOOLEXP int   ufRead        (FILE *fl, char *buff, int sz);
_TOOLEXP char *ufReadLine    (FILE *fl);
_TOOLEXP bool  ufWriteLine   (FILE *fl, const char *line);
_TOOLEXP bool  ufRewind      (FILE *fl);
_TOOLEXP bool  ufReWrite     (FILE *fl, int encode);
_TOOLEXP char *ufGetTmpBuffer(FILE *fl, size_t sz);
_TOOLEXP int   ufPrintf      (FILE *fl, const char *fmt, ...);
_TOOLEXP void  ufSetEncode   (FILE *fl, int enc);
_TOOLEXP int   ufGetEncode   (FILE *fl);
_TOOLEXP bool  ufError       (FILE *fl);
_TOOLEXP int   ufFileno      (FILE *fl);
_TOOLEXP bool  ufSetAsIsFlag (FILE *fl, bool newFlag);

_TOOLEXP int   ufGetEncodeFromStr (const char *encStr);
_TOOLEXP int   GetDefTextEncodeIn (void);
_TOOLEXP int   GetDefTextEncodeOut(void);

// -----------------------------------------------------------------------------
#ifdef USE_UFILE
    _TOOLEXP FILE *fopen_rs (const char *fileName, const char *mode);
    _TOOLEXP int   access_rs(const char *fileName, int mode);
    _TOOLEXP int   access_s_rs(const char *fileName, int mode);
    _TOOLEXP int   sopen_rs (const char *fileName, int oflag, int shareFlag, int pmode);
    _TOOLEXP FILE *fdopen_rs(int fd, const char *mode);
    _TOOLEXP int   unlink_rs(const char *fileName);
    _TOOLEXP int   rename_rs(const char *from, const char *to);

    _TOOLEXP const char *getenv_rs(const char *key);
    _TOOLEXP int   putenv_rs(const char *val);
    _TOOLEXP char *fullpath_rs(char *absPath, const char *relPath, size_t maxLen);
    _TOOLEXP int   stat_rs(const char *path, struct _stat *buffer);

    #if _MSC_VER >= 1400
        _TOOLEXP int   stati64_rs(const char *path, struct _stati64 *buffer);
        _TOOLEXP int   stat32i64_rs(const char *path, struct _stat32i64 *buffer);
    #else
        #define  stati64_rs     stat_rs
        #define  stat32i64_rs   stat_rs
    #endif

    _TOOLEXP int   utime_rs(const char *path, struct _utimbuf *times);
    _TOOLEXP int   rmdir_rs(const char *dirName);
    _TOOLEXP int   mkdir_rs(const char *dirName);
#endif // USE_UFILE

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
#ifndef USE_UFILE

    #define  fopen_rs(fileName, mode)                     fopen(fileName, mode)
    #define  access_rs(fileName, mode)                    access(fileName, mode)
    #define  access_s_rs(fileName, mode)                  _access_s(fileName, mode)
    #define  sopen_rs(fileName, oflag, shareFlag, pmode)  sopen(fileName, oflag, shareFlag, pmode)
    #define  fdopen_rs(fd, mode)                          fdopen(fd, mode)
    #define  unlink_rs(fileName)                          unlink(fileName)
    #define  rename_rs(from, to)                          rename(from, to)

    #define  getenv_rs(key)                         getenv(key)
    #define  putenv_rs(val)                         putenv(val)
    #define  fullpath_rs(absPath, relPath, maxLen)  _fullpath(absPath, relPath, maxLen)
//    #define  stat_rs(path, buffer)                  stat(path,buffer)
    #define  stat_rs(path, buffer)                  rs_macro_stat(path, buffer)
    #define  utime_rs(path, times)                  utime(path, times)
    #define  rmdir_rs(dirName)                      rmdir(dirName)
    #define  mkdir_rs(dirName)                      mkdir(dirName)

#endif

#endif // __UNIFILE_HPP

/* <-- EOF --> */