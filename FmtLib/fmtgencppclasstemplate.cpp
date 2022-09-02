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
    stream << "// RsbParty.hpp" << Qt::endl;
    createClassDeclaration(pTable, stream);

    stream << Qt::endl << "// RsbParty.cpp" << Qt::endl;
    createClassDefenition(pTable, stream);

    stream << Qt::endl << "// cbptmsg.h" << Qt::endl;
    createRslClassDeclaration(pTable, stream);

    stream << Qt::endl << "// ptr_pt.c" << Qt::endl;
    createRslClassDefenition(pTable, stream);

    return data;
}

void FmtGenCppClassTemplate::createClassDeclaration(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    stream << "class _BANKKERNELEXP " << className << Qt::endl;
    stream << "{" << Qt::endl;
    stream << "public:" << Qt::endl;
    stream << tab << QString("%1();").arg(className) << Qt::endl;
    stream << tab << QString("%1(int32 PartyID);").arg(className) << Qt::endl;
    stream << tab << QString("~%1();").arg(className) << Qt::endl;
    stream << tab << QString("BTRVFILE *GetBf();") << Qt::endl;
    stream << tab << QString("void Init(int32 PartyID);") << Qt::endl;
    stream << Qt::endl;

    stream << tab << QString("bool First();") << Qt::endl;
    stream << tab << QString("bool Next();") << Qt::endl;
    stream << tab << QString("bool Last();") << Qt::endl;
    stream << tab << QString("bool Previous();") << Qt::endl << Qt::endl;

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
                stream << ");" << Qt::endl;
            }
        }
    }

    stream << Qt::endl;

    stream << tab << "int Add(";
    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        if (i != 0)
            stream << ", ";
        stream << pFld->getCppTypeName(true) << " " << pFld->getCppDecl(true);
    }
    stream << ");" << Qt::endl;
    stream << tab << "int Delete();" << Qt::endl << Qt::endl;

    stream << "protected:" << Qt::endl;
    stream << tab << "int GetContext();" << Qt::endl;
    stream << tab << "void SetSKF();" << Qt::endl;
    stream << tab << "int Update();" << Qt::endl;
    stream << tab << FmtTableStructName(pTable->name()) << " *RecBuf();" << Qt::endl;
    stream << tab << FmtTableStructName(pTable->name()) << " m_RecBuf;" << Qt::endl;
    stream << tab << "int32 m_PartyID;" << Qt::endl;
    stream << tab << FormatName(prm.GenUnion.sNameMask, pTable) << " m_PrimaryKey;" << Qt::endl;
    stream << "};";
}

void FmtGenCppClassTemplate::createClassDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());
    stream << QString("%1::%1()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    stream << QString("%1::%1(int32 PartyID)").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "Init(PartyID);" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    stream << QString("%1::~%1()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    stream << QString("void %1::Init(int32 PartyID)").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("memset(&m_RecBuf, 0, sizeof(%1));").arg(structName) << Qt::endl;
    stream << tab << QString("memset(&m_PrimaryKey, 0, sizeof(%1));").arg(FormatName(prm.GenUnion.sNameMask, pTable)) << Qt::endl;
    stream << tab << "m_PartyID = PartyID;" << Qt::endl;
    stream << tab << "SetSKF();" << Qt::endl;
    stream << tab << "First();" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    stream << QString("void %1::SetSKF()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("// SKF_%1();").arg(structName) << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    stream << QString("BTRVFILE *%1::GetBf()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("return File%1;").arg(structName) << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    stream << QString("int %1::GetContext()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    if (pTable->pkIDx() < 0)
        stream << tab << QString("int stat = -1; // Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
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
        stream << ", &m_RecBuf);" << Qt::endl;
    }
    stream << tab << QString("return stat;") << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    stream << QString("%2 *%1::RecBuf()").arg(className).arg(structName) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("return (%1*)GetBf()->RecBuf;").arg(structName) << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

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

    stream << QString("bool %1::First()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "int stat = 0;" << Qt::endl;
    stream << tab << "SetSKF();" << Qt::endl;
    stream << tab << "if((stat = bfGet(GetBf(), BgetF)) == 0)" << Qt::endl;
    stream << tab << "{" << Qt::endl;

    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << Qt::endl;
        }
        stream << tab << tab << "stat = GetContext();" << Qt::endl;
    }
    stream << tab << "}" << Qt::endl;
    stream << tab << QString("return !stat;") << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
}

