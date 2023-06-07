#include "RumblarD8.h"
#include <QWidget>
#include <QtGui>
#include <QFileDialog>
#include <Qt>


RumblarD8::RumblarD8(Grid* g, QWidget *parent)
    : QMainWindow(parent)
{
    continuar = false;
    ejecutar = false;

    ui.setupUi(this);

    //Connect button signal to appropriate slot
    connect(ui.boton_ejecutar, SIGNAL(released()), this, SLOT(handleButton()));

    connect(ui.boton_buscar, SIGNAL(released()), this, SLOT(buscarFichero()));
    connect(ui.boton_buscar2, SIGNAL(released()), this, SLOT(buscarFichero2()));
    connect(ui.boton_buscar3, SIGNAL(released()), this, SLOT(buscarFichero3()));
    connect(ui.boton_buscar4, SIGNAL(released()), this, SLOT(buscarFichero4()));
	connect(ui.boton_buscar7, SIGNAL(released()), this, SLOT(buscarFichero7()));

    connect(ui.m_check, SIGNAL(clicked(bool)), this, SLOT(SizeCHange(g,bool)));
    /*connect(ui.boton_buscar5, SIGNAL(released()), this, SLOT(buscarFichero5()));
    connect(ui.boton_buscar6, SIGNAL(released()), this, SLOT(buscarFichero6()));*/

}

//DEM
void RumblarD8::buscarFichero() {

    QStringList filename;
    const QStringList filters({ "ASCII files (*.asc)" });
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile("./DEM/")
        << QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());

    QFileDialog dialog;
    dialog.setAttribute(Qt::WA_QuitOnClose, false);
    dialog.setSidebarUrls(urls);
    dialog.setNameFilters(filters);
    dialog.setFileMode(QFileDialog::AnyFile);

    if (dialog.exec()) {
        filename = dialog.selectedFiles();
		ui.filename_DEM->setText(filename[0]);
    }
}

//USOS
void RumblarD8::buscarFichero2() {

    QStringList filename;
    const QStringList filters({ "ASCII files (*.asc)"});
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile("./USOS/")
        << QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());

    QFileDialog dialog;
    dialog.setSidebarUrls(urls);
    dialog.setNameFilters(filters);
    dialog.setFileMode(QFileDialog::AnyFile);

    if (dialog.exec()) {
        filename = dialog.selectedFiles();
		ui.filename_USO->setText(filename[0]);
    }
}

// MAPA DE LLUVIA
void RumblarD8::buscarFichero7() {

	QStringList filename;
	const QStringList filters({ "ASCII files (*.asc)" });
	QList<QUrl> urls;
	urls << QUrl::fromLocalFile("./INPUT_RAINMAP/")
		<< QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());

	QFileDialog dialog;
	dialog.setSidebarUrls(urls);
	dialog.setNameFilters(filters);
	dialog.setFileMode(QFileDialog::AnyFile);

	if (dialog.exec()) {
		filename = dialog.selectedFiles();
		ui.filename_RAIN->setText(filename[0]);
	}
}

//JUNCTION
void RumblarD8::buscarFichero3() {

    QStringList filename;
    const QStringList filters({ "CSV files (*.csv)" });
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile("./")
        << QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());

    QFileDialog dialog;
    dialog.setSidebarUrls(urls);
    dialog.setNameFilters(filters);
    dialog.setFileMode(QFileDialog::ExistingFile);
    
    if (dialog.exec()) {
        filename = dialog.selectedFiles();
		ui.filename_JUNCTION->setText(filename[0]);
    }
}

//PARAMETROS
void RumblarD8::buscarFichero4() {

	QStringList filename;
	const QStringList filters({ "CSV files (*.csv)" });
	QList<QUrl> urls;
	urls << QUrl::fromLocalFile("./")
		<< QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());

	QFileDialog dialog;
	dialog.setSidebarUrls(urls);
	dialog.setNameFilters(filters);
	dialog.setFileMode(QFileDialog::ExistingFile);

	if (dialog.exec()) {
		filename = dialog.selectedFiles();
		ui.filename_JUNCTION_2->setText(filename[0]);
	}
}

//SALIDA FICHERO
/*void RumblarD8::buscarFichero5() {

    QString filename;
    const QStringList filters({ "CSV files (*.csv)"});
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile("./SALIDAS")
        << QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());

    QFileDialog dialog;
    dialog.setSidebarUrls(urls);
    dialog.setNameFilters(filters);
    dialog.setFileMode(QFileDialog::ExistingFile);

    filename = dialog.getSaveFileName();

    ui.filaname_OUT->setText(filename);
}*/

//SALIDA IMAGEN
/*void RumblarD8::buscarFichero6() {

    QString filename;
    const QStringList filters({ "BMP files (*.bmp)" });
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile("./BMP")
        << QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());

    QFileDialog dialog;
    dialog.setSidebarUrls(urls);
    dialog.setNameFilters(filters);
    dialog.setFileMode(QFileDialog::ExistingFile);
    filename = dialog.getSaveFileName();

    ui.imgname_OUT->setText(filename);
}*/

void RumblarD8::SizeCHange(bool checked, Grid* g)
{
    if (checked)
    {
        g->generateIPFiles = true;
    }
    else
    {
        g->generateIPFiles = false;
    }
    ui.m_bouton->setVisible(checked);
}

void RumblarD8::handleButton()
{
    if (!continuar) {
        continuar = true;
        //change the text y desactivo todas las opciones ya que no se tendran en cuenta
        ui.boton_ejecutar->setText("Continue");
        ui.filename_DEM->setEnabled(false);
        ui.filename_JUNCTION->setEnabled(false);
        ui.filename_JUNCTION_2->setEnabled(false);
        ui.filename_USO->setEnabled(false);
		ui.filename_RAIN->setEnabled(false);
        ui.filaname_OUT->setEnabled(false);
        ui.imgname_OUT->setEnabled(false);
        ui.nIteraciones->setEnabled(false);
        ui.edit_umbral->setEnabled(false);
        ui.m_check->setEnabled(false);
		ui.boton_buscar->setEnabled(false);
		ui.boton_buscar2->setEnabled(false);
		ui.boton_buscar3->setEnabled(false);
		ui.boton_buscar4->setEnabled(false);
		ui.boton_buscar7->setEnabled(false);
    }
    else {
        ejecutar = true;
    }
    this->close();
}

RumblarD8::~RumblarD8()
{}

