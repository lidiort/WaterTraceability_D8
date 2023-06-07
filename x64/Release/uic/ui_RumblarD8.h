/********************************************************************************
** Form generated from reading UI file 'RumblarD8.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUMBLARD8_H
#define UI_RUMBLARD8_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RumblarD8Class
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RumblarD8Class)
    {
        if (RumblarD8Class->objectName().isEmpty())
            RumblarD8Class->setObjectName("RumblarD8Class");
        RumblarD8Class->resize(600, 400);
        menuBar = new QMenuBar(RumblarD8Class);
        menuBar->setObjectName("menuBar");
        RumblarD8Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RumblarD8Class);
        mainToolBar->setObjectName("mainToolBar");
        RumblarD8Class->addToolBar(mainToolBar);
        centralWidget = new QWidget(RumblarD8Class);
        centralWidget->setObjectName("centralWidget");
        RumblarD8Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(RumblarD8Class);
        statusBar->setObjectName("statusBar");
        RumblarD8Class->setStatusBar(statusBar);

        retranslateUi(RumblarD8Class);

        QMetaObject::connectSlotsByName(RumblarD8Class);
    } // setupUi

    void retranslateUi(QMainWindow *RumblarD8Class)
    {
        RumblarD8Class->setWindowTitle(QCoreApplication::translate("RumblarD8Class", "RumblarD8", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RumblarD8Class: public Ui_RumblarD8Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUMBLARD8_H
