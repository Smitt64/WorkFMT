#ifndef GENCPPSETTINGS_H
#define GENCPPSETTINGS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class GenCppSettings;
}

typedef struct
{
    bool fGenStruct;
    struct
    {
        bool fAlignNames;
        bool fUseComments;
    }GenStruct;

    bool fGenUnion;
    struct
    {
        QString sNameMask;
        qint16 iSegmentNameType;
        bool fAlignNames;
        qint16 iUseComments;
    }GenUnion;

    bool fGenEnum;
    struct
    {
        QString sNameMask;
        qint16 iNameType;
    }GenEnum;

    bool fGenBTRVFILE;
    bool fGenOpenFunc;

    bool fGenFindFunctions;
    struct
    {
        qint16 iDefaultType;
        qint16 iNameType;
    }FindFunc;

    bool fGenSkf;
    struct
    {
        bool fAllSkf;
        bool fDefaultSkf;
        qint16 iNameType;
    }SkfFunc;
}GenCppSettingsParams;

class QButtonGroup;
class GenCppSettings : public QDialog
{
    Q_OBJECT

public:
    enum UNION_COMMENT
    {
        uc_NoComment,
        uc_CommentFld,
        uc_Commentseg
    };

    enum UNION_SEGMENT_NAME
    {
        usn_Short = 0,
        usn_FromNames
    };

    enum FINDFUNC_DEFAULT_TYPE
    {
        ffdf_PkKey = 0,
        ffdf_AutoInk
    };

    explicit GenCppSettings(QWidget *parent = 0);
    ~GenCppSettings();

    static void ReadGenSettings(GenCppSettingsParams *Prm);

private slots:
    void onAccept();

private:
    Ui::GenCppSettings *ui;
    QSettings *pSetttings;

    QButtonGroup *m_pUnionCommentGroup, *m_pFfDefaultCommentGroup;
};

#endif // GENCPPSETTINGS_H
