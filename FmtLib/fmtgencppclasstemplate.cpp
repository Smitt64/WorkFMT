#include "fmtgencppclasstemplate.h"
#include "fmtcore.h"
#include "fmtfield.h"
#include "fmtindex.h"
#include "fmtsegment.h"
#include "fmtgeninputservicecpptemplate.h"

FmtGenCppClassTemplate::FmtGenCppClassTemplate()
{
}

FmtGenCppClassTemplate::~FmtGenCppClassTemplate()
{

}

QByteArray FmtGenCppClassTemplate::makeContent(FmtSharedTablePtr pTable)
{
    QByteArray data;
    QTextStream stream(&data, QIODevice::WriteOnly);
    GenCppSettings::ReadGenSettings(&prm);

    if (pTable->pkIDx() >= 0)
        pkUnionName = getUnionKeyName(pTable, pTable->pkIDx());

    QString className = getClassName(pTable);
    stream << "// RsbParty.hpp" << endl;
    createClassDeclaration(pTable, stream);

    stream << endl << "// RsbParty.cpp" << endl;
    createClassDefenition(pTable, stream);

    stream << endl << "// cbptmsg.h" << endl;
    createRslClassDeclaration(pTable, stream);

    stream << endl << "// ptr_pt.c" << endl;
    createRslClassDefenition(pTable, stream);

    return data;
}

void FmtGenCppClassTemplate::createClassDeclaration(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    stream << "class _BANKKERNELEXP " << className << endl;
    stream << "{" << endl;
    stream << "public:" << endl;
    stream << tab << QString("%1();").arg(className) << endl;
    stream << tab << QString("%1(int32 PartyID);").arg(className) << endl;
    stream << tab << QString("~%1();").arg(className) << endl;
    stream << tab << QString("BTRVFILE *GetBf();") << endl;
    stream << tab << QString("void Init(int32 PartyID);") << endl;
    stream << endl;

    stream << tab << QString("bool First();") << endl;
    stream << tab << QString("bool Next();") << endl;
    stream << tab << QString("bool Last();") << endl;
    stream << tab << QString("bool Previous();") << endl << endl;

    if (pTable->hasUniqueIndexes())
    {
        QStringList indexes = pTable->uniqueIndeces();

        foreach (const QString &str, indexes) {
            bool ok = false;
            FmtNumber5 id = static_cast<FmtNumber5>(str.toInt(&ok));

            if (ok)
            {
                FmtIndex *pIndex = pTable->tableIndex(id);
                stream << tab << QString("int Get(");

                for (int i = 0; i < pIndex->segmentsCount(); i++)
                {
                    FmtSegment *pSegment = pIndex->segment(i);
                    if (i != 0)
                        stream << ", ";
                    stream << pSegment->field()->getCppTypeName(true) << " " << pSegment->field()->getCppDecl(true);
                }
                stream << ");" << endl;
            }
        }
    }

    stream << endl;

    stream << tab << "int Add(";
    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        if (i != 0)
            stream << ", ";
        stream << pFld->getCppTypeName(true) << " " << pFld->getCppDecl(true);
    }
    stream << ");" << endl;
    stream << tab << "int Delete();" << endl << endl;

    stream << "protected:" << endl;
    stream << tab << "int GetContext();" << endl;
    stream << tab << "void SetSKF();" << endl;
    stream << tab << "int Update();" << endl;
    stream << tab << FmtTableStructName(pTable->name()) << " *RecBuf();" << endl;
    stream << tab << FmtTableStructName(pTable->name()) << " m_RecBuf;" << endl;
    stream << tab << "int32 m_PartyID;" << endl;
    stream << tab << FormatName(prm.GenUnion.sNameMask, pTable) << " m_PrimaryKey;" << endl;
    stream << "};";
}

