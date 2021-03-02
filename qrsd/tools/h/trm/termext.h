/*-----------------------------------------------------------------------*-

 File Name   : termext.h   

 Sybsystem   : DLM SDK  
 Description : Интерфейс для создания расширений терминала

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 October 6,1997  Sergei Kubrin (K78) - Create file
 Junuary 24,1998 Sergei Kubrin (K78) - адоптация для DLM SDK
-*-----------------------------------------------------------------------*/

#ifndef __TERMEXT_H
#define __TERMEXT_H

#ifdef BUILD_DLM
    #error  "You can not include termext.h when build RSL extantion!"
#endif

#include "trm/srventry.h"

#define  BACKENTRYDEFINED

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif


#ifndef  RSL_NO_TERMEXP
    //=========================================================================
    //   Функции экспортируемые из term.exe 
    //   (могут вызываться из расширения терминала
    //   Чтобы использовать эти функции нельзя включать в исходный текст 
    //   для расширения терминала файл rsldll.h !!! 



    //  Эта функция для расширений терминала отличается от используемой
    //  в расширениях RSL !!!!
    void MsgBox(const char **str, int nStr);

    void ShowMessage(const char **str, int nStr);
    int  ReadIniFile(char *name, int (DLMAPIC *fun)(char *key, char *name, void *data), void *data);

    int  RunProg(
                 char *program,   /*  Запускаемая программа                       */
                 char *com,       /*  Командная строка                            */
                 char *initmes,   /*  Стартовое сообщение  или  NULL              */
                 char *pausemes,  /*  Заключительное сообщение и пауза или NULL   */
                 int   swap_size  /*  Размер в KB необходимый для программы       */
                );

    // -----------------------------------------------------------------------------
    // Функции аналогичные стандартным, но для работы с русскими буквами          
    int    toupc(int ch);
    int    stricmpR(const char *str1, const char *str2);
    int    strnicmpR(const char *str1, const char *str2, size_t len);
    int    toDownc(int ch);
    char  *struprR(char *str);
    char  *strnuprR(char *str, size_t len);
    char  *strlwrR(char *str);
    char  *strnlwrR(char *str, size_t len);

    char  *MakeFirstPath(char *Buff, const char *file, const char *IncDir, const char *ext);
    int    FindIncFile(char *Buff, const char *file, const char *IncPath, const char *ext, int fCurDir);
    char  *RunPath(char *name);  /*  name должна вмещать полный путь */

    void   RslSplitFile(const char *fname, char *dir, char *name, char *ext);
    char  *RslMergeFile(char *buff, const char *dir, const char *name, const char *ext);
    int    TestExistFile(const char *file);

    bool   fs_sendAsync(const void *in, size_t sz, const char *tblName, int cmdId);
    bool   fs_transactAsync(const void *in, size_t sz, const char *tblName, int cmdId,
                            void *out, size_t szOut, size_t *szOutPtr, unsigned long timeOut);

            
    int   DLMAPI fs_event(TFSEvent *ev, int waitTime);

    unsigned  DLMAPI GetBuffSize();

    #ifdef _INC_WINDOWS
        typedef struct TFsComplete
              {
               HANDLE    ev;
               void     *asyncObj;
               char      reserv[8];
              } TFsComplete;

        bool fs_beginAsync(const void *in, size_t sz, const char *tblName, int cmdId,
                           void *out, size_t szOut, size_t *szOutPtr, TFsComplete *cmplPtr);
        bool fs_resultAsync(TFsComplete *cmplPtr, DWORD timeOut);
        void fs_cancelAsync(TFsComplete *cmplPtr);
        bool fs_sendAsyncEx(const void *in, size_t sz, const char *tblName, int cmdId, TFsComplete *cmplPtr);
        bool fs_resultAsyncEx(TFsComplete *cmplPtr, DWORD timeOut, void *out, size_t szOut, size_t *szOutPtr);
    #endif
             

    #define  fs_appVersion()                   AppartmentPtr->vtbl->version(AppartmentPtr)
    #define  fs_getSendBuff(sz, tblName, cmd)  AppartmentPtr->vtbl->getSendBuff(AppartmentPtr, sz, tblName, cmd)
    #define  fs_getReplyBuff(sz)               AppartmentPtr->vtbl->getReplyBuff(AppartmentPtr, sz)
    #define  fs_sendMessage(mes)               AppartmentPtr->vtbl->sendMessage(AppartmentPtr, mes)
    #define  fs_transactMessage(mes, sz)       AppartmentPtr->vtbl->transactMessage(AppartmentPtr, mes, sz)

    #define  fs_addFilterProc(proc,data)       AppartmentPtr->vtbl->addFilterProc(AppartmentPtr, proc, data)
    #define  fs_remFilterProc(proc)            AppartmentPtr->vtbl->remFilterProc(AppartmentPtr, proc)
    #define  fs_addIdleProc(proc,data)         AppartmentPtr->vtbl->addIdleProc(AppartmentPtr, proc, data)
    #define  fs_remIdleProc(proc)              AppartmentPtr->vtbl->remIdleProc(AppartmentPtr, proc)

    #define  fs_getEventApp(ev, tm)            AppartmentPtr->vtbl->getEvent(AppartmentPtr, ev, tm)
    #define  fs_addEventApp(ev, capture)       AppartmentPtr->vtbl->addEvent(AppartmentPtr, ev, capture)

    #define  fs_stopSession(doAbort)           AppartmentPtr->vtbl->stopSession(AppartmentPtr, doAbort)

    //=========================================================================


    #ifdef RSL_MD_FLAT
        typedef struct
              {
               int    (* ptr_toupc)        (int ch);
               int    (* ptr_stricmpR)     (char *str1, char *str2);
               int    (* ptr_strnicmpR)    (char *str1, char *str2, int len);
               int    (* ptr_toDownc)      (int ch);
               char  *(* ptr_struprR)      (char *str);
               char  *(* ptr_strnuprR)     (char *str, int len);
               char  *(* ptr_strlwrR)      (char *str);
               char  *(* ptr_strnlwrR)     (char *str, int len);
               char  *(* ptr_MakeFirstPath)(char *Buff, const char *file, const char *IncDir, const char *ext);
               int    (* ptr_FindIncFile)  (char *Buff, const char *file, const char *IncPath, const char *ext, int fCurDir);
               char  *(* ptr_RunPath)      (char *name);  /*  name должна вмещать полный путь */
               void   (* ptr_RslSplitFile) (const char *fname, char *dir, char *name, char *ext);
               char  *(* ptr_RslMergeFile) (char *buff, const char *dir, const char *name, const char *ext);
               int    (* ptr_TestExistFile)(const char *file);
               int    (* ptr_ReadIniFile)  (char *name, int (DLMAPIC *fun)(char *key, char *name, void *data), void *data);
               int    (* ptr_RunProg)      (char *prog, char *com, char *initmes, char *pausemes, int swap_size);
               void   (* ptr_MsgBox)       (const char **str, int nStr);
               void   (* ptr_ShowMessage)  (const char **str, int nStr);
               bool   (* ptr_sendAsync)    (const void *in, size_t sz, const char *tblName, int cmdId);
               bool   (* ptr_transactAsync)(const void *in, size_t sz, const char *tblName, int cmdId,
                                            void *out, size_t szOut, size_t *szOutPtr, unsigned long timeOut);

                #ifdef _INC_WINDOWS
                    bool  (* ptr_beginAsync)(const void *in, size_t sz, const char *tblName, int cmdId,
                                             void *out, size_t szOut, size_t *szOutPtr, TFsComplete *cmplPtr);
                    bool  (* ptr_resultAsync)(TFsComplete *cmplPtr, DWORD timeOut);
                    void  (* ptr_cancelAsync)(TFsComplete *cmplPtr);

                    bool  (* ptr_sendAsyncEx)(const void *in, size_t sz, const char *tblName, int cmdId, TFsComplete *cmplPtr);
                    bool  (* ptr_resultAsyncEx)(TFsComplete *cmplPtr, DWORD timeOut, void *out, size_t szOut, size_t *szOutPtr);  

                #else
                    void  (* ptr_fun1)(void);
                    void  (* ptr_fun2)(void);
                    void  (* ptr_fun3)(void);
                    void  (* ptr_fun4)(void);
                    void  (* ptr_fun5)(void);
                #endif

               int (DLMAPI *ptr_event)(TFSEvent *ev, int waitTime);
               unsigned (DLMAPI *ptr_GetBuffSize)(void);
              } TRMCALLTBL;


        // -----------------------------------------------------------------------------
        extern BACKENTRY  *ExeExports;

        // -----------------------------------------------------------------------------
        #define  TRMCALL(fun)  ((TRMCALLTBL *)ExeExports)->ptr_##fun

