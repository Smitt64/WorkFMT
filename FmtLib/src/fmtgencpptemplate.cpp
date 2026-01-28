#include "fmtgencpptemplate.h"
#include "fmtcore.h"
#include "fmtfield.h"
#include "fmtindex.h"
#include "gencppsettings.h"
#include "fmtsegment.h"

const QString getTab()
{
    return QString(4, QChar(QChar::Space));
}

QTextStream &tab(QTextStream &s)
{
    s << getTab();
    return s;
}

// ----------------------------------------------------------------------------

FmtGenCppTemplate::FmtGenCppTemplate()
    : FmtGenInterface()
{
    //pSetttings = settings();
}

FmtGenCppTemplate::~FmtGenCppTemplate()
{

}

void FmtGenCppTemplate::initSettings()
{
    GenCppSettings::ReadGenSettings(&prm);
}

QMap<QString, QByteArray> FmtGenCppTemplate::makeContent(FmtSharedTablePtr pTable)
{
    QByteArray data;
    QTextStream stream(&data, QIODevice::WriteOnly);
    GenCppSettings::ReadGenSettings(&prm);

    qDeleteAll(m_BlocksStore);
    m_BlocksStore.clear();
    m_Blocks.clear();

    CreateBlocks(pTable);

    if (prm.fGenStruct)
    {
        createStruct(pTable, stream);
        stream << Qt::endl;
    }

    if (prm.fGenUnion && pTable->indecesCount())
    {
        createKeysUnion(pTable, stream);
        stream << Qt::endl;
    }

    if (prm.fGenEnum && pTable->indecesCount())
    {
        createKeysEnum(pTable, stream);
        stream << Qt::endl;
    }

    createOpenFuncDecl(pTable, stream);
    stream << Qt::endl;
    createOpenFunc(pTable, stream);
    stream << Qt::endl;
    createInitFunc(pTable, stream);
    stream << Qt::endl;

    if (prm.fGenSkf)
    {
        createSkfDeclFunctions(pTable, stream, FmtGenCppTemplate::SkfMode_Create);
        createSkfFunctions(pTable, stream);

        if (pTable->indecesCount())
            stream << Qt::endl;
    }

    createFindFunctions(pTable, stream);
    createDeclExtern(pTable, stream);

    stream.flush();

    return QMap<QString, QByteArray>{{QString(), data}};
}

void FmtGenCppTemplate::propertyEditor(QWidget *parent)
{
    GenCppSettings dlg(parent);

    dlg.exec();
}

qint16 FmtGenCppTemplate::calcMaxCppLenght(qint16 *maxfieldname, const FmtSharedTablePtr &pTable)
{
    qint16 len = 0, fieldname = 0;

    for (int i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *fld = pTable->field(i);
        len = qMax<qint16>(len, static_cast<qint16>(fmtCppStructDbTypeName(fld->type()).length()));
        fieldname = qMax<qint16>(fieldname, static_cast<qint16>(fld->getCppDecl().length()));
    }

    if (maxfieldname)
        *maxfieldname = fieldname;

    return len;
}

void FmtGenCppTemplate::createOpenFuncDecl(const FmtSharedTablePtr &pTable, QTextStream &stream, bool inlineComment)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    if (!inlineComment)
        WriteTableComment(pTable, stream);
    stream << QString("BTRVFILE *File%1 = NULL; ").arg(block->m_StructName);

    if (inlineComment)
        WriteTableComment(pTable, stream, inlineComment);

    stream << Qt::endl;
    AppendFunctionDeclExtern(block, QString("BTRVFILE *File%1").arg(block->m_StructName));
}

void FmtGenCppTemplate::createStruct(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    WriteTableComment(pTable, stream);
    stream << "typedef struct" << Qt::endl;
    stream << "{" << Qt::endl;

    for (int i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *fld = pTable->field(i);
        QString typeName = fld->getCppTypeName();
        QString cppDecl = QString("%1;").arg(fld->getCppDecl());

        if (prm.GenStruct.fAlignNames)
            typeName = typeName.leftJustified(block->m_MaxCppLenght);

        if (prm.GenStruct.fAlignNames)
            cppDecl = cppDecl.leftJustified(block->m_MaxFldLenght + 1);

        stream << tab << typeName << " ";
        stream << cppDecl;

        if (prm.GenStruct.fUseComments)
            stream << " // " << fld->comment();

        stream << Qt::endl;
    }
    stream << "} " << block->m_StructName << ";" << Qt::endl;
}