void FmtGenCppClassTemplate::createClassDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());
    stream << QString("%1::%1()").arg(className) << endl;
    stream << "{" << endl;
    stream << "}" << endl << endl;

    stream << QString("%1::%1(int32 PartyID)").arg(className) << endl;
    stream << "{" << endl;
    stream << tab << "Init(PartyID);" << endl;
    stream << "}" << endl << endl;

    stream << QString("%1::~%1()").arg(className) << endl;
    stream << "{" << endl;
    stream << "}" << endl << endl;

    stream << QString("void %1::Init(int32 PartyID)").arg(className) << endl;
    stream << "{" << endl;
    stream << tab << QString("memset(&m_RecBuf, 0, sizeof(%1));").arg(structName) << endl;
    stream << tab << QString("memset(&m_PrimaryKey, 0, sizeof(%1));").arg(FormatName(prm.GenUnion.sNameMask, pTable)) << endl;
    stream << tab << "m_PartyID = PartyID;" << endl;
    stream << tab << "SetSKF();" << endl;
    stream << tab << "First();" << endl;
    stream << "}" << endl << endl;

    stream << QString("void %1::SetSKF()").arg(className) << endl;
    stream << "{" << endl;
    stream << tab << QString("// SKF_%1();").arg(structName) << endl;
    stream << "}" << endl << endl;

    stream << QString("BTRVFILE *%1::GetBf()").arg(className) << endl;
    stream << "{" << endl;
    stream << tab << QString("return File%1;").arg(structName) << endl;
    stream << "}" << endl << endl;

    stream << QString("int %1::GetContext()").arg(className) << endl;
    stream << "{" << endl;
    if (pTable->pkIDx() < 0)
        stream << tab << QString("int stat = -1; // Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
    else
    {
        stream << tab << QString("int stat = Find%1(").arg(structName);
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            if (i != 0)
                stream << ", ";
            stream << QString("m_PrimaryKey.%1.%2").arg(pkUnionName).arg(pSegment->field()->undecorateName());
        }
        stream << ", &m_RecBuf);" << endl;
    }
    stream << tab << QString("return stat;") << endl;
    stream << "}" << endl << endl;

    stream << QString("%2 *%1::RecBuf()").arg(className).arg(structName) << endl;
    stream << "{" << endl;
    stream << tab << QString("return (%1*)GetBf()->RecBuf;").arg(structName) << endl;
    stream << "}" << endl << endl;

    createFirstDefenition(pTable, stream);
    createNextDefenition(pTable, stream);
    createLastDefenition(pTable, stream);
    createPreviousDefenition(pTable, stream);
    createUpdateDefenition(pTable, stream);
    createGetDefenition(pTable, stream);
    createDeleteDefenition(pTable, stream);
    createAddDefenition(pTable, stream);
}

