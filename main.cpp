#include "RumblarD8.h"
#include <QtWidgets/QApplication>
#include <QtWidgets>
#include <QtGui>
#include <iostream>
#include "mates.h"
#include "tin.h"
#include "grid.h"
#include "EasyBMP.hpp"
#include <Windows.h>
#include <ctime>

#include <mutex>
#include <Windows.h>
#include <limits>

typedef std::numeric_limits< double > dbl;

using namespace std;


/* Reducción de mallas de terrenos usando DropHeuristic */
extern void dropHeuristic(char* nameFile, unsigned nPuntos, float error);


/* Conveirte un numero al rango 0-255*/
int get_color(float n, float v_max) {

	int normalizado = 0;

	normalizado = (n * 256) / v_max;

	return normalizado;
}


void dibujarBMP(Grid* grid, int n, float UMBRAL_PTOS_CARACTERISTICOS, std::string nombre) {
	EasyBMP::RGBColor fondo(255, 255, 255);
	EasyBMP::Image img(grid->ncolumnas, grid->nfilas, nombre, fondo);

	//PINTAMOS RUMBLAR
	for (auto it = grid->mapRumblar.begin(); it != grid->mapRumblar.end(); ++it) {
		img.DrawCircle(it->first.first, it->first.second, 1, EasyBMP::RGBColor(0, 0, 200));
	}

	//PINTAMOS JUNCTIONS DE FICHERO
	/*for (auto it = grid->mapJunctions.begin(); it != grid->mapJunctions.end(); ++it) {
		img.DrawCircle(it->first.second, it->first.first, 3, EasyBMP::RGBColor(0, 200, 120));
	}*/

	estructura_proyecto aux = grid->mapJunctions;

	Celda celda;
	int color = 0;
	for (unsigned i = 0; i < grid->nfilas; i++) {
		for (unsigned j = 0; j < grid->ncolumnas; j++) {
			celda = gridLeeCelda(grid, i, j);

			//numero que tengo que convertir a un rango 0-255 para mostrar el color
			color = get_color(celda.max_caudal, grid->v_maxCumulo);
			if (color != 0) {
				img.SetPixel(j, i, EasyBMP::RGBColor(color, 0, 0));
				// si es mayor que el x% del maximo acumulado, entoncxes es un punto caracteristico
				if ((celda.max_caudal > (UMBRAL_PTOS_CARACTERISTICOS * grid->v_maxCumulo)) && (celda.cumulo != 0)) {
					img.DrawCircle(j, i, 3, EasyBMP::RGBColor(255, 0, 0));

					std::pair<float, float> claveXY(i, j);
					std::vector<std::map<int, float>> posiciones;

					grid->mapJunctions.insert({ claveXY, posiciones });
				}
			}
		}
	}

	//PINTAMOS JUNCTIONS
	for (auto it = aux.begin(); it != aux.end(); ++it) {
		img.DrawCircle(it->first.first, it->first.second, 3, EasyBMP::RGBColor(0, 255, 0));
	}


	img.Write();

}