void FmtGenCppClassTemplate::createNextDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream << QString("bool %1::Next()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "int stat = 0;" << Qt::endl;
    stream << tab << "if((stat = GetContext()) != 0)" << Qt::endl;
    stream << tab << tab << "return First();" << Qt::endl << Qt::endl;

    stream << tab << "bool finded = false;" << Qt::endl;
    stream << tab << "SetSKF();" << Qt::endl;
    stream << tab << "for(int Op = BgetF; (stat = bfGet(GetBf(), Op)) == 0; Op = BgetN)" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    stream << tab << tab << "if(finded) break;" << Qt::endl << Qt::endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
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
        stream << ")" << Qt::endl << tab << tab << tab << "finded = true;" << Qt::endl;
    }
    stream << tab << "}" << Qt::endl << Qt::endl;

    stream << tab << "if(!stat && finded)" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << Qt::endl;
        }
        stream << tab << tab << "stat = GetContext();" << Qt::endl;
    }
    stream << tab << "}" << Qt::endl;

    stream << tab << QString("return !stat;") << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
}

void FmtGenCppClassTemplate::createLastDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream << QString("bool %1::Last()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "int stat = 0;" << Qt::endl;
    stream << tab << "SetSKF();" << Qt::endl;
    stream << tab << "if((stat = bfGet(GetBf(), BgetL)) == 0)" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << Qt::endl;
        }
        stream << tab << tab << "stat = GetContext();" << Qt::endl;
    }
    stream << tab << "}" << Qt::endl;
    stream << tab << QString("return !stat;") << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
}

void FmtGenCppClassTemplate::createPreviousDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream << QString("bool %1::Previous()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "int stat = 0;" << Qt::endl;

    stream << tab << "if((stat = GetContext()) != 0)" << Qt::endl;
    stream << tab << tab << "return Last();" << Qt::endl << Qt::endl;

    stream << tab << "bool finded = false;" << Qt::endl;
    stream << tab << "SetSKF();" << Qt::endl;
    stream << tab << "for(int Op = BgetL; (stat = bfGet(GetBf(), Op)) == 0; Op = BgetP)" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    stream << tab << tab << "if(finded) break;" << Qt::endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
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
        stream << ")" << Qt::endl << tab << tab << tab << "finded = true;" << Qt::endl;
    }
    stream << tab << "}" << Qt::endl << Qt::endl;

    stream << tab << "if(!stat && finded)" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << Qt::endl;
        }
        stream << tab << tab << "stat = GetContext();" << Qt::endl;
    }
    stream << tab << "}" << Qt::endl;

    stream << tab << QString("return !stat;") << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
}

