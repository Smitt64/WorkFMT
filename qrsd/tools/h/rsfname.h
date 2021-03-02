//-*--------------------------------------------------------------------------*-
//
// File Name   : rsfname.h
//
// Copyright (c) 1991 - 2005 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// January 14,1999  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __RSFNAME_H
#define __RSFNAME_H

//
// Path conversion functions
//

#include <stdlib.h>
#include <sys/types.h>

#include "tooldef.h"
#include "pexpdef.h"
#include "apptldef.h"
#include "platform.h"
#include "bdate.h"
#include "unixio.h"
#include "rscdefs.h"
#include "rstypes.h"


#define  PUT_INDIC  0
#define  USE_INDIC  1
#define  REM_INDIC  2

typedef int (TFileCopyIndicPtr)(int code, size_t fileSize, size_t curSz, void *parm);

enum
   {
    FEnomem  = -1,    // ������������ ������
    FEnofile = -2,    // ���� �� ������
    FEnoextt = -3,    // ����������� ���������� ��������� RSEXTT.D32
    FEremote = -4,    // ������ ����/������ ��� ���������� �������� �� ���������
    FEbreak  = -998,  // �������� �������� �������������
    FEcancel = -999   // �������� �������� �������������
   };

#include "packpshn.h"

typedef struct
      {
       char     fname[_MAX_PATH];
       bdate    fdate;
       btime    ftime;
       size_t   fsize;
       time_t   atime;   // ����� � UTC
       time_t   mtime;   // ����� � UTC
       time_t   ctime;   // ����� � UTC
      } TFileInfo;

// �� ��, ��� � TFileInfo, �� � ���������� 64-��������� ����� �����
typedef struct
      {
       char     fname[_MAX_PATH];
       bdate    fdate;
       btime    ftime;
       int64_t  fsize;   // ������ ����� (64 ����)
       time_t   atime;   // ����� � UTC
       time_t   mtime;   // ����� � UTC
       time_t   ctime;   // ����� � UTC
      } TFileInfoi64;

#include "packpop.h"

#define  SYNC_DEFAULT    SyncProcDefault   // ����� ����������� ������� (���������� ����� ������ ����)

#define  SYNC_TOTERM     SyncProcToTerm    // ����������� ����� �� �������� (� �������� �� ���������� �����)
#define  SYNC_TOSERV     SyncProcToServ    // ����������� ����� �� ������ (� �������� �� ���������� �����)

#define  SYNC_FROMTERM   SyncProcFromTerm  // ����������� ����� � ��������� (���������� ��� �������)
#define  SYNC_FROMSERV   SyncProcFromServ  // ����������� ����� � ������� (���������� ��� �������)

#define  SYNC_CHECKOUT   SyncProcCheckOut  // ����������� ����� �� �������� (� �������� �� ���������� ����� ������� �����)
#define  SYNC_CHECKIN    SyncProcCheckIn   // ����������� ����� �� ������ (���������� ��� �������)

typedef size_t (TSyncFileProc)(const TFileInfo *local, const TFileInfo *remote, void *parm);

