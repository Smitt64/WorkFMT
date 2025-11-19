#ifndef REGPARMWIZARD_H
#define REGPARMWIZARD_H

#include <QWizard>
#include <QMap>
#include <QSqlDatabase>
#include <QTranslator>

class QMenu;
class ActionPage;
class ConnactionPage;
class RegParmModel;
class ViewDatPage;
class QAbstractItemModel;
class WordContentPage;
class QTextDocument;
class WordPreviewRegPage;
class RegParmWizard : public QWizard
{
    Q_OBJECT

public:
    enum PageId
    {
        PageAction = 0,
        PageConnection,
        PageViewDat,
        PageEnterWordTable,
        PageWordPreviewReg
    };

    enum ActionType
    {
        ActionAddRegToDat = 0,
        ActionViewDat,
        ActionAddFromWord
    };

    explicit RegParmWizard(QWidget *parent = nullptr);
    virtual ~RegParmWizard();

    void addUserField(const QString &name, const QVariant &value);
    const QVariant &userField(const QString &name);
    QString userValName(const QVariant &value) const;

    ActionType selectedAction() const;

    void resetDatModel();
    QAbstractItemModel *datModel();

    QTextDocument *wordContentDocument();

private slots:
    void onSettingsClicked();
    void onRestartClicked();

private:
    void setupUi();
    void setupConnections();

    ActionPage *m_pActionPage;
    ConnactionPage *m_pConnectionPage;
    ViewDatPage *m_pViewPage;
    WordContentPage *m_pWordContentPage;
    WordPreviewRegPage *m_pWordPreviewRegPage;

    QMap<QString, QVariant> m_Fields;
    QMenu *m_pHelpMenu;

    QSqlDatabase m_DatDatabase;
    QScopedPointer<RegParmModel> m_pDatModel;

    QTranslator qt_translator;
};

#endif // REGPARMWIZARD_H
