/********************************************************************************
** Form generated from reading UI file 'filetypespage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILETYPESPAGE_H
#define UI_FILETYPESPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_FileTypesPage
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tableView;

    void setupUi(QWizardPage *FileTypesPage)
    {
        if (FileTypesPage->objectName().isEmpty())
            FileTypesPage->setObjectName(QString::fromUtf8("FileTypesPage"));
        FileTypesPage->resize(509, 383);
        verticalLayout = new QVBoxLayout(FileTypesPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableView = new QTableView(FileTypesPage);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout->addWidget(tableView);


        retranslateUi(FileTypesPage);

        QMetaObject::connectSlotsByName(FileTypesPage);
    } // setupUi

    void retranslateUi(QWizardPage *FileTypesPage)
    {
        FileTypesPage->setWindowTitle(QCoreApplication::translate("FileTypesPage", "WizardPage", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileTypesPage: public Ui_FileTypesPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILETYPESPAGE_H
