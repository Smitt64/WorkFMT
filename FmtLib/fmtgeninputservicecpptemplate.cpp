#include "fmtgeninputservicecpptemplate.h"
#include "fmtcore.h"

FmtGenInputServiceCppTemplate::FmtGenInputServiceCppTemplate()
    : FmtGenInterface()
{

}

FmtGenInputServiceCppTemplate::~FmtGenInputServiceCppTemplate()
{

}

QByteArray FmtGenInputServiceCppTemplate::makeContent(FmtSharedTablePtr pTable)
{
    QByteArray data;
    QTextStream stream(&data, QIODevice::WriteOnly);
    QString inputServiceClassName = getInputServiceClassName(pTable);
    QString serviceMemberName = getInputServicePtMemberName(pTable);

    stream << "// *.h" << endl;
    createCacheClassDeclaration(pTable, stream);

    stream << endl << "// bf_*.cpp " << endl;
    createCacheClassDefenition(pTable, stream);

    stream << endl << "// AlInpSrv.hpp, enum eRSB_INPUT_SERVICE" << endl;
    stream << getEnumRSB_INPUT_SERVICE(pTable) << ", // " << pTable->comment();

    stream << endl << "// RsbParty.hpp " << endl;
    createInputServiceAdmOpClass(pTable, stream);

    stream << endl << "// RsbParty.hpp " << endl;
    stream << "// class _BANKKERNELEXP TRsbParty" << endl;
    stream << "// ..." << endl;
    stream << "// TAdressInpSrv m_AdressSrv;" << endl;
    stream << QString("%1 %2;").arg(inputServiceClassName).arg(serviceMemberName) << endl;

    stream << endl;
    stream << "// class TRsbPartyCPP: public TRsbPtrWrapper<TRsbParty>" << endl;
    stream << "// ..." << endl;
    stream << "// if (!m_pObject->m_TaxExempSrv.IsInitService()) RSBEXCEPT_THROW_ONSTAT(m_pObject->InitChildService(&(m_pObject->m_TaxExempSrv)));" << endl;
    stream << QString("if (!m_pObject->%1.IsInitService()) RSBEXCEPT_THROW_ONSTAT(m_pObject->InitChildService(&(m_pObject->%1)));")
              .arg(serviceMemberName)
           << endl;

    stream << endl << "// RsbParty.cpp " << endl;
    stream << "// TRsbParty::TRsbParty(int32 PartyID, bool Original, bool InitChild):" << endl;
    stream << "// ..." << endl;
    stream << "// AddService(&m_OffformlnkSrv);" << endl;
    stream << QString("AddService(&%1);").arg(serviceMemberName) << endl;

    stream << endl;
    stream << "// TFileSelectID* TRsbParty::GetSelectIDForService(int SrvType, void* RecBuf)" << endl;
    stream << "// ..." << endl;
    stream << "// case RSB_OFFFORMLNK_SERVICE:" << endl;
    createGetSelectIDForService(pTable, stream);

    return data;
}

void FmtGenInputServiceCppTemplate::createGetSelectIDForService(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString select = getSelectTypedefName(pTable);
    QString cacheClass = getCacheClassName(pTable);
    QString serviceTypedef = getInputServiceTypedef(pTable);
    QString inputServiceClassName = getInputServiceClassName(pTable);

    stream << QString("case %1:").arg(getEnumRSB_INPUT_SERVICE(pTable)) << endl;
    stream << "{" << endl;
    stream << tab << QString("%1* mSelect = new %1(PartyRec->PartyID);").arg(select) << endl;
    stream << tab << "SelectID = (TFileSelectID*)mSelect;" << endl;
    stream << "}" << endl;
    stream << "break;";
}

void FmtGenInputServiceCppTemplate::createCacheClassDeclaration(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString select = getSelectTypedefName(pTable);
    QString cacheClass = getCacheClassName(pTable);
    stream << QString("typedef TPartySelect %1;").arg(select) << endl;
    stream << QString("class _BANKKERNELEXP %1").arg(cacheClass) << endl;
    stream << tab << QString(": public TRsbFileCacheMemLog<%1>").arg(select) << endl;

    stream << "{" << endl;
    stream << "public:" << endl;
    stream << tab << QString("virtual ~%1();").arg(cacheClass) << endl;
    stream << tab << QString("%1();").arg(cacheClass) << endl;
    stream << endl;

    stream << tab << "virtual void SetFileFilter(TFileSelectID* SelectID);" << endl;
    stream << "protected:" << endl;
    stream << tab << "virtual int OnBeforeInsert(TFileSelectID* CacheSelect, TFileSelectID* OrdinarySelect, void* RecData);" << endl;
    stream << tab << "virtual int OnBeforeCloneInsert(TFileSelectID* OriginalSelect, TFileSelectID* CloneSelect, void* RecData);" << endl;
    stream << tab << QString::fromUtf8("// Имя файла для которого создан кэш.") << endl;
    stream << tab << QString("FCACHE_DECLARE_FILE_NAME(\"%1\");").arg(FmtGetTableFileName(pTable->name())) << endl;
    stream << "};" << endl;

    stream << QString("typedef TRsbCacheWrapper<%1> %2;").arg(cacheClass).arg(getCacheWrapperName(pTable)) << endl;
}