#ifdef __cplusplus
extern "C" {
#endif

_TOOLEXP void  RslSplitFile(const char *fname, char *dir, char *name, char *ext);
_TOOLEXP char *RslMergeFile(char *buff, const char *dir, const char *name, const char *ext);
_TOOLEXP int   RslGetDir(int drv, char *buff);
_TOOLEXP char *RslNativePath(char *buff);

#define  BtrvGetDir(drv, buff)  (RslGetDir((drv), (buff)))

// ��������� ����� �����.
_TOOLEXP char *GetFName(const char *Path, char *Name);
_TOOLEXP const char *GetFNamePtr(const char *path);

_TOOLEXP int iGetFileInfo(const char *fname, bdate *fdate, btime *ftime, long *fsize);
_TOOLEXP int GetFileInfo(const char *fname, bdate *fdate, btime *ftime, size_t *fsize);
_TOOLEXP int GetFileInfoEx(const char *fname, TFileInfo *fi);

// ��������� �������� ������ �����, ����������� int32
_TOOLEXP int iGetFileInfoi64(const char *fname, bdate *fdate, btime *ftime, int64_t *fsize);
_TOOLEXP int GetFileInfoExi64(const char *fname, TFileInfoi64 *fi);

_TOOLEXP int CompareFileInfo(const TFileInfo *src, const TFileInfo *dest);

_RSPNLEXP  int    FileCopyIndic(int code, size_t fileSize, size_t curSz, void *data);
_RSPNLEXP  int    AskFileReplace(const char *title, const TFileInfo *src, const TFileInfo *dest, int ask);
_RSPNLEXP  int    CopyFileToTerm(const char *fname);

_APPTLFUNC int    FindIncFile(char *Buff, const char *file, const char *IncPath, const char *ext, int fCurDir);
_APPTLFUNC int    FindIncFileEx(char *Buff, const char *file, const char *incPath, const char *ext, int fCurDir,
                                int needTestForExist);

_APPTLFUNC int    RslFindDBTable(char *Buff, const char *file, const char *incPath, const char *ext, int fCurDir);
_APPTLFUNC int    RslFindOSFile(char *Buff, const char *file, const char *incPath, const char *ext, int fCurDir);

_APPTLFUNC int    LastFileError(void);
_APPTLFUNC void   SetFileError(int error, const char *fname);
_APPTLFUNC const char *TextFileError(int error);

_APPTLFUNC size_t SendFile(const char *srcName, const char *destName, const char *dirList,
                           TFileCopyIndicPtr *indicProc, void *data);

_APPTLFUNC size_t SendFileEx(const char *srcName, const char *destName, const char *dirList,
                             TFileCopyIndicPtr *indicProc, void *data, bool forceCopy);

_APPTLFUNC size_t RecvFile(const char *srcName, const char *destName, const char *dirList,
                           TFileCopyIndicPtr *indicProc, void *data);

_APPTLFUNC size_t CopyRemoteFile(const char *srcName, const char *destName);
_APPTLFUNC size_t CopyLocalFile(const char *srcName, const char *destName, const char *dirList,
                                TFileCopyIndicPtr *indicProc, void *data);

_APPTLFUNC size_t CopyLocalFileEx(const char *srcName, const char *destName, const char *dirList,
                                  TFileCopyIndicPtr *indicProc, void *data, bool forceCopy);

_APPTLFUNC int    GetRemoteCurDir(char *buff, int szBuff);

_APPTLFUNC int    GetRemoteFileInfo(const char *fileName, bdate *fdate, btime *ftime, size_t *fsize, char *rmtFile);
_APPTLFUNC int    GetRemoteFileInfoi64(const char *fileName, bdate *fdate, btime *ftime, int64_t *fsize, char *rmtFile);
_APPTLFUNC int    GetRemoteFileInfoEx(const char *fileName, TFileInfo *fi);
_APPTLFUNC int    GetRemoteFileInfoExi64(const char *fileName, TFileInfoi64 *fi);

_APPTLFUNC int    RenameRemoteFile(const char *srcName, const char *dstName);
_APPTLFUNC int    MakeRemoteDir(const char *dirName);

_APPTLFUNC int    RemoteExistDir(const char *dirName, int *stat);

_APPTLFUNC int    UnlinkRemoteDir(const char *dirName);
_APPTLFUNC int    UnlinkRemoteFile(const char *fileName);

_APPTLFUNC size_t SyncFile(const char *szLocalFile, const char *szRemoteFile,
                           const char *szDirList, const char *szDefExt, TSyncFileProc pSyncProc, void *pParm);

_APPTLFUNC size_t SyncProcDefault (const TFileInfo *local, const TFileInfo *remote, void *parm);
_APPTLFUNC size_t SyncProcToTerm  (const TFileInfo *local, const TFileInfo *remote, void *parm);
_APPTLFUNC size_t SyncProcToServ  (const TFileInfo *local, const TFileInfo *remote, void *parm);
_APPTLFUNC size_t SyncProcFromTerm(const TFileInfo *local, const TFileInfo *remote, void *parm);
_APPTLFUNC size_t SyncProcFromServ(const TFileInfo *local, const TFileInfo *remote, void *parm);
_APPTLFUNC size_t SyncProcCheckOut(const TFileInfo *local, const TFileInfo *remote, void *parm);
_APPTLFUNC size_t SyncProcCheckIn (const TFileInfo *local, const TFileInfo *remote, void *parm);

_TOOLEXP int  IsAbsPath(const char *path);
_TOOLEXP int  IsRelativePath(const char *path);
_TOOLEXP void MakeSysFileName(char *fileName, char sysChar);
_TOOLEXP char *MakeFirstPath(char *Buff, const char *file, const char *IncDir, const char *ext);
_TOOLEXP char *MakeEllipsisPath(char *buf, size_t sz, const char *file);

_TOOLEXP int  RslTestFile(const char *file);

const char *GetRemoteFullPath(const char *in, char *out, bool *isDir);
unsigned long GetRemoteLogicalDrives(void);
bool IsRemotePathDirectory(const char *in);

/*******************************************************************************

   RslFindInc - ����� ������ � ������ ���������
   ~~~~~~~~~~
   ���������:
          buf - ��������� ���� � �����;
        fname - ��� ����� ��� ������;
      incPath - ������ ��������� ��� ������;
          ext - ���������� ����� (���� �� ������ � fname);
      fCurDir - ������ � ������� ��������;
 testForExist - �������-�������� �� ������������� �����.

   ���������� !0, ���� ���� ������.

*******************************************************************************/

// �������� �� ������������� �����
// ���������� 1, ���� ���� ����������, � 0 � ��������� ������.
typedef int (testFile_ptr)(const char *file);

_TOOLEXP int RslFindInc(char *buf, const char *file, const char *incPath,
                        const char *ext, int fCurDir, testFile_ptr *testForExist);


_TOOLEXP const char *progPath(void);
_TOOLEXP const char *progPathEx(bool isAnsi);
_TOOLEXP char *RunPath(char *name);
_TOOLEXP char *RunPathEx(char* name, bool isAnsi);

/*  name ������ ������� ������ ���� */
_TOOLEXP char *IniFilePath(char *name);
/*  name ������ ������� ������ ���� */

typedef int (*ProcKeyValue_f)(char *key, char *name, void *data);
// ��������� �� ������� ��������� ������ �������� �� .ini-�����

_TOOLEXP   int    ReadConfigEx(char *name, ProcKeyValue_f fun, void *data, const char *homeDir);
_APPTLFUNC int    ReadConfig(char *name, ProcKeyValue_f fun, void *data);
// ���������� 0 - ���� ������������ ������ � ������� ���������
// -1 - ���� ������������ �� ������
// > 0 ��� ������ �� fun

//
// �������� ��� �� ��� ��������.
// ������ ����������� � ������ buf, ���� ��� ������ size ���������� ��� ����������
// ����������.
// ���������� ���� � ������ �������� ������ ��� ����������� ������ ������.
//
// ���� �������� doCorrect �� ����� 0, ������� ���������� ����� '\\' �� �������� ������,
//    ��������: c:\dir1\\dir2\  -> c:\dir\dir2\.
//
_TOOLEXP   size_t ReplaceTag(char *buf, size_t size, const char *tag, const char *value, int doCorrect);
_TOOLEXP   size_t ReplaceTagInDirList(char *dirlst, size_t size, const char *tag, const char *value);

_APPTLFUNC void   ReplaceStdTags(char *buf, size_t size);


_TOOLEXP int RsMkDir(const char *dir);

char *RslGetAppDataPath(char *szBuffer, const char *szProductName, const char *szCompanyName);
int   RslReadFileToBuffer(const char *szFileName, void **pBuffer, size_t *pSize);
int   RslWriteBufferToFile(const char *szFileName, void *pBuffer, size_t nSize);
const char* RslGetPersonalFolder(char* path);

_TOOLEXP int CorrectPath(char *buff);

// ������������� ����. ���������� � ����� ������ ��� ������������� ������ '\'
_TOOLEXP void CorrectPath2(char *buff);


#ifdef __cplusplus
}
#endif

#endif //#ifndef __RSFNAME_H

/* <-- EOF --> */