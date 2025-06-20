#ifndef TYPESORAPGPARAM_H
#define TYPESORAPGPARAM_H

#include "Types.h"
#include "TypesExternalDb.h"
#include "TextXmlLoader.h"
#include "TextMultiReplacer.h"
#include "HttpFileServerIntf.h"
#include "ExtDbBuilder.h"

#include "SharedInterfaceAdapt.h"

enum DebugSourceClass {
  e_prsOther = 0,
  e_prsBoolean,
  e_prsArithmetic,
  e_prsSelect,
  e_prsInsert,
  e_prsSQMultiStr,
  e_prsFuncCall,
  e_prsCaseWhen,
  e_prsException,
  e_prsExecute,
  e_prsToRule,
  e_prsXmlElement,
  e_prsXmlTable,
  e_prsAnalytic,
  e_outOvlFuncMatch,
  e_prsEnd
};

class __SHARED_DIRECTION SOraPgParam;

class __SHARED_DIRECTION SOraPgParamHttpServer {
  SOraPgParam & parent_;
public:
  SOraPgParamHttpServer(SOraPgParam & parent);
  void enableJarTransmitterHttp();
};

class __SHARED_DIRECTION SOraPgParam
{
  friend class SOraPgParamHttpServer;
public:

   struct __SHARED_DIRECTION STextReplacer
   {
     virtual void load( const string_adapt &fpath, const string_adapt &sloc) = 0;
   };

   struct __SHARED_DIRECTION SFuncArgNumerator
   {
     //typedef std::map<ustring, ustring, StrCmdNoCase> t_muu;
     //STextReplacer(SOraPgParam *parent);
     virtual void load( const string_adapt &fpath, const string_adapt &sloc) = 0;
     //void replaceUniqueLiterals(const t_muu &lmap); // для подстановки данных текущей схемы: &current_user& ...
     //t_muu mReplace_;
     //SOraPgParam *parent_;
   };


   typedef boost::shared_ptr<SOraPgParam> sp_t;

   virtual ~SOraPgParam() {}
   //virtual SOraPgParam* clone() const = 0;

   virtual SOraPgParam& userTableSpace(const ustring_adapt &v) = 0;
   virtual SOraPgParam& indexTableSpace(const ustring_adapt &v) = 0;
   virtual SOraPgParam& userName(const ustring_adapt &v)  = 0;
   virtual SOraPgParam& jdkBinPath(const ustring_adapt &v) = 0;
   virtual ExtDbBuilder::up_t buildExtDb() = 0;
   virtual SOraPgParam& setWorkPath(const ustring_adapt &v) = 0;
   
 /*
   template<class HTTPSERVIMPL>
   SOraPgParamHttpServer enableHttpFileServer(const std::string & my_external_ip_addr, uint16_t listen_port, typename std::enable_if<std::is_base_of<HttpFileServerIntf, HTTPSERVIMPL>::value>::type* = nullptr) {
     http_file_server_ = boost::make_shared<HTTPSERVIMPL>(my_external_ip_addr, listen_port);
      return SOraPgParamHttpServer(*this);
   }*/
   virtual SOraPgParamHttpServer enableHttpFileServer(boost::shared_ptr<HttpFileServerIntf> http_serv) = 0;
   
protected:
   virtual void enableJarTransmitterHttp() = 0;
public:

   virtual void loadConvException( const string_adapt &fpath, const string_adapt &sloc) = 0;
   virtual void setUserParam( ustring_adapt userTableSpace_, ustring_adapt indexTableSpace_, ustring_adapt userName_) = 0;
   virtual void setUserName(const ustring_adapt & userName) = 0;
   virtual void setDebugCfgFile( const string_adapt & file_name) = 0;

   virtual STextReplacer* sysTableReplacer() = 0;
   virtual STextReplacer* columnTypeReplacer() = 0;
   virtual STextReplacer* rowTypeReplacer() = 0;
   virtual STextReplacer* funcReplacer() = 0;
   virtual STextReplacer* wordReplacer() = 0;
   virtual STextReplacer* statementReplacer() = 0;
   virtual SFuncArgNumerator* execImmediateNumerator() = 0;
   virtual TextXmlLoaderSet* userMemberReplacer() = 0;
   virtual TextMultiReplacer* pgsqlAutoTypeCastMap() = 0;
   virtual STextReplacer* pgsqlTypeAliasFuncSign() = 0;

   virtual void reset() = 0;

   //virtual const std::vector<ustring> & getGttCreateList() const = 0;


}; // class

class __SHARED_DIRECTION SOraLoaderToPgParam
{
public:
   typedef vector_string_adapt msgList_t;
   virtual ~SOraLoaderToPgParam(){}
   virtual msgList_t& getWarningList() = 0;
   virtual ustring_adapt tableName() = 0;
   virtual ustring_adapt columnNames() = 0;
   virtual ExtDbBuilder::up_t buildExtDb() = 0;
};

namespace Ora2Pg {
  __SHARED_DIRECTION ustring_adapt normalizeColumnName(const ustring_adapt & name);
}

#endif