// -----------------------------------------------------------------------------
        #if !defined(TRM_NO_MACRO)
            #define  toupc             ((TRMCALLTBL *)ExeExports)->ptr_toupc
            #define  stricmpR          ((TRMCALLTBL *)ExeExports)->ptr_stricmpR
            #define  strnicmpR         ((TRMCALLTBL *)ExeExports)->ptr_strnicmpR
            #define  toDownc           ((TRMCALLTBL *)ExeExports)->ptr_toDownc
            #define  struprR           ((TRMCALLTBL *)ExeExports)->ptr_struprR
            #define  strnuprR          ((TRMCALLTBL *)ExeExports)->ptr_strnuprR
            #define  strlwrR           ((TRMCALLTBL *)ExeExports)->ptr_strlwrR
            #define  strnlwrR          ((TRMCALLTBL *)ExeExports)->ptr_strnlwrR
            #define  MakeFirstPath     ((TRMCALLTBL *)ExeExports)->ptr_MakeFirstPath
            #define  FindIncFile       ((TRMCALLTBL *)ExeExports)->ptr_FindIncFile
            #define  RunPath           ((TRMCALLTBL *)ExeExports)->ptr_RunPath
            #define  RslSplitFile      ((TRMCALLTBL *)ExeExports)->ptr_RslSplitFile
            #define  RslMergeFile      ((TRMCALLTBL *)ExeExports)->ptr_RslMergeFile
            #define  TestExistFile     ((TRMCALLTBL *)ExeExports)->ptr_TestExistFile
            #define  ReadIniFile       ((TRMCALLTBL *)ExeExports)->ptr_ReadIniFile
            #define  RunProg           ((TRMCALLTBL *)ExeExports)->ptr_RunProg
            #define  MsgBox            ((TRMCALLTBL *)ExeExports)->ptr_MsgBox
            #define  ShowMessage       ((TRMCALLTBL *)ExeExports)->ptr_ShowMessage
            #define  fs_sendAsync      ((TRMCALLTBL *)ExeExports)->ptr_sendAsync
            #define  fs_transactAsync  ((TRMCALLTBL *)ExeExports)->ptr_transactAsync
            #define  fs_beginAsync     ((TRMCALLTBL *)ExeExports)->ptr_beginAsync
            #define  fs_resultAsync    ((TRMCALLTBL *)ExeExports)->ptr_resultAsync
            #define  fs_cancelAsync    ((TRMCALLTBL *)ExeExports)->ptr_cancelAsync
                                   
            #define  fs_sendAsyncEx    ((TRMCALLTBL *)ExeExports)->ptr_sendAsyncEx
            #define  fs_resultAsyncEx  ((TRMCALLTBL *)ExeExports)->ptr_resultAsyncEx
            #define  fs_event          ((TRMCALLTBL *)ExeExports)->ptr_event
            #define  GetBuffSize       ((TRMCALLTBL *)ExeExports)->ptr_GetBuffSize
        #endif // TRM_NO_MACRO
    #else  // RSL_MD_FLAT
        // -----------------------------------------------------------------------------
        #define TRMCALL(fun)  fun
    #endif // RSL_MD_FLAT

#endif // RSL_NO_TERMEXP

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif // __FSMES_H

/* <-- EOF --> */