void FmtGenCppTemplate::createKeysUnion(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    stream << "typedef union" << Qt::endl;
    stream << "{" << Qt::endl;
    for (qint16 i = 0; i < pTable->indecesCount(); i++)
    {
        FmtIndex *index = pTable->tableIndex(i);

        stream << tab << "struct" << Qt::endl;
        stream << tab << "{" << Qt::endl;

        for (int j = 0; j < index->segmentsCount(); j++)
        {
            FmtSegment *segment = index->segment(j);
            FmtField *fld = segment->field();

            stream << tab << tab;

            QString typeName = fmtCppStructDbBaseTypeName(fld->type());
            QString cppDecl = QString("%1;").arg(fld->getCppDecl());

            if (prm.GenUnion.fAlignNames)
                typeName = typeName.leftJustified(block->m_MaxCppLenght);

            if (prm.GenUnion.fAlignNames)
                cppDecl = cppDecl.leftJustified(block->m_MaxFldLenght);

            stream << typeName << " " << cppDecl;

            QString comment;
            switch(prm.GenUnion.iUseComments)
            {
            case GenCppSettings::uc_CommentFld:
                comment = fld->comment();
                break;
            case GenCppSettings::uc_Commentseg:
                comment = segment->comment();
                break;
            }

            if (!comment.isEmpty())
                stream << " // " << comment;

            stream << Qt::endl;
        }

        stream << tab << "} " << block->m_IndexUnionValue[index] << ";" << Qt::endl;

        if (i + 1 != pTable->indecesCount())
            stream << Qt::endl;
    }
    stream << "} " << block->m_UnionName << ";" << Qt::endl;
}

void FmtGenCppTemplate::createKeysEnum(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    stream << "typedef enum" << Qt::endl;
    stream << "{" << Qt::endl;

    for (qint16 i = 0; i < pTable->indecesCount(); i++)
    {
        FmtIndex *index = pTable->tableIndex(i);
        stream << tab << block->m_IndexEnumValue[index];
        if (i + 1 != pTable->indecesCount())
            stream << ",";
        stream  << Qt::endl;
    }

    stream << "} " << block->m_EnumName << ";" << Qt::endl;
}

QString FmtGenCppTemplate::normalizeFieldName(const QString &m_Name)
{
    QString name = m_Name.mid(0,2).toLower() == "t_" ? m_Name.mid(2) : m_Name;
    return name;
}

QString FmtGenCppTemplate::getBFileName(const QString &table) const
{
    return QString("%1.%2")
            .arg(FmtTableStructName(table).toLower())
            .arg(FmtGetTableExtension(table).toLower());
}

void FmtGenCppTemplate::createOpenFunc(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    QString szKeyStruct = "0";
    if (pTable->indecesCount())
        szKeyStruct = QString("sizeof(%2)").arg(block->m_UnionName);

    QString openName = QString("int iOpen%1(int OpenMode)").arg(block->m_StructName);
    m_HighlightingRuleList.append({QRegularExpression(QString("\\biOpen%1\\b").arg(block->m_StructName)), FormatFunction});

    WriteTableComment(pTable, stream);
    stream << openName << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << "char FileName[_MAX_PATH];" << Qt::endl;
    stream << tab << QString("getFileName(FileName, \"%1\");").arg(getBFileName(pTable->name())) << Qt::endl;
    stream << tab << QString("return bfOpen(&File%1, FileName, OpenMode, sizeof(%1), %2, 0, NULL, NULL, NULL, NULL);")
              .arg(block->m_StructName).arg(szKeyStruct) << Qt::endl;
    stream << "}";

    AppendFunctionDeclExtern(block, openName);
    stream  << Qt::endl;
}

