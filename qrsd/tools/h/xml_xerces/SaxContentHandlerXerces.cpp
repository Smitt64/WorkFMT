// Filename: SaxContentHandler.cpp
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added registration of wide-char string element handler.

#include "XmlCommon.h"
#include "SaxContentHandlerXerces.h"
#include "XmlDataTypes.h"

#include "minmax.h"

#include <xercesc/sax2/Attributes.hpp>

//AV 04.04.2011 process string any size
//#define MAX_ELEMENT_DATA_SIZE   16000

CSaxContentHandlerXerces::CSaxContentHandlerXerces()
{
    m_depth = 0;

    m_attachElementHandler  = NULL;
    m_attachWElementHandler = NULL;
}

CSaxContentHandlerXerces::~CSaxContentHandlerXerces()
{
    m_attachElementHandler  = NULL;
    m_attachWElementHandler = NULL;
}

void CSaxContentHandlerXerces::setDocumentLocator(const Locator* const locator)
{
}
        
void CSaxContentHandlerXerces::startDocument()
{
    m_depth = 0;
}
        
void CSaxContentHandlerXerces::endDocument()
{
}
        
void CSaxContentHandlerXerces::startPrefixMapping
(
	const	XMLCh* const	prefix,
	const	XMLCh* const	uri
)
{
}
            
void CSaxContentHandlerXerces::endPrefixMapping
(
	const	XMLCh* const	prefix
)
{
}
        
void CSaxContentHandlerXerces::startElement
(
    const   XMLCh* const    uri,
    const   XMLCh* const    localname,
    const   XMLCh* const    qname
    , const Attributes&	attrs
)
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
        CXmlElement::MapToString(localname, XMLString::stringLen(localname), elementName);
        xmlElement.SetElementName(elementName);

        // Add attributes to the start element.
        l = attrs.getLength(); //pAttributes->getLength(&l);
        for (int i = 0; i < l; ++i)
        {
            const wchar_t* ln = NULL;
            const wchar_t* vl = NULL;
            int lnl = 0, vll = 0;
            
            lnl = XMLString::stringLen(ln = attrs.getLocalName(i));//pAttributes->getLocalName(i, &ln, &lnl); 
            vll = XMLString::stringLen(vl = attrs.getValue(i));//pAttributes->getValue(i, &vl, &vll);
            
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
            throw SAXException();
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
        CWXmlElement::MapToString(localname, XMLString::stringLen(localname), elementName);
        xmlElement.SetElementName(elementName);

        // Add attributes to the start element.
        l = attrs.getLength(); //pAttributes->getLength(&l);
        for (int i = 0; i < l; ++i)
        {
            const wchar_t* ln = NULL;
            const wchar_t* vl = NULL;
            int lnl = 0, vll = 0;
            
            lnl = XMLString::stringLen(ln = attrs.getLocalName(i)); //pAttributes->getLocalName(i, &ln, &lnl); 
            vll = XMLString::stringLen(vl = attrs.getValue(i)); //pAttributes->getValue(i, &vl, &vll);
            
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
            throw SAXException(); //return E_FAIL;
        }
    }

    //return S_OK;
}
   
void CSaxContentHandlerXerces::endElement
(
	const XMLCh* const uri,
	const XMLCh* const localname,
	const XMLCh* const qname
)
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
        CXmlElement::MapToString(localname, XMLString::stringLen(localname), elementName);
        xmlElement.SetElementName(elementName);

        // Delegate end element to element handler.
        m_attachElementHandler->OnXmlEndElement(xmlElement);

        // Check if stop parsing was requested.
        if ( m_attachElementHandler->OnXmlAbortParse(xmlElement) )
        {
            // Abort parse.
            throw SAXException(); //return E_FAIL;
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
        CWXmlElement::MapToString(localname, XMLString::stringLen(localname), elementName);
        xmlElement.SetElementName(elementName);

        // Delegate end element to element handler.
        m_attachWElementHandler->OnXmlEndElement(xmlElement);

        // Check if stop parsing was requested.
        if ( m_attachWElementHandler->OnXmlAbortParse(xmlElement) )
        {
            // Abort parse.
            throw SAXException();
        }
    }

    //return S_OK;
}
        
void CSaxContentHandlerXerces::characters
(
  const   XMLCh* const    chars,
  const XMLSize_t       length
)
{
    // Let's check if it's all whitespace to avoid unnecessary copying
    // and notifications to the registered element handler.
    bool hasValidChars = false;
    for(int i = 0; i < length; ++i)
    {
        if ( !iswspace(chars[i]) )
        {
            hasValidChars = true;
            break;
        }
    }
    if ( !hasValidChars )
        return;

    // Get length of characters.
    //int len = min(cchChars, MAX_ELEMENT_DATA_SIZE-1);
    //AV 04.04.2011 process string any size
    int len = length;

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
           
        wcstombs(szData, chars, len);
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
        wcsncpy(wszData, chars, len);
        wszData[len] = 0;

        // Delegate element data to element handler.
        m_attachWElementHandler->OnXmlElementData(wszData, m_depth);

           operator delete (wszData);
        }
        //


    }

    //return S_OK;
}
        
void CSaxContentHandlerXerces::ignorableWhitespace
(
  const   XMLCh* const    chars, 
  const XMLSize_t       length
)
{
}

void CSaxContentHandlerXerces::processingInstruction
(
    const   XMLCh* const    target
    , const XMLCh* const    data
)
{
}
        
void CSaxContentHandlerXerces::skippedEntity
(
	const	XMLCh* const	name
)
{
}

void CSaxContentHandlerXerces::AttachElementHandler(IXmlElementHandler* pElementHandler)
{
    m_attachElementHandler  = pElementHandler;
    m_attachWElementHandler = NULL;
}

void CSaxContentHandlerXerces::AttachElementHandler(IWXmlElementHandler* pWElementHandler)
{
    m_attachWElementHandler = pWElementHandler;
    m_attachElementHandler  = NULL;
}

void CSaxContentHandlerXerces::DetachElementHandler()
{
    m_attachElementHandler  = NULL;
    m_attachWElementHandler = NULL;
}

// END