void FmtGenCppClassTemplate::createUpdateDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());
    QString keysName = FormatName(prm.GenUnion.sNameMask, pTable);

    stream << QString("int %1::Update()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "int stat = 0;" << Qt::endl;
    stream << tab << QString("%1 CachePtr;").arg(FmtGenInputServiceCppTemplate::getCacheWrapperName(pTable)) << Qt::endl;
    stream << tab << QString("%1 ZeroKey;").arg(FormatName(prm.GenUnion.sNameMask, pTable)) << Qt::endl;
    stream << tab << QString("memset(&ZeroKey, 0, sizeof(%1));").arg(keysName) << Qt::endl << Qt::endl;

    stream << tab << QString("if (!memcmp(&ZeroKey, &m_PrimaryKey, sizeof(%1)))").arg(keysName) << Qt::endl;
    stream << tab << "{" << Qt::endl;
    stream << tab << tab << "m_RecBuf.PartyID = m_PartyID;" << Qt::endl;
    stream << tab << tab << QString("memcpy(RecBuf(), &m_RecBuf, sizeof(%1));").arg(structName) << Qt::endl;
    stream << tab << tab << "stat = bfOp(GetBf(), Binsert);" << Qt::endl;
    stream << tab << "}" << Qt::endl;
    stream << tab << "else" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    stream << tab << tab << QString("memcpy(RecBuf(), &m_RecBuf, sizeof(%1));").arg(structName) << Qt::endl;
    stream << tab << tab << "stat = bfOp(GetBf(), Bupdate);" << Qt::endl;
    stream << tab << "}" << Qt::endl << Qt::endl;

    stream << tab << "if(!stat)" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    stream << tab << tab << QString("memcpy(&m_RecBuf, RecBuf(), sizeof(%1));").arg(structName) << Qt::endl;
    stream << tab << tab << QString("CachePtr->SetChangedFlag(&%1(m_PartyID), true);").arg(FmtGenInputServiceCppTemplate::getSelectTypedefName(pTable)) << Qt::endl;
    stream << tab << "}" << Qt::endl << Qt::endl;

    stream << tab << QString("return stat;") << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
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
                stream << ")" << Qt::endl;
                stream << "{" << Qt::endl;
                stream << tab << "int stat = RECNOFOUND;" << Qt::endl;
                stream << tab << "TRsbSaveBFAttr s(GetBf());" << Qt::endl;
                stream << tab << QString("%1 buf;").arg(structName) << Qt::endl << Qt::endl;

                stream << tab << QString("if (!Find%1(").arg(structName);
                for (int i = 0; i < pIndex->segmentsCount(); i++)
                {
                    FmtSegment *pSegment = pIndex->segment(i);
                    if (i != 0)
                        stream << ", ";
                    stream << pSegment->field()->undecorateName();
                }
                stream << ", &buf))" << Qt::endl;
                stream << tab << "{" << Qt::endl;
                stream << tab << tab << "if (buf.PartyID == m_PartyID)" << Qt::endl;
                stream << tab << tab << "{" << Qt::endl;

                if (pTable->pkIDx() < 0)
                    stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
                else
                {
                    FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

                    for (int i = 0; i < pIndex->segmentsCount(); i++)
                    {
                        FmtSegment *pSegment = pIndex->segment(i);
                        stream << tab << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << Qt::endl;
                    }
                    stream << tab << tab << tab << "stat = GetContext();" << Qt::endl;
                }

                stream << tab << tab << tab << "s.Restore();" << Qt::endl;
                stream << tab << tab << tab << "GetContext();" << Qt::endl;
                stream << tab << tab << tab << "stat = 0;" << Qt::endl;

                stream << tab << tab << "}" << Qt::endl;
                stream << tab << "}" << Qt::endl;
                stream << tab << "return stat;" << Qt::endl;
                stream << "}" << Qt::endl << Qt::endl;
            }
        }
    }
}

void FmtGenCppClassTemplate::createDeleteDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());
    //QString keysName = FormatName(prm.GenUnion.sNameMask, pTable);

    stream << QString("int %1::Delete()").arg(className) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "int stat = 0;" << Qt::endl;
    stream << tab << "stat = GetContext();" << Qt::endl;
    stream << tab << "if (!stat)" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    stream << tab << tab << QString("stat = bfOp(File%1, Bdelete)?1:0;").arg(structName) << Qt::endl << Qt::endl;
    stream << tab << tab << QString("%1 CachePtr;").arg(FmtGenInputServiceCppTemplate::getCacheWrapperName(pTable)) << Qt::endl;
    stream << tab << tab << QString("if (!stat) CachePtr->SetChangedFlag(&%1(m_PartyID), true);").arg(FmtGenInputServiceCppTemplate::getSelectTypedefName(pTable)) << Qt::endl;
    stream << tab << "}" << Qt::endl;

    stream << tab << QString("return stat;") << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
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
    stream << ")" << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "int stat = 0;" << Qt::endl;
    stream << tab << QString("%1 buf;").arg(structName) << Qt::endl;
    stream << tab << QString("memset(&buf, 0, sizeof(%1));").arg(structName) << Qt::endl;

    for (FmtNumber5 j = 0; j < pTable->fieldsCount(); j++)
    {
        FmtField *fld = pTable->field(j);
        QString fldName = fld->undecorateName();

        if (fld->isStringType())
            stream << tab << QString("strncpyz(buf.%1, %1, klen(%2,%1));").arg(fldName, structName) << Qt::endl;
        else
            stream << tab << QString("buf.%1 = %1;").arg(fldName) << Qt::endl;
    }
    stream << tab << "buf.PartyID = m_PartyID;" << Qt::endl << Qt::endl;

    stream << tab << QString("memcpy(RecBuf(), &buf, sizeof(%1));").arg(structName) << Qt::endl;
    stream << tab << QString("stat = bfOp(File%1, Binsert)?1:0;").arg(structName) << Qt::endl << Qt::endl;

    stream << tab << "if (!stat)" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("m_PrimaryKey.%1.%2 = RecBuf()->%2;").arg(pkUnionName).arg(pSegment->field()->undecorateName()) << Qt::endl;
        }
        stream << tab << tab << "stat = GetContext();" << Qt::endl;
        stream << tab << tab << QString("%1 CachePtr;").arg(FmtGenInputServiceCppTemplate::getCacheWrapperName(pTable)) << Qt::endl;
        stream << tab << tab << QString("if (!stat) CachePtr->SetChangedFlag(&%1(m_PartyID), true);").arg(FmtGenInputServiceCppTemplate::getSelectTypedefName(pTable)) << Qt::endl;
    }
    stream << tab << "}" << Qt::endl;
    stream << tab << QString("return stat;") << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
}