void comparaASCFile(string fichero1, string fichero2) {


	float cumulo;
	float cumuloTotal = 0;
	float cumulo2;
	float cumuloTotal2 = 0;
	float min1 = 9999, min2 = 9999;
	float max1 = 0, max2 = 0;
	int uso;
	string str;

	ifstream f(fichero1, ifstream::in);
	if (!f) {
		std::cout << "Error en la apertura del f1";
		exit(1);
	}

	ifstream u(fichero2, ifstream::in);
	if (!u) {
		std::cout << "Error en la apertura del f2";
		exit(1);
	}

	// Se obtienen las filas y columnas
	float nColumnas, nFilas;
	f >> str >> nColumnas;
	f >> str >> nFilas;

	// Se obtienen las coordenadas UTM x y
	float xCorner, yCorner;
	f >> str >> xCorner;
	f >> str >> yCorner;

	// Se obtiene el tamaño de la celda
	float cellSize;
	f >> str >> cellSize;

	std::cout << "---------- INFORMACION DEM ----------" << endl;
	std::cout << "- nfilas=" << nFilas << endl;
	std::cout << "- ncolumnas=" << nColumnas << endl;
	std::cout << "- xCorner=" << xCorner << endl;
	std::cout << "- yCorner=" << yCorner << endl;
	std::cout << "- cellSize=" << cellSize << endl;
	std::cout << "-------------------------------------" << endl;

	int numCeldas = nFilas * nColumnas;
	for (unsigned i = 0; i < nFilas; i++) {
		for (unsigned j = 0; j < nColumnas; j++) {
			f >> cumulo;
			u >> cumulo2;
			cumuloTotal += cumulo;
			cumuloTotal2 += cumulo2;
			if (cumulo > max1) {
				max1 = cumulo;
			}
			if (cumulo2 > max2) {
				max2 = cumulo2;
			}
			if (cumulo < min1) {
				min1 = cumulo;
			}
			if (cumulo2 < min2) {
				min2 = cumulo2;
			}
		}
	}
	f.close();
	std::cout << "---------- FICHERO QGIS ----------" << endl;
	std::cout << "- Media=" << cumuloTotal / numCeldas << endl;
	std::cout << "- Maximo valor=" << max1 << endl;
	std::cout << "- Minimo valor=" << min1 << endl;
	std::cout << "-------------------------------------" << endl;

	std::cout << "---------- FICHERO D8 ----------" << endl;
	std::cout << "- Media=" << cumuloTotal2 / numCeldas << endl;
	std::cout << "- Maximo valor=" << max2 << endl;
	std::cout << "- Minimo valor=" << min2 << endl;
	std::cout << "-------------------------------------" << endl;
}

std::string currentDateTime() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	char buffer[128];
	strftime(buffer, sizeof(buffer), "%m-%d-%Y_%H-%M-%S", now);
	return buffer;
}

/** Programa principal*/
int main(int argc, char** argv) {

	//comparaASCFile("Flow_accumulation.asc", "miD8-acumulacion.txt");

	//-------------------------------------------------------------------//
	//----------------------------RumblarD8-------------------------------//
	//-------------------------------------------------------------------//
	Grid* grid;
	QApplication a(argc, argv);
	RumblarD8 w(grid);
	w.setFixedSize(w.width(), w.height());
	w.show();
	
	
	a.exec();
	// a partir de aqui se ejecuta al pulsar el boton Run

	// Leer entradas de texto
	string inputDEM = w.ui.filename_DEM->text().toStdString();
	string usosSueloDEM = w.ui.filename_USO->text().toStdString();
	string strMAPA_LLUVIA = w.ui.filename_RAIN->text().toStdString();
	string ficheroJUNCTIONS = w.ui.filename_JUNCTION->text().toStdString();
	string ficheroPARAMETROS = w.ui.filename_JUNCTION_2->text().toStdString();
	float UMBRAL = stof(w.ui.edit_umbral->text().toStdString());
	int ITERACIONES = stoi(w.ui.nIteraciones->text().toStdString());
	string strFICHERO_OUT = w.ui.filaname_OUT->text().toStdString();
	string strIMAGEN_OUT = w.ui.imgname_OUT->text().toStdString();	

	string auxIT;
	if (ITERACIONES == 0) {
		auxIT = "Fin";
	}
	else {
		auxIT = to_string(ITERACIONES);
	}
	string aux = strFICHERO_OUT + "_th" + to_string(int(UMBRAL)) + "_it" + auxIT + "_" + currentDateTime();

	string fichero_filename = "OUTPUT_FILE/" + aux + ".csv";
	string imagen_filename = "OUTPUT_BMP/" + strIMAGEN_OUT + "_th" + to_string(int(UMBRAL)) + "_it" + auxIT + "_" + currentDateTime() + ".bmp";

	if (ITERACIONES == 0) {
		ITERACIONES = 999999;
	}

	//-------------------------------------------------------------------//
	//-------------------------------------------------------------------//
	if (w.get_continuar()) {
		//string inputDEM = "DEM/DEM_20m_NOSINKS_cotaembalse_50m.asc";
		//std::map<std::pair<int, int>, std::vector<std::map<int, float>>> mapJunctions;
		std::cout << "Cargando fichero DEM de entrada: " << inputDEM << "..." << std::endl;
		try {
			// Se carga el fichero DEM de entrada
			grid = GridNuevo(inputDEM, usosSueloDEM, ficheroJUNCTIONS, ficheroPARAMETROS, strMAPA_LLUVIA, &a);
			grid->filename = to_string(grid->cellSize) + "m_" + aux;
			grid->generateIPFiles = w.ui.m_check->isChecked();
			fichero_filename = "OUTPUT_FILE/" + grid->filename + ".csv";
		}
		catch (std::exception& e) {
			std::cerr << "Error al cargar el fichero DEM de entrada: " << std::endl;
			std::cerr << e.what() << std::endl;
			QString error = e.what();
			QMessageBox::warning(a.focusWidget(), "Error", "Error loading the input DEM file: " + error);
			return 1;
		}

		std::cout << std::endl << "Ejecutando algoritmo de extraccion de redes de drenaje .";
		try {
			gridRedDrenaje(grid, ITERACIONES, fichero_filename);
		}
		catch (std::exception& e) {
			std::cerr << "Error en la ejecucion del algoritmo de red de drenaje: " << std::endl;
			std::cerr << e.what() << std::endl;
			QString error = e.what();
			QMessageBox::warning(a.focusWidget(), "Error", "Error in the execution of the drainage network algorithm: " + error);
			return 1;
		}

		try {
			dibujarBMP(grid, ITERACIONES, (UMBRAL / 100), imagen_filename);
			std::cout << "\nVisualizacion exportada con exito" << std::endl;
		}
		catch (std::exception& e) {
			std::cerr << "Error al exportar la informacion en una imagen 2D: " << std::endl;
			std::cerr << e.what() << std::endl;
			QString error = e.what();
			QMessageBox::warning(a.focusWidget(), "Error", "Error when exporting information in a 2D image: " + error);
			return 1;
		}


		QString ruta = QString::fromStdString(imagen_filename);
		QGraphicsScene scene;
		QGraphicsView view(&scene);
		QGraphicsPixmapItem item(QPixmap(ruta, "BMP"));
		scene.addItem(&item);
		view.show();
		QMessageBox::information(a.focusWidget(), "Info", "Check the resulting image and decide whether you want to get the information from the points marked in red. Then close the image window.\nPath: /" + QString::fromUtf8(imagen_filename.c_str()));
		w.show();
		a.exec();

		std::cout << "Compruebe la imagen resultante y decida si quiere obtener la información de los puntos marcados en rojo..." << endl;
		if (w.get_ejecutar()) {
			std::cout << "\n\nEjecutando algoritmo de extraccion de redes de drenaje y obtencion de puntos caracteristicos .";
			try {
				gridRedDrenaje(grid, ITERACIONES, fichero_filename);
				QMessageBox::information(a.focusWidget(), "Info", "Output file generated.\nPath: /" + QString::fromUtf8(fichero_filename.c_str()));
			}
			catch (std::exception& e) {
				std::cerr << "Error en la ejecucion del algoritmo de red de drenaje(2): " << std::endl;
				std::cerr << e.what() << std::endl;
				QString error = e.what();
				QMessageBox::warning(a.focusWidget(), "Error", "Error in the execution of the drainage network algorithm(2): " + error);
				return 1;
			}
		}

		//gridImprimir(grid, "prueba131022.txt");
		//gridCumuloImprimir(grid, "salida.csv");

		std::cout << "\nFin del proceso: CORRECTO" << endl;
		QMessageBox::information(a.focusWidget(), "Info", "The process has been successfully completed.");
	}

	return 0;

}