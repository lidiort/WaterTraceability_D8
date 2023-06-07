/********************************************************************************
** Form generated from reading UI file 'RumblarD8.ui'
**
** Created by: Qt User Interface Compiler version 6.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RumblarD8_H
#define UI_RumblarD8_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>

QT_BEGIN_NAMESPACE

class Ui_RumblarD8
{
public:
    QWidget* centralwidget;
    QGroupBox* groupBox;
    QLineEdit* filename_DEM;
    QLineEdit* filename_USO;
	QLineEdit* filename_RAIN;
    QSpinBox* nIteraciones;
    QLabel* label;
    QLabel* label_2;
	QLabel* label_9;
    QGroupBox* groupBox_3;
    QLabel* label_3;
    QLineEdit* filename_JUNCTION;
    QLabel* label_4;
    QSpinBox* edit_umbral;
    QLabel* label_5;
    QLabel* label_8;
    QLineEdit* filename_JUNCTION_2;
    QGroupBox* groupBox_2;
    QPushButton* m_bouton;
    QCheckBox* m_check;
    QLabel* label_6;
    QLabel* label_check;
    QLineEdit* filaname_OUT;
    QLabel* label_7;
    QLineEdit* imgname_OUT;
    QPushButton* boton_ejecutar;
    QMenuBar* menubar;
    QStatusBar* statusbar;
    QPushButton* boton_buscar;
    QPushButton* boton_buscar2;
    QPushButton* boton_buscar3;
    QPushButton* boton_buscar4;
    /*QPushButton* boton_buscar5;
    QPushButton* boton_buscar6;*/
	QPushButton* boton_buscar7;

    void setupUi(QMainWindow* RumblarD8)
    {
        if (RumblarD8->objectName().isEmpty())
            RumblarD8->setObjectName("RumblarD8");
        RumblarD8->resize(571, 633);
        centralwidget = new QWidget(RumblarD8);
        centralwidget->setObjectName("centralwidget");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(30, 20, 511, 381));

        // filename DEM ¬ USO
        filename_DEM = new QLineEdit(groupBox);
        filename_DEM->setObjectName("filename_DEM");
        filename_DEM->setGeometry(QRect(200, 50, 225, 22));
        filename_USO = new QLineEdit(groupBox);
        filename_USO->setObjectName("filename_USO");
        filename_USO->setGeometry(QRect(200, 90, 225, 22));
		filename_RAIN = new QLineEdit(groupBox);
		filename_RAIN->setObjectName("filename_RAIN");
		filename_RAIN->setGeometry(QRect(200, 130, 225, 22));

        boton_buscar = new QPushButton(groupBox);
        boton_buscar->setObjectName("boton_buscar");
        boton_buscar->setGeometry(QRect(427, 50, 25, 22));
        boton_buscar2 = new QPushButton(groupBox);
        boton_buscar2->setObjectName("boton_buscar2");
        boton_buscar2->setGeometry(QRect(427, 90, 25, 22));
		boton_buscar7 = new QPushButton(groupBox);
		boton_buscar7->setObjectName("boton_buscar7");
		boton_buscar7->setGeometry(QRect(427, 130, 25, 22));
        //

        nIteraciones = new QSpinBox(groupBox);
        nIteraciones->setObjectName("nIteraciones");
        nIteraciones->setGeometry(QRect(200, 340, 61, 22));
        nIteraciones->setMaximum(9999);
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 50, 141, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 90, 171, 16));
		label_9 = new QLabel(groupBox);
		label_9->setObjectName("label_7");
		label_9->setGeometry(QRect(30, 130, 171, 16));
        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(20, 170, 461, 111));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(30, 30, 141, 16));

        // FILENAME JUNCTION
        filename_JUNCTION = new QLineEdit(groupBox_3);
        filename_JUNCTION->setObjectName("filename_JUNCTION");
        filename_JUNCTION->setGeometry(QRect(180, 30, 225, 22));
        boton_buscar3 = new QPushButton(groupBox_3);
        boton_buscar3->setObjectName("boton_buscar3");
        boton_buscar3->setGeometry(QRect(407, 30, 25, 22));

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(30, 70, 141, 16));

        edit_umbral = new QSpinBox(groupBox_3);
        edit_umbral->setObjectName("edit_umbral");
        edit_umbral->setGeometry(QRect(180, 70, 61, 22));
        edit_umbral->setMaximum(100);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(40, 340, 141, 16));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(40, 300, 141, 16));

        filename_JUNCTION_2 = new QLineEdit(groupBox);
        filename_JUNCTION_2->setObjectName("filename_JUNCTION_2");
        filename_JUNCTION_2->setGeometry(QRect(200, 300, 225, 22));
        boton_buscar4 = new QPushButton(groupBox);
        boton_buscar4->setObjectName("boton_buscar3");
        boton_buscar4->setGeometry(QRect(427, 300, 25, 22));


        groupBox_3->raise();
        filename_DEM->raise();
        filename_USO->raise();
        nIteraciones->raise();
        label->raise();
        label_2->raise();
        label_5->raise();
        label_8->raise();
        filename_JUNCTION_2->raise();
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(30, 410, 511, 160));
        

        label_check = new QLabel(groupBox_2);
        label_check->setObjectName("label_6");
        label_check->setGeometry(QRect(30, 40, 141, 16));

        m_bouton = new QPushButton("Module 1");
        m_bouton->setCursor(Qt::PointingHandCursor);
        m_check = new QCheckBox(groupBox_2);
        m_check->move(200, 40);
        m_check->setChecked(true);


        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(30, 80, 141, 16));

        filaname_OUT = new QLineEdit(groupBox_2);
        filaname_OUT->setObjectName("filaname_OUT");
        filaname_OUT->setGeometry(QRect(200, 80, 255, 22));
        /*boton_buscar5 = new QPushButton(groupBox_2);
        boton_buscar5->setObjectName("boton_buscar3");
        boton_buscar5->setGeometry(QRect(427, 40, 25, 22));*/

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(30, 120, 141, 16));

        imgname_OUT = new QLineEdit(groupBox_2);
        imgname_OUT->setObjectName("imgname_OUT");
        imgname_OUT->setGeometry(QRect(200, 120, 255, 22));
        /*boton_buscar6 = new QPushButton(groupBox_2);
        boton_buscar6->setObjectName("boton_buscar3");
        boton_buscar6->setGeometry(QRect(427, 80, 25, 22));*/


        boton_ejecutar = new QPushButton(centralwidget);
        boton_ejecutar->setObjectName("boton_ejecutar");
        boton_ejecutar->setGeometry(QRect(467, 580, 75, 24));
        RumblarD8->setCentralWidget(centralwidget);
        groupBox_2->raise();
        groupBox->raise();
        boton_ejecutar->raise();
        menubar = new QMenuBar(RumblarD8);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 571, 22));
        RumblarD8->setMenuBar(menubar);
        statusbar = new QStatusBar(RumblarD8);
        statusbar->setObjectName("statusbar");
        RumblarD8->setStatusBar(statusbar);

        retranslateUi(RumblarD8);

        QMetaObject::connectSlotsByName(RumblarD8);
    } // setupUi

    void retranslateUi(QMainWindow* RumblarD8)
    {
        RumblarD8->setWindowTitle(QCoreApplication::translate("RumblarD8", "RUMBLAR ALGORITHM D8", nullptr));
        groupBox->setTitle(QCoreApplication::translate("RumblarD8", "Input", nullptr));
        filename_DEM->setText(QCoreApplication::translate("RumblarD8", "INPUT_DEM/", nullptr));
        filename_USO->setText(QCoreApplication::translate("RumblarD8", "INPUT_LANDUSE/", nullptr));
		filename_RAIN->setText(QCoreApplication::translate("RumblarD8", "INPUT_RAINMAP/", nullptr));
        label->setText(QCoreApplication::translate("RumblarD8", "DEM File (*)", nullptr));
        label_2->setText(QCoreApplication::translate("RumblarD8", "Land use file (*)", nullptr));
		label_9->setText(QCoreApplication::translate("RumblarD8", "Rain map file (*)", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("RumblarD8", "Representative Points (RP)", nullptr));
        label_3->setText(QCoreApplication::translate("RumblarD8", "RP file", nullptr));
        filename_JUNCTION->setText(QCoreApplication::translate("RumblarD8", "INPUT_RP/", nullptr));
        label_4->setText(QCoreApplication::translate("RumblarD8", "Threshold [0,100] (*)", nullptr));
        //edit_umbral->setText(QCoreApplication::translate("RumblarD8", "70", nullptr));
        edit_umbral->setValue(50);
        label_5->setText(QCoreApplication::translate("RumblarD8", "Number of iterations (*)", nullptr));
        label_8->setText(QCoreApplication::translate("RumblarD8", "Absorption parameters (*)", nullptr));
        filename_JUNCTION_2->setText(QCoreApplication::translate("RumblarD8", "INPUT_PARAMETERS/parameters.csv", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("RumblarD8", "Output", nullptr));
        label_6->setText(QCoreApplication::translate("RumblarD8", "File name (*)", nullptr));
        label_check->setText(QCoreApplication::translate("RumblarD8", "Generate RP files", nullptr));
        filaname_OUT->setText(QCoreApplication::translate("RumblarD8", "out_file", nullptr));
        label_7->setText(QCoreApplication::translate("RumblarD8", "Image name (*)", nullptr));
        imgname_OUT->setText(QCoreApplication::translate("RumblarD8", "img", nullptr));
        boton_ejecutar->setText(QCoreApplication::translate("RumblarD8", "Run", nullptr));
        boton_buscar->setText(QCoreApplication::translate("RumblarD8", "...", nullptr));
        boton_buscar2->setText(QCoreApplication::translate("RumblarD8", "...", nullptr));
        boton_buscar3->setText(QCoreApplication::translate("RumblarD8", "...", nullptr));
        boton_buscar4->setText(QCoreApplication::translate("RumblarD8", "...", nullptr));
        /*boton_buscar5->setText(QCoreApplication::translate("RumblarD8", "...", nullptr));
        boton_buscar6->setText(QCoreApplication::translate("RumblarD8", "...", nullptr));*/
		boton_buscar7->setText(QCoreApplication::translate("RumblarD8", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RumblarD8 : public Ui_RumblarD8 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RumblarD8_H