void FmtGenCppClassTemplate::createRslClassDeclaration(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);
    stream << "class _BANKKERNELEXP " << rslClassName << " : " << className << Qt::endl;
    stream << "{" << Qt::endl;
    stream << "public:" << Qt::endl;
    stream << tab << QString("%1(TGenObject *pThis = NULL);").arg(rslClassName) << Qt::endl;
    stream << tab << QString("virtual ~%1();").arg(rslClassName) << Qt::endl;
    stream << tab << QString("void Attach(int32 PartyID, %1 *pThisSrv);").arg(FmtGenInputServiceCppTemplate::getInputServiceClassName(pTable)) << Qt::endl;
    stream << tab << QString("operator TGenObject* () const { return m_pRslObj; }") << Qt::endl;
    stream << Qt::endl;

    stream << tab << QString("RSL_CLASS(%1)").arg(rslClassName) << Qt::endl;
    for (FmtFldIndex i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        stream << tab << QString("RSL_GETPROP_DECL(%1);").arg(pFld->undecorateName()) << Qt::endl;
        stream << tab << QString("RSL_PUTPROP_DECL(%1);").arg(pFld->undecorateName()) << Qt::endl;
    }
    stream << tab << QString("RSL_METHOD_DECL(First);") << Qt::endl;
    stream << tab << QString("RSL_METHOD_DECL(Last);") << Qt::endl;
    stream << tab << QString("RSL_METHOD_DECL(Next);") << Qt::endl;
    stream << tab << QString("RSL_METHOD_DECL(Previous);") << Qt::endl;

    stream << tab << QString("RSL_METHOD_DECL(Add);") << Qt::endl;
    stream << tab << QString("RSL_METHOD_DECL(Delete);") << Qt::endl;
    stream << tab << QString("RSL_METHOD_DECL(Get);") << Qt::endl;

    stream << Qt::endl;
    stream << "private:" << Qt::endl;
    stream << tab << QString("void Initialise(int firstParmOffs);") << Qt::endl;
    stream << tab << QString("TGenObject* m_pRslObj;") << Qt::endl;
    stream << tab << QString("%1 *m_pThisSrv;").arg(FmtGenInputServiceCppTemplate::getInputServiceClassName(pTable)) << Qt::endl;

    stream << "};";
}

