/*@H*********************************************************************
*        File Name:   iutil.h            November 25,1993            
*      Description:   
*  Public routines:  
* Private routines:  
**********************************************************************@H*/

#ifndef __IUTIL_H
#define __IUTIL_H

// -----------------------------------------------------------------------------
#define  loop   for(;;)


// -----------------------------------------------------------------------------                     
#ifdef   RSL_MD_FLAT
    #define  ILOCAL
    #define  IFARD
#else
    #define  ILOCAL  _near _pascal

    #if defined(_Windows) && !defined(__DPMI16__)
        #define  IFARD
    #else
        #define  IFARD    _far
    #endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

int FindOSFile(char *buff, const char *file, const char *incPath, const char *ext, int fCurDir);
int FindDBFile(char *buff, const char *file, const char *incPath, const char *ext, int fCurDir, int isSQL);

extern bool  RslUseBlockMem;
void *iNewMemBlock(size_t size);

#ifdef BUILD_DLM
    void *DLMAPI iNewMem    (size_t size);
    void *DLMAPI iNewMemNull(size_t size);
    void  DLMAPI iDoneMem   (void *ptr);

#else
    void *iNewMem    (size_t size);
    void *iNewMemNull(size_t size);

    #ifdef MEMCHECK
        void _FreeMem     (void *ptr, char *file, int line);
        void _FreeMemBlock(void *ptr, char *file, int line);

        #define  iDoneMem(ptr)       _FreeMem(ptr, __FILE__, __LINE__)
        #define  iDoneMemBlock(ptr)  _FreeMemBlock(ptr, __FILE__, __LINE__)

    #else
        void iDoneMem(void *ptr);
        void iDoneMemBlock(void *ptr);
    #endif  // MEMCHECK
#endif

bool sc_isletter(int sc_ch, HRD inst);
bool sc_isdigit (int sc_ch);
bool sc_isspace (int sc_ch);
int  sc_stricmp (const char *str1, const char *str2);

char *sc_strupr (char *str);
char *sc_strnupr(char *str, size_t len);
char *sc_strlwr (char *str);
char *sc_strnlwr(char *str, size_t len);

void InsertAP(char *ioutbuf);
void DelEndSpace(char *str);

#ifndef NOSTRTRIM
    #ifndef StrTrim
    RSLLINK void RSLAPI StrTrim(char *str);
    #endif

    RSLLINK void RSLAPI StrTrimHead(char *str);
    RSLLINK void RSLAPI StrTrimTail(char *str);
#endif

void ShiftRight(char *str, int size);

unsigned long GetMemUsage(void);


// For backward compatibility

#define  iTRUE   true
#define  iFALSE  false

void  RslSetBtime(btime *bt);

const char *SysGetPropertyImpl(const char *key);

// -----------------------------------------------------------------------------
// Вывод информации в лог-файл RSTrace из RSL
RSLLINK int RSLAPI RslToRSTrace(HRD inst, const char *mod_name, int curr_line, const char *categ, const char *proc_name, int level, const char *msg);

// Получить текущий модуль и строку в нем
RSLLINK const char *RSLAPI RslGetCurrentModule(HRD inst, int *line);

#ifdef __cplusplus
}
#endif

#endif  // iutil_h

/* <-- EOF --> */