void FmtGenCppClassTemplate::createFirstDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream << QString("bool %1::First()").arg(className) << endl;
    stream << "{" << endl;
    stream << tab << "int stat = 0;" << endl;
    stream << tab << "SetSKF();" << endl;
    stream << tab << "if((stat = bfGet(GetBf(), BgetF)) == 0)" << endl;
    stream << tab << "{" << endl;

    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << endl;
        }
        stream << tab << tab << "stat = GetContext();" << endl;
    }
    stream << tab << "}" << endl;
    stream << tab << QString("return !stat;") << endl;
    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createNextDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream << QString("bool %1::Next()").arg(className) << endl;
    stream << "{" << endl;
    stream << tab << "int stat = 0;" << endl;
    stream << tab << "if((stat = GetContext()) != 0)" << endl;
    stream << tab << tab << "return First();" << endl << endl;

    stream << tab << "bool finded = false;" << endl;
    stream << tab << "SetSKF();" << endl;
    stream << tab << "for(int Op = BgetF; (stat = bfGet(GetBf(), Op)) == 0; Op = BgetN)" << endl;
    stream << tab << "{" << endl;
    stream << tab << tab << "if(finded) break;" << endl << endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
    else
    {
        stream << tab << tab << "if (";
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            if (i != 0)
                stream << " && ";
            stream << QString("m_PrimaryKey.%1.%2 == RecBuf()->%2").arg(pkUnionName).arg(pSegment->field()->undecorateName());
        }
        stream << ")" << endl << tab << tab << tab << "finded = true;" << endl;
    }
    stream << tab << "}" << endl << endl;

    stream << tab << "if(!stat && finded)" << endl;
    stream << tab << "{" << endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << endl;
        }
        stream << tab << tab << "stat = GetContext();" << endl;
    }
    stream << tab << "}" << endl;

    stream << tab << QString("return !stat;") << endl;
    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createLastDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream << QString("bool %1::Last()").arg(className) << endl;
    stream << "{" << endl;
    stream << tab << "int stat = 0;" << endl;
    stream << tab << "SetSKF();" << endl;
    stream << tab << "if((stat = bfGet(GetBf(), BgetL)) == 0)" << endl;
    stream << tab << "{" << endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << endl;
        }
        stream << tab << tab << "stat = GetContext();" << endl;
    }
    stream << tab << "}" << endl;
    stream << tab << QString("return !stat;") << endl;
    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createPreviousDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream << QString("bool %1::Previous()").arg(className) << endl;
    stream << "{" << endl;
    stream << tab << "int stat = 0;" << endl;

    stream << tab << "if((stat = GetContext()) != 0)" << endl;
    stream << tab << tab << "return Last();" << endl << endl;

    stream << tab << "bool finded = false;" << endl;
    stream << tab << "SetSKF();" << endl;
    stream << tab << "for(int Op = BgetL; (stat = bfGet(GetBf(), Op)) == 0; Op = BgetP)" << endl;
    stream << tab << "{" << endl;
    stream << tab << tab << "if(finded) break;" << endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
    else
    {
        stream << tab << tab << "if (";
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            if (i != 0)
                stream << " && ";
            stream << QString("m_PrimaryKey.%1.%2 == RecBuf()->%2").arg(pkUnionName).arg(pSegment->field()->undecorateName());
        }
        stream << ")" << endl << tab << tab << tab << "finded = true;" << endl;
    }
    stream << tab << "}" << endl << endl;

    stream << tab << "if(!stat && finded)" << endl;
    stream << tab << "{" << endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << endl;
        }
        stream << tab << tab << "stat = GetContext();" << endl;
    }
    stream << tab << "}" << endl;

    stream << tab << QString("return !stat;") << endl;
    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createUpdateDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());
    QString keysName = FormatName(prm.GenUnion.sNameMask, pTable);

    stream << QString("int %1::Update()").arg(className) << endl;
    stream << "{" << endl;
    stream << tab << "int stat = 0;" << endl;
    stream << tab << QString("%1 CachePtr;").arg(FmtGenInputServiceCppTemplate::getCacheWrapperName(pTable)) << endl;
    stream << tab << QString("%1 ZeroKey;").arg(FormatName(prm.GenUnion.sNameMask, pTable)) << endl;
    stream << tab << QString("memset(&ZeroKey, 0, sizeof(%1));").arg(keysName) << endl << endl;

    stream << tab << QString("if (!memcmp(&ZeroKey, &m_PrimaryKey, sizeof(%1)))").arg(keysName) << endl;
    stream << tab << "{" << endl;
    stream << tab << tab << "m_RecBuf.PartyID = m_PartyID;" << endl;
    stream << tab << tab << QString("memcpy(RecBuf(), &m_RecBuf, sizeof(%1));").arg(structName) << endl;
    stream << tab << tab << "stat = bfOp(GetBf(), Binsert);" << endl;
    stream << tab << "}" << endl;
    stream << tab << "else" << endl;
    stream << tab << "{" << endl;
    stream << tab << tab << QString("memcpy(RecBuf(), &m_RecBuf, sizeof(%1));").arg(structName) << endl;
    stream << tab << tab << "stat = bfOp(GetBf(), Bupdate);" << endl;
    stream << tab << "}" << endl << endl;

    stream << tab << "if(!stat)" << endl;
    stream << tab << "{" << endl;
    stream << tab << tab << QString("memcpy(&m_RecBuf, RecBuf(), sizeof(%1));").arg(structName) << endl;
    stream << tab << tab << QString("CachePtr->SetChangedFlag(&%1(m_PartyID), true);").arg(FmtGenInputServiceCppTemplate::getSelectTypedefName(pTable)) << endl;
    stream << tab << "}" << endl << endl;

    stream << tab << QString("return stat;") << endl;
    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createGetDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());
    QString keysName = FormatName(prm.GenUnion.sNameMask, pTable);

    if (pTable->hasUniqueIndexes())
    {
        QStringList indexes = pTable->uniqueIndeces();

        foreach (const QString &str, indexes) {
            bool ok = false;
            FmtNumber5 id = static_cast<FmtNumber5>(str.toInt(&ok));

            if (ok)
            {
                FmtIndex *pIndex = pTable->tableIndex(id);
                stream << QString("int %1::Get(").arg(className);

                for (int i = 0; i < pIndex->segmentsCount(); i++)
                {
                    FmtSegment *pSegment = pIndex->segment(i);
                    if (i != 0)
                        stream << ", ";
                    stream << pSegment->field()->getCppTypeName(true) << " " << pSegment->field()->getCppDecl(true);
                }
                stream << ")" << endl;
                stream << "{" << endl;
                stream << tab << "int stat = RECNOFOUND;" << endl;
                stream << tab << "TRsbSaveBFAttr s(GetBf());" << endl;
                stream << tab << QString("%1 buf;").arg(structName) << endl << endl;

                stream << tab << QString("if (!Find%1(").arg(structName);
                for (int i = 0; i < pIndex->segmentsCount(); i++)
                {
                    FmtSegment *pSegment = pIndex->segment(i);
                    if (i != 0)
                        stream << ", ";
                    stream << pSegment->field()->undecorateName();
                }
                stream << ", &buf))" << endl;
                stream << tab << "{" << endl;
                stream << tab << tab << "if (buf.PartyID == m_PartyID)" << endl;
                stream << tab << tab << "{" << endl;

                if (pTable->pkIDx() < 0)
                    stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
                else
                {
                    FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

                    for (int i = 0; i < pIndex->segmentsCount(); i++)
                    {
                        FmtSegment *pSegment = pIndex->segment(i);
                        stream << tab << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << endl;
                    }
                    stream << tab << tab << tab << "stat = GetContext();" << endl;
                }

                stream << tab << tab << tab << "s.Restore();" << endl;
                stream << tab << tab << tab << "GetContext();" << endl;
                stream << tab << tab << tab << "stat = 0;" << endl;

                stream << tab << tab << "}" << endl;
                stream << tab << "}" << endl;
                stream << tab << "return stat;" << endl;
                stream << "}" << endl << endl;
            }
        }
    }
}