void FmtGenCppClassTemplate::createRslClassDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);

    stream << QString("%1::%1(TGenObject *pThis)").arg(rslClassName) << Qt::endl;
    stream << tab << ": m_pRslObj(pThis), m_pThisSrv(NULL)" << Qt::endl;
    stream << "{" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    stream << QString("%1::~%1()").arg(rslClassName) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    stream << QString("void %1::Attach(int32 PartyID, %2 *pThisSrv)").arg(rslClassName).arg(FmtGenInputServiceCppTemplate::getInputServiceClassName(pTable)) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "Init(PartyID);" << Qt::endl << tab << "m_pThisSrv = pThisSrv;";
    stream << "}" << Qt::endl << Qt::endl;

    stream << QString("void %1::Initialise(int firstParmOffs)").arg(rslClassName) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "int stat = 0;" << Qt::endl;
    stream << tab << "TRsbParmFromRsl Parm;" << Qt::endl;
    stream << tab << "BEGIN_RSBEXCEPTION_HANDLER;" << Qt::endl;
    stream << tab << "if (!Parm[firstParmOffs].cvtTo(V_INTEGER))" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    stream << tab << tab << "int32 PartyID = (int32)Parm[firstParmOffs];" << Qt::endl;
    stream << tab << tab << "Init(PartyID);" << Qt::endl;
    stream << tab << "}" << Qt::endl;
    stream << tab << "else" << Qt::endl;
    stream << tab << tab << "stat = 1;" << Qt::endl;
    stream << tab << "END_RSBEXCEPTION_HANDLER_INSTAT(stat);" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    for (FmtFldIndex i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        createRslFldPropertyDefenition(pTable, pFld, stream);
    }

    createRslClassMoveFuncDefenition(pTable, "First", stream);
    createRslClassMoveFuncDefenition(pTable, "Last", stream);
    createRslClassMoveFuncDefenition(pTable, "Next", stream);
    createRslClassMoveFuncDefenition(pTable, "Previous", stream);
    createRslClassMoveFuncDefenition(pTable, "Delete", stream, true);

    createRslClassGetDefenition(pTable, stream);
    createRslAddDefenition(pTable, stream);
    createRslClassDefines(pTable, stream);
}

void FmtGenCppClassTemplate::createRslFldPropertyDefenition(const FmtSharedTablePtr &pTable, FmtField *pfld, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);

    stream <<  QString("RSL_GETPROP_IMP(%1, %2)").arg(rslClassName).arg(pfld->undecorateName()) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("ServiceActivator sact((TRsbInputServiceAbs *)m_pThisSrv);") << Qt::endl;
    stream << tab << QString("GetContext();") << Qt::endl << Qt::endl;

    if (!pfld->isString())
    {
        stream << tab << QString("ValueSet(retVal, %1, &m_RecBuf.%2);")
                  .arg(fmtRslTypeName(pfld->type()))
                  .arg(pfld->undecorateName()) << Qt::endl;
    }
    else
    {
        stream << tab << QString("ValueSet(retVal, %1, m_RecBuf.%2);")
                  .arg(fmtRslTypeName(pfld->type()))
                  .arg(pfld->undecorateName()) << Qt::endl;
    }
    stream << tab << "return 0;" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;

    stream <<  QString("RSL_PUTPROP_IMP(%1, %2)").arg(rslClassName).arg(pfld->undecorateName()) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("ServiceActivator sact((TRsbInputServiceAbs *)m_pThisSrv);") << Qt::endl << Qt::endl;
    stream << tab << QString("TRsbParmFromRsl vlKind;") << Qt::endl;
    stream << tab << QString("int stat = 0;") << Qt::endl;

    stream << tab << QString("if (vlKind[1].cvtTo(%1))").arg(fmtRslTypeName(pfld->type())) << Qt::endl;
    stream << tab << "{" << Qt::endl;
    stream << tab << tab << QString("if (CnvType(newVal, %1))").arg(fmtRslTypeName(pfld->type())) << Qt::endl;
    stream << tab << tab << "{" << Qt::endl;
    stream << tab << tab << tab << "GetContext();" << Qt::endl;
    if (!pfld->isString())
    {
        QString format;
        if (pfld->type() != fmtt_CHR && pfld->type() != fmtt_UCHR)
            format = QString("m_RecBuf.%1 = newVal->value.%2;");
        else
            format = QString("m_RecBuf.%1 = newVal->value.%2 ? SET_CHAR : UNSET_CHAR;");

        stream << tab << tab << tab << format
               .arg(pfld->undecorateName())
               .arg(fmtRslValueName(pfld->type())) << Qt::endl;
    }
    else
    {
        QString structName = FmtTableStructName(pTable->name());
        stream << tab << tab << tab << QString("strncpyz(m_RecBuf.%1, newVal->value.string, klen(%2, %1));").arg(pfld->undecorateName(), structName) << Qt::endl;;
    }

    stream << tab << tab << tab << QString("stat = Update();") << Qt::endl;
    stream << tab << tab << tab << QString("if (stat) ErrRSLType(stat);") << Qt::endl;
    stream << tab << tab << "}" << Qt::endl;
    stream << tab << tab << "else" << Qt::endl;
    stream << tab << tab << tab << "ErrRSLType(1);" << Qt::endl;
    stream << tab << "}" << Qt::endl;
    stream << tab << "else" << Qt::endl;
    stream << tab << tab << "ErrRSLType(0);" << Qt::endl;
    stream << tab << "return 0;" << Qt::endl;

    stream << "}" << Qt::endl << Qt::endl;
}

