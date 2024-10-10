#ifndef XMLPARSERBASE_H
#define XMLPARSERBASE_H

#include "XmlElementHandler.h"

class CXmlParserBase
{
 public:
    //CXmlParserImpl();
   virtual ~CXmlParserBase() {}

    // Is the parser available (e.g. was the COM class created properly?).
    virtual bool IsReady() const = 0;

    // Attach XML events handler.
    virtual void AttachElementHandler(IXmlElementHandler *pElementHandler) = 0;
    virtual void AttachElementHandler(IWXmlElementHandler *pWElementHandler) = 0;
    virtual void DetachElementHandler() = 0;

    // Set parser feature options.
    virtual bool SetParserFeature(const std::wstring &featureName, bool value) = 0;
    virtual bool GetParserFeature(const std::wstring &featureName, bool &value) const = 0;

    // Add/remove XSD schemas for validation.
    virtual bool AddValidationSchema(const std::wstring &namespaceURI, const std::wstring &xsdPath) = 0;
    virtual bool RemoveValidationSchema(const std::wstring &namespaceURI) = 0;

    // Parse a local file path, or a HTTP URL path.
    virtual bool Parse(const std::wstring &xmlPath) = 0;
    virtual bool ParseString(const std::wstring &xml) = 0;

private:
    // Our own XML events handler.
    IXmlElementHandler        *m_attachElementHandler;
    IWXmlElementHandler       *m_attachWElementHandler;
};

#endif