void FmtGenCppClassTemplate::createDeleteDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());
    //QString keysName = FormatName(prm.GenUnion.sNameMask, pTable);

    stream << QString("int %1::Delete()").arg(className) << endl;
    stream << "{" << endl;
    stream << tab << "int stat = 0;" << endl;
    stream << tab << "stat = GetContext();" << endl;
    stream << tab << "if (!stat)" << endl;
    stream << tab << "{" << endl;
    stream << tab << tab << QString("stat = bfOp(File%1, Bdelete)?1:0;").arg(structName) << endl << endl;
    stream << tab << tab << QString("%1 CachePtr;").arg(FmtGenInputServiceCppTemplate::getCacheWrapperName(pTable)) << endl;
    stream << tab << tab << QString("if (!stat) CachePtr->SetChangedFlag(&%1(m_PartyID), true);").arg(FmtGenInputServiceCppTemplate::getSelectTypedefName(pTable)) << endl;
    stream << tab << "}" << endl;

    stream << tab << QString("return stat;") << endl;
    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createAddDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());
    stream << QString("int %1::Add(").arg(className);
    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        if (i != 0)
            stream << ", ";
        stream << pFld->getCppTypeName(true) << " " << pFld->getCppDecl(true);
    }
    stream << ")" << endl;
    stream << "{" << endl;
    stream << tab << "int stat = 0;" << endl;
    stream << tab << QString("%1 buf;").arg(structName) << endl;
    stream << tab << QString("memset(&buf, 0, sizeof(%1));").arg(structName) << endl;

    for (FmtNumber5 j = 0; j < pTable->fieldsCount(); j++)
    {
        FmtField *fld = pTable->field(j);
        QString fldName = fld->undecorateName();

        if (fld->isStringType())
            stream << tab << QString("strncpyz(buf.%1, %1, klen(%2,%1));").arg(fldName, structName) << endl;
        else
            stream << tab << QString("buf.%1 = %1;").arg(fldName) << endl;
    }
    stream << tab << "buf.PartyID = m_PartyID;" << endl << endl;

    stream << tab << QString("memcpy(RecBuf(), &buf, sizeof(%1));").arg(structName) << endl;
    stream << tab << QString("stat = bfOp(File%1, Binsert)?1:0;").arg(structName) << endl << endl;

    stream << tab << "if (!stat)" << endl;
    stream << tab << "{" << endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << endl;
        }
        stream << tab << tab << "stat = GetContext();" << endl;
        stream << tab << tab << QString("%1 CachePtr;").arg(FmtGenInputServiceCppTemplate::getCacheWrapperName(pTable)) << endl;
        stream << tab << tab << QString("if (!stat) CachePtr->SetChangedFlag(&%1(m_PartyID), true);").arg(FmtGenInputServiceCppTemplate::getSelectTypedefName(pTable)) << endl;
    }
    stream << tab << "}" << endl;
    stream << tab << QString("return stat;") << endl;
    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createRslClassDeclaration(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);
    stream << "class _BANKKERNELEXP " << rslClassName << " : " << className << endl;
    stream << "{" << endl;
    stream << "public:" << endl;
    stream << tab << QString("%1(TGenObject *pThis = NULL);").arg(rslClassName) << endl;
    stream << tab << QString("virtual ~%1();").arg(rslClassName) << endl;
    stream << tab << QString("void Attach(int32 PartyID, %1 *pThisSrv);").arg(FmtGenInputServiceCppTemplate::getInputServiceClassName(pTable)) << endl;
    stream << tab << QString("operator TGenObject* () const { return m_pRslObj; }") << endl;
    stream << endl;

    stream << tab << QString("RSL_CLASS(%1)").arg(rslClassName) << endl;
    for (FmtFldIndex i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        stream << tab << QString("RSL_GETPROP_DECL(%1);").arg(pFld->undecorateName()) << endl;
        stream << tab << QString("RSL_PUTPROP_DECL(%1);").arg(pFld->undecorateName()) << endl;
    }
    stream << tab << QString("RSL_METHOD_DECL(First);") << endl;
    stream << tab << QString("RSL_METHOD_DECL(Last);") << endl;
    stream << tab << QString("RSL_METHOD_DECL(Next);") << endl;
    stream << tab << QString("RSL_METHOD_DECL(Previous);") << endl;

    stream << tab << QString("RSL_METHOD_DECL(Add);") << endl;
    stream << tab << QString("RSL_METHOD_DECL(Delete);") << endl;
    stream << tab << QString("RSL_METHOD_DECL(Get);") << endl;

    stream << endl;
    stream << "private:" << endl;
    stream << tab << QString("void Initialise(int firstParmOffs);") << endl;
    stream << tab << QString("TGenObject* m_pRslObj;") << endl;
    stream << tab << QString("%1 *m_pThisSrv;").arg(FmtGenInputServiceCppTemplate::getInputServiceClassName(pTable)) << endl;

    stream << "};";
}

