// Filename: SaxContentHandler.cpp
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added registration of wide-char string element handler.

#include "XmlCommon.h"
#include "SaxContentHandler.h"
#include "XmlDataTypes.h"

#include "minmax.h"

//AV 04.04.2011 process string any size
//#define MAX_ELEMENT_DATA_SIZE   16000

CSaxContentHandler::CSaxContentHandler()
{
    m_depth = 0;

    m_attachElementHandler  = NULL;
    m_attachWElementHandler = NULL;
}

CSaxContentHandler::~CSaxContentHandler()
{
    m_attachElementHandler  = NULL;
    m_attachWElementHandler = NULL;
}

long __stdcall CSaxContentHandler::QueryInterface(const struct _GUID &riid,void ** ppvObject)
{
    // Not implemented as this class is not a COM object.
    return 0;
}

unsigned long __stdcall CSaxContentHandler::AddRef()
{
    // Not implemented as this class is not a COM object.
    return 0;
}

unsigned long __stdcall CSaxContentHandler::Release()
{
    // Not implemented as this class is not a COM object.
    return 0;
}

HRESULT STDMETHODCALLTYPE CSaxContentHandler::putDocumentLocator( 
    /* [in] */ ISAXLocator __RPC_FAR *pLocator)
{
    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CSaxContentHandler::startDocument()
{
    m_depth = 0;

    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CSaxContentHandler::endDocument( void)
{
    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CSaxContentHandler::startPrefixMapping( 
    /* [in] */ const wchar_t __RPC_FAR *pwchPrefix,
    /* [in] */ int cchPrefix,
    /* [in] */ const wchar_t __RPC_FAR *pwchUri,
    /* [in] */ int cchUri)
{
    return S_OK;
}
            
HRESULT STDMETHODCALLTYPE CSaxContentHandler::endPrefixMapping( 
    /* [in] */ const wchar_t __RPC_FAR *pwchPrefix,
    /* [in] */ int cchPrefix)
{
    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CSaxContentHandler::startElement( 
    /* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
    /* [in] */ int cchNamespaceUri,
    /* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
    /* [in] */ int cchLocalName,
    /* [in] */ const wchar_t __RPC_FAR *pwchRawName,
    /* [in] */ int cchRawName,
    /* [in] */ ISAXAttributes __RPC_FAR *pAttributes)
{
    int l = 0;

    ++m_depth;

    if ( m_attachElementHandler != NULL )
    {
        // SINGLE-CHAR case...

        // Set the start element name.
        CXmlElement xmlElement;
        xmlElement.SetStartElement(true);
        xmlElement.SetDepth(m_depth);
        std::string elementName;
        CXmlElement::MapToString(pwchLocalName, cchLocalName, elementName);
        xmlElement.SetElementName(elementName);

        // Add attributes to the start element.
        pAttributes->getLength(&l);
        for (int i = 0; i < l; ++i)
        {
            const wchar_t* ln = NULL;
            const wchar_t* vl = NULL;
            int lnl = 0, vll = 0;
            
            pAttributes->getLocalName(i, &ln, &lnl); 
            pAttributes->getValue(i, &vl, &vll);
            
            CXmlAttribute attribute;
            CXmlAttribute::SetAttribute(ln, lnl, vl, vll, attribute);
            xmlElement.AddAttribute(attribute);
        }

        // Delegate start element to element handler.
        m_attachElementHandler->OnXmlStartElement(xmlElement);

        // Check if stop parsing was requested.
        if ( m_attachElementHandler->OnXmlAbortParse(xmlElement) )
        {
            // Abort parse.
            return E_FAIL;
        }
    }
    else if ( m_attachWElementHandler != NULL )
    {
        // WIDE-CHAR case...

        // Set the start element name.
        CWXmlElement xmlElement;
        xmlElement.SetStartElement(true);
        xmlElement.SetDepth(m_depth);
        std::wstring elementName;
        CWXmlElement::MapToString(pwchLocalName, cchLocalName, elementName);
        xmlElement.SetElementName(elementName);

        // Add attributes to the start element.
        pAttributes->getLength(&l);
        for (int i = 0; i < l; ++i)
        {
            const wchar_t* ln = NULL;
            const wchar_t* vl = NULL;
            int lnl = 0, vll = 0;
            
            pAttributes->getLocalName(i, &ln, &lnl); 
            pAttributes->getValue(i, &vl, &vll);
            
            CWXmlAttribute attribute;
            CWXmlAttribute::SetAttribute(ln, lnl, vl, vll, attribute);
            xmlElement.AddAttribute(attribute);
        }

        // Delegate start element to element handler.
        m_attachWElementHandler->OnXmlStartElement(xmlElement);

        // Check if stop parsing was requested.
        if ( m_attachWElementHandler->OnXmlAbortParse(xmlElement) )
        {
            // Abort parse.
            return E_FAIL;
        }
    }

    return S_OK;
}
   
HRESULT STDMETHODCALLTYPE CSaxContentHandler::endElement( 
    /* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
    /* [in] */ int cchNamespaceUri,
    /* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
    /* [in] */ int cchLocalName,
    /* [in] */ const wchar_t __RPC_FAR *pwchRawName,
    /* [in] */ int cchRawName)
{
    --m_depth;

    if ( m_attachElementHandler != NULL )
    {
        // SINGLE-CHAR case...

        // Set the end element name.
        CXmlElement xmlElement;
        xmlElement.SetStartElement(false);
        xmlElement.SetDepth(m_depth);
        std::string elementName;
        CXmlElement::MapToString(pwchLocalName, cchLocalName, elementName);
        xmlElement.SetElementName(elementName);

        // Delegate end element to element handler.
        m_attachElementHandler->OnXmlEndElement(xmlElement);

        // Check if stop parsing was requested.
        if ( m_attachElementHandler->OnXmlAbortParse(xmlElement) )
        {
            // Abort parse.
            return E_FAIL;
        }
    }
    else if ( m_attachWElementHandler != NULL )
    {
        // WIDE-CHAR case...

        // Set the end element name.
        CWXmlElement xmlElement;
        xmlElement.SetStartElement(false);
        xmlElement.SetDepth(m_depth);
        std::wstring elementName;
        CWXmlElement::MapToString(pwchLocalName, cchLocalName, elementName);
        xmlElement.SetElementName(elementName);

        // Delegate end element to element handler.
        m_attachWElementHandler->OnXmlEndElement(xmlElement);

        // Check if stop parsing was requested.
        if ( m_attachWElementHandler->OnXmlAbortParse(xmlElement) )
        {
            // Abort parse.
            return E_FAIL;
        }
    }

    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CSaxContentHandler::characters( 
    /* [in] */ const wchar_t __RPC_FAR *pwchChars,
    /* [in] */ int cchChars)
{
    // Let's check if it's all whitespace to avoid unnecessary copying
    // and notifications to the registered element handler.
    bool hasValidChars = false;
    for(int i = 0; i < cchChars; ++i)
    {
        if ( !iswspace(pwchChars[i]) )
        {
            hasValidChars = true;
            break;
        }
    }
    if ( !hasValidChars )
        return S_OK;

    // Get length of characters.
    //int len = min(cchChars, MAX_ELEMENT_DATA_SIZE-1);
    //AV 04.04.2011 process string any size
    int len = cchChars;

    //AV 04.04.2011 replace szData[MAX_ELEMENT_DATA_SIZE] or wszData[MAX_ELEMENT_DATA_SIZE] for "operator new"

    if ( m_attachElementHandler != NULL )
    {
        // SINGLE-CHAR case...

        //AV 04.04.2011
        //char szData[MAX_ELEMENT_DATA_SIZE];
        char  *szData = (char *)operator new( (len + 1) * sizeof(char) );
        if( szData )
        {
        // Let's do the copying and notify element handler.
           
        wcstombs(szData, pwchChars, len);
        szData[len] = 0;

        // Delegate element data to element handler.
        m_attachElementHandler->OnXmlElementData(szData, m_depth);

           operator delete (szData);
        }
        //
    }
    else if ( m_attachWElementHandler != NULL )
    {
        // WIDE-CHAR case...

        //AV 04.04.2011
        //wchar_t wszData[MAX_ELEMENT_DATA_SIZE];
        wchar_t *wszData = (wchar_t *)operator new( (len + 1) * sizeof(wchar_t) );
        if( wszData )
        {
        // Let's do the copying and notify element handler.
           
           //AV 07.04.2011 здесь используем len, как количество символов wchar_t для копирования
        wcsncpy(wszData, pwchChars, len);
        wszData[len] = 0;

        // Delegate element data to element handler.
        m_attachWElementHandler->OnXmlElementData(wszData, m_depth);

           operator delete (wszData);
        }
        //


    }

    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CSaxContentHandler::ignorableWhitespace( 
    /* [in] */ const wchar_t __RPC_FAR *pwchChars,
    /* [in] */ int cchChars)
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CSaxContentHandler::processingInstruction( 
    /* [in] */ const wchar_t __RPC_FAR *pwchTarget,
    /* [in] */ int cchTarget,
    /* [in] */ const wchar_t __RPC_FAR *pwchData,
    /* [in] */ int cchData)
{
    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CSaxContentHandler::skippedEntity( 
    /* [in] */ const wchar_t __RPC_FAR *pwchVal,
    /* [in] */ int cchVal)
{
    return S_OK;
}

void CSaxContentHandler::AttachElementHandler(IXmlElementHandler* pElementHandler)
{
    m_attachElementHandler  = pElementHandler;
    m_attachWElementHandler = NULL;
}

void CSaxContentHandler::AttachElementHandler(IWXmlElementHandler* pWElementHandler)
{
    m_attachWElementHandler = pWElementHandler;
    m_attachElementHandler  = NULL;
}

void CSaxContentHandler::DetachElementHandler()
{
    m_attachElementHandler  = NULL;
    m_attachWElementHandler = NULL;
}

// END
