// Filename: SaxContentHandler.h
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added registration of wide-char string element handler.

#ifndef SAX_CONTENT_HANDLER_INCLUDED
#define SAX_CONTENT_HANDLER_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Use MSXML 4.0.
//#import <msxml4.dll> raw_interfaces_only 
//using namespace MSXML2;
#include <msxml6.h>

#include "XmlElementHandler.h"

// CSaxContentHandler
// Receive content events during parsing and delegate
// them to the attached element handler.
class CSaxContentHandler : public ISAXContentHandler
{
public:
    CSaxContentHandler();
    virtual ~CSaxContentHandler();
    
    virtual long __stdcall          QueryInterface(const struct _GUID &, void **);
    virtual unsigned long __stdcall AddRef(void);
    virtual unsigned long __stdcall Release(void);
    
    virtual HRESULT STDMETHODCALLTYPE putDocumentLocator( 
        /* [in] */ ISAXLocator __RPC_FAR *pLocator);
        
    virtual HRESULT STDMETHODCALLTYPE startDocument(void);
    
    virtual HRESULT STDMETHODCALLTYPE endDocument(void);
    
    virtual HRESULT STDMETHODCALLTYPE startPrefixMapping( 
        /* [in] */ const wchar_t __RPC_FAR *pwchPrefix,
        /* [in] */ int cchPrefix,
        /* [in] */ const wchar_t __RPC_FAR *pwchUri,
        /* [in] */ int cchUri);
        
    virtual HRESULT STDMETHODCALLTYPE endPrefixMapping( 
        /* [in] */ const wchar_t __RPC_FAR *pwchPrefix,
        /* [in] */ int cchPrefix);
        
    virtual HRESULT STDMETHODCALLTYPE startElement( 
            /* [in] */ const wchar_t *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ const wchar_t *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ const wchar_t *pwchQName,
            /* [in] */ int cchQName,
            /* [in] */ ISAXAttributes *pAttributes);
        
    virtual HRESULT STDMETHODCALLTYPE endElement( 
        /* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
        /* [in] */ int cchNamespaceUri,
        /* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
        /* [in] */ int cchLocalName,
        /* [in] */ const wchar_t __RPC_FAR *pwchRawName,
        /* [in] */ int cchRawName);
        
    virtual HRESULT STDMETHODCALLTYPE characters( 
        /* [in] */ const wchar_t __RPC_FAR *pwchChars,
        /* [in] */ int cchChars);
        
    virtual HRESULT STDMETHODCALLTYPE ignorableWhitespace( 
        /* [in] */ const wchar_t __RPC_FAR *pwchChars,
        /* [in] */ int cchChars);
        
    virtual HRESULT STDMETHODCALLTYPE processingInstruction( 
        /* [in] */ const wchar_t __RPC_FAR *pwchTarget,
        /* [in] */ int cchTarget,
        /* [in] */ const wchar_t __RPC_FAR *pwchData,
        /* [in] */ int cchData);
        
    virtual HRESULT STDMETHODCALLTYPE skippedEntity( 
        /* [in] */ const wchar_t __RPC_FAR *pwchName,
        /* [in] */ int cchName);    
        
    // Attach/detach XML events handler.
    void AttachElementHandler(IXmlElementHandler*  pElementHandler);
    void AttachElementHandler(IWXmlElementHandler* pWElementHandler);
    void DetachElementHandler();
    
private:
    int  m_depth;
    
    IXmlElementHandler*  m_attachElementHandler;
    IWXmlElementHandler* m_attachWElementHandler;
};

#endif // SAX_CONTENT_HANDLER_INCLUDED

// END