void FmtGenCppClassTemplate::createRslClassDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);

    stream << QString("%1::%1(TGenObject *pThis)").arg(rslClassName) << endl;
    stream << tab << ": m_pRslObj(pThis), m_pThisSrv(NULL)" << endl;
    stream << "{" << endl;
    stream << "}" << endl << endl;

    stream << QString("%1::~%1(TGenObject *pThis)").arg(rslClassName) << endl;
    stream << "{" << endl;
    stream << "}" << endl << endl;

    stream << QString("void %1::Attach(int32 PartyID, %2 *pThisSrv)").arg(rslClassName).arg(FmtGenInputServiceCppTemplate::getInputServiceClassName(pTable)) << endl;
    stream << "{" << endl;
    stream << tab << "Init(PartyID);" << endl << tab << "m_pThisSrv = pThisSrv;";
    stream << "}" << endl << endl;

    stream << QString("void %1::Initialise(int firstParmOffs)").arg(rslClassName) << endl;
    stream << "{" << endl;
    stream << tab << "int stat = 0;" << endl;
    stream << tab << "TRsbParmFromRsl Parm;" << endl;
    stream << tab << "BEGIN_RSBEXCEPTION_HANDLER;" << endl;
    stream << tab << "if (!Parm[firstParmOffs].cvtTo(V_INTEGER))" << endl;
    stream << tab << "{" << endl;
    stream << tab << tab << "int32 PartyID = (int32)Parm[firstParmOffs];" << endl;
    stream << tab << tab << "Init(PartyID);" << endl;
    stream << tab << "}" << endl;
    stream << tab << "else" << endl;
    stream << tab << tab << "stat = 1;" << endl;
    stream << tab << "END_RSBEXCEPTION_HANDLER_INSTAT(stat);" << endl;
    stream << "}" << endl << endl;

    for (FmtFldIndex i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        createRslFldPropertyDefenition(pTable, pFld, stream);
    }

    createRslClassMoveFuncDefenition(pTable, "First", stream);
    createRslClassMoveFuncDefenition(pTable, "Last", stream);
    createRslClassMoveFuncDefenition(pTable, "Next", stream);
    createRslClassMoveFuncDefenition(pTable, "Previous", stream);
    createRslClassMoveFuncDefenition(pTable, "Delete", stream);

    createRslClassGetDefenition(pTable, stream);
    createRslAddDefenition(pTable, stream);
    createRslClassDefines(pTable, stream);
}