void FmtGenCppTemplate::createInitFunc(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    QString initName = QString("void Init%1(%1 *RecBuf)").arg(block->m_StructName);

    stream << initName << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("memset(RecBuf, 0, sizeof(%1));").arg(block->m_StructName) << Qt::endl;

    bool isFirst = true;
    for (int i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *fld = pTable->field(i);
        QString zeroConstant = fmtZeroConstant(fld->type());

        if (!zeroConstant.isEmpty())
        {
            if (isFirst)
            {
                stream  << Qt::endl;
                isFirst = false;
            }

            stream << tab << QString("RecBuf->%1 = %2;")
                      .arg(fld->undecorateName())
                      .arg(zeroConstant)
                   << Qt::endl;
        }
    }
    stream << "}";

    AppendFunctionDeclExtern(block, initName);
    stream  << Qt::endl;
}

void FmtGenCppTemplate::CreateFindFuncForIndex(FmtIndex *pIndex, const FmtSharedTablePtr &pTable, QTextStream &stream, bool isDefault, int IndexNum)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    QString Name;

    if (isDefault)
        Name = FmtTableStructName(pTable->name()).toUpper();
    else
    {
        if (prm.FindFunc.iNameType == GenCppSettings::usn_Short)
        {
            Name = QString("%1%2")
                    .arg(block->m_StructName)
                    .arg(IndexNum);
        }
        else
        {
            Name = block->m_StructName + "_";
            for (int i = 0; i < pIndex->segmentsCount(); i++)
            {
                Name += normalizeFieldName(pIndex->segment(i)->field()->name()).toLower();
                if (i + 1 != pIndex->segmentsCount())
                    Name += "_";
            }
        }
    }

    Name = QString("int Find") + Name.toUpper() + QString("(");

    QString funcParams, fillKeyBuf;
    for (int i = 0; i < pIndex->segmentsCount(); i++)
    {
        FmtSegment *seg = pIndex->segment(i);
        FmtField *fld = seg->field();

        funcParams += QString("%1 %2")
                .arg(fld->getCppTypeName(true))
                .arg(fld->getCppDecl(true));

        funcParams += ", ";

        if (fld->type() != fmtt_STRING && fld->type() != fmtt_SNR && fld->type() != fmtt_UCHR)
        {
            fillKeyBuf += getTab() + QString("KB.%1.%2 = %2;\n")
                    .arg(block->m_IndexUnionValue[pIndex])
                    .arg(fld->undecorateName());
        }
        else
        {
            fillKeyBuf += getTab() + QString("strncpyz(KB.%1.%2, %2, klen(%3,%2));\n")
                    .arg(block->m_IndexUnionValue[pIndex])
                    .arg(fld->undecorateName())
                    .arg(block->m_StructName);
        }
    }

    Name += funcParams + QString("%1 *buff)").arg(block->m_StructName);

    stream << Name << Qt::endl << "{" << Qt::endl;
    stream << tab << block->m_UnionName << " KB;" << Qt::endl;
    stream << tab << QString("memset(&KB, 0, sizeof(%1));").arg(block->m_UnionName) << Qt::endl << Qt::endl;

    stream << fillKeyBuf << Qt::endl;
    stream << tab << QString("return CB_FindRecord(&File%1, iOpen%1, %2, &KB, buff, NULL);")
              .arg(block->m_StructName)
              .arg(block->m_IndexEnumValue[pIndex])
           << Qt::endl;
    stream << "}" << Qt::endl;

    AppendFunctionDeclExtern(block, Name);
}

void FmtGenCppTemplate::createFindFunctions(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    bool created = false;
    FmtIndex *pDefaultIndex = Q_NULLPTR;
    if (prm.FindFunc.iDefaultType == GenCppSettings::ffdf_PkKey)
    {
        if (pTable->pkIDx() != -1)
        {
            pDefaultIndex = pTable->tableIndex(pTable->pkIDx());
            if (pDefaultIndex->isUnique())
            {
                CreateFindFuncForIndex(pDefaultIndex, pTable, stream, true, -1);
                created = true;
            }
            else
                pDefaultIndex = Q_NULLPTR;
        }
    }

    if (created)
        stream << Qt::endl;

    int IndexNum = 1;
    for (qint16 i = 0; i < pTable->indecesCount(); i++)
    {
        FmtIndex *index = pTable->tableIndex(i);

        if (index != pDefaultIndex && index->isUnique())
        {
            CreateFindFuncForIndex(index, pTable, stream, false, IndexNum);
            stream << Qt::endl;
            IndexNum ++;
        }
    }
}

