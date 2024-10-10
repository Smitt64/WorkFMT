// Filename: SaxErrorHandler.h
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added registration of wide-char string element handler
//                   and ReportError() helper function.

#ifndef SAX_ERROR_HANDLER_INCLUDED
#define SAX_ERROR_HANDLER_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Use MSXML 4.0.
//#import <msxml4.dll> raw_interfaces_only 
//using namespace MSXML2;
#include <msxml6.h>

#include "XmlElementHandler.h"

// CSaxErrorHandler
// Receive error events during parsing and delegate
// them to the attached element handler.
class CSaxErrorHandler : public ISAXErrorHandler  
{
public:
    CSaxErrorHandler();
    virtual ~CSaxErrorHandler();
    
    virtual long __stdcall          QueryInterface(const struct _GUID &,void ** );
    virtual unsigned long __stdcall AddRef(void);
    virtual unsigned long __stdcall Release(void);
    
    virtual HRESULT STDMETHODCALLTYPE error( 
        /* [in] */ ISAXLocator __RPC_FAR *pLocator,
        /* [in] */ const wchar_t * pwchErrorMessage,
        /* [in] */ HRESULT errCode);
        
    virtual HRESULT STDMETHODCALLTYPE fatalError( 
        /* [in] */ ISAXLocator __RPC_FAR *pLocator,
        /* [in] */ const wchar_t * pwchErrorMessage,
        /* [in] */ HRESULT errCode);
        
    virtual HRESULT STDMETHODCALLTYPE ignorableWarning( 
        /* [in] */ ISAXLocator __RPC_FAR *pLocator,
        /* [in] */ const wchar_t * pwchErrorMessage,
        /* [in] */ HRESULT errCode);
        
    // Attach/detach XML events handler.
    void AttachElementHandler(IXmlElementHandler* pElementHandler);
    void AttachElementHandler(IWXmlElementHandler* pWElementHandler);
    void DetachElementHandler();
    
private:
    void ReportError(
        /* [in] */ ISAXLocator*    pLocator,
        /* [in] */ const wchar_t* pwchErrorMessage,
        /* [in] */ HRESULT         errCode);

    IXmlElementHandler*  m_attachElementHandler;
    IWXmlElementHandler* m_attachWElementHandler;
};

#endif // SAX_ERROR_HANDLER_INCLUDED

// END
