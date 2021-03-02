/********************************************************************************
** Form generated from reading UI file 'datpage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATPAGE_H
#define UI_DATPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_DatPage
{
public:

    void setupUi(QWizardPage *DatPage)
    {
        if (DatPage->objectName().isEmpty())
            DatPage->setObjectName(QString::fromUtf8("DatPage"));
        DatPage->resize(468, 380);

        retranslateUi(DatPage);

        QMetaObject::connectSlotsByName(DatPage);
    } // setupUi

    void retranslateUi(QWizardPage *DatPage)
    {
        DatPage->setWindowTitle(QCoreApplication::translate("DatPage", "WizardPage", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DatPage: public Ui_DatPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATPAGE_H
