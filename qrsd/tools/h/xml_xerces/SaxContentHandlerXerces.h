// Filename: SaxContentHandler.h
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added registration of wide-char string element handler.

#ifndef SAX_CONTENT_HANDLER_XERCES_INCLUDED
#define SAX_CONTENT_HANDLER_XERCES_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XmlElementHandler.h"

#include <xercesc/sax2/DefaultHandler.hpp>

XERCES_CPP_NAMESPACE_USE

// CSaxContentHandler
// Receive content events during parsing and delegate
// them to the attached element handler.
class CSaxContentHandlerXerces : public  DefaultHandler
{
public:
    CSaxContentHandlerXerces();
    virtual ~CSaxContentHandlerXerces();
    

    void setDocumentLocator(const Locator* const locator) override;
    //virtual HRESULT STDMETHODCALLTYPE putDocumentLocator( 
    //    /* [in] */ ISAXLocator __RPC_FAR *pLocator);
       
    void startDocument() override;
    //virtual HRESULT STDMETHODCALLTYPE startDocument(void);
    
    void endDocument() override;
    //virtual HRESULT STDMETHODCALLTYPE endDocument(void);
    
    void startPrefixMapping
	  (
		  const	XMLCh* const	prefix,
		  const	XMLCh* const	uri
	  )  override;
    //virtual HRESULT STDMETHODCALLTYPE startPrefixMapping( 
    //    /* [in] */ const wchar_t __RPC_FAR *pwchPrefix,
    //    /* [in] */ int cchPrefix,
    //    /* [in] */ const wchar_t __RPC_FAR *pwchUri,
    //    /* [in] */ int cchUri);
       
    void endPrefixMapping
	  (
		  const	XMLCh* const	prefix
	  )  override;
    //virtual HRESULT STDMETHODCALLTYPE endPrefixMapping( 
    //    /* [in] */ const wchar_t __RPC_FAR *pwchPrefix,
    //    /* [in] */ int cchPrefix);
        
    void startElement
    (
        const   XMLCh* const    uri,
        const   XMLCh* const    localname,
        const   XMLCh* const    qname
        , const Attributes&	attrs
    ) override;
    //virtual HRESULT STDMETHODCALLTYPE startElement( 
    //        /* [in] */ const wchar_t *pwchNamespaceUri,
    //        /* [in] */ int cchNamespaceUri,
    //        /* [in] */ const wchar_t *pwchLocalName,
    //        /* [in] */ int cchLocalName,
    //        /* [in] */ const wchar_t *pwchQName,
    //        /* [in] */ int cchQName,
    //        /* [in] */ ISAXAttributes *pAttributes);
     
    void endElement
	  (
		  const XMLCh* const uri,
		  const XMLCh* const localname,
		  const XMLCh* const qname
	  ) override;
    //virtual HRESULT STDMETHODCALLTYPE endElement( 
    //    /* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
    //    /* [in] */ int cchNamespaceUri,
    //    /* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
    //    /* [in] */ int cchLocalName,
    //    /* [in] */ const wchar_t __RPC_FAR *pwchRawName,
    //    /* [in] */ int cchRawName);
    
    void characters
    (
      const   XMLCh* const    chars,
      const XMLSize_t       length
    ) override;
    //virtual HRESULT STDMETHODCALLTYPE characters( 
    //    /* [in] */ const wchar_t __RPC_FAR *pwchChars,
    //    /* [in] */ int cchChars);
        
    void ignorableWhitespace
    (
      const   XMLCh* const    chars, 
      const XMLSize_t       length
    ) override;
    //virtual HRESULT STDMETHODCALLTYPE ignorableWhitespace( 
    //    /* [in] */ const wchar_t __RPC_FAR *pwchChars,
    //    /* [in] */ int cchChars);
      
    void processingInstruction
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    ) override;
    //virtual HRESULT STDMETHODCALLTYPE processingInstruction( 
    //    /* [in] */ const wchar_t __RPC_FAR *pwchTarget,
    //    /* [in] */ int cchTarget,
    //    /* [in] */ const wchar_t __RPC_FAR *pwchData,
    //    /* [in] */ int cchData);
    
   void skippedEntity
	 (
		 const	XMLCh* const	name
	 ) override;
    //virtual HRESULT STDMETHODCALLTYPE skippedEntity( 
    //    /* [in] */ const wchar_t __RPC_FAR *pwchName,
    //    /* [in] */ int cchName);    
        
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