void FmtGenCppTemplate::createSkfDeclFunctions(const FmtSharedTablePtr &pTable, QTextStream &stream, const int &Mode)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    if (prm.SkfFunc.fAllSkf && (Mode & SkfMode_Display) == SkfMode_Display)
    {
         stream << QString("extern void %1(int keynum, %2);")
                  .arg(block->m_SkfDefaultFunc)
                  .arg(block->m_SkfDefaultParams) << Qt::endl;
    }

    if ((Mode & SkfMode_Create) != SkfMode_Create)
        return;

    for (qint16 k = 0; k < pTable->indecesCount(); k++)
    {
        FmtIndex *pIndex = pTable->tableIndex(k);

        QString params;
        for (int i = 0; i < pIndex->segmentsCount(); i++)
        {
            FmtField *fld = pIndex->segment(i)->field();

            if (i != 0)
                params += ", ";

            params += QString("%1 %2")
                    .arg(fld->getCppTypeName(true))
                    .arg(fld->getCppDecl(true));

        }

        block->m_SkfFullFuncNameValue[pIndex] = QString("void %1(%2)")
                .arg(block->m_SkfNameValue[pIndex])
                .arg(params);

        AppendFunctionDeclExtern(block, block->m_SkfFullFuncNameValue[pIndex]);
    }
}

void FmtGenCppTemplate::createSkfKfReturnSegment(const QString &fldName, const QString &keyName, QTextStream &stream, bool asString, bool descOrder)
{
    if (!asString)
    {
        stream << QString("TV->%1.%2").arg(keyName, fldName);
        stream << (descOrder ? " >= " : " <= ");
        stream << QString("KB->%1.%2").arg(keyName, fldName) << " && " << Qt::endl;

        stream << tab << tab;
        stream << QString("KB->%1.%2").arg(keyName, fldName);
        stream << (descOrder ? " >= " : " <= ");
        stream << QString("BV->%1.%2").arg(keyName, fldName);
    }
    else
    {
        stream << QString("strcmpR(TV->%1.%2, KB->%1.%2)").arg(keyName, fldName);
        stream << (descOrder ? " >= 0 " : " <= 0") << " && " << Qt::endl;
        stream << tab << tab;
        stream << QString("strcmpR(TV->%1.%2, KB->%1.%2)").arg(keyName, fldName);
        stream << (descOrder ? " >= 0 " : " <= 0 ");
    }
}

void FmtGenCppTemplate::createSkfKfFunctions(FmtIndex *pIndex, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pIndex->table());
    if (!block)
        return;

    m_HighlightingRuleList.append({QRegularExpression(QString("\\bKF_%1_%2\\b")
                                   .arg(block->m_StructName)
                                   .arg(pIndex->indexNumber())), FormatFunction});

    stream << QString("static int KF_%1_%2(BTRVFILE *bf)")
              .arg(block->m_StructName).arg(pIndex->indexNumber()) << Qt::endl;
    stream << "{" << Qt::endl;
    stream << tab << QString("%1 *KB = (%1 *)bf->KeyBuf;").arg(block->m_UnionName) << Qt::endl;
    stream << tab << QString("%1 *TV = (%1 *)bf->KeyTopVal;").arg(block->m_UnionName) << Qt::endl;
    stream << tab << QString("%1 *BV = (%1 *)bf->KeyBotVal;").arg(block->m_UnionName) << Qt::endl;
    stream << Qt::endl;
    stream << tab << "return !(" << Qt::endl;
    for (int i = 0; i < pIndex->segmentsCount(); i++)
    {
        FmtSegment *seg = pIndex->segment(i);
        QString fldName = seg->field()->undecorateName();
        QString keyName = block->m_IndexUnionValue[pIndex];

        if (i != 0)
            stream  <<" && " << Qt::endl;

        stream << tab << tab;
        switch(seg->field()->type())
        {
        case fmtt_INT:
        case fmtt_LONG:
        case fmtt_BIGINT:
        case fmtt_FLOAT:
        case fmtt_DOUBLE:
        case fmtt_MONEY:
        case fmtt_NUMERIC:
        case fmtt_DATE:
        case fmtt_TIME:
            createSkfKfReturnSegment(fldName, keyName, stream, false, seg->descOrder());
            break;
        case fmtt_STRING:
        case fmtt_SNR:
            createSkfKfReturnSegment(fldName, keyName, stream, true, seg->descOrder());
            break;
        case fmtt_UCHR:
        case fmtt_CHR:
            if (seg->field()->size() > 1)
                createSkfKfReturnSegment(fldName, keyName, stream, true, seg->descOrder());
            else
                createSkfKfReturnSegment(fldName, keyName, stream, false, seg->descOrder());
            break;
        }
    }
    stream << ");" << Qt::endl;
    stream << "}" << Qt::endl << Qt::endl;
}