void FmtGenInputServiceCppTemplate::createCacheClassDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString select = getSelectTypedefName(pTable);
    QString cacheClass = getCacheClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream << QString("%1::~%1()").arg(cacheClass) << endl;
    stream << "{}" << endl << endl;

    stream << QString("%1::%1() :").arg(cacheClass) << endl;
    stream << tab << QString("TRsbFileCacheMemLog<%1>(&File%2, FCACHE_GET_FNAME(%3), iOpen%2, ReadOnly)")
           .arg(select)
           .arg(structName)
           .arg(cacheClass) << endl;
    stream << "{" << endl;
    stream << "}" << endl;

    stream << endl;

    stream << QString("void %1::SetFileFilter(TFileSelectID * SelectID)").arg(cacheClass) << endl;
    stream << "{" << endl;
    stream << tab << QString("%1 *PTSPFSSel = (%1*)SelectID;").arg(select) << endl;
    stream << endl;
    stream << tab << QString("SKF_%1(PTSPFSSel->m_PartyID);").arg(structName) << endl;
    stream << tab << QString("File%1->RecFltr = NULL;").arg(structName) << endl;
    stream << "}" << endl;

    stream << endl;

    stream << QString("int %1::OnBeforeInsert(TFileSelectID * CacheSelect, TFileSelectID *OrdinarySelect, void *RecData)").arg(cacheClass) << endl;
    stream << "{" << endl;
    stream << tab << QString("%1 *PTSPFSCacheSel = (%1*)CacheSelect;").arg(select) << endl;
    stream << tab << QString("%1 *PTSPFSOrdSel = (%1*)OrdinarySelect;").arg(select) << endl;
    stream << tab << QString("%1 *pt = (%1*)RecData;").arg(structName) << endl;
    stream << endl;
    stream << tab << QString("pt->PartyID = PTSPFSOrdSel->m_PartyID;") << endl;
    stream << endl;
    stream << tab << "return MDBE_OK;" << endl;
    stream << "}" << endl;

    stream << endl;

    stream << QString("int %1::OnBeforeCloneInsert(TFileSelectID* OriginalSelect, TFileSelectID* CloneSelect, void* RecData)").arg(cacheClass) << endl;
    stream << "{" << endl;
    stream << tab << QString("%1 *Sel = (%1*)CloneSelect;").arg(select) << endl;
    stream << tab << QString("%1 *pt = (%1*)RecData;").arg(structName) << endl;
    stream << endl;
    stream << tab << QString("pt->PartyID = Sel->m_PartyID;") << endl;
    stream << tab << "return MDBE_OK;" << endl;
    stream << "}" << endl;
}

void FmtGenInputServiceCppTemplate::createInputServiceAdmOpClass(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString select = getSelectTypedefName(pTable);
    QString cacheClass = getCacheClassName(pTable);
    QString serviceTypedef = getInputServiceTypedef(pTable);
    QString inputServiceClassName = getInputServiceClassName(pTable);

    stream << "//////////////////////////////////////////////////////////////////////////" << endl;
    stream << QString("typedef TRsbInputServiceAdmOp<%1, %2> %3;")
              .arg(cacheClass)
              .arg(getEnumRSB_INPUT_SERVICE(pTable))
              .arg(serviceTypedef) << endl;
    stream << endl;
    stream << QString("class _BANKKERNELEXP %1 : public %2")
              .arg(inputServiceClassName)
              .arg(serviceTypedef) << endl;
    stream << "{" << endl;
    stream << "public:" << endl;
    stream << tab << QString("%1() {}").arg(inputServiceClassName) << endl;
    stream << tab << QString("~%1() {}").arg(inputServiceClassName) << endl;
    stream << "private:" << endl;
    stream << tab << "friend class TRsbParty;" << endl;
    stream << "};" << endl;
}

QString FmtGenInputServiceCppTemplate::getSelectTypedefName(const FmtSharedTablePtr &pTable)
{
    return QString("TParty%1Select").arg(FmtTableStructName(pTable->name()));
}

QString FmtGenInputServiceCppTemplate::getCacheClassName(const FmtSharedTablePtr &pTable)
{
    return QString("TCache%1").arg(FmtTableStructName(pTable->name()));
}

QString FmtGenInputServiceCppTemplate::getCacheWrapperName(const FmtSharedTablePtr &pTable)
{
    return QString("TRsbParty%1CachePtr").arg(FmtTableStructName(pTable->name()));
}

QString FmtGenInputServiceCppTemplate::getEnumRSB_INPUT_SERVICE(const FmtSharedTablePtr &pTable)
{
    return QString("RSB_%1_SERVICE").arg(FmtTableStructName(pTable->name()));
}

QString FmtGenInputServiceCppTemplate::getInputServiceTypedef(const FmtSharedTablePtr &pTable)
{
    return QString("TRsbInputService%1").arg(FmtTableStructName(pTable->name()));
}

QString FmtGenInputServiceCppTemplate::getInputServiceClassName(const FmtSharedTablePtr &pTable)
{
    return QString("TParty%1InpSrv").arg(FmtTableStructName(pTable->name()));
}

QString FmtGenInputServiceCppTemplate::getInputServicePtMemberName(const FmtSharedTablePtr &pTable)
{
    QString strucname = FmtTableStructName(pTable->name()).toLower();
    strucname[0] = strucname[0].toUpper();
    return QString("m_%1Srv").arg(strucname);
}