void FmtGenCppClassTemplate::createRslClassMoveFuncDefenition(const FmtSharedTablePtr &pTable, const QString &func, QTextStream &stream, const bool &intstat)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);
    stream <<  QString("RSL_METHOD_IMP(%1, %2)").arg(rslClassName).arg(func) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("ServiceActivator sact((TRsbInputServiceAbs *)m_pThisSrv);") << Qt::endl << Qt::endl;

    if (!intstat)
        stream << tab << QString("bool stat = %1::%2();").arg(className).arg(func) << Qt::endl;
    else
        stream << tab << QString("bool stat = %1::%2() ? false : true;").arg(className).arg(func) << Qt::endl;

    stream << tab << QString("V_BOOL_Ctype bRet = stat;") << Qt::endl;
    stream << tab << QString("ValueSet(retVal, V_BOOL, &bRet);") << Qt::endl;
    stream << tab << QString("return 0;") << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
}

void FmtGenCppClassTemplate::createRslClassGetDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream <<  QString("RSL_METHOD_IMP(%1, Get)").arg(rslClassName) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("ServiceActivator sact((TRsbInputServiceAbs *)m_pThisSrv);") << Qt::endl << Qt::endl;
    stream << tab << QString("int stat = 0;") << Qt::endl;
    stream << tab << QString("TRsbParmFromRsl vlKind;") << Qt::endl << Qt::endl;

    stream << tab << "enum" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());

        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            stream << tab << tab << QString("prm_%1%2,").arg(pSegment->field()->undecorateName(), i == 0 ? QString(" = 1") : QString());
            stream << Qt::endl;
        }
    }
    stream << tab << "};" << Qt::endl << Qt::endl;

    if (pTable->pkIDx() < 0)
        stream << tab << tab << QString("// Table pkIDx is %1").arg(pTable->pkIDx()) << Qt::endl;
    else
    {
        FmtIndex *pIndex = pTable->tableIndex(pTable->pkIDx());
        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtSegment *pSegment = pIndex->segment(i);
            FmtField *pFld = pSegment->field();

            stream << tab << QString("%1 %2;").arg(pFld->getCppTypeName(), pFld->getCppDecl()) << Qt::endl;
            stream << tab << QString("if (!stat && !vlKind[prm_%1].cvtTo(%2))").arg(pFld->undecorateName(), fmtRslTypeName(pFld->type())) << Qt::endl;
            stream << tab << tab << "stat = 1;" << Qt::endl;
            stream << tab << QString("else") << Qt::endl;

            if (pFld->isString())
            {
                stream << tab << tab << QString("strncpyz(%1, (const char*)vlKind[prm_%1], klen(%2, %1));")
                          .arg(pFld->undecorateName())
                          .arg(structName) << Qt::endl << Qt::endl;
            }
            else
            {
                if (pFld->type() != fmtt_CHR && pFld->type() != fmtt_UCHR)
                    stream << tab << tab << QString("%1 = vlKind[prm_%1];")
                              .arg(pFld->undecorateName()) << Qt::endl << Qt::endl;
                else
                    stream << tab << tab << QString("%1 = ((bool)vlKind[prm_%1]) ? SET_CHAR : UNSET_CHAR;")
                              .arg(pFld->undecorateName()) << Qt::endl << Qt::endl;
            }
        }
    }

    stream << tab << "if (!stat)" << Qt::endl << tab << "{" << Qt::endl;
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
    stream << ");" << Qt::endl;
    stream << tab << tab << QString("ValueSet(retVal, V_INTEGER, &stat);") << Qt::endl;
    stream << tab << "}" << Qt::endl << Qt::endl;

    stream << tab << "return stat;" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
}

