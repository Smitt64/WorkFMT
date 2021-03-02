// Filename: XmlElementHandler.h
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added IWXmlElementHandler interface for wide-char string support.

#ifndef XML_ELEMENT_HANDLER_INCLUDED
#define XML_ELEMENT_HANDLER_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XmlDataTypes.h"
#include <string>

// IXmlElementHandler (single-char string version)
// An application-specific class should implement this
// interface in order to handle XML events that are
// raised during parsing.
class IXmlElementHandler
{
public:
    // Handle XML content events during parsing.
    virtual void OnXmlStartElement(const CXmlElement& xmlElement) = 0;
    virtual void OnXmlElementData(const std::string& elementData, int depth) = 0;
    virtual void OnXmlEndElement(const CXmlElement& xmlElement) = 0;
    
    // Handle XML error events during parsing.
    virtual void OnXmlError(int line, int column, const std::string& errorText, unsigned long errorCode) = 0;
    
    // Return true to stop parsing earlier.
    virtual bool OnXmlAbortParse(const CXmlElement& xmlElement) = 0;
};

// IWXmlElementHandler (wide-char string version)
// An application-specific class should implement this
// interface in order to handle XML events that are
// raised during parsing.
class IWXmlElementHandler
{
public:
    // Handle XML content events during parsing.
    virtual void OnXmlStartElement(const CWXmlElement& xmlElement) = 0;
    virtual void OnXmlElementData(const std::wstring& elementData, int depth) = 0;
    virtual void OnXmlEndElement(const CWXmlElement& xmlElement) = 0;
    
    // Handle XML error events during parsing.
    virtual void OnXmlError(int line, int column, const std::wstring& errorText, unsigned long errorCode) = 0;
    
    // Return true to stop parsing earlier.
    virtual bool OnXmlAbortParse(const CWXmlElement& xmlElement) = 0;
};

#endif // XML_ELEMENT_HANDLER_INCLUDED

// END

