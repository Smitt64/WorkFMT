//-*--------------------------------------------------------------------------*-
//
// File Name   : rsladapter.h
//
// Copyright (c) 1991 - 2009 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// June 16,2009  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef _RSLADAPTER_H_
#define _RSLADAPTER_H_

// -----------------------------------------------------------------------------
struct TValue : public VALUE
     {
      TValue()
      {
       ValueMake(this);
      }

     ~TValue()
      {
       ValueClear(this);
      }
     };

// -----------------------------------------------------------------------------
class ToolsDataProvBookmark : public IBtBookmark
    {
       unsigned    refCount;
       long        pos;
       TGenObject *obj;
       HRD         hInstance;

     public:
       ToolsDataProvBookmark(HRD inst, long pos_, TGenObject *obj_) :
                            refCount(1), pos(pos_), obj(obj_), hInstance(inst)
       {
        if(obj)
          ADDREF(obj);
       }

       long getPos()
       {
        return pos;
       }

       TGenObject *getObj()
       {
        return obj;
       }

       virtual int  __stdcall releaseBmk()
       {
        unsigned  count = --refCount;

        if(!count)
         {
          if(obj)
           {
            InstSwitch  sw(hInstance);

            RELEASE(obj);
           }

          delete this;
         }

        return count;
       }

       virtual int  __stdcall addRefBmk()
       {
        return ++refCount;
       }

       virtual bool __stdcall compareBmk (const IBtBookmark*  ptr) const
       {
        if(pos && ((ToolsDataProvBookmark *)ptr)->pos)
          return pos == ((ToolsDataProvBookmark *)ptr)->pos;
        else if(obj && ((ToolsDataProvBookmark *)ptr)->obj)
          return obj == ((ToolsDataProvBookmark *)ptr)->obj;

        return false;
       }

    };

