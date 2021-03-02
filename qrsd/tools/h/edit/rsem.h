

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Dec 20 14:37:40 2006
 */
/* Compiler settings for .\RSEM.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
//#error this stub requires an updated version of <rpcndr.h>
// Use those declarations insted of error
#  define MIDL_INTERFACE(x)   struct
#  define DECLSPEC_UUID(x)
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __RSEM_h__
#define __RSEM_h__

// -----------------------------------------------------------------------------
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

/* Forward Declarations */

#ifndef __IRSEditor_FWD_DEFINED__
#define __IRSEditor_FWD_DEFINED__
    typedef interface IRSEditor IRSEditor;
#endif  /* __IRSEditor_FWD_DEFINED__ */

// -----------------------------------------------------------------------------
#ifndef __IRSEditorEx_FWD_DEFINED__
#define __IRSEditorEx_FWD_DEFINED__
    typedef interface IRSEditorEx IRSEditorEx;
#endif  /* __IRSEditorEx_FWD_DEFINED__ */

// -----------------------------------------------------------------------------
#ifndef __IRSEditorEx2_FWD_DEFINED__
#define __IRSEditorEx2_FWD_DEFINED__
    typedef interface IRSEditorEx2 IRSEditorEx2;
#endif  /* __IRSEditorEx2_FWD_DEFINED__ */

// -----------------------------------------------------------------------------
#ifndef __IRSBank_FWD_DEFINED__
#define __IRSBank_FWD_DEFINED__
    typedef interface IRSBank IRSBank;
#endif  /* __IRSBank_FWD_DEFINED__ */

// -----------------------------------------------------------------------------
#ifndef __IRSEManager_FWD_DEFINED__
#define __IRSEManager_FWD_DEFINED__
    typedef interface IRSEManager IRSEManager;
#endif  /* __IRSEManager_FWD_DEFINED__ */

// -----------------------------------------------------------------------------
#ifndef __IRSEManager2_FWD_DEFINED__
#define __IRSEManager2_FWD_DEFINED__
    typedef interface IRSEManager2 IRSEManager2;
#endif  /* __IRSEManager2_FWD_DEFINED__ */

// -----------------------------------------------------------------------------
#ifndef __IRSEditor_FWD_DEFINED__
#define __IRSEditor_FWD_DEFINED__
    typedef interface IRSEditor IRSEditor;
#endif  /* __IRSEditor_FWD_DEFINED__ */

// -----------------------------------------------------------------------------
#ifndef __IRSBank_FWD_DEFINED__
#define __IRSBank_FWD_DEFINED__
    typedef interface IRSBank IRSBank;
#endif  /* __IRSBank_FWD_DEFINED__ */

// -----------------------------------------------------------------------------
#ifndef __IRSEditorEx_FWD_DEFINED__
#define __IRSEditorEx_FWD_DEFINED__
    typedef interface IRSEditorEx IRSEditorEx;
#endif  /* __IRSEditorEx_FWD_DEFINED__ */

// -----------------------------------------------------------------------------
#ifndef __IRSEditorEx2_FWD_DEFINED__
#define __IRSEditorEx2_FWD_DEFINED__
    typedef interface IRSEditorEx2 IRSEditorEx2;
#endif  /* __IRSEditorEx2_FWD_DEFINED__ */


// -----------------------------------------------------------------------------
#ifndef __RSEManager_FWD_DEFINED__
#define __RSEManager_FWD_DEFINED__

    #ifdef __cplusplus
        typedef class RSEManager   RSEManager;
    #else
        typedef struct RSEManager  RSEManager;
    #endif /* __cplusplus */

#endif  /* __RSEManager_FWD_DEFINED__ */

