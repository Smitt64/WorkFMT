// Filename: XmlParser.h
// 2005-07-15 nschan Initial revision.
// 2005-07-28 nschan Added CWXmlParser wrapper class for wide-char string support.

#ifndef XML_PARSER_INCLUDED
#define XML_PARSER_INCLUDED

#include "tooldef.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Include all public header files in the wrapper
// framework to make it easier for clients to
// use the library.
#include "XmlDataTypes.h"
#include "XmlElementHandler.h"
#include "XmlParserBase.h"

// Include STL string.
#include <string>


#ifdef _DEBUG
    // Точно версию MSVS искать лень, но в 2017 этой библиотеки точно нет
    // (а также atlsn.lib и atlsnd.lib).
    #if defined(_MSC_VER) && (_MSC_VER < 1900)
        #pragma comment(lib, "atlsd.lib")
    #endif
#endif

// Forward class declarations.
//class CXmlParserImpl;

class CXmlParserFactory {
public:
  static CXmlParserFactory & inst() {
    static CXmlParserFactory t;
    return t;
  }
  CXmlParserBase* makeParser();
protected:
  CXmlParserFactory();
  ~CXmlParserFactory();
  CXmlParserBase* makeParserImpl();

  size_t parser_id_;
  CXmlParserBase *next_parser_;
};

// CXmlParser (single-char string version)
// This is the main wrapper class for SAX2 parsing.
class _TOOLEXP CXmlParser
{
public:
    CXmlParser();
    ~CXmlParser();

    // Is the parser available (e.g. was the COM class created properly?).
    bool IsReady() const;

    // Attach XML events handler.
    void AttachElementHandler(IXmlElementHandler* pElementHandler);
    void DetachElementHandler();

    // Set parser feature options.
    bool SetParserFeature(const std::string& featureName, bool value);
    bool GetParserFeature(const std::string& featureName, bool& value) const;

    // Add/remove XSD schemas for validation. The namespaceURI
    // can be an empty string.
    bool AddValidationSchema(const std::string& namespaceURI, const std::string& xsdPath);
    bool RemoveValidationSchema(const std::string& namespaceURI);

    // Parse a local file path, or a HTTP URL path.
    bool Parse(const std::string& xmlPath);
    bool ParseString(const std::string& xml);

private:
    // Use the impl technique so we can hide the implementation
    // and not require wrapper clients to import MSXML types.
    // CXmlParserImpl uses wide-char strings natively.
    CXmlParserBase* m_impl;
};

// CWXmlParser (wide-char string version)
// This is the main wrapper class for SAX2 parsing.
class _TOOLEXP CWXmlParser
{
public:
    CWXmlParser();
    ~CWXmlParser();

    // Is the parser available (e.g. was the COM class created properly?).
    bool IsReady() const;

    // Attach XML events handler.
    void AttachElementHandler(IWXmlElementHandler* pWElementHandler);
    void DetachElementHandler();

    // Set parser feature options.
    bool SetParserFeature(const std::wstring& featureName, bool value);
    bool GetParserFeature(const std::wstring& featureName, bool& value) const;

    // Add/remove XSD schemas for validation. The namespaceURI
    // can be an empty string.
    bool AddValidationSchema(const std::wstring& namespaceURI, const std::wstring& xsdPath);
    bool RemoveValidationSchema(const std::wstring& namespaceURI);

    // Parse a local file path, or a HTTP URL path.
    bool Parse(const std::wstring& xmlPath);
    bool ParseString(const std::wstring& xmlPath);

private:
    // Use the impl technique so we can hide the implementation
    // and not require wrapper clients to import MSXML types.
    // CXmlParserImpl uses wide-char strings natively.
    CXmlParserBase* m_impl;
};

#endif // XML_PARSER_INCLUDED

// END