void FmtGenCppClassTemplate::createRslFldPropertyDefenition(const FmtSharedTablePtr &pTable, FmtField *pfld, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);

    stream <<  QString("RSL_GETPROP_IMP(%1, %2)").arg(rslClassName).arg(pfld->undecorateName()) << endl;
    stream << "{" << endl;
    stream << tab << QString("ServiceActivator sact((TRsbInputServiceAbs *)m_pThisSrv);") << endl;
    stream << tab << QString("GetContext();") << endl << endl;

    if (!pfld->isString())
    {
        stream << tab << QString("ValueSet(retVal, %1, &m_RecBuf.%2);")
                  .arg(fmtRslTypeName(pfld->type()))
                  .arg(pfld->undecorateName()) << endl;
    }
    else
    {
        stream << tab << QString("ValueSet(retVal, %1, m_RecBuf.%2);")
                  .arg(fmtRslTypeName(pfld->type()))
                  .arg(pfld->undecorateName()) << endl;
    }
    stream << tab << "return 0;" << endl;
    stream << "}" << endl << endl;

    stream <<  QString("RSL_PUTPROP_IMP(%1, %2)").arg(rslClassName).arg(pfld->undecorateName()) << endl;
    stream << "{" << endl;
    stream << tab << QString("ServiceActivator sact((TRsbInputServiceAbs *)m_pThisSrv);") << endl << endl;
    stream << tab << QString("TRsbParmFromRsl vlKind;") << endl;
    stream << tab << QString("int stat = 0;") << endl;

    stream << tab << QString("if (vlKind[1].cvtTo(%1))").arg(fmtRslTypeName(pfld->type())) << endl;
    stream << tab << "{" << endl;
    stream << tab << tab << QString("if (CnvType(newVal, %1))").arg(fmtRslTypeName(pfld->type())) << endl;
    stream << tab << tab << "{" << endl;
    stream << tab << tab << tab << "GetContext();" << endl;
    if (!pfld->isString())
    {
        QString format;
        if (pfld->type() != fmtt_CHR && pfld->type() != fmtt_UCHR)
            format = QString("m_RecBuf.%1 = newVal->value.%2;");
        else
            format = QString("m_RecBuf.%1 = newVal->value.%2 ? SET_CHAR : UNSET_CHAR;");
        stream << tab << tab << tab << format
               .arg(pfld->undecorateName())
               .arg(fmtRslValueName(pfld->type())) << endl;
    }
    stream << tab << tab << tab << QString("stat = Update();") << endl;
    stream << tab << tab << tab << QString("if (stat) ErrRSLType(stat);") << endl;
    stream << tab << tab << "}" << endl;
    stream << tab << tab << "else" << endl;
    stream << tab << tab << tab << "ErrRSLType(1);" << endl;
    stream << tab << "}" << endl;
    stream << tab << "else" << endl;
    stream << tab << tab << "ErrRSLType(0);" << endl;
    stream << tab << "return 0;" << endl;

    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createRslClassMoveFuncDefenition(const FmtSharedTablePtr &pTable, const QString &func, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);
    stream <<  QString("RSL_METHOD_IMP(%1, %2)").arg(rslClassName).arg(func) << endl;
    stream << "{" << endl;
    stream << tab << QString("ServiceActivator sact((TRsbInputServiceAbs *)m_pThisSrv);") << endl << endl;
    stream << tab << QString("bool stat = %1::%2();").arg(className).arg(func) << endl;
    stream << tab << QString("V_BOOL_Ctype bRet = stat;") << endl;
    stream << tab << QString("ValueSet(retVal, V_BOOL, &bRet);") << endl;
    stream << tab << QString("return 0;") << endl;
    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createRslClassGetDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream <<  QString("RSL_METHOD_IMP(%1, Get)").arg(rslClassName) << endl;
    stream << "{" << endl;
    stream << tab << QString("ServiceActivator sact((TRsbInputServiceAbs *)m_pThisSrv);") << endl << endl;
    stream << tab << QString("int stat = 0;") << endl;
    stream << tab << QString("TRsbParmFromRsl vlKind;") << endl << endl;

    stream << tab << "enum" << endl;
    stream << tab << "{" << endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("prm_%1,").arg(pSegment->field()->undecorateName()) << endl;
        }
    }
    stream << tab << "};" << endl << endl;

    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());
        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            FmtField *pFld = pSegment->field();

            stream << tab << QString("%1 %2;").arg(pFld->getCppTypeName()).arg(pFld->getCppDecl()) << endl;
            stream << tab << QString("if (!stat && !vlKind[prm_%1].cvtTo(%2))").arg(pFld->undecorateName()).arg(fmtRslTypeName(pFld->type())) << endl;
            stream << tab << tab << "stat = 1;" << endl;
            stream << tab << QString("else") << endl;

            if (pFld->isString())
            {
                stream << tab << tab << QString("strncpyz(%1, (const char*)vlKind[prm_%1], klen(%2, %1));")
                          .arg(pFld->undecorateName())
                          .arg(structName) << endl << endl;
            }
            else
            {
                if (pFld->type() != fmtt_CHR && pFld->type() != fmtt_UCHR)
                    stream << tab << tab << QString("%1 = vlKind[prm_%1];")
                              .arg(pFld->undecorateName()) << endl << endl;
                else
                    stream << tab << tab << QString("%1 = ((bool)vlKind[prm_%1]) ? SET_CHAR : UNSET_CHAR;")
                              .arg(pFld->undecorateName()) << endl << endl;
            }
        }
    }

    stream << tab << "if (!stat)" << endl << tab << "{" << endl;
    stream << tab << tab << QString("stat = %1::Get(").arg(className);

    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("/* Table pkIDx is %1 */").arg(pTable->pkIDx());
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());
        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            FmtField *pFld = pSegment->field();

            if (i != 0)
                stream << ", ";

            if (pFld->type() != fmtt_CHR && pFld->type() != fmtt_UCHR)
                stream << pFld->undecorateName();
            else
                stream << QString("%1 ? SET_CHAR : UNSET_CHAR").arg(pFld->undecorateName());
        }
    }
    stream << ");" << endl;
    stream << tab << tab << QString("ValueSet(retVal, V_INTEGER, &stat);") << endl;
    stream << tab << "}" << endl << endl;

    stream << tab << "return stat;" << endl;
    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createRslAddDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream <<  QString("RSL_METHOD_IMP(%1, Add)").arg(rslClassName) << endl;
    stream << "{" << endl;
    stream << tab << QString("ServiceActivator sact((TRsbInputServiceAbs *)m_pThisSrv);") << endl << endl;
    stream << tab << QString("int stat = 0;") << endl;
    stream << tab << QString("TRsbParmFromRsl vlKind;") << endl << endl;

    stream << tab << "enum" << endl;
    stream << tab << "{" << endl;
    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        stream << tab << tab << QString("prm_%1,").arg(pFld->undecorateName()) << endl;
    }
    stream << tab << "};" << endl << endl;

    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);

        stream << tab << QString("%1 %2;").arg(pFld->getCppTypeName()).arg(pFld->getCppDecl()) << endl;
        stream << tab << QString("if (!stat && !vlKind[prm_%1].cvtTo(%2))").arg(pFld->undecorateName()).arg(fmtRslTypeName(pFld->type())) << endl;
        stream << tab << tab << "stat = 1;" << endl;
        stream << tab << QString("else") << endl;

        if (pFld->isString())
        {
            stream << tab << tab << QString("strncpyz(%1, (const char*)vlKind[prm_%1], klen(%2, %1));")
                      .arg(pFld->undecorateName())
                      .arg(structName) << endl << endl;
        }
        else
        {
            if (pFld->type() != fmtt_CHR && pFld->type() != fmtt_UCHR)
                stream << tab << tab << QString("%1 = vlKind[prm_%1];")
                          .arg(pFld->undecorateName()) << endl << endl;
            else
                stream << tab << tab << QString("%1 = ((bool)vlKind[prm_%1]) ? SET_CHAR : UNSET_CHAR;")
                          .arg(pFld->undecorateName()) << endl << endl;
        }
    }

    stream << tab << "if (!stat)" << endl << tab << "{" << endl;
    stream << tab << tab << QString("stat = %1::Add(").arg(className);

    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);

        if (i != 0)
            stream << ", ";

        stream << pFld->undecorateName();
    }

    stream << ");" << endl;
    stream << tab << tab << QString("ValueSet(retVal, V_INTEGER, &stat);") << endl;
    stream << tab << "}" << endl << endl;

    stream << tab << "return stat;" << endl;
    stream << "}" << endl << endl;
}