// -----------------------------------------------------------------------------
class  ToolsDataAdapter : public IBtUserDataSource
    {
       HRD            hInstance;
       TGenObject    *pThis;
       TGenObject    *rec;


       long           idNext;   
       long           idPrev;   
       long           idFirst;   
       long           idLast;   
       long           idGetBmk;   
       long           idToBmk;

       long           idInsert;
       long           idUpdate;
       long           idDelete;

       long           idGetCols;
       long           idLastStatus;

     public:

       ToolsDataAdapter(TGenObject *pThis_ = NULL) : pThis(pThis_), rec(NULL)
       {
        hInstance = RslGetInstance();

        RslObjMemberFromName(pThis, "moveFirst",       &idFirst);
        RslObjMemberFromName(pThis, "moveLast",        &idLast);
        RslObjMemberFromName(pThis, "moveNext",        &idNext);
        RslObjMemberFromName(pThis, "movePrev",        &idPrev);
        RslObjMemberFromName(pThis, "getBookmark",     &idGetBmk);
        RslObjMemberFromName(pThis, "moveToBookmark",  &idToBmk);
        RslObjMemberFromName(pThis, "recordInsert",    &idInsert);
        RslObjMemberFromName(pThis, "recordUpdate",    &idUpdate);
        RslObjMemberFromName(pThis, "recordDelete",    &idDelete);

        RslObjMemberFromName(pThis, "getColumnsInfo",  &idGetCols);
        RslObjMemberFromName(pThis, "getLastStatus",   &idLastStatus);
       }

      ~ToolsDataAdapter()
       {
       }

       TGenObject *getColumnsInfo() const
       {
        TValue  retVal;

        if(idGetCols != -1 && RslObjInvoke(pThis, idGetCols, RSL_DISP_RUN, 0, NULL, &retVal) != -1)
         {
          if(retVal.v_type == V_GENOBJ && RslIsTArray(P_GOBJ(retVal.value.obj)))
           {
            // Return weal ref!!!
            return P_GOBJ(retVal.value.obj);
           }
         }

        return NULL;
       }


       int getLastStatus() const
       {
        int     stat = 0;
        TValue  retVal;

        TExtErrErrInfo  e;


        if(RslGetLastErrorInfo(&e))
         {
          void *obj;

          if(e.userError && e.version >= 2 && e.extErr && e.extErr->v_type == V_GENOBJ &&
             RslUniCast("TDbError", P_GOBJ(e.extErr->value.obj), &obj))
           {
            long  id;
            TGenObject  *eObj = P_GOBJ(e.extErr->value.obj);

            RslObjMemberFromName(eObj, "stat", &id);

            if(id != -1 && RslObjInvoke(eObj, id, RSL_DISP_GET, 0, NULL, &retVal) != -1 && retVal.v_type == V_INTEGER)
              stat = retVal.value.intval;
           }
         }

        if(!stat)
         {
          if(idLastStatus != -1 && RslObjInvoke(pThis, idLastStatus, RSL_DISP_RUN, 0, NULL, &retVal) != -1 &&
             retVal.v_type == V_INTEGER)
            stat = retVal.value.intval;
         }

        if(!stat)
          stat = BEmaxexterr;

        return stat;
       }

       int openBf(BTRVFILE **bf)
       {
        int  stat = BEmaxexterr;
        TRecHandlerInfo  data;

        if(rec && GetRecordHandlerInfo (rec,  &data))
         {
          long             idFileName;
          TValue           retVal;
          TRecordHandler  *rh = (TRecordHandler *)CastToCLSID(rec, RslTRecHandlerID());
          const char      *fileName = rh->dicname;

          RslObjMemberFromName(pThis, "getFileName", &idFileName);

          if(idFileName != -1 && RslObjInvoke(pThis, idFileName, RSL_DISP_RUN, 0, NULL, &retVal) != -1)
           {
            if(retVal.v_type == V_STRING)
              fileName = retVal.value.string;
           }

          stat = bfOpenUser(bf, fileName, this, data.size, sizeof(long), NULL, NULL, data.buff);

          if(!stat)
            ADDREF(pThis);
         }

        return stat;
       }



       RSL_CLASS(ToolsDataAdapter)

       RSL_METHOD_DECL(setCurrentRecord)
       {
        VALUE  *v;

        GetParm(1, &v);

        if(v->v_type == V_GENOBJ && CastToCLSID(P_GOBJ(v->value.obj), RslTRecHandlerID()) != NULL)
         {
          if(rec)
            RELEASE(rec);

          rec = P_GOBJ(v->value.obj);

          ADDREF(rec);
         }

        return 0;
       }

       RSL_GETPROP_DECL(Prop2)
       {
        return 0;
       }


       RSL_INIT_DECL()
       {
        VALUE  *v;

        GetParm(*firstParmOffs, &v);

        if(v->v_type == V_GENOBJ && CastToCLSID(P_GOBJ(v->value.obj), RslTRecHandlerID()) != NULL)
         {
          rec = P_GOBJ(v->value.obj);

          ADDREF(rec);
         }
       }

       RSL_FINAL_DECL()
       {
        if(rec)
          RELEASE(rec);
       }

       int makeOper(long id, int flags, void *DATA_BUF, int *DATA_LEN)
       {
        int      stat = BEmaxexterr;
        TValue   retVal;

        if(id != -1)
         {
          if(RslObjInvoke(pThis, id, RSL_DISP_RUN, 0, NULL, &retVal) != -1)
           {
            if(retVal.v_type == V_BOOL)
              stat = retVal.value.boolval ? 0 : BEeof;
           }
          else
            stat = getLastStatus ();
         }

        return stat;
       }


    //===================================================================================

       virtual void __stdcall onBfClose()
       {
        InstSwitch  sw(hInstance);

        RELEASE(pThis);
       }

       virtual int __stdcall getBookmark(BKMHANDLE *handle, bool *isSimple) const
       {
        InstSwitch  sw(hInstance);
        TValue  retVal;
        int     stat = BEinoper;
        ToolsDataProvBookmark  *ptr = NULL;

        if(idGetBmk != -1)
         {
          if(RslObjInvoke(pThis, idGetBmk, RSL_DISP_RUN, 0, NULL, &retVal) != -1)
           {
            if(retVal.v_type == V_INTEGER)
              ptr = new ToolsDataProvBookmark(hInstance, retVal.value.intval, NULL);
            else if(retVal.v_type == V_GENOBJ)
              ptr = new ToolsDataProvBookmark(hInstance, 0, P_GOBJ(retVal.value.obj));

            if(ptr)
             {
              *handle   = (BKMHANDLE)(IBtBookmark *)ptr;
              *isSimple = false;

              return 0;
             }
           }
          else
            stat = getLastStatus();
         }

        *handle   = 0;
        *isSimple = true;

        return stat;
       }

       virtual int __stdcall moveToBookmark(BKMHANDLE handle, int flags, void *DATA_BUF, int *DATA_LEN, void *KEY_BUF, int KEY_LEN, signed char KEY_NUM)
       {
        int  stat = BEinpos;

        if(handle)
         {
          InstSwitch  sw(hInstance);
          VALUE    posVal;  // Do not clear posVal!
          TValue   retVal;

          ToolsDataProvBookmark *ptr = (ToolsDataProvBookmark *)(IBtBookmark *)handle;

          if(ptr->getPos())
           {
            long  pos = ptr->getPos ();

            ValueSet(&posVal, V_INTEGER, &pos);
           }
          else
           {
            TGenObject *pos = ptr->getObj();

            ValueSet(&posVal, V_GENOBJ, pos);
           }

          if(idToBmk != -1)
           {
            if(RslObjInvoke(pThis, idToBmk, RSL_DISP_RUN, 1, &posVal, &retVal) != -1)
             {
              if(retVal.v_type == V_BOOL)
                stat = retVal.value.boolval ? 0 : BEeof;
             }
            else 
              stat = getLastStatus ();
           }
         }

        return stat;
       }


       virtual int __stdcall makeOperation(int op, int flags, void *DATA_BUF, int *DATA_LEN, void *KEY_BUF, int KEY_LEN, signed char KEY_NUM)
       {
        InstSwitch  sw(hInstance);

        switch(op)
             {
              case BgetF:   return makeOper(idFirst,  flags, DATA_BUF, DATA_LEN);
              case BgetL:   return makeOper(idLast,   flags, DATA_BUF, DATA_LEN);
              case BgetN:   return makeOper(idNext,   flags, DATA_BUF, DATA_LEN);
              case BgetP:   return makeOper(idPrev,   flags, DATA_BUF, DATA_LEN);
              case Binsert: return makeOper(idInsert, flags, DATA_BUF, DATA_LEN);
              case Bupdate: return makeOper(idUpdate, flags, DATA_BUF, DATA_LEN);
              case Bdelete: return makeOper(idDelete, flags, DATA_BUF, DATA_LEN);
             }

        return BEinoper;
       }

       char *setText(char **to, const char *from, char *bufPtr)
       {
        *to = bufPtr;

        strcpy(*to, from);

        bufPtr += strlen(from) + 1;

        return bufPtr;
       }

       virtual BTFileInfo * __stdcall getMetaInfo()
       {
        if(!rec)
          return NULL;

        InstSwitch  sw(hInstance);

        TRecordHandler  *rh  = (TRecordHandler *)CastToCLSID(rec, RslTRecHandlerID());
        BTRSTRUCTURE    *str = rh->str;
        TBtStructure    *bs  = &rh->str->str;
        const char      *pName;
        int              i;
        size_t           textSize = 0;


        textSize += strlen(bs->name) + 1;
        textSize += strlen(bs->own) + 1;

        for(i = 0, pName = str->names; i < bs->nfields; ++i)
          {
           size_t  l = strlen(pName) + 1;

           textSize += l;
           pName    += l;
          }

        size_t  fullSize = sizeof(BTFileInfo) + sizeof(BTFieldInfo)*bs->nfields + textSize;

        BTFileInfo  *file = (BTFileInfo *)malloc(fullSize);

        if(!file)
          return NULL;

        BTFieldInfo  *fld     = (BTFieldInfo *)(file + 1);
        char         *textPtr = (char *)(fld + bs->nfields);

        textPtr = setText(&file->FileName, bs->name, textPtr);
        textPtr = setText(&file->FileOwner, bs->own, textPtr);

        file->FileDesc   = NULL;
        file->FileFlags  = 0;
        file->OwnerType  = 0;
        file->MaxVarLen  = bs->maxvarlen;
        file->RecordLen  = GetBuffSize(str);
        file->BLOBFlag   = 0;
        file->Reserve    = 0;
        file->Dictionary = NULL;
        file->nFields    = bs->nfields;
        file->nKeys      = 0;
        file->Fields     = fld;
        file->Keys       = NULL;
        file->PageSize   = 0;

        ResBtrField      *rslFld = bs->fields;
        ResBtrFieldAttr  *atr    = bs->fAttr;

        for(i = 0, pName = str->names; i < bs->nfields; ++i, ++fld, ++rslFld)
          {
           size_t  l = strlen(pName) + 1;

           textPtr = setText(&fld->FldName, pName, textPtr);

           pName += l;

           fld->FldDesc   = NULL;
           fld->FldType   = rslFld->fieldtype;
           fld->FldSize   = rslFld->size;
           fld->FldOffset = rslFld->pos;
           fld->FldLength = 10;
           fld->FldPoint  = 0;
           fld->Hidden    = 0;
           fld->Reserve   = 0;

           if(fld->FldType == FT_FLOAT || fld->FldType == FT_DOUBLE || fld->FldType == FT_DOUBLEM || fld->FldType == FT_NUMERIC || 
              fld->FldType == FT_FLOATG || fld->FldType == FT_DOUBLEG || fld->FldType == FT_DOUBLER || 
              fld->FldType == FT_LDMON || fld->FldType == FT_LDMONR || fld->FldType == FT_LDOUBLE10)
             fld->FldPoint = 2;

  #ifdef USE_NUMERIC
           if(fld->FldType == FT_NUMERIC && !isSql)
            {
             int  decPoint = (atr && atr->decPoint) ? atr->decPoint : 4;

             fld->FldPoint = MKFVP(decPoint, 2);
            }
  #endif

           if(atr)
            {
             fld->FldLength = atr->outLen;
             fld->Hidden    = atr->hidden;

             ++atr;
            }
          }

        return file;
       }

       virtual void __stdcall freeMetaInfo(BTFileInfo *info)
       {
        free(info);
       }
    };

#endif // _RSLADAPTER_H_

/* <-- EOF --> */