void FmtGenCppTemplate::createSkfFunctions(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    for (qint16 k = 0; k < pTable->indecesCount(); k++)
    {
        FmtIndex *pIndex = pTable->tableIndex(k);
        createSkfKfFunctions(pIndex, stream);
    }

    int count = 0;
    for (qint16 k = 0; k < pTable->indecesCount(); k++)
    {
        FmtIndex *pIndex = pTable->tableIndex(k);

        if (count)
            stream << Qt::endl;

        count ++;
        stream << block->m_SkfFullFuncNameValue[pIndex] << Qt::endl;
        stream << "{" << Qt::endl;
        stream << tab << QString("%1 *TV = (%1 *)File%2->KeyTopVal;").arg(block->m_UnionName, block->m_StructName) << Qt::endl;
        stream << tab << QString("%1 *BV = (%1 *)File%2->KeyBotVal;").arg(block->m_UnionName, block->m_StructName) << Qt::endl;

        stream << Qt::endl;
        stream << tab << QString("File%1->KeyNum = %2;")
               .arg(block->m_StructName, block->m_IndexEnumValue[pIndex]) << Qt::endl;
        stream << tab << QString("File%1->KeyFltr = KF_%1_%2;")
               .arg(block->m_StructName)
               .arg(pIndex->indexNumber())<< Qt::endl;
        stream << Qt::endl;

        for (int j = 0; j < pIndex->segmentsCount(); j++)
        {
            FmtSegment *seg = pIndex->segment(j);
            FmtField *fld = seg->field();
            QString fldName = fld->undecorateName();
            QString keyName = block->m_IndexUnionValue[pIndex];
            if (j != 0)
                stream << Qt::endl;

            if (fld->isStringType() || (fld->type() == fmtt_UCHR && fld->size() > 1))
            {
                bool descOrder = seg->descOrder();
                auto _getTV = [descOrder]() -> QString
                {
                    return descOrder ? "DB_UINT8_MAX" : "DB_UINT8_ZERO";
                };

                auto _getBV = [descOrder]() -> QString
                {
                    return descOrder ? "DB_UINT8_ZERO" : "DB_UINT8_MAX";
                };

                stream << tab << "if (" << fldName << ")" << Qt::endl << "\t{" << Qt::endl;
                stream << tab << tab << QString("strcpy(TV->%1.%2, %2);")
                       .arg(keyName, fldName)<< Qt::endl;
                stream << tab << tab << QString("strcpy(BV->%1.%2, %2);")
                       .arg(keyName, fldName)<< Qt::endl;
                stream << tab << "}" << Qt::endl << "\telse" << Qt::endl << "\t{" << Qt::endl;
                stream << tab << tab << QString("memset(TV->%1.%2, %3, sizeof(TV->%1.%2));")
                       .arg(keyName, fldName, _getTV())<< Qt::endl;
                stream << tab << tab << QString("memset(BV->%1.%2, %3, sizeof(BV->%1.%2));")
                       .arg(keyName, fldName, _getBV())<< Qt::endl;

                stream << tab << tab << QString("TV->%1.%2[sizeof(TV->%1.%2) - 1] = '\\0';")
                       .arg(keyName, fldName)<< Qt::endl;
                stream << tab << tab << QString("BV->%1.%2[sizeof(BV->%1.%2) - 1] = '\\0';")
                       .arg(keyName, fldName)<< Qt::endl;
                stream << tab << "}" << Qt::endl;
            }
            else if (fld->type() == fmtt_DATE)
                WrapSkfAssignValue(stream, keyName, fldName, "BDATE_MIN", "BDATE_MAX", "BDATE_ZERO", seg->descOrder());
            else if (fld->type() == fmtt_TIME)
                WrapSkfAssignValue(stream, keyName, fldName, "BTIME_MIN", "BTIME_MAX", "BTIME_ZERO", seg->descOrder());
            else if (fld->type() == fmtt_INT)
                WrapSkfAssignValue(stream, keyName, fldName, "DB_INT16_MIN", "DB_INT16_MAX", "0", seg->descOrder());
            else if (fld->type() == fmtt_LONG)
                WrapSkfAssignValue(stream, keyName, fldName, "DB_INT32_MIN", "DB_INT32_MAX", "0", seg->descOrder());
            else if (fld->type() == fmtt_BIGINT)
                WrapSkfAssignValue(stream, keyName, fldName, "DB_INT64_MIN", "DB_INT64_ZERO", "0", seg->descOrder());
            else if (fld->type() == fmtt_CHR || fld->type() == fmtt_UCHR)
                WrapSkfAssignValue(stream, keyName, fldName, "0", "255", "0", seg->descOrder());
        }
        stream << "}" << Qt::endl;
    }
}

void FmtGenCppTemplate::WrapSkfAssignValue(QTextStream &stream, const QString &keyName, const QString &fldName, const QString &minval, const QString &maxval, const QString &zeroval, const bool &descOrder)
{
    auto _getTV = [minval, maxval, descOrder]() -> QString
    {
        return descOrder ? maxval : minval;
    };

    auto _getBV = [minval, maxval, descOrder]() -> QString
    {
        return descOrder ? minval : maxval;
    };

    stream << tab << "if (" << fldName << " != " << zeroval << ")" << Qt::endl << "\t{" << Qt::endl;
    stream << tab << tab << QString("TV->%1.%2 = %2;")
           .arg(keyName, fldName)<< Qt::endl;
    stream << tab << tab << QString("BV->%1.%2 = %2;")
           .arg(keyName, fldName)<< Qt::endl;
    stream << tab << "}" << Qt::endl << tab << "else" << Qt::endl << tab << "{" << Qt::endl;
    stream << tab << tab << QString("TV->%1.%2 = %3;")
           .arg(keyName, fldName, _getTV())<< Qt::endl;
    stream << tab << tab << QString("BV->%1.%2 = %3;")
           .arg(keyName, fldName, _getBV())<< Qt::endl;
    stream << tab << "}" << Qt::endl;
}

QString FmtGenCppTemplate::FormatName(const QString &Mask, const GenCppTemplateBlock *block)
{
    QString result = Mask;
    return result.replace("${StructName}", block->m_StructName);
}

GenCppSettingsParams *FmtGenCppTemplate::param()
{
    return &prm;
}