void FmtGenCppClassTemplate::createRslAddDefenition(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString className = getClassName(pTable);
    QString rslClassName = getRslClassName(pTable);
    QString structName = FmtTableStructName(pTable->name());

    stream <<  QString("RSL_METHOD_IMP(%1, Add)").arg(rslClassName) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("ServiceActivator sact((TRsbInputServiceAbs *)m_pThisSrv);") << Qt::endl << Qt::endl;
    stream << tab << QString("int stat = 0;") << Qt::endl;
    stream << tab << QString("TRsbParmFromRsl vlKind;") << Qt::endl << Qt::endl;

    stream << tab << "enum" << Qt::endl;
    stream << tab << "{" << Qt::endl;
    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        stream << tab << tab << QString("prm_%1%2,").arg(pFld->undecorateName(), i == 0 ? QString(" = 1") : QString()) << Qt::endl;
    }
    stream << tab << "};" << Qt::endl << Qt::endl;

    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);

        stream << tab << QString("%1 %2;").arg(pFld->getCppTypeName()).arg(pFld->getCppDecl()) << Qt::endl;
        stream << tab << QString("if (!stat && !vlKind[prm_%1].cvtTo(%2))").arg(pFld->undecorateName()).arg(fmtRslTypeName(pFld->type())) << Qt::endl;
        stream << tab << tab << "stat = 1;" << Qt::endl;
        stream << tab << QString("else") << Qt::endl;

        if (pFld->isString())
        {
            stream << tab << tab << QString("strncpyz(%1, (const char*)vlKind[prm_%1], klen(%2, %1));")
                      .arg(pFld->undecorateName())
                      .arg(structName) << Qt::endl << Qt::endl;
        }
        else
        {
            if (pFld->type() != fmtt_CHR && pFld->type() != fmtt_UCHR)
                stream << tab << tab << QString("%1 = vlKind[prm_%1];")
                          .arg(pFld->undecorateName()) << Qt::endl << Qt::endl;
            else
                stream << tab << tab << QString("%1 = ((bool)vlKind[prm_%1]) ? SET_CHAR : UNSET_CHAR;")
                          .arg(pFld->undecorateName()) << Qt::endl << Qt::endl;
        }
    }

    stream << tab << "if (!stat)" << Qt::endl << tab << "{" << Qt::endl;
    stream << tab << tab << QString("stat = %1::Add(").arg(className);

    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);

        if (i != 0)
            stream << ", ";

        stream << pFld->undecorateName();
    }

    stream << ");" << Qt::endl;
    stream << tab << tab << QString("ValueSet(retVal, V_INTEGER, &stat);") << Qt::endl;
    stream << tab << "}" << Qt::endl << Qt::endl;

    stream << tab << "return stat;" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
}

void FmtGenCppClassTemplate::createRslClassDefines(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    QString rslClassName = getRslClassName(pTable);

    stream <<  QString("RSL_CLASS_BEGIN(%1)").arg(rslClassName) << Qt::endl;
    stream <<  QString("RSL_METH(First)") << Qt::endl;
    stream <<  QString("RSL_METH(Last)") << Qt::endl;
    stream <<  QString("RSL_METH(Next)") << Qt::endl;
    stream <<  QString("RSL_METH(Previous)") << Qt::endl << Qt::endl;

    stream <<  QString("RSL_METH(Add)") << Qt::endl;
    stream <<  QString("RSL_METH(Delete)") << Qt::endl;
    stream <<  QString("RSL_METH(Get)") << Qt::endl << Qt::endl;

    for (FmtNumber5 i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *pFld = pTable->field(i);
        stream << QString("RSL_PROP_METH2(%1)").arg(pFld->undecorateName()) << Qt::endl;
    }
    stream <<  QString("RSL_CLASS_END") << Qt::endl << Qt::endl;
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
