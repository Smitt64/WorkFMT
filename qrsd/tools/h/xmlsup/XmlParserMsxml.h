#ifndef XMLPARSERMSXML_H
#define XMLPARSERMSXML_H

#include "XmlParserBase.h"

#include "XmlCommon.h"
#include "SaxContentHandler.h"
#include "SaxErrorHandler.h"

// CXmlParserImpl //////////////////////////////////////////////////////

class CXmlParserImpl : public CXmlParserBase
{
 public:
    CXmlParserImpl();
    CXmlParserImpl(int empty);
    ~CXmlParserImpl();

    // Is the parser available (e.g. was the COM class created properly?).
    bool IsReady() const override;

    // Attach XML events handler.
    void AttachElementHandler(IXmlElementHandler *pElementHandler) override;
    void AttachElementHandler(IWXmlElementHandler *pWElementHandler) override;
    void DetachElementHandler() override;

    // Set parser feature options.
    bool SetParserFeature(const std::wstring &featureName, bool value) override;
    bool GetParserFeature(const std::wstring &featureName, bool &value) const override;

    // Add/remove XSD schemas for validation.
    bool AddValidationSchema(const std::wstring &namespaceURI, const std::wstring &xsdPath) override;
    bool RemoveValidationSchema(const std::wstring &namespaceURI) override;

    // Parse a local file path, or a HTTP URL path.
    bool Parse(const std::wstring &xmlPath) override;
    bool ParseString(const std::wstring &xml) override;

 protected:
    void CreateSaxReader();
    void CreateSchemaCache();

    // SAX reader.
    ISAXXMLReader             *m_reader;

    // Content and error handlers to be used with the SAX reader.
    CSaxContentHandler        *m_contentHandler;
    CSaxErrorHandler          *m_errorHandler;

    // Schema cache for XSD validation.
    IXMLDOMSchemaCollection2  *m_schemaCache;

    // Our own XML events handler.
    IXmlElementHandler        *m_attachElementHandler;
    IWXmlElementHandler       *m_attachWElementHandler;
};

#endif