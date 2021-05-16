/********************************************************************************
** Form generated from reading UI file 'seldirspage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELDIRSPAGE_H
#define UI_SELDIRSPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_SelDirsPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLineEdit *sourceEdit;
    QToolButton *sourceButton;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *hotfixEdit;
    QToolButton *hotfixButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWizardPage *SelDirsPage)
    {
        if (SelDirsPage->objectName().isEmpty())
            SelDirsPage->setObjectName(QString::fromUtf8("SelDirsPage"));
        SelDirsPage->resize(499, 372);
        verticalLayout = new QVBoxLayout(SelDirsPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(SelDirsPage);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        sourceEdit = new QLineEdit(SelDirsPage);
        sourceEdit->setObjectName(QString::fromUtf8("sourceEdit"));
        sourceEdit->setReadOnly(true);

        horizontalLayout->addWidget(sourceEdit);

        sourceButton = new QToolButton(SelDirsPage);
        sourceButton->setObjectName(QString::fromUtf8("sourceButton"));

        horizontalLayout->addWidget(sourceButton);


        verticalLayout->addLayout(horizontalLayout);

        label_2 = new QLabel(SelDirsPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        hotfixEdit = new QLineEdit(SelDirsPage);
        hotfixEdit->setObjectName(QString::fromUtf8("hotfixEdit"));
        hotfixEdit->setReadOnly(true);

        horizontalLayout_2->addWidget(hotfixEdit);

        hotfixButton = new QToolButton(SelDirsPage);
        hotfixButton->setObjectName(QString::fromUtf8("hotfixButton"));

        horizontalLayout_2->addWidget(hotfixButton);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 257, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(SelDirsPage);

        QMetaObject::connectSlotsByName(SelDirsPage);
    } // setupUi

    void retranslateUi(QWizardPage *SelDirsPage)
    {
        SelDirsPage->setWindowTitle(QCoreApplication::translate("SelDirsPage", "WizardPage", nullptr));
        label->setText(QCoreApplication::translate("SelDirsPage", "\320\232\320\260\321\202\320\260\320\273\320\276\320\263 \321\201 \320\270\321\201\321\205\320\276\320\264\320\275\321\213\320\274\320\270 \321\204\320\260\320\271\320\273\320\260\320\274\320\270:", nullptr));
#if QT_CONFIG(tooltip)
        sourceEdit->setToolTip(QCoreApplication::translate("SelDirsPage", "\320\232\320\260\321\202\320\260\320\273\320\276\320\263 \321\201 \320\270\321\201\321\205\320\276\320\264\320\275\321\213\320\274\320\270 \321\204\320\260\320\271\320\273\320\260\320\274\320\270", nullptr));
#endif // QT_CONFIG(tooltip)
        sourceButton->setText(QCoreApplication::translate("SelDirsPage", "...", nullptr));
        label_2->setText(QCoreApplication::translate("SelDirsPage", "\320\232\320\260\321\202\320\260\320\273\320\276\320\263 \321\205\320\276\321\202\321\204\320\270\320\272\321\201\320\260:", nullptr));
#if QT_CONFIG(tooltip)
        hotfixEdit->setToolTip(QCoreApplication::translate("SelDirsPage", "\320\232\320\260\321\202\320\260\320\273\320\276\320\263 \321\205\320\276\321\202\321\204\320\270\320\272\321\201\320\260", nullptr));
#endif // QT_CONFIG(tooltip)
        hotfixButton->setText(QCoreApplication::translate("SelDirsPage", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelDirsPage: public Ui_SelDirsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELDIRSPAGE_H
