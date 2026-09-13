#include "fmtgeninputservicecpptemplate.h"
#include "fmtcore.h"

FmtGenInputServiceCppTemplate::FmtGenInputServiceCppTemplate()
    : FmtGenInterface()
{

}

FmtGenInputServiceCppTemplate::~FmtGenInputServiceCppTemplate()
{

}

GenHighlightingRuleList FmtGenInputServiceCppTemplate::highlightingRuleList() const
{
    return m_HighlightingRuleList;
}

QStringList FmtGenInputServiceCppTemplate::tabs()
{
    return { HEADER_H, BF_CPP, ALINPSRV_HPP, RSBPARTY_HPP, RSBPARTY_CPP };
}

QMap<QString, QByteArray> FmtGenInputServiceCppTemplate::makeContent(FmtSharedTablePtr pTable)
{
    QMap<QString, QByteArray> data =
    {
        { HEADER_H, QByteArray() },
        { BF_CPP, QByteArray() },
        { ALINPSRV_HPP, QByteArray() },
        { RSBPARTY_HPP, QByteArray() },
        { RSBPARTY_CPP, QByteArray() }
    };

    QMap<QString, QTextStream*> stream =
    {
        { HEADER_H, new QTextStream(&data[HEADER_H], QIODevice::WriteOnly) },
        { BF_CPP, new QTextStream(&data[BF_CPP], QIODevice::WriteOnly) },
        { ALINPSRV_HPP, new QTextStream(&data[ALINPSRV_HPP], QIODevice::WriteOnly) },
        { RSBPARTY_HPP, new QTextStream(&data[RSBPARTY_HPP], QIODevice::WriteOnly) },
        { RSBPARTY_CPP, new QTextStream(&data[RSBPARTY_CPP], QIODevice::WriteOnly) }
    };

    QString inputServiceClassName = getInputServiceClassName(pTable);
    QString serviceMemberName = getInputServicePtMemberName(pTable);
    QString StructName = FmtTableStructName(pTable->name());

    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(inputServiceClassName)), FormatElemType});
    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(StructName)), FormatElemType});

    //(*stream[HEADER_H]) << "// *.h" << Qt::endl;
    createCacheClassDeclaration(pTable, (*stream[HEADER_H]));

    //(*stream[BF_CPP]) << Qt::endl << "// bf_*.cpp " << Qt::endl;
    createCacheClassDefenition(pTable, (*stream[BF_CPP]));

    (*stream[ALINPSRV_HPP]) << "// AlInpSrv.hpp, enum eRSB_INPUT_SERVICE" << Qt::endl;
    (*stream[ALINPSRV_HPP]) << getEnumRSB_INPUT_SERVICE(pTable) << ", // " << pTable->comment();

    (*stream[RSBPARTY_HPP]) << "// RsbParty.hpp " << Qt::endl;
    createInputServiceAdmOpClass(pTable, (*stream[RSBPARTY_HPP]));

    (*stream[RSBPARTY_HPP]) << "// RsbParty.hpp " << Qt::endl;
    (*stream[RSBPARTY_HPP]) << "// class _BANKKERNELEXP TRsbParty" << Qt::endl;
    (*stream[RSBPARTY_HPP]) << "// ..." << Qt::endl;
    (*stream[RSBPARTY_HPP]) << "// TAdressInpSrv m_AdressSrv;" << Qt::endl;
    (*stream[RSBPARTY_HPP]) << QString("%1 %2;").arg(inputServiceClassName).arg(serviceMemberName) << Qt::endl;

    (*stream[RSBPARTY_HPP]) << Qt::endl;
    (*stream[RSBPARTY_HPP]) << "// class TRsbPartyCPP: public TRsbPtrWrapper<TRsbParty>" << Qt::endl;
    (*stream[RSBPARTY_HPP]) << "// ..." << Qt::endl;
    (*stream[RSBPARTY_HPP]) << "// if (!m_pObject->m_TaxExempSrv.IsInitService()) RSBEXCEPT_THROW_ONSTAT(m_pObject->InitChildService(&(m_pObject->m_TaxExempSrv)));" << Qt::endl;
    (*stream[RSBPARTY_HPP]) << QString("if (!m_pObject->%1.IsInitService()) RSBEXCEPT_THROW_ONSTAT(m_pObject->InitChildService(&(m_pObject->%1)));")
              .arg(serviceMemberName)
           << Qt::endl;

    (*stream[RSBPARTY_CPP]) << "// RsbParty.cpp " << Qt::endl;
    (*stream[RSBPARTY_CPP]) << "// TRsbParty::TRsbParty(int32 PartyID, bool Original, bool InitChild):" << Qt::endl;
    (*stream[RSBPARTY_CPP]) << "// ..." << Qt::endl;
    (*stream[RSBPARTY_CPP]) << "// AddService(&m_OffformlnkSrv);" << Qt::endl;
    (*stream[RSBPARTY_CPP]) << QString("AddService(&%1);").arg(serviceMemberName) << Qt::endl;

    (*stream[RSBPARTY_CPP]) << Qt::endl;
    (*stream[RSBPARTY_CPP]) << "// TFileSelectID* TRsbParty::GetSelectIDForService(int SrvType, void* RecBuf)" << Qt::endl;
    (*stream[RSBPARTY_CPP]) << "// ..." << Qt::endl;
    (*stream[RSBPARTY_CPP]) << "// case RSB_OFFFORMLNK_SERVICE:" << Qt::endl;
    createGetSelectIDForService(pTable, (*stream[RSBPARTY_CPP]));

    for (auto it = stream.begin(); it != stream.end(); ++it)
    {
        it.value()->flush();
        delete it.value();
        it.value() = nullptr;
    }

    return data;
}

