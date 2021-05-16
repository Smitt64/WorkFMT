/********************************************************************************
** Form generated from reading UI file 'filelistpage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILELISTPAGE_H
#define UI_FILELISTPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_FileListPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWizardPage *FileListPage)
    {
        if (FileListPage->objectName().isEmpty())
            FileListPage->setObjectName(QString::fromUtf8("FileListPage"));
        FileListPage->resize(510, 389);
        verticalLayout = new QVBoxLayout(FileListPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(FileListPage);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        plainTextEdit = new QPlainTextEdit(FileListPage);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        verticalLayout->addWidget(plainTextEdit);


        retranslateUi(FileListPage);

        QMetaObject::connectSlotsByName(FileListPage);
    } // setupUi

    void retranslateUi(QWizardPage *FileListPage)
    {
        FileListPage->setWindowTitle(QCoreApplication::translate("FileListPage", "WizardPage", nullptr));
        label->setText(QCoreApplication::translate("FileListPage", "\320\241\320\277\320\270\321\201\320\276\320\272 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\275\321\213\321\205 \321\204\320\260\320\271\320\273\320\276\320\262:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileListPage: public Ui_FileListPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILELISTPAGE_H
