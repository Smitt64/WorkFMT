#ifndef ORALOADERPARSER_H
#define ORALOADERPARSER_H

#include "TypesOraPgParam.h"
#include <vector>
#include "FileTextLoader.h"
#include "OraSqlParser.h"

#include "SharedInterfaceAdapt.h"


class __SHARED_DIRECTION OraLoaderParser : public virtual FileTextLoader
{
public:
   typedef boost::shared_ptr<OraLoaderParser> sp_t;
   static sp_t make(OraSqlParserType type);

   virtual SOraLoaderToPgParam* param() = 0;
   virtual ustring_adapt convertOraToPg(ustring_adapt &err, ustring_adapt &tail) = 0;

   virtual void reset() = 0;

};

#endif