void FmtGenInputServiceCppTemplate::createGetSelectIDForService(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString select = getSelectTypedefName(pTable);
    QString cacheClass = getCacheClassName(pTable);
    QString serviceTypedef = getInputServiceTypedef(pTable);
    QString inputServiceClassName = getInputServiceClassName(pTable);

    stream << QString("case %1:").arg(getEnumRSB_INPUT_SERVICE(pTable)) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("%1* mSelect = new %1(PartyRec->PartyID);").arg(select) << Qt::endl;
    stream << tab << "SelectID = (TFileSelectID*)mSelect;" << Qt::endl;
    stream << "}" << Qt::endl;
    stream << "break;";
}

void FmtGenInputServiceCppTemplate::createCacheClassDeclaration(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString select = getSelectTypedefName(pTable);
    QString cacheClass = getCacheClassName(pTable);
    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(select)), FormatElemType});
    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(cacheClass)), FormatElemType});

    stream << QString("typedef TPartySelect %1;").arg(select) << Qt::endl;
    stream << QString("class _BANKKERNELEXP %1").arg(cacheClass) << Qt::endl;
    stream << tab << QString(": public TRsbFileCacheMemLog<%1>").arg(select) << Qt::endl;

    stream << "{" << Qt::endl;
    stream << "public:" << Qt::endl;
    stream << tab << QString("virtual ~%1();").arg(cacheClass) << Qt::endl;
    stream << tab << QString("%1();").arg(cacheClass) << Qt::endl;
    stream << Qt::endl;

    stream << tab << "virtual void SetFileFilter(TFileSelectID* SelectID);" << Qt::endl;
    stream << "protected:" << Qt::endl;
    stream << tab << "virtual int OnBeforeInsert(TFileSelectID* CacheSelect, TFileSelectID* OrdinarySelect, void* RecData);" << Qt::endl;
    stream << tab << "virtual int OnBeforeCloneInsert(TFileSelectID* OriginalSelect, TFileSelectID* CloneSelect, void* RecData);" << Qt::endl;
    stream << tab << QString::fromUtf8("// Имя файла для которого создан кэш.") << Qt::endl;
    stream << tab << QString("FCACHE_DECLARE_FILE_NAME(\"%1\");").arg(FmtGetTableFileName(pTable->name())) << Qt::endl;
    stream << "};" << Qt::endl;

    QString CacheWrapperName = getCacheWrapperName(pTable);
    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(CacheWrapperName)), FormatElemType});
    stream << QString("typedef TRsbCacheWrapper<%1> %2;").arg(cacheClass).arg(CacheWrapperName) << Qt::endl;
}