#ifdef __cplusplus
extern "C"{
#endif

// -----------------------------------------------------------------------------
void *__RPC_USER MIDL_user_allocate(size_t);
void  __RPC_USER MIDL_user_free(void *);


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#ifndef __IRSEditor_INTERFACE_DEFINED__
#define __IRSEditor_INTERFACE_DEFINED__

/* interface IRSEditor */
/* [unique][helpstring][dual][uuid][object] */

// -----------------------------------------------------------------------------
EXTERN_C const IID  IID_IRSEditor;

// -----------------------------------------------------------------------------
#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1C8FA5DC-9879-4F06-91F9-8FCC2E05C5A7")
    IRSEditor : public IDispatch
    {
     public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveAllFiles(void) = 0;

        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReloadAllFiles(void) = 0;

        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFile(
                                       /* [in] */          BSTR          fileName,
                                       /* [in] */          int           mode,
                                       /* [in] */          VARIANT_BOOL  newView,
                                       /* [retval][out] */ VARIANT_BOOL *stat) = 0;

        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CheckOut(
                                       /* [in] */          BSTR          fileName,
                                       /* [retval][out] */ VARIANT_BOOL *stat) = 0;

        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CheckIn(
                                       /* [in] */          BSTR          fileName,
                                       /* [retval][out] */ VARIANT_BOOL *stat) = 0;

        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UndoCheckOut(
                                       /* [in] */ BSTR fileName) = 0;

    };

#else   /* C style interface */

    typedef struct IRSEditorVtbl
          {
           BEGIN_INTERFACE

              HRESULT(STDMETHODCALLTYPE *QueryInterface)(
                                            IRSEditor                    *This,
                                            /* [in] */          REFIID    riid,
                                            /* [iid_is][out] */ void    **ppvObject);

              ULONG(STDMETHODCALLTYPE *AddRef)(IRSEditor *This);

              ULONG(STDMETHODCALLTYPE *Release)(IRSEditor *This);

              HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
                                            IRSEditor        *This,
                                            /* [out] */ UINT *pctinfo);

              HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
                                            IRSEditor              *This,
                                            /* [in] */  UINT        iTInfo,
                                            /* [in] */  LCID        lcid,
                                            /* [out] */ ITypeInfo **ppTInfo);

              HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
                                            IRSEditor                     *This,
                                            /* [in] */           REFIID    riid,
                                            /* [size_is][in] */  LPOLESTR *rgszNames,
                                            /* [in] */           UINT      cNames,
                                            /* [in] */           LCID      lcid,
                                            /* [size_is][out] */ DISPID   *rgDispId);

              /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
                                            IRSEditor                  *This,
                                            /* [in] */      DISPID      dispIdMember,
                                            /* [in] */      REFIID      riid,
                                            /* [in] */      LCID        lcid,
                                            /* [in] */      WORD        wFlags,
                                            /* [out][in] */ DISPPARAMS *pDispParams,
                                            /* [out] */     VARIANT    *pVarResult,
                                            /* [out] */     EXCEPINFO  *pExcepInfo,
                                            /* [out] */     UINT       *puArgErr);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *SaveAllFiles)(IRSEditor *This);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *ReloadAllFiles)(IRSEditor *This);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *LoadFile)(
                                            IRSEditor                        *This,
                                            /* [in] */          BSTR          fileName,
                                            /* [in] */          int           mode,
                                            /* [in] */          VARIANT_BOOL  newView,
                                            /* [retval][out] */ VARIANT_BOOL *stat);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *CheckOut)(
                                            IRSEditor                        *This,
                                            /* [in] */          BSTR          fileName,
                                            /* [retval][out] */ VARIANT_BOOL *stat);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *CheckIn)(
                                            IRSEditor                        *This,
                                            /* [in] */          BSTR          fileName,
                                            /* [retval][out] */ VARIANT_BOOL *stat);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *UndoCheckOut)(
                                            IRSEditor       *This,
                                            /* [in] */ BSTR  fileName);

           END_INTERFACE
          } IRSEditorVtbl;

    interface IRSEditor
    {
     CONST_VTBL struct IRSEditorVtbl  *lpVtbl;
    };

    // -----------------------------------------------------------------------------
    #ifdef COBJMACROS

        #define  IRSEditor_QueryInterface(This,riid,ppvObject)   \
                           (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

        #define  IRSEditor_AddRef(This)  \
                           (This)->lpVtbl -> AddRef(This)

        #define  IRSEditor_Release(This) \
                           (This)->lpVtbl -> Release(This)


        #define  IRSEditor_GetTypeInfoCount(This,pctinfo)        \
                           (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

        #define  IRSEditor_GetTypeInfo(This,iTInfo,lcid,ppTInfo) \
                           (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

        #define  IRSEditor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)       \
                           (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

        #define  IRSEditor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) \
                           (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


        #define  IRSEditor_SaveAllFiles(This)    \
                           (This)->lpVtbl -> SaveAllFiles(This)

        #define  IRSEditor_ReloadAllFiles(This)  \
                           (This)->lpVtbl -> ReloadAllFiles(This)

        #define  IRSEditor_LoadFile(This,fileName,mode,newView,stat)     \
                           (This)->lpVtbl -> LoadFile(This,fileName,mode,newView,stat)

        #define  IRSEditor_CheckOut(This,fileName,stat)  \
                           (This)->lpVtbl -> CheckOut(This,fileName,stat)

        #define  IRSEditor_CheckIn(This,fileName,stat)   \
                           (This)->lpVtbl -> CheckIn(This,fileName,stat)

        #define  IRSEditor_UndoCheckOut(This,fileName)   \
                           (This)->lpVtbl -> UndoCheckOut(This,fileName)

    #endif /* COBJMACROS */


#endif  /* C style interface */


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEditor_SaveAllFiles_Proxy(IRSEditor *This);

void __RPC_STUB IRSEditor_SaveAllFiles_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                            PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEditor_ReloadAllFiles_Proxy(IRSEditor *This);

void __RPC_STUB IRSEditor_ReloadAllFiles_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                              PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEditor_LoadFile_Proxy(
                               IRSEditor                        *This,
                               /* [in] */          BSTR          fileName,
                               /* [in] */          int           mode,
                               /* [in] */          VARIANT_BOOL  newView,
                               /* [retval][out] */ VARIANT_BOOL *stat);

void __RPC_STUB IRSEditor_LoadFile_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                        PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEditor_CheckOut_Proxy(
                               IRSEditor                        *This,
                               /* [in] */          BSTR          fileName,
                               /* [retval][out] */ VARIANT_BOOL *stat);

void __RPC_STUB IRSEditor_CheckOut_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                        PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEditor_CheckIn_Proxy(
                               IRSEditor                        *This,
                               /* [in] */          BSTR          fileName,
                               /* [retval][out] */ VARIANT_BOOL *stat);

void __RPC_STUB IRSEditor_CheckIn_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                       PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEditor_UndoCheckOut_Proxy(
                               IRSEditor       *This,
                               /* [in] */ BSTR  fileName);

void __RPC_STUB IRSEditor_UndoCheckOut_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                            PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

#endif  /* __IRSEditor_INTERFACE_DEFINED__ */


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#ifndef __IRSEditorEx_INTERFACE_DEFINED__
#define __IRSEditorEx_INTERFACE_DEFINED__

/* interface IRSEditorEx */
/* [unique][helpstring][dual][uuid][object] */

// -----------------------------------------------------------------------------
EXTERN_C const IID  IID_IRSEditorEx;

// -----------------------------------------------------------------------------
#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("13581642-E261-46c9-AF09-535D7285B8E0")
    IRSEditorEx : public IDispatch
    {
     public:
       virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDesignerId(
                                      /* [in] */ DWORD id) = 0;

    };

#else   /* C style interface */

    typedef struct IRSEditorExVtbl
          {
           BEGIN_INTERFACE

              HRESULT(STDMETHODCALLTYPE *QueryInterface)(
                                     IRSEditorEx                *This,
                                     /* [in] */          REFIID  riid,
                                     /* [iid_is][out] */ void  **ppvObject);

              ULONG(STDMETHODCALLTYPE *AddRef)(IRSEditorEx *This);

              ULONG(STDMETHODCALLTYPE *Release)(IRSEditorEx *This);

              HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
                                     IRSEditorEx      *This,
                                     /* [out] */ UINT *pctinfo);

              HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
                                     IRSEditorEx            *This,
                                     /* [in] */  UINT        iTInfo,
                                     /* [in] */  LCID        lcid,
                                     /* [out] */ ITypeInfo **ppTInfo);

              HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
                                     IRSEditorEx                   *This,
                                     /* [in] */           REFIID    riid,
                                     /* [size_is][in] */  LPOLESTR *rgszNames,
                                     /* [in] */           UINT      cNames,
                                     /* [in] */           LCID      lcid,
                                     /* [size_is][out] */ DISPID   *rgDispId);

              /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
                                     IRSEditorEx                *This,
                                     /* [in] */      DISPID      dispIdMember,
                                     /* [in] */      REFIID      riid,
                                     /* [in] */      LCID        lcid,
                                     /* [in] */      WORD        wFlags,
                                     /* [out][in] */ DISPPARAMS *pDispParams,
                                     /* [out] */     VARIANT    *pVarResult,
                                     /* [out] */     EXCEPINFO  *pExcepInfo,
                                     /* [out] */     UINT       *puArgErr);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *SetDesignerId)(
                                     IRSEditorEx      *This,
                                     /* [in] */ DWORD  id);

           END_INTERFACE
          } IRSEditorExVtbl;

    interface IRSEditorEx
    {
     CONST_VTBL struct IRSEditorExVtbl  *lpVtbl;
    };


    // -----------------------------------------------------------------------------
    #ifdef COBJMACROS

        #define  IRSEditorEx_QueryInterface(This,riid,ppvObject) \
                             (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

        #define  IRSEditorEx_AddRef(This)        \
                             (This)->lpVtbl -> AddRef(This)

        #define  IRSEditorEx_Release(This)       \
                             (This)->lpVtbl -> Release(This)


        #define  IRSEditorEx_GetTypeInfoCount(This,pctinfo)      \
                             (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

        #define  IRSEditorEx_GetTypeInfo(This,iTInfo,lcid,ppTInfo)       \
                             (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

        #define  IRSEditorEx_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)     \
                             (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

        #define  IRSEditorEx_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)       \
                             (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

        #define  IRSEditorEx_SetDesignerId(This,id)      \
                             (This)->lpVtbl -> SetDesignerId(This,id)

    #endif /* COBJMACROS */

#endif  /* C style interface */


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEditorEx_SetDesignerId_Proxy(
                       IRSEditorEx      *This,
                       /* [in] */ DWORD  id);


void __RPC_STUB IRSEditorEx_SetDesignerId_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                               PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

#endif  /* __IRSEditorEx_INTERFACE_DEFINED__ */


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#ifndef __IRSEditorEx2_INTERFACE_DEFINED__
#define __IRSEditorEx2_INTERFACE_DEFINED__

/* interface IRSEditorEx2 */
/* [unique][helpstring][dual][uuid][object] */

// -----------------------------------------------------------------------------
EXTERN_C const IID  IID_IRSEditorEx2;

// -----------------------------------------------------------------------------
#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("97BE82F8-9FEC-401b-8DAB-51D9528A19F3")
    IRSEditorEx2 : public IDispatch
    {
     public:
       virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRslDirs(
                                      /* [retval][out] */ BSTR *ret) = 0;

       virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindServerFile(
                                      /* [in] */          BSTR   fileName,
                                      /* [out] */         BSTR  *localName,
                                      /* [out] */         BSTR  *remoteName,
                                      /* [retval][out] */ DWORD *exist) = 0;

    };

#else   /* C style interface */

    typedef struct IRSEditorEx2Vtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
            IRSEditorEx2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);

        ULONG ( STDMETHODCALLTYPE *AddRef )(
            IRSEditorEx2 * This);

        ULONG ( STDMETHODCALLTYPE *Release )(
            IRSEditorEx2 * This);

        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )(
            IRSEditorEx2 * This,
            /* [out] */ UINT *pctinfo);

        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )(
            IRSEditorEx2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);

        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )(
            IRSEditorEx2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )(
            IRSEditorEx2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);

        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRslDirs )(
            IRSEditorEx2 * This,
            /* [retval][out] */ BSTR *ret);

        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindServerFile )(
            IRSEditorEx2 * This,
            /* [in] */ BSTR fileName,
            /* [out] */ BSTR *localName,
            /* [out] */ BSTR *remoteName,
            /* [retval][out] */ DWORD *exist);

        END_INTERFACE
    } IRSEditorEx2Vtbl;

    interface IRSEditorEx2
    {
        CONST_VTBL struct IRSEditorEx2Vtbl *lpVtbl;
    };


    // -----------------------------------------------------------------------------
    #ifdef COBJMACROS

        #define  IRSEditorEx2_QueryInterface(This,riid,ppvObject)        \
                              (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

        #define  IRSEditorEx2_AddRef(This)       \
                              (This)->lpVtbl -> AddRef(This)

        #define  IRSEditorEx2_Release(This)      \
                              (This)->lpVtbl -> Release(This)


        #define  IRSEditorEx2_GetTypeInfoCount(This,pctinfo)     \
                              (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

        #define  IRSEditorEx2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)      \
                              (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

        #define  IRSEditorEx2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
                              (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

        #define  IRSEditorEx2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)      \
                              (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


        #define  IRSEditorEx2_GetRslDirs(This,ret)       \
                              (This)->lpVtbl -> GetRslDirs(This,ret)

        #define  IRSEditorEx2_FindServerFile(This,fileName,localName,remoteName,exist)   \
                              (This)->lpVtbl -> FindServerFile(This,fileName,localName,remoteName,exist)

    #endif /* COBJMACROS */


#endif  /* C style interface */

// -----------------------------------------------------------------------------
/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEditorEx2_GetRslDirs_Proxy(
                                                              IRSEditorEx2             *This,
                                                              /* [retval][out] */ BSTR *ret);

// -----------------------------------------------------------------------------
void __RPC_STUB IRSEditorEx2_GetRslDirs_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                             PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

// -----------------------------------------------------------------------------
/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEditorEx2_FindServerFile_Proxy(
                                                              IRSEditorEx2              *This,
                                                              /* [in] */          BSTR   fileName,
                                                              /* [out] */         BSTR  *localName,
                                                              /* [out] */         BSTR  *remoteName,
                                                              /* [retval][out] */ DWORD *exist);

// -----------------------------------------------------------------------------
void __RPC_STUB IRSEditorEx2_FindServerFile_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                                 PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

#endif  /* __IRSEditorEx2_INTERFACE_DEFINED__ */


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#ifndef __IRSBank_INTERFACE_DEFINED__
#define __IRSBank_INTERFACE_DEFINED__

/* interface IRSBank */
/* [unique][helpstring][dual][uuid][object] */

// -----------------------------------------------------------------------------
EXTERN_C const IID IID_IRSBank;

// -----------------------------------------------------------------------------
#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1B19C591-644D-4F14-A8BB-8AC8799E0F61")
    IRSBank : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CallRsBank(
            /* [in] */ long id,
            /* [in] */ BSTR parms,
            /* [retval][out] */ VARIANT_BOOL *stat) = 0;

    };

#else   /* C style interface */

    typedef struct IRSBankVtbl
          {
           BEGIN_INTERFACE

              HRESULT(STDMETHODCALLTYPE *QueryInterface)(
                                     IRSBank                     *This,
                                     /* [in] */          REFIID   riid,
                                     /* [iid_is][out] */ void   **ppvObject);

              ULONG(STDMETHODCALLTYPE *AddRef)(IRSBank *This);

              ULONG(STDMETHODCALLTYPE *Release)(IRSBank *This);

              HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
                                     IRSBank          *This,
                                     /* [out] */ UINT *pctinfo);

              HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
                                     IRSBank                *This,
                                     /* [in] */  UINT        iTInfo,
                                     /* [in] */  LCID        lcid,
                                     /* [out] */ ITypeInfo **ppTInfo);

              HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
                                     IRSBank                       *This,
                                     /* [in] */           REFIID    riid,
                                     /* [size_is][in] */  LPOLESTR *rgszNames,
                                     /* [in] */           UINT      cNames,
                                     /* [in] */           LCID      lcid,
                                     /* [size_is][out] */ DISPID   *rgDispId);

              /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
                                     IRSBank                    *This,
                                     /* [in] */      DISPID      dispIdMember,
                                     /* [in] */      REFIID      riid,
                                     /* [in] */      LCID        lcid,
                                     /* [in] */      WORD        wFlags,
                                     /* [out][in] */ DISPPARAMS *pDispParams,
                                     /* [out] */     VARIANT    *pVarResult,
                                     /* [out] */     EXCEPINFO  *pExcepInfo,
                                     /* [out] */     UINT       *puArgErr);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *CallRsBank)(
                                     IRSBank                          *This,
                                     /* [in] */          long          id,
                                     /* [in] */          BSTR          parms,
                                     /* [retval][out] */ VARIANT_BOOL *stat);

           END_INTERFACE
          } IRSBankVtbl;

    interface IRSBank
    {
     CONST_VTBL struct IRSBankVtbl  *lpVtbl;
    };


    // -----------------------------------------------------------------------------
    #ifdef COBJMACROS

        #define  IRSBank_QueryInterface(This,riid,ppvObject)     \
                         (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

        #define  IRSBank_AddRef(This)    \
                         (This)->lpVtbl -> AddRef(This)

        #define  IRSBank_Release(This)   \
                         (This)->lpVtbl -> Release(This)


        #define  IRSBank_GetTypeInfoCount(This,pctinfo)  \
                         (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

        #define  IRSBank_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
                         (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

        #define  IRSBank_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) \
                         (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

        #define  IRSBank_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
                         (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


        #define  IRSBank_CallRsBank(This,id,parms,stat)  \
                         (This)->lpVtbl -> CallRsBank(This,id,parms,stat)

    #endif /* COBJMACROS */

#endif  /* C style interface */

// -----------------------------------------------------------------------------
/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSBank_CallRsBank_Proxy(
                                                 IRSBank                          *This,
                                                 /* [in] */          long          id,
                                                 /* [in] */          BSTR          parms,
                                                 /* [retval][out] */ VARIANT_BOOL *stat);

// -----------------------------------------------------------------------------
void __RPC_STUB IRSBank_CallRsBank_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                        PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);


#endif  /* __IRSBank_INTERFACE_DEFINED__ */


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#ifndef __IRSEManager_INTERFACE_DEFINED__
#define __IRSEManager_INTERFACE_DEFINED__

/* interface IRSEManager */
/* [unique][helpstring][dual][uuid][object] */

// -----------------------------------------------------------------------------
EXTERN_C const IID  IID_IRSEManager;

// -----------------------------------------------------------------------------
#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9550EE92-694D-46D2-A999-EFE2BF31306F")
    IRSEManager : public IDispatch
    {
     public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RegisterEditor(
                                       /* [in] */  IUnknown *pEd,
                                       /* [in] */  BSTR      displayName,
                                       /* [out] */ DWORD    *cookie) = 0;

        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnregisterEditor(
                                       /* [in] */  DWORD cookie) = 0;

        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCount(
                                       /* [out] */ DWORD *count) = 0;

        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInfoById(
                                       /* [in] */  DWORD  id,
                                       /* [out] */ BSTR  *displayName,
                                       /* [out] */ DWORD *cookie) = 0;

        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetEditor(
                                       /* [in] */  DWORD      cookie,
                                       /* [out] */ IUnknown **ret) = 0;

    };

#else   /* C style interface */

    typedef struct IRSEManagerVtbl
          {
           BEGIN_INTERFACE

              HRESULT(STDMETHODCALLTYPE *QueryInterface)(
                                     IRSEManager                *This,
                                     /* [in] */          REFIID  riid,
                                     /* [iid_is][out] */ void  **ppvObject);

              ULONG(STDMETHODCALLTYPE *AddRef)(IRSEManager *This);

              ULONG(STDMETHODCALLTYPE *Release)(IRSEManager *This);

              HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
                                     IRSEManager      *This,
                                     /* [out] */ UINT *pctinfo);

              HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
                                     IRSEManager            *This,
                                     /* [in] */  UINT        iTInfo,
                                     /* [in] */  LCID        lcid,
                                     /* [out] */ ITypeInfo **ppTInfo);

              HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
                                     IRSEManager                   *This,
                                     /* [in] */           REFIID    riid,
                                     /* [size_is][in] */  LPOLESTR *rgszNames,
                                     /* [in] */           UINT      cNames,
                                     /* [in] */           LCID      lcid,
                                     /* [size_is][out] */ DISPID   *rgDispId);

              /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
                                     IRSEManager                *This,
                                     /* [in] */      DISPID      dispIdMember,
                                     /* [in] */      REFIID      riid,
                                     /* [in] */      LCID        lcid,
                                     /* [in] */      WORD        wFlags,
                                     /* [out][in] */ DISPPARAMS *pDispParams,
                                     /* [out] */     VARIANT    *pVarResult,
                                     /* [out] */     EXCEPINFO  *pExcepInfo,
                                     /* [out] */     UINT       *puArgErr);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *RegisterEditor)(
                                     IRSEManager          *This,
                                     /* [in] */  IUnknown *pEd,
                                     /* [in] */  BSTR      displayName,
                                     /* [out] */ DWORD    *cookie);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *UnregisterEditor)(
                                     IRSEManager      *This,
                                     /* [in] */ DWORD  cookie);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *GetCount)(
                                     IRSEManager       *This,
                                     /* [out] */ DWORD *count);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *GetInfoById)(
                                     IRSEManager       *This,
                                     /* [in] */  DWORD  id,
                                     /* [out] */ BSTR  *displayName,
                                     /* [out] */ DWORD *cookie);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *GetEditor)(
                                     IRSEManager           *This,
                                     /* [in] */  DWORD      cookie,
                                     /* [out] */ IUnknown **ret);

           END_INTERFACE
          } IRSEManagerVtbl;

    interface IRSEManager
    {
     CONST_VTBL struct IRSEManagerVtbl  *lpVtbl;
    };

    // -----------------------------------------------------------------------------
    #ifdef COBJMACROS

        #define  IRSEManager_QueryInterface(This,riid,ppvObject) \
                             (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

        #define  IRSEManager_AddRef(This)        \
                             (This)->lpVtbl -> AddRef(This)

        #define  IRSEManager_Release(This)       \
                             (This)->lpVtbl -> Release(This)

        #define  IRSEManager_GetTypeInfoCount(This,pctinfo)      \
                             (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

        #define  IRSEManager_GetTypeInfo(This,iTInfo,lcid,ppTInfo)       \
                             (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

        #define  IRSEManager_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)     \
                             (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

        #define  IRSEManager_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)       \
                             (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

        #define  IRSEManager_RegisterEditor(This,pEd,displayName,cookie) \
                             (This)->lpVtbl -> RegisterEditor(This,pEd,displayName,cookie)

        #define  IRSEManager_UnregisterEditor(This,cookie)       \
                             (This)->lpVtbl -> UnregisterEditor(This,cookie)

        #define  IRSEManager_GetCount(This,count)        \
                             (This)->lpVtbl -> GetCount(This,count)

        #define  IRSEManager_GetInfoById(This,id,displayName,cookie)     \
                             (This)->lpVtbl -> GetInfoById(This,id,displayName,cookie)

        #define  IRSEManager_GetEditor(This,cookie,ret)  \
                             (This)->lpVtbl -> GetEditor(This,cookie,ret)

    #endif /* COBJMACROS */

#endif  /* C style interface */


// -----------------------------------------------------------------------------
/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEManager_RegisterEditor_Proxy(
                       IRSEManager          *This,
                       /* [in] */  IUnknown *pEd,
                       /* [in] */  BSTR      displayName,
                       /* [out] */ DWORD    *cookie);

// -----------------------------------------------------------------------------
void __RPC_STUB IRSEManager_RegisterEditor_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                                PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

// -----------------------------------------------------------------------------
/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEManager_UnregisterEditor_Proxy(
                       IRSEManager      *This,
                       /* [in] */ DWORD  cookie);

// -----------------------------------------------------------------------------
void __RPC_STUB IRSEManager_UnregisterEditor_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                                  PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

// -----------------------------------------------------------------------------
/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEManager_GetCount_Proxy(
                       IRSEManager       *This,
                       /* [out] */ DWORD *count);

// -----------------------------------------------------------------------------
void __RPC_STUB IRSEManager_GetCount_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                          PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

// -----------------------------------------------------------------------------
/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEManager_GetInfoById_Proxy(
                       IRSEManager       *This,
                       /* [in] */  DWORD  id,
                       /* [out] */ BSTR  *displayName,
                       /* [out] */ DWORD *cookie);

// -----------------------------------------------------------------------------
void __RPC_STUB IRSEManager_GetInfoById_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                             PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEManager_GetEditor_Proxy(
                       IRSEManager           *This,
                       /* [in] */  DWORD      cookie,
                       /* [out] */ IUnknown **ret);

// -----------------------------------------------------------------------------
void __RPC_STUB IRSEManager_GetEditor_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                           PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

#endif  /* __IRSEManager_INTERFACE_DEFINED__ */


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#ifndef __IRSEManager2_INTERFACE_DEFINED__
#define __IRSEManager2_INTERFACE_DEFINED__

/* interface IRSEManager2 */
/* [unique][helpstring][dual][uuid][object] */

// -----------------------------------------------------------------------------
EXTERN_C const IID  IID_IRSEManager2;

// -----------------------------------------------------------------------------
#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A45D345E-8974-4ce5-AFA1-A80F18A6E86A")
    IRSEManager2 : public IRSEManager
    {
     public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RegisterEditorEx(
                                       /* [in] */  IUnknown *pEd,
                                       /* [in] */  BSTR      displayName,
                                       /* [in] */  DWORD     procId,
                                       /* [out] */ DWORD    *cookie) = 0;

        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetEditorEx(
                                       /* [in] */  DWORD      cookie,
                                       /* [out] */ IUnknown **ret,
                                       /* [out] */ DWORD     *procId) = 0;
    };

#else   /* C style interface */

    typedef struct IRSEManager2Vtbl
          {
           BEGIN_INTERFACE

              HRESULT(STDMETHODCALLTYPE *QueryInterface)(
                                   IRSEManager2 *This,
                                   /* [in] */          REFIID   riid,
                                   /* [iid_is][out] */ void   **ppvObject);

              ULONG(STDMETHODCALLTYPE *AddRef)(IRSEManager2 *This);

              ULONG(STDMETHODCALLTYPE *Release)(IRSEManager2 *This);

              HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
                                   IRSEManager2     *This,
                                   /* [out] */ UINT *pctinfo);

              HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
                                   IRSEManager2           *This,
                                   /* [in] */  UINT        iTInfo,
                                   /* [in] */  LCID        lcid,
                                   /* [out] */ ITypeInfo **ppTInfo);

              HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
                                   IRSEManager2                  *This,
                                   /* [in] */           REFIID    riid,
                                   /* [size_is][in] */  LPOLESTR *rgszNames,
                                   /* [in] */           UINT      cNames,
                                   /* [in] */           LCID      lcid,
                                   /* [size_is][out] */ DISPID   *rgDispId);

              /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
                                   IRSEManager2               *This,
                                   /* [in] */      DISPID      dispIdMember,
                                   /* [in] */      REFIID      riid,
                                   /* [in] */      LCID        lcid,
                                   /* [in] */      WORD        wFlags,
                                   /* [out][in] */ DISPPARAMS *pDispParams,
                                   /* [out] */     VARIANT    *pVarResult,
                                   /* [out] */     EXCEPINFO  *pExcepInfo,
                                   /* [out] */     UINT       *puArgErr);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *RegisterEditor)(
                                   IRSEManager2         *This,
                                   /* [in] */  IUnknown *pEd,
                                   /* [in] */  BSTR      displayName,
                                   /* [out] */ DWORD    *cookie);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *UnregisterEditor)(
                                   IRSEManager2     *This,
                                   /* [in] */  DWORD cookie);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *GetCount)(
                                   IRSEManager2      *This,
                                   /* [out] */ DWORD *count);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *GetInfoById)(
                                   IRSEManager2      *This,
                                   /* [in] */  DWORD  id,
                                   /* [out] */ BSTR  *displayName,
                                   /* [out] */ DWORD *cookie);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *GetEditor)(
                                   IRSEManager2          *This,
                                   /* [in] */  DWORD      cookie,
                                   /* [out] */ IUnknown **ret);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *RegisterEditorEx)(
                                   IRSEManager2         *This,
                                   /* [in] */  IUnknown *pEd,
                                   /* [in] */  BSTR      displayName,
                                   /* [in] */  DWORD     procId,
                                   /* [out] */ DWORD    *cookie);

              /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *GetEditorEx)(
                                   IRSEManager2          *This,
                                   /* [in] */  DWORD      cookie,
                                   /* [out] */ IUnknown **ret,
                                   /* [out] */ DWORD     *procId);

           END_INTERFACE
          } IRSEManager2Vtbl;

    interface IRSEManager2
    {
     CONST_VTBL struct IRSEManager2Vtbl  *lpVtbl;
    };

    // -----------------------------------------------------------------------------
    #ifdef COBJMACROS

        #define  IRSEManager2_QueryInterface(This,riid,ppvObject)        \
                              (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

        #define  IRSEManager2_AddRef(This)       \
                              (This)->lpVtbl -> AddRef(This)

        #define  IRSEManager2_Release(This)      \
                              (This)->lpVtbl -> Release(This)


        #define  IRSEManager2_GetTypeInfoCount(This,pctinfo)     \
                              (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

        #define  IRSEManager2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)      \
                              (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

        #define  IRSEManager2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
                              (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

        #define  IRSEManager2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)      \
                              (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


        #define  IRSEManager2_RegisterEditor(This,pEd,displayName,cookie)        \
                              (This)->lpVtbl -> RegisterEditor(This,pEd,displayName,cookie)

        #define  IRSEManager2_UnregisterEditor(This,cookie)      \
                              (This)->lpVtbl -> UnregisterEditor(This,cookie)

        #define  IRSEManager2_GetCount(This,count)       \
                              (This)->lpVtbl -> GetCount(This,count)

        #define  IRSEManager2_GetInfoById(This,id,displayName,cookie)    \
                              (This)->lpVtbl -> GetInfoById(This,id,displayName,cookie)

        #define  IRSEManager2_GetEditor(This,cookie,ret) \
                              (This)->lpVtbl -> GetEditor(This,cookie,ret)


        #define  IRSEManager2_RegisterEditorEx(This,pEd,displayName,procId,cookie)       \
                              (This)->lpVtbl -> RegisterEditorEx(This,pEd,displayName,procId,cookie)

        #define  IRSEManager2_GetEditorEx(This,cookie,ret,procId)        \
                              (This)->lpVtbl -> GetEditorEx(This,cookie,ret,procId)

    #endif /* COBJMACROS */

#endif  /* C style interface */

// -----------------------------------------------------------------------------
/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEManager2_RegisterEditorEx_Proxy(
                                                              IRSEManager2         *This,
                                                              /* [in] */  IUnknown *pEd,
                                                              /* [in] */  BSTR      displayName,
                                                              /* [in] */  DWORD     procId,
                                                              /* [out] */ DWORD    *cookie);

// -----------------------------------------------------------------------------
void __RPC_STUB IRSEManager2_RegisterEditorEx_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                                   PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);

// -----------------------------------------------------------------------------
/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRSEManager2_GetEditorEx_Proxy(
                                                              IRSEManager2          *This,
                                                              /* [in] */  DWORD      cookie,
                                                              /* [out] */ IUnknown **ret,
                                                              /* [out] */ DWORD     *procId);

// -----------------------------------------------------------------------------
void __RPC_STUB IRSEManager2_GetEditorEx_Stub(IRpcStubBuffer *This, IRpcChannelBuffer *_pRpcChannelBuffer,
                                              PRPC_MESSAGE _pRpcMessage, DWORD *_pdwStubPhase);


#endif  /* __IRSEManager2_INTERFACE_DEFINED__ */


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#ifndef __RSEMLib_LIBRARY_DEFINED__
#define __RSEMLib_LIBRARY_DEFINED__

    /* library RSEMLib */
    /* [helpstring][version][uuid] */

    // -----------------------------------------------------------------------------
    EXTERN_C const IID    LIBID_RSEMLib;
    EXTERN_C const CLSID  CLSID_RSEManager;

    #ifdef __cplusplus
        class DECLSPEC_UUID("8A3B5358-17E1-4608-811D-653819A1B435")
        RSEManager;
    #endif
#endif /* __RSEMLib_LIBRARY_DEFINED__ */

// -----------------------------------------------------------------------------
/* Additional Prototypes for ALL interfaces */
unsigned long  __RPC_USER BSTR_UserSize     (unsigned long *, unsigned long,   BSTR *);
unsigned char *__RPC_USER BSTR_UserMarshal  (unsigned long *, unsigned char *, BSTR *);
unsigned char *__RPC_USER BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR *);
void           __RPC_USER BSTR_UserFree     (unsigned long *, BSTR *);

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */