#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_RumblarD8.h"
#include "grid.h"

class RumblarD8 : public QMainWindow
{
    Q_OBJECT

public:
    RumblarD8(Grid* g, QWidget *parent = nullptr);
    ~RumblarD8();

    bool get_ejecutar() { return ejecutar; };
    bool get_continuar() { return continuar; };

//private:
    Ui::RumblarD8 ui;

private slots:

    void handleButton();
    void buscarFichero();
    void buscarFichero2();
    void buscarFichero3();
    void buscarFichero4();
	void buscarFichero7();
    void SizeCHange(bool checked, Grid* g);
    //void buscarFichero5();
    //void buscarFichero6();

private:
    bool continuar;
    bool ejecutar;

};
