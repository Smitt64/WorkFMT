/*-----------------------------------------------------------------------*-

 File Name   : rslobj.h  

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 July 9,1997  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __RSLOBJ_H
#define __RSLOBJ_H


#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------
typedef struct
      {
       VALUE     value;
       SYMVAR    *prop;
      } TPropVal;

// -----------------------------------------------
typedef struct
      {
       TGenObject        derived;

    // Приватные данные объекта
       LOBJ              elem;
       unsigned short    refCount;

       SYMCLASS         *sym;

       TPropVal         *propPtr;
       TMethodChunkEx   *methodPtr;

       int               nProp;
       int               nMethod;
       int               propDelta;
       int               methodDelta;
       SYMEXTCLASS      *extClass;
       TGenObject       *extObj;

       CNTLIST           weakRefList;

    #ifdef DEBUG
       char             *clsName;
    #endif

    #ifndef NO_GC
       unsigned short    generation;  // Used to collect cyclic objects
       unsigned short    marknumber;  // Used to report references
    #endif
      } TRslObject;

#ifndef RSL_DLM

    // -----------------------------------------------
    struct FndClassData
         {
          int           nProp;
          int           nMethod;
          int           methodDelta;
          int           propDelta;
          SYMEXTCLASS  *extClass;

          // add by Tokmashev (26.01.2001)
          // int nPropBase;
          // int nMethodBase;
          SYMCLASS     *ownerClass;
         };

    // -----------------------------------------------------------------------------
    TGenObject                *RslObjInst        (SYMCLASS *sym);
    TGenObject                *RslObjInstExt     (SYMEXTCLASS *sym, TGenObject *outer);
    RSLLINK SYMCLASS * RSLAPI  FindRslClass      (const char *typeName);
    void                       CalculateClass    (SYMCLASS *sym, struct FndClassData *data);
    RSLLINK ISYMBOL  * RSLAPI  FindMethodProp    (SYMCLASS *sym, const char *name);
    RSLLINK bool RSLAPI        FindMethodPropEx  (SYMCLASS *sym, FndWithData *data);
    bool                       FindMethodPropEx2 (SYMCLASS *sym, FndWithData *data, bool isUppercase);
    ISYMBOL                   *FindMethodProp2   (SYMCLASS *sym, const char *name, bool fndPrivate, bool isUppercase);
    ISYMBOL                   *FindMethodPropCase(SYMCLASS *sym, const char *name, bool fndPrivate);


    int           InitRslProvider(CLNT_PRVD_HANDLE *clntId);
    TGenObject   *CreateRslObject(CLNT_PRVD_HANDLE clntId, const char *typeName);
    void          DoneRslProvider(CLNT_PRVD_HANDLE clntId);

    int           RunMethod(int id, TRslObject *obj, HRD inst);
    uintptr_t     RslObjCLSID(void);
    void          FindExtClass(SYMCLASS *sym, struct FndClassData *data);

#endif // RSL_DLM

#ifdef __cplusplus
}
#endif

#endif // __RSLOBJ_H

/* <-- EOF --> */