#include "fmtgencpptemplate.h"
#include "fmtcore.h"
#include "fmtfield.h"
#include "fmtindex.h"
#include "gencppsettings.h"
#include "fmtsegment.h"

FmtGenCppTemplate::FmtGenCppTemplate()
    : FmtGenInterface()
{
    //pSetttings = settings();
}

QByteArray FmtGenCppTemplate::makeContent(QSharedPointer<FmtTable> pTable)
{
    QByteArray data;
    QTextStream stream(&data, QIODevice::WriteOnly);
    GenCppSettings::ReadGenSettings(&prm);

    qDeleteAll(m_BlocksStore);
    m_BlocksStore.clear();
    m_Blocks.clear();

    CreateBlocks(pTable);

    createOpenFuncDecl(pTable, stream);
    if (prm.fGenSkf && pTable->hasNonUniqueIndexes())
    {
        createSkfDeclFunctions(pTable, stream);
    }

    stream << endl;
    createOpenFunc(pTable, stream);
    stream << endl;

    createFindFunctions(pTable, stream);

    if (prm.fGenStruct)
    {
        createStruct(pTable, stream);
        stream << endl;
    }

    if (prm.fGenUnion)
    {
        createKeysUnion(pTable, stream);
        stream << endl;
    }

    if (prm.fGenEnum)
    {
        createKeysEnum(pTable, stream);
        stream << endl;
    }

    stream.flush();

    return data;
}

void FmtGenCppTemplate::propertyEditor(QWidget *parent)
{
    GenCppSettings dlg(parent);

    dlg.exec();
}

qint16 FmtGenCppTemplate::calcMaxCppLenght(qint16 *maxfieldname, const QSharedPointer<FmtTable> &pTable)
{
    qint16 len = 0, fieldname = 0;

    for (int i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *fld = pTable->field(i);
        len = qMax<qint16>(len, (qint16)fmtCppStructTypeName(fld->type()).length());
        fieldname = qMax<qint16>(fieldname, fld->getCppDecl().length());
    }

    if (maxfieldname)
        *maxfieldname = fieldname;

    return len;
}

void FmtGenCppTemplate::createOpenFuncDecl(const QSharedPointer<FmtTable> &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    stream << "// " << pTable->comment() << endl;
    stream << QString("extern BTRVFILE *File%1;").arg(block->m_StructName) << endl;
    stream << QString("extern int iOpen%1 (int OpenMode);").arg(block->m_StructName) << endl;
}

void FmtGenCppTemplate::createStruct(const QSharedPointer<FmtTable> &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    stream << "// " << pTable->comment() << endl;
    stream << "typedef struct" << endl;
    stream << "{" << endl;

    for (int i = 0; i < pTable->fieldsCount(); i++)
    {
        FmtField *fld = pTable->field(i);
        QString typeName = fld->getCppTypeName();
        QString cppDecl = QString("%1;").arg(fld->getCppDecl());

        if (prm.GenStruct.fAlignNames)
            typeName = typeName.leftJustified(block->m_MaxCppLenght);

        if (prm.GenStruct.fAlignNames)
            cppDecl = cppDecl.leftJustified(block->m_MaxFldLenght + 1);

        stream << "\t" << typeName << " ";
        stream << cppDecl;

        if (prm.GenStruct.fUseComments)
            stream << " // " << fld->comment();

        stream << endl;
    }
    stream << "} " << block->m_StructName << ";" << endl;
}

void FmtGenCppTemplate::createKeysUnion(const QSharedPointer<FmtTable> &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    stream << "typedef union" << endl;
    stream << "{" << endl;
    for (int i = 0; i < pTable->indecesCount(); i++)
    {
        FmtIndex *index = pTable->tableIndex(i);

        stream << "\tstruct" << endl;
        stream << "\t{" << endl;

        for (int j = 0; j < index->segmentsCount(); j++)
        {
            FmtSegment *segment = index->segment(j);
            FmtField *fld = segment->field();

            stream << "\t\t";

            QString typeName = fld->getCppTypeName();
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

            stream << endl;
        }

        stream << "\t} " << block->m_IndexUnionValue[index] << ";" << endl;

        if (i + 1 != pTable->indecesCount())
            stream << endl;
    }
    stream << "} " << block->m_UnionName << ";" << endl;
}

void FmtGenCppTemplate::createKeysEnum(const QSharedPointer<FmtTable> &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    stream << "typedef enum" << endl;
    stream << "{" << endl;

    for (int i = 0; i < pTable->indecesCount(); i++)
    {
        FmtIndex *index = pTable->tableIndex(i);
        stream << "\t" << block->m_IndexEnumValue[index];
        if (i + 1 != pTable->indecesCount())
            stream << ",";
        stream  << endl;
    }

    stream << "} " << block->m_EnumName << ";" << endl;
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

void FmtGenCppTemplate::createOpenFunc(const QSharedPointer<FmtTable> &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    stream << QString("int iOpen%1 (int OpenMode)").arg(block->m_StructName) << endl;
    stream << "{" << endl;
    stream << "\tchar FileName[_MAX_PATH];" << endl;
    stream << QString("\tgetFileName(FileName, \"%1\");").arg(getBFileName(pTable->name())) << endl;
    stream << QString("\treturn bfOpen(&File%1, FileName, OpenMode, sizeof(%1), sizeof(%2), 0, NULL, NULL, NULL, NULL);")
              .arg(block->m_StructName).arg(block->m_UnionName) << endl;
    stream << "}" << endl;
}

void FmtGenCppTemplate::CreateFindFuncForIndex(FmtIndex *pIndex, const QSharedPointer<FmtTable> &pTable, QTextStream &stream, bool isDefault, int IndexNum)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    QString Name;

    stream << "int Find";
    if (isDefault)
    {
        Name = FmtTableStructName(pTable->name()).toUpper();
    }
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

    stream << Name.toUpper() << "(";

    QString funcParams, fillKeyBuf;
    for (int i = 0; i < pIndex->segmentsCount(); i++)
    {
        FmtSegment *seg = pIndex->segment(i);
        FmtField *fld = seg->field();

        funcParams += QString("%1 %2")
                .arg(fld->getCppTypeName(true))
                .arg(fld->getCppDecl(true));

        if (i + 1 != pIndex->segmentsCount())
            funcParams += ", ";

        if (fld->type() != fmtt_STRING && fld->type() != fmtt_SNR)
        {
            fillKeyBuf += QString("\tKB.%1.%2 = %2;\n")
                    .arg(block->m_IndexUnionValue[pIndex])
                    .arg(fld->undecorateName());
        }
        else
        {
            fillKeyBuf += QString("\tstrncpyz(KB.%1.%2, %2, klen(%3,%2));\n")
                    .arg(block->m_IndexUnionValue[pIndex])
                    .arg(fld->undecorateName())
                    .arg(block->m_StructName);
        }
    }

    stream << funcParams << ")" << endl << "{" << endl;
    stream << "\t" << block->m_UnionName << " KB;" << endl;
    stream << QString("\tmemset(&KB, 0, sizeof(%1));").arg(block->m_UnionName) << endl << endl;

    stream << fillKeyBuf << endl;
    stream << QString("\treturn CB_FindRecord(&File%1, iOpen%1, %2, &KB, buff, NULL);")
              .arg(block->m_StructName)
              .arg(block->m_IndexEnumValue[pIndex])
           << endl;
    stream << "}" << endl;
}

void FmtGenCppTemplate::createFindFunctions(const QSharedPointer<FmtTable> &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    FmtIndex *pDefaultIndex = NULL;
    if (prm.FindFunc.iDefaultType == GenCppSettings::ffdf_PkKey)
    {
        if (pTable->pkIDx() != -1)
        {
            pDefaultIndex = pTable->tableIndex(pTable->pkIDx());
            if (pDefaultIndex->isUnique())
                CreateFindFuncForIndex(pDefaultIndex, pTable, stream, true, -1);
            else
                pDefaultIndex = NULL;
        }
    }

    stream << endl;

    int IndexNum = 1;
    for (int i = 0; i < pTable->indecesCount(); i++)
    {
        FmtIndex *index = pTable->tableIndex(i);

        if (index != pDefaultIndex && index->isUnique())
        {
            CreateFindFuncForIndex(index, pTable, stream, false, IndexNum);
            stream << endl;
            IndexNum ++;
        }
    }
}

void FmtGenCppTemplate::createSkfDeclFunctions(const QSharedPointer<FmtTable> &pTable, QTextStream &stream)
{
    GenCppTemplateBlock *block = getTemplateBlock(pTable);
    if (!block)
        return;

    if (prm.SkfFunc.fAllSkf)
    {
         stream << QString("extern void %1(int keynum, %2);")
                  .arg(block->m_SkfDefaultFunc)
                  .arg(block->m_SkfDefaultParams) << endl;
    }

    for (int k = 0; k < pTable->indecesCount(); k++)
    {
        FmtIndex *pIndex = pTable->tableIndex(k);

        if (pIndex->isUnique())
            continue;

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

        stream << QString("extern %1;")
                  .arg(block->m_SkfFullFuncNameValue[pIndex]) << endl;
    }
}

QString FmtGenCppTemplate::FormatName(QString &Mask, const GenCppTemplateBlock *block)
{
    return Mask.replace("${StructName}", block->m_StructName);
}

void FmtGenCppTemplate::CreateBlocks(const QSharedPointer<FmtTable> &pTable)
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

    QList<FmtField*> fields;
    int iSkfNameIndex = 1;
    for (int i = 0; i < pTable->indecesCount(); i++)
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
                EnumName += normalizeFieldName(index->segment(i)->field()->name()).toLower();
        }

        block->m_IndexEnumValue[index] = EnumName.toUpper();

        if (prm.GenUnion.iSegmentNameType == GenCppSettings::usn_Short)
        {
            UnionName = QString("k%1")
                    .arg(index->indexNumber() + 1);
        }
        else if (prm.GenUnion.iSegmentNameType == GenCppSettings::usn_FromNames)
        {
            for (int i = 0; i < index->segmentsCount(); i++)
                UnionName += normalizeFieldName(index->segment(i)->field()->name()).toLower();
        }

        block->m_IndexUnionValue[index] = UnionName;

        if (!index->isUnique())
        {
            if (prm.SkfFunc.iNameType == GenCppSettings::usn_Short)
            {
                SkfName = QString("SKF_%1_%2")
                        .arg(block->m_StructName)
                        .arg(iSkfNameIndex ++);
            }
            else if (prm.SkfFunc.iNameType == GenCppSettings::usn_FromNames)
            {
                SkfName = QString("SKF_%1_")
                        .arg(block->m_StructName);
                for (int i = 0; i < index->segmentsCount(); i++)
                    SkfName += normalizeFieldName(index->segment(i)->field()->name()).toUpper();
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

GenCppTemplateBlock *FmtGenCppTemplate::getTemplateBlock(const QSharedPointer<FmtTable> &pTable)
{
    GenCppTemplateBlock *block = NULL;

    if (m_Blocks.contains(pTable.data()))
        block =  m_Blocks[pTable.data()];

    return block;
}