void FmtGenCppTemplate::CreateBlocks(const FmtSharedTablePtr &pTable)
{
    if (m_Blocks.contains(pTable.data()))
        return;

    GenCppTemplateBlock *block = new GenCppTemplateBlock;
    m_BlocksStore.append(block);
    m_Blocks[pTable.data()] = block;

    block->m_MaxCppLenght = calcMaxCppLenght(&block->m_MaxFldLenght, pTable);
    block->m_StructName = FmtTableStructName(pTable->name());
    block->m_UnionName = FormatName(prm.GenUnion.sNameMask, block);
    block->m_EnumName = FormatName(prm.GenEnum.sNameMask, block);
    block->m_SkfDefaultFunc = QString("SKF_%1")
            .arg(block->m_StructName);

    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(block->m_StructName)), FormatElemType});
    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(block->m_UnionName)), FormatElemType});
    m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(block->m_EnumName)), FormatElemType});

    QList<FmtField*> fields;
    for (qint16 i = 0; i < pTable->indecesCount(); i++)
    {
        QString EnumName, UnionName, SkfName;
        FmtIndex *index = pTable->tableIndex(i);

        if (prm.GenEnum.iNameType == GenCppSettings::usn_Short)
        {
            EnumName = QString("%1_KEY%2")
                    .arg(block->m_StructName)
                    .arg(index->indexNumber() + 1);
        }
        else if (prm.GenEnum.iNameType == GenCppSettings::usn_FromNames)
        {
            EnumName = block->m_StructName;
            for (int i = 0; i < index->segmentsCount(); i++)
                EnumName += "_" + normalizeFieldName(index->segment(i)->field()->name()).toLower();
        }

        block->m_IndexEnumValue[index] = EnumName.toUpper();
        m_HighlightingRuleList.append({QRegularExpression(QString("\\b%1\\b").arg(EnumName)), FormatEnum});

        if (prm.GenUnion.iSegmentNameType == GenCppSettings::usn_Short)
        {
            UnionName = QString("k%1")
                    .arg(index->indexNumber() + 1);
        }
        else if (prm.GenUnion.iSegmentNameType == GenCppSettings::usn_FromNames)
        {
            for (int i = 0; i < index->segmentsCount(); i++)
                UnionName += "_" + normalizeFieldName(index->segment(i)->field()->name()).toLower();
        }

        block->m_IndexUnionValue[index] = UnionName;

        if (!index->isPrimary())
        {
            if (prm.SkfFunc.iNameType == GenCppSettings::usn_Short)
            {
                SkfName = QString("SKF_%1_%2")
                        .arg(block->m_StructName)
                        .arg(index->indexNumber());
            }
            else if (prm.SkfFunc.iNameType == GenCppSettings::usn_FromNames)
            {
                SkfName = QString("SKF_%1_")
                        .arg(block->m_StructName);
                for (int i = 0; i < index->segmentsCount(); i++)
                    SkfName += normalizeFieldName(index->segment(i)->field()->name()).toUpper();
            }
        }
        else {
            SkfName = QString("SKF_%1")
                    .arg(block->m_StructName);
        }

        block->m_SkfNameValue[index] = SkfName;

        for (int i = 0; i < index->segmentsCount(); i++)
        {
            FmtSegment *seg = index->segment(i);
            FmtField *fld = seg->field();

            if (!fields.contains(fld))
                fields.append(fld);
        }
    }

    foreach (const FmtField *fld, fields)
    {
        block->m_SkfDefaultParams += QString("%1 %2")
                .arg(fld->getCppTypeName(true))
                .arg(fld->getCppDecl(true));

        if (fld != fields.last())
            block->m_SkfDefaultParams += ", ";
    }
}

GenCppTemplateBlock *FmtGenCppTemplate::getTemplateBlock(const FmtSharedTablePtr &pTable)
{
    return getTemplateBlock(pTable.data());
}

GenCppTemplateBlock *FmtGenCppTemplate::getTemplateBlock(const FmtTable *pTable)
{
    GenCppTemplateBlock *block = Q_NULLPTR;
    FmtTable *cTable = const_cast<FmtTable*>(pTable);
    if (m_Blocks.contains(cTable))
        block =  m_Blocks[cTable];

    return block;
}

void FmtGenCppTemplate::AppendFunctionDeclExtern(GenCppTemplateBlock *block, const QString &func)
{
    block->m_Decl.append(QString("extern %1;").arg(func));
}

void FmtGenCppTemplate::WriteTableComment(const QSharedPointer<FmtTable> &pTable, QTextStream &stream, bool inlineComment)
{
    stream << QString("// %1").arg(pTable->comment());

    if (!inlineComment)
        stream << Qt::endl;
}

void FmtGenCppTemplate::createDeclExtern(const FmtSharedTablePtr &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);

    if (!block)
        return;

    WriteTableComment(pTable, stream);
    foreach (const QString &decl, block->m_Decl) {
        stream << decl << Qt::endl;
    }
}

GenHighlightingRuleList FmtGenCppTemplate::highlightingRuleList() const
{
    return m_HighlightingRuleList;
}

QStringList FmtGenCppTemplate::tabs()
{
    return {};
}