void FmtGenCppClassTemplate::createRslClassDefines(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString rslClassName = getRslClassName(pTable);

    stream <<  QString("RSL_CLASS_BEGIN(TRsbPartyPtresidenthistRSL)").arg(rslClassName) << endl;
    stream <<  QString("RSL_METH(First)") << endl;
    stream <<  QString("RSL_METH(Last)") << endl;
    stream <<  QString("RSL_METH(Next)") << endl;
    stream <<  QString("RSL_METH(Previous)") << endl << endl;

    stream <<  QString("RSL_METH(Add)") << endl;
    stream <<  QString("RSL_METH(Delete)") << endl;
    stream <<  QString("RSL_METH(Get)") << endl << endl;

    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        stream << QString("RSL_PROP_METH2(%1)").arg(pFld->undecorateName()) << endl;
    }
    stream <<  QString("RSL_CLASS_END") << endl << endl;
}

QString FmtGenCppClassTemplate::getClassName(const FmtSharedTablePtr &pTable)
{
    QString strucname = FmtTableStructName(pTable->name()).toLower();
    strucname[0] = strucname[0].toUpper();
    return QString("TRsbParty%1").arg(strucname);
}

QString FmtGenCppClassTemplate::getRslClassName(const FmtSharedTablePtr &pTable)
{
    return QString("%1RSL").arg(getClassName(pTable));
}

QString FmtGenCppClassTemplate::FormatName(QString &Mask, const FmtSharedTablePtr &pTable)
{
    return Mask.replace("${StructName}", FmtTableStructName(pTable->name()));
}

QString FmtGenCppClassTemplate::normalizeFieldName(const QString &m_Name)
{
    QString name = m_Name.mid(0,2).toLower() == "t_" ? m_Name.mid(2) : m_Name;
    return name;
}

QString FmtGenCppClassTemplate::getUnionKeyName(const FmtSharedTablePtr &pTable, const FmtNumber5 &key)
{
    QString keyName;
    FmtIndex *pIndex = pTable->tableIndex(key);
    if (prm.GenUnion.iSegmentNameType == GenCppSettings::usn_Short)
    {
        keyName = QString("k%1")
                .arg(pIndex->indexNumber() + 1);
    }
    else if (prm.GenUnion.iSegmentNameType == GenCppSettings::usn_FromNames)
    {
        for (int i = 0; i < pIndex->segmentsCount(); i++)
            keyName += normalizeFieldName(pIndex->segment(i)->field()->name()).toLower();
    }
    return keyName;
}
