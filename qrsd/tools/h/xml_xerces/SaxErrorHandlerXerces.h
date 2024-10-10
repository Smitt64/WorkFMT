// Filename: SaxErrorHandler.h
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added registration of wide-char string element handler
//                   and ReportError() helper function.

#ifndef SAX_ERROR_HANDLER_XERCES_INCLUDED
#define SAX_ERROR_HANDLER_XERCES_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Use MSXML 4.0.
//#import <msxml4.dll> raw_interfaces_only 
//using namespace MSXML2;
#include <msxml6.h>

#include "XmlElementHandler.h"
#include <xercesc/sax2/DefaultHandler.hpp>

XERCES_CPP_NAMESPACE_USE

// CSaxErrorHandler
// Receive error events during parsing and delegate
// them to the attached element handler.
class CSaxErrorHandlerXerces : public DefaultHandler  
{
public:
    CSaxErrorHandlerXerces();
    virtual ~CSaxErrorHandlerXerces();
    
    void error(const SAXParseException& exc) override;
    //virtual HRESULT STDMETHODCALLTYPE error( 
    //    /* [in] */ ISAXLocator __RPC_FAR *pLocator,
    //    /* [in] */ const wchar_t * pwchErrorMessage,
    //    /* [in] */ HRESULT errCode);
      
    void fatalError(const SAXParseException& exc) override;
    //virtual HRESULT STDMETHODCALLTYPE fatalError( 
    //    /* [in] */ ISAXLocator __RPC_FAR *pLocator,
    //    /* [in] */ const wchar_t * pwchErrorMessage,
    //    /* [in] */ HRESULT errCode);
        
    void warning(const SAXParseException& exc) override;
    //virtual HRESULT STDMETHODCALLTYPE ignorableWarning( 
    //    /* [in] */ ISAXLocator __RPC_FAR *pLocator,
    //    /* [in] */ const wchar_t * pwchErrorMessage,
    //    /* [in] */ HRESULT errCode);
        
    // Attach/detach XML events handler.
    void AttachElementHandler(IXmlElementHandler* pElementHandler);
    void AttachElementHandler(IWXmlElementHandler* pWElementHandler);
    void DetachElementHandler();
    
private:
    void ReportError(
        const SAXParseException& exc);

    IXmlElementHandler*  m_attachElementHandler;
    IWXmlElementHandler* m_attachWElementHandler;
};

#endif // SAX_ERROR_HANDLER_INCLUDED

// END