void FmtGenInputServiceCppTemplate::createCacheClassDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString select = getSelectTypedefName(pTable);
    QString cacheClass = getCacheClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream << QString("%1::~%1()").arg(cacheClass) << Qt::endl;
    stream << "{}" << Qt::endl << Qt::endl;

    stream << QString("%1::%1() :").arg(cacheClass) << Qt::endl;
    m_HighlightingRuleList.append({QRegularExpression(QString("\\biOpen%1\\b").arg(structName)), FormatFunction});
    stream << tab << QString("TRsbFileCacheMemLog<%1>(&File%2, FCACHE_GET_FNAME(%3), iOpen%2, ReadOnly)")
           .arg(select)
           .arg(structName)
           .arg(cacheClass) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << "}" << Qt::endl;

    stream << Qt::endl;

    stream << QString("void %1::SetFileFilter(TFileSelectID * SelectID)").arg(cacheClass) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("%1 *PTSPFSSel = (%1*)SelectID;").arg(select) << Qt::endl;
    stream << Qt::endl;
    stream << tab << QString("SKF_%1(PTSPFSSel->m_PartyID);").arg(structName) << Qt::endl;
    stream << tab << QString("File%1->RecFltr = NULL;").arg(structName) << Qt::endl;
    stream << "}" << Qt::endl;

    stream << Qt::endl;

    stream << QString("int %1::OnBeforeInsert(TFileSelectID * CacheSelect, TFileSelectID *OrdinarySelect, void *RecData)").arg(cacheClass) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("%1 *PTSPFSCacheSel = (%1*)CacheSelect;").arg(select) << Qt::endl;
    stream << tab << QString("%1 *PTSPFSOrdSel = (%1*)OrdinarySelect;").arg(select) << Qt::endl;
    stream << tab << QString("%1 *pt = (%1*)RecData;").arg(structName) << Qt::endl;
    stream << Qt::endl;
    stream << tab << QString("pt->PartyID = PTSPFSOrdSel->m_PartyID;") << Qt::endl;
    stream << Qt::endl;
    stream << tab << "return MDBE_OK;" << Qt::endl;
    stream << "}" << Qt::endl;

    stream << Qt::endl;

    stream << QString("int %1::OnBeforeCloneInsert(TFileSelectID* OriginalSelect, TFileSelectID* CloneSelect, void* RecData)").arg(cacheClass) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("%1 *Sel = (%1*)CloneSelect;").arg(select) << Qt::endl;
    stream << tab << QString("%1 *pt = (%1*)RecData;").arg(structName) << Qt::endl;
    stream << Qt::endl;
    stream << tab << QString("pt->PartyID = Sel->m_PartyID;") << Qt::endl;
    stream << tab << "return MDBE_OK;" << Qt::endl;
    stream << "}" << Qt::endl;
}

void FmtGenInputServiceCppTemplate::createInputServiceAdmOpClass(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString select = getSelectTypedefName(pTable);
    QString cacheClass = getCacheClassName(pTable);
    QString serviceTypedef = getInputServiceTypedef(pTable);
    QString inputServiceClassName = getInputServiceClassName(pTable);
    QString inputServiceEnumName = getEnumRSB_INPUT_SERVICE(pTable);
    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(serviceTypedef)), FormatElemType});
    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(inputServiceEnumName)), FormatEnum});

    stream << "//////////////////////////////////////////////////////////////////////////" << Qt::endl;
    stream << QString("typedef TRsbInputServiceAdmOp<%1, %2> %3;")
              .arg(cacheClass)
              .arg(inputServiceEnumName)
              .arg(serviceTypedef) << Qt::endl;
    stream << Qt::endl;
    stream << QString("class _BANKKERNELEXP %1 : public %2")
              .arg(inputServiceClassName)
              .arg(serviceTypedef) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << "public:" << Qt::endl;
    stream << tab << QString("%1() {}").arg(inputServiceClassName) << Qt::endl;
    stream << tab << QString("~%1() {}").arg(inputServiceClassName) << Qt::endl;
    stream << "private:" << Qt::endl;
    stream << tab << "friend class TRsbParty;" << Qt::endl;
    stream << "};" << Qt::endl;
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
