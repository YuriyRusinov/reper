/********************************************************************************
** Form generated from reading UI file 'table_view.ui'
**
** Created: Mon 12. Nov 15:57:35 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABLE_VIEW_H
#define UI_TABLE_VIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_table_viewClass
{
public:

    void setupUi(QWidget *table_viewClass)
    {
        if (table_viewClass->objectName().isEmpty())
            table_viewClass->setObjectName(QString::fromUtf8("table_viewClass"));
        table_viewClass->resize(600, 400);

        retranslateUi(table_viewClass);

        QMetaObject::connectSlotsByName(table_viewClass);
    } // setupUi

    void retranslateUi(QWidget *table_viewClass)
    {
        table_viewClass->setWindowTitle(QApplication::translate("table_viewClass", "table_view", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class table_viewClass: public Ui_table_viewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABLE_VIEW_H
