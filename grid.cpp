/***************************************************************************
 *   Copyright (C) 2008 by Lidia Ortega   *
 *   lidia@ujaen.es   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <sstream>
#include <regex>
#include <iomanip>

#include <filesystem>

#include "grid.h"
#include "mates.h"
#include "hgtloader.h"

#include <QtWidgets>
#include <QtGui>

typedef std::numeric_limits< double > dbl;

using namespace std;

namespace fs = std::filesystem;


void leeAscFile(Grid* g, string namefile) {

	g->nfilas = g->ncolumnas = 0;
	float x, y, h;
	long int xx, yy, ayy, axx;
	char c;
	unsigned cont = 0;
	unsigned nvertices = 0;
	bool flag = true; //hacer algo diferente la primera vez

	ifstream f(namefile, ifstream::in);
	if (!f) {
		std::cout << "Error en la apertura del fichero";
		exit(1);
	}
	// recorremos el fichero para conocer filas y columnas
	while (!f.eof()) {
		f >> x >> c >> y >> c >> h;
		xx = (long int)x;
		yy = (long int)y;
		nvertices++;
		//std::cout << setw(8)  << xx << setw(12)  <<  yy << setw(8)  << h << endl;
		std::cout << xx << yy << h << endl;

		//la primera vez no existe el anterior
		if (flag) flag = false;
		else {
			// averiguramos filas y columnas
			if (ayy == yy) { cont++; g->cellSize = abs(axx - xx); }
			else { g->nfilas++; g->ncolumnas = cont; cont = 0; g->cellSize = abs(ayy - yy); }
		}
		ayy = yy;
		axx = xx;
	}
	g->nfilas++; g->ncolumnas++;
	std::cout << "nfilas=" << g->nfilas << " ncolumnas=" << g->ncolumnas << endl;
	std::cout << "numero vertices:" << nvertices << endl;
	std::cout << "ancho=" << g->cellSize << " alto=" << g->cellSize << endl;

	g->celdas = new Celda * [g->nfilas];
	if (!g->celdas) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
	for (unsigned i = 0; i < g->nfilas; i++) {
		g->celdas[i] = new Celda[g->ncolumnas]; if (!g->celdas[i]) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
	}

	f.close();
	f.open(namefile, ios::in);

	for (unsigned i = 0; i < g->nfilas; i++) {
		for (unsigned j = 0; j < g->ncolumnas; j++) {
			f >> x >> c >> y >> c >> h;
			Celda c = celdaNuevo(h,false);
			g->celdas[i][j] = c;
			if (i == 0 && j == 0) g->infizq = verticeNuevo((long int)x, (long int)y, h); //cuidado, el 
			if (i == g->nfilas - 1 && j == g->ncolumnas - 1)
				g->supder = verticeNuevo((long int)x, (long int)y, h);
		}
	}
	f.close();
}

void leeASCFile(Grid* g, string namefile_DEM, string namefile_usos, string namefile_junctions, string rainmapFile, QApplication* a) {

	//inicializacion
	g->v_maxDesnivel = 0;
	g->generateIPFiles = false;
	g->filename = "";

	float h;
	int uso;
	string str;

	ifstream f(namefile_DEM, ifstream::in);
	if (!f) {
		std::cout << "Error en la apertura del DEM";
		QMessageBox::warning(a->focusWidget(), "Error", "Error in the opening of the DEM");
		exit(1);
	}

	ifstream u(namefile_usos, ifstream::in);
	if (!u) {
		std::cout << "Error en la apertura del USO DEL SUELO";
		QMessageBox::warning(a->focusWidget(), "Error", "Error in the opening of LAND USE");
		exit(1);
	}

	// recorremos las primeras líneas de cabezera
	g->v_maxCumulo = 0;

	// Se obtienen las filas y columnas
	float nColumnas, nFilas;
	f >> str >> nColumnas;
	f >> str >> nFilas;

	// Se obtienen las coordenadas UTM de los corners x y
	float xCorner, yCorner;
	f >> str >> xCorner;
	f >> str >> yCorner;

	// Se obtiene el tamaño de la celda
	float cellSize;
	f >> str >> cellSize;

	g->nfilas = nFilas;
	g->ncolumnas = nColumnas;
	g->cornerX = xCorner;
	g->cornerY = yCorner;
	g->cellSize = cellSize;

	leeJunctions(g, namefile_junctions);

	ifstream rm(rainmapFile, ifstream::in);
	if (rm) {
		loadRainMap(g, rainmapFile);
	}

	std::cout << "---------- INFORMACION DEM ----------" << endl;
	std::cout << "- nfilas=" << g->nfilas << endl;
	std::cout << "- ncolumnas=" << g->ncolumnas << endl;
	std::cout << "- xCorner=" << g->cornerX << endl;
	std::cout << "- yCorner=" << g->cornerY << endl;
	std::cout << "- cellSize=" << g->cellSize << endl;
	std::cout << "-------------------------------------" << endl;

	// IGNORO LÍNEAS //
	float nColumnasUsos, nFilasUsos;
	u >> str >> nColumnasUsos;
	u >> str >> nFilasUsos;
	if ((nColumnas != nColumnasUsos) || nFilas != nFilasUsos) {
		std::cout << "Las dimensiones de filas y columnas de los ficheros DEM y USOS DEL SUELO deben ser identicas";
		QMessageBox::warning(a->focusWidget(), "Error", "The row and column dimensions of the DEM and LAND USE files must be identical. \nDEM: " + QString::fromUtf8(to_string(nFilas)) + "x" + QString::fromUtf8(to_string(nColumnas)) +
			"\nLAND USE: " + QString::fromUtf8(to_string(nFilasUsos)) + "x" + QString::fromUtf8(to_string(nColumnasUsos)));
		exit(1);
	}
	/*getline(u, str);
	getline(u, str);*/
	getline(u, str);
	getline(u, str);
	getline(u, str);
	getline(u, str);

	g->celdas = new Celda * [g->nfilas];
	if (!g->celdas) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
	for (unsigned i = 0; i < g->nfilas; i++) {
		g->celdas[i] = new Celda[g->ncolumnas]; if (!g->celdas[i]) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
	}

	std::vector<int> codigos;
	std::vector<int>::iterator it;

	for (unsigned i = 0; i < g->nfilas; i++) {
		for (unsigned j = 0; j < g->ncolumnas; j++) {
			f >> h;
			u >> uso;
			if (uso < 0 || h < 0) {
				exit(1);
				std::cout << "Error en la apertura del los ficheros";
				QMessageBox::warning(a->focusWidget(), "Error", "Error in the opening of LAND USE and DEM");
				exit(1);
			}

			bool junction = false;
			std::map<char, int>::iterator it;
			std::pair<float, float> pair(i, j);
			if (g->mapJunctions.find(pair) != g->mapJunctions.end() ) {
				junction = true;
			}

			if (h == -9999) h = 0;

			float xUTM = (i * g->cellSize) + g->cornerX;
			float yUTM = (g->nfilas - j) * g->cellSize + g->cornerY; //ToDo

			Celda c = celdaNuevo(h, junction, xUTM, yUTM);

			c.uso_suelo = uso;
			g->celdas[i][j] = c;
			//para depurar agrego los atributos i,j
			g->celdas[i][j].i = i;
			g->celdas[i][j].j = j;
		}
	}
	f.close();
}

void leeHgtFile(Grid* g, string namefile) {
	HGTLoader hgtl;
	vector<vector<int> > elevMap;
	hgtl.load(namefile, elevMap);

	// ahora pasamos la información del vector de stl al grid
	g->nfilas = g->ncolumnas = 1021;
	g->celdas = new Celda * [g->nfilas];
	if (!g->celdas) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
	for (unsigned i = 0; i < g->nfilas; i++) {
		g->celdas[i] = new Celda[g->ncolumnas]; if (!g->celdas[i]) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
	}

	for (int f = 0; f < g->nfilas; f++) {
		for (int c = 0; c < g->ncolumnas; c++) {
			Celda cel = celdaNuevo(elevMap[f][c], false);
			g->celdas[f][c] = cel;
		}
	}
}

void leeParametros(Grid* g, string namefile_parametros) {
	int i = 0;
	ifstream archivo(namefile_parametros);
	string linea;
	char delimitador = ',';

	// Leemos la primer línea para descartarla
	getline(archivo, linea);
	// Leemos todas las líneas
	while (getline(archivo, linea))
	{
		stringstream stream(linea); // Convertir la cadena a un stream
		string idUso,uso, porcentajeUso;

		getline(stream, idUso, delimitador);
		getline(stream, uso, delimitador);
		getline(stream, porcentajeUso, delimitador);
		g->mapPorcAbsorcion.insert({ std::stoi(idUso),std::stof(porcentajeUso) });
	}
	archivo.close();
}

void leeJunctions(Grid* g, std::string namefile_junctions) {
	
	//---------------------------------------------//
	// PUNTOS RUMBLAR - REPRESENTACIÓN DEL PANTANO //
	//---------------------------------------------//

	ifstream archivo("INPUT_PARAMETERS/PTOS_RUMBLAR_ID_H_XY.csv"); 
	//ifstream archivo("junctions.csv");
	string linea;
	char delimitador = ',';

	float xNormalizado, yNormalizado;
	std::string key;

	// Leemos la primer línea para descartarla
	getline(archivo, linea);
	// Leemos todas las líneas
	while (getline(archivo, linea))
	{
		stringstream stream(linea); // Convertir la cadena a un stream
		string fid, H, X, Y;

		getline(stream, fid, delimitador);
		getline(stream, H, delimitador);
		getline(stream, X, delimitador);
		getline(stream, Y, delimitador);

		// Se calcula la posición de la matriz que se corresponde con las coordenadas UTM dadas

		xNormalizado = (stof(X) - g->cornerX) / g->cellSize; // la x son las columnas
		yNormalizado = g->nfilas - (stof(Y) - g->cornerY) / g->cellSize; //la y son las filas

		std::pair<float,float> claveXY(xNormalizado, yNormalizado);
		std::vector<std::map<int,float>> posiciones;

		g->mapRumblar.insert({ claveXY, posiciones });
	}
	archivo.close();

	//------------------------------------------//
	// JUNCTIONS RUMBLAR - LECTURA DE JUNCTIONS //
	//------------------------------------------//
	ifstream archivo2(namefile_junctions);

	getline(archivo2, linea);
	while (getline(archivo2, linea))
	{
		stringstream stream(linea); // Convertir la cadena a un stream
		string fid, H, X, Y;

		getline(stream, fid, delimitador);
		getline(stream, H, delimitador);
		getline(stream, X, delimitador);
		getline(stream, Y, delimitador);

		// Se calcula la posición de la matriz que se corresponde con las coordenadas UTM dadas
		xNormalizado = (stof(X) - g->cornerX) / g->cellSize; // la x son las columnas
		yNormalizado = g->nfilas - (stof(Y) - g->cornerY) / g->cellSize; //la y son las filas

		std::pair<float, float> claveXY(yNormalizado, xNormalizado);
		std::vector<std::map<int, float>> posiciones;

		g->mapJunctions.insert({ claveXY, posiciones });
	}
	archivo2.close();

}

void loadRainMap(Grid* g, std::string file)
{
	ifstream f(file);
	string line;

	int nColumnas, nFilas;
	f >> line >> nColumnas;
	f >> line >> nFilas;

	// saltar las lineas restantes que hay antes de la matriz
	getline(f, line);
	getline(f, line);
	getline(f, line);
	getline(f, line);
	getline(f, line);
	getline(f, line);
	
	g->rainmap = new float* [g->nfilas];

	for (unsigned i = 0; i < g->nfilas; i++) {
		g->rainmap[i] = new float[g->ncolumnas];

		for (unsigned j = 0; j < g->ncolumnas; j++) {
			float value;
			f >> value;

			// buscar el valor minimo y maximo
			if (value < g->minRainValue) g->minRainValue = value;
			if (value > g->maxRainValue) g->maxRainValue = value;

			g->rainmap[i][j] = value;
		}
	}

	f.close();
}

float setValueIntoRange(Grid* g, float value, float vmin, float vmax)
{
	return vmin + ((value - g->minRainValue) / (g->maxRainValue - g->minRainValue)) * (vmax - vmin);
}

/**crea un grid a partir de la información de un fichero .asc*/
Grid* GridNuevo(string namefile_DEM, string namefile_usos, string namefile_junctions, string namefile_parametros, string rainmapFile, QApplication* a) {

	Grid* g = new Grid();
	if (!g) { std::cout << "Error al inicializar memoria" << endl; exit(1); }

	g->segundaVuelta = false;
	g->rainmap = 0;
	g->minRainValue = FLT_MAX;
	g->maxRainValue = 0.0;

	// Lee parametros de configuración
	leeParametros(g, namefile_parametros);
	leeASCFile(g, namefile_DEM, namefile_usos, namefile_junctions, rainmapFile, a);

	return g;
}

void gridImprimir(Grid* g, std::string namefile) {
	ofstream f(namefile, ofstream::out);
	if (!f) {
		std::cout << "Error en la apertura del fichero";
		exit(1);
	}
	Celda ce;
	char c;
	for (unsigned i = 0; i < g->nfilas; i++) {
		for (unsigned j = 0; j < g->ncolumnas; j++) {
			ce = gridLeeCelda(g, i, j);
			if (celdaSink(ce)) { c = 'O'; f << c; continue; }
			switch (celdaDir(ce)) {
				case 0: c = '*'; break; // son hoyos
				case 1: c = '>'; break; //e
				case 2: c = '%'; break; //se
				case 4: c = '!'; break; //s
				case 8: c = ','; break; //so
				case 16: c = '<'; break;//o
				case 32: c = '"'; break; //no
				case 64: c = '|'; break; //n
				case 128: c = '/'; break;//ne
				case 10: c = '='; break;
			}
			f << c;
		}
		f << endl;
	}

	f << "nfilas" << g->nfilas << endl;
	f << "ncolumnas " << g->ncolumnas << endl;
	f << "cellSize " << g->cellSize << endl;
	f << "infizq " << g->infizq.x << " " << g->infizq.y << " " << endl;
	f << "supder " << g->supder.x << " " << g->supder.y << " " << endl;
}

/*
xNormalizado = (unsigned int)ceil((stof(XUTM) - g->cornerX) / g->cellSize); // la x son las columnas
yNormalizado = (unsigned int)ceil(g->nfilas - (stof(YUTM) - g->cornerY) / g->cellSize); //la y son las filas
*/

void gridCumuloImprimir(Grid *g, std::string namefile) {
	ofstream f(namefile, ofstream::out);
	if (!f) {
		std::cout << "Error en la apertura del fichero";
		exit(1);
	}
	Celda ce;
	char c;
	float xUTM, yUTM;
	for (unsigned i = 0; i < g->nfilas; i++) {
		for (unsigned j = 0; j < g->ncolumnas; j++) {
			ce = gridLeeCelda(g, i, j);
			xUTM = (i * g->cellSize) + g->cornerX;
			yUTM = g->cornerY - g->nfilas - (j * g->cellSize);
			f << std::fixed << setprecision(dbl::max_digits10);
			f << xUTM << "," << yUTM;
			auto iter = ce.map_codSuelo.begin();
			while (iter != ce.map_codSuelo.end()) {
				f << std::fixed << setprecision(dbl::max_digits10);
				f << "," << iter->first << "," << iter->second;
				++iter;
			}
			f << endl;
		}
	}
	f.close();
}

Celda gridLeeCelda(Grid* g, unsigned i, unsigned j) {
	if (i<0 || i>g->nfilas) { std::cout << "Error en los rangos de acceso" << endl; exit(1); }
	if (j<0 || j>g->ncolumnas) { std::cout << "Error en los rangos de acceso" << endl; exit(1); }
	return g->celdas[i][j];
}

void gridModiCelda(Grid* g, unsigned i, unsigned j, Celda c) {
	if (i<0 || i>g->nfilas) { std::cout << "Error en los rangos de acceso" << endl; exit(1); }
	if (j<0 || j>g->ncolumnas) { std::cout << "Error en los rangos de acceso" << endl; exit(1); }
	g->celdas[i][j] = c;
}

void gridDestruye(Grid** g) {
	for (unsigned i = 0; i < (*g)->nfilas; i++) delete[](*g)->celdas[i];
	delete[](*g)->celdas;
	delete[](*g);
}

bool gridCeldaExterior(Grid* g, unsigned i, unsigned j) {
	if (i == 0 || j == 0 || i == g->nfilas || j == g->ncolumnas) return true;
	return false;

}

unsigned short gridDirCelda(Grid* g, unsigned ii, unsigned jj) {
	int i = (int)ii;
	int j = (int)jj;
	unsigned filadiag = i, filaorto = i, filatot = i, fila = i;
	unsigned columnadiag = j, columnaorto = j, columnatot = j, columna = j;
	float hij = celdaAltura(g->celdas[i][j]);
	float hmin = hij; // la inicializo a cualquier valor buscando el minimo
	float haux;
	int imenos1, imas1, jmas1, jmenos1;

	if (i - 1 < 0) imenos1 = 0;  else imenos1 = i - 1;
	if (i + 1 == g->nfilas) imas1 = i; else imas1 = i + 1;
	if (j - 1 < 0) jmenos1 = 0; else jmenos1 = j - 1;
	if (j + 1 == g->ncolumnas) jmas1 = j; else jmas1 = j + 1;

	float hmin_diag = 9e9;
	float hmin_orto = 9e9;
	float hmintotal = 9e9; //el minimo general

	//bool orto=false, diag=false;
	for (unsigned k = imenos1; k <= imas1; k++) {
		for (unsigned l = jmenos1; l <= jmas1; l++) {
			if (!(k == i && l == j)) {
				haux = celdaAltura(g->celdas[k][l]);
				// minimo ortogonal
				if (k == i || l == j) {
					if (haux <= hmin_orto) { hmin_orto = haux; filaorto = k; columnaorto = l; }
					//orto = true;
				}
				//minimo diagonal
				if (k != i && l != j) {
					if (haux <= hmin_diag) { hmin_diag = haux; filadiag = k; columnadiag = l; }
					//diag = true;
				}
				if (hmin_diag < hmin_orto) { 
					hmintotal = hmin_diag; 
					filatot = filadiag; 
					columnatot = columnatot; 
				}
				else { hmintotal = hmin_orto; filatot = filaorto; columnatot = columnaorto; }
			}
		}
	}
	// el valor mayor de flujo en hacia donde va el agua, la ortogonal o la diagonal
	float grosor = (float)g->cellSize;
	if (hij <= hmintotal) // se trata de un hoyo, lo relleno
	{
		hmin = hmintotal; celdaSetSink(&g->celdas[i][j]); fila = filatot; columna = columnatot;
	}
	else
		if (fabs(hij - hmin_orto) / grosor > fabs(hij - hmin_diag) / (sqrt(2) * grosor)) //va ortogonalmente
		{
			hmin = hmin_orto; fila = filaorto; columna = columnaorto;
		}
		else { hmin = hmin_diag; fila = filadiag; columna = columnadiag; }

	//if (fila==i && columna==j && !gridCeldaExterior(g,i,j)) { celdaSetAltura(&(g->celdas[i][j]),hmin); return 0;} // hay un hoyo
	//if (fila==i && columna==j && gridCeldaExterior(g,i,j)) {celdaSetAltura(&(g->celdas[i][j]),hmin); return 10;} // punto de desague del grid
	if (fila == i && columna == j + 1) return 1; //hacia la derecha
	if (fila == i + 1 && columna == j + 1) return 2; //hacia la sur este
	if (fila == i + 1 && columna == j) return 4; //hacia abajo
	if (fila == i + 1 && columna == j - 1) return 8; //hacia el suroeste
	if (fila == i && columna == j - 1) return 16; //hacia la izquierda
	if (fila == i - 1 && columna == j - 1) return 32; //hacia el noroeste
	if (fila == i - 1 && columna == j) return 64; //hacia el norte
	if (fila == i - 1 && columna == j + 1) return 128; //hacia el noreste
}

void gridSetDirection(Grid* g) {
	unsigned long  hoyos = 0;
	for (unsigned i = 0; i < g->nfilas; i++) {
		for (unsigned j = 0; j < g->ncolumnas; j++) {
			unsigned short dir = gridDirCelda(g, i, j);
			celdaSetDir(&(g->celdas[i][j]), dir);
			if (celdaSink(g->celdas[i][j])) hoyos++;
		}
	}
	//std::cout << "El numero de hoyos es: " << hoyos << endl;
}

unsigned short gridCeldaDir(Grid* g, unsigned i, unsigned j) {
	Celda c = gridLeeCelda(g, i, j);
	return celdaDir(c);
}


/** establece toddas aquellas celdas que se consideran como hoyos*/
void gridSetDobleSink(Grid* g) {
	int ii, jj;
	unsigned cont = 0;
	for (unsigned i = 1; i < g->nfilas - 1; i++) {
		for (unsigned j = 1; j < g->ncolumnas - 1; j++) {
			// para cada casilla miro el vecino al que apunto para ver si también me apunta a mí; en tal caso ambos son sink
			unsigned direccion = gridCeldaDir(g, i, j);
			switch (direccion) {
				//case 0: ii=i; jj=j; celdaSetSink(&g->celdas[i][j]); break; 
				// vuelco en (i+1) (j)
			case 1: ii = i; jj = j + 1; if (gridCeldaDir(g, ii, jj) == 16) { celdaSetSink(&g->celdas[i][j]); celdaSetSink(&g->celdas[ii][jj]); cont++; } break;
				// vuelco a (i+1) (j+1)
			case 2: ii = i + 1; jj = j + 1; if (gridCeldaDir(g, ii, jj) == 32) { celdaSetSink(&g->celdas[i][j]); celdaSetSink(&g->celdas[ii][jj]); cont++; } break;
				//vuelco a (i) (j+1)
			case 4: ii = i + 1; jj = j; if (gridCeldaDir(g, ii, jj) == 64) { celdaSetSink(&g->celdas[i][j]); celdaSetSink(&g->celdas[ii][jj]); cont++; } break;
				// vuelco a sw, (i-1) (j+1)
			case 8: ii = i + 1; jj = j - 1; if (gridCeldaDir(g, ii, jj) == 128) { celdaSetSink(&g->celdas[i][j]); celdaSetSink(&g->celdas[ii][jj]); cont++; } break;
				// vuelco a la izquierda (i-1)(j)
			case 16: ii = i; jj = j - 1; if (gridCeldaDir(g, ii, jj) == 1) { celdaSetSink(&g->celdas[i][j]); celdaSetSink(&g->celdas[ii][jj]); cont++; } break;
				// vuelco al nw (i-1)(j-1)
			case 32: ii = i - 1; jj = j - 1; if (gridCeldaDir(g, ii, jj) == 2) { celdaSetSink(&g->celdas[i][j]); celdaSetSink(&g->celdas[ii][jj]); cont++; } break;
				//vuelco arriba (i)(j-1)
			case 64: ii = i - 1; jj = j; if (gridCeldaDir(g, ii, jj) == 4) { celdaSetSink(&g->celdas[i][j]); celdaSetSink(&g->celdas[ii][jj]); cont++; } break;
				// vuelco al ne (i+1)(j-1)
			case 128: ii = i - 1; jj = j + 1; if (gridCeldaDir(g, ii, jj) == 8) { celdaSetSink(&g->celdas[i][j]); celdaSetSink(&g->celdas[ii][jj]); cont++; } break;
				//case 10: ii=i; jj=j; celdaSetSink(&g->celdas[i][j]); break; //son hoyos en la periferia, por donde sale agua del grid
			}
		}
	}
	//std::cout << "El numero de sinks dobles es: " << cont << endl;
}


/** suaviaza el terreno*/
void gridSuaviza(Grid* g) {
	float h, newh;
	// solo suavizamos 
	std::cout << "entramos en suavizado" << endl;
	float* old_h = new float[g->nfilas * g->ncolumnas];
	if (!old_h) { std::cout << "Error en la asignación de memoria" << endl; exit(1); }

	// guardo el tiempo t-1
	for (unsigned i = 1; i < g->nfilas - 1; i++)
		for (unsigned j = 1; j < g->ncolumnas - 1; j++)
			old_h[i * g->ncolumnas + j] = 0.25 * celdaAltura(g->celdas[i][j])
			+ 0.125 * (celdaAltura(g->celdas[i + 1][j]) + celdaAltura(g->celdas[i - 1][j]) + celdaAltura(g->celdas[i][j - 1]) + celdaAltura(g->celdas[i][j + 1]))
			+ 0.0625 * (celdaAltura(g->celdas[i - 1][j - 1]) + celdaAltura(g->celdas[i - 1][i + 1]) + celdaAltura(g->celdas[i + 1][j - 1]) + celdaAltura(g->celdas[i + 1][j + 1]));

	for (unsigned i = 1; i < g->nfilas - 1; i++)
		for (unsigned j = 1; j < g->ncolumnas - 1; j++)
			celdaSetAltura(&g->celdas[i][j], old_h[i * g->ncolumnas + j]);

}


void gridRedDrenaje(Grid* g, int MAX_ITERACIONES, std::string nombreFichero) {

	unsigned direccion;

	// calcular la dirección del flujo y rellenar huecos
	if (!g->segundaVuelta) {
		gridSetDirection(g);
		gridSetDobleSink(g);
	}
	//gridSuaviza(g);

	float* recibido_old = new float[g->nfilas * g->ncolumnas];
	if (!recibido_old) { std::cout << "Error en la asignación de memoria 1" << endl; exit(1); }
	float* recibido_new = new float[g->nfilas * g->ncolumnas];
	if (!recibido_new) { std::cout << "Error en la asignación de memoria 2" << endl; exit(1); }

	unsigned nf = g->nfilas;
	unsigned nc = g->ncolumnas;

	std::vector<int>::iterator it;
	int porcentaje;

	// valor de lluvia
	float rain = 0;
	// valores minimo y maximo del intervalo en el que se van a convertir los valores de lluvia
	const float vmin = 0.8;
	const float vmax = 1.2;

	// inicializamos cada elemento de la matriz a 1, la matriz servirá para contabilizar en cada celda el numero de casillas de las cuales recibe agua en un ciclo
	for (unsigned i = 0; i < g->nfilas; i++)
		for (unsigned j = 0; j < g->ncolumnas; j++) {

			//busco en el mapa el valor del porcentaje
			std::map<int, float>::const_iterator pos = g->mapPorcAbsorcion.find(g->celdas[i][j].uso_suelo);
			porcentaje = pos->second;

			// si se ha introducido el mapa de lluvia, se convierte el valor de lluvia en el intervalo dado por vmin y vmax
			// si no, se pone por defecto a 1.0
			rain = g->rainmap == 0 ? 1.0 : setValueIntoRange(g, g->rainmap[i][j], vmin, vmax);

			// recibido_old[i * nc + j] = calculoCaudalSegunUso( porcentaje, 1.0 );
			recibido_old[i * nc + j] = calculoCaudalSegunUso(porcentaje, rain);
			recibido_new[i * nc + j] = 0.0;
			celdaSetCaudal(&g->celdas[i][j], 0.0);
		}
	// volcar agua en cada iteración
	bool seguir = true;
	int ii, jj;
	unsigned iter = 0;
	unsigned cont = 0;
	unsigned old_cont = 1; // cualquier valor distinto de 0
	unsigned bucle = 0; // solo depuracion

	// auxMaxCumulo para conocer el maximo cumulo de agua en una celda
	float auxMaxCumulo = 0.0;
	while (seguir && iter != MAX_ITERACIONES) {
		old_cont = cont;
		cont = 0;
		bucle++;
		iter++;
		seguir = true; //durante esta iteración alguien debe ponerla a true, sino el proceso finaliza
		
		std::cout << "\n\tIt: " << iter;

		/*if ((iter % 10) == 0) {
			std::std::cout << ".";
		}*/

		for (int i = 0; i < g->nfilas; i++) {
			for (int j = 0; j < g->ncolumnas; j++) {

				Celda c_anterior = gridLeeCelda(g, i, j);
				direccion = celdaDir(c_anterior);
				switch (direccion) {
					// son hoyos, por el momento no muevo su agua
					//case 0: ii=i; jj=j; break; 
					// vuelco (i+1) (j)
				case 1: ii = i; jj = j + 1; break;
					// vuelco a (i+1) (j+1)
				case 2: ii = i + 1; jj = j + 1; break;
					//vuelco a (i) (j+1)
				case 4: ii = i + 1; jj = j; break;
					// vuelco a sw, (i-1) (j+1)
				case 8: ii = i + 1; jj = j - 1; break;
					// vuelco a la izquierda (i-1)(j)
				case 16: ii = i; jj = j - 1; break;
					// vuelco al nw (i-1)(j-1)
				case 32: ii = i - 1; jj = j - 1; break;
					//vuelco arriba (i)(j-1)
				case 64: ii = i - 1; jj = j; break;
					// vuelco al ne (i+1)(j-1)
				case 128: ii = i - 1; jj = j + 1; break;
					//case 10: ii=i; jj=j;  break; //son hoyos en la periferia, por donde sale agua del grid
				}

				// si mi vecino está dentro del rango válido entonces le cedo el agua que recibo e incremento su caudal
				if (recibido_old[i * nc + j] != 0) {
					if (0 <= ii && ii < g->nfilas && 0 <= j && j < g->ncolumnas) {
						try {
							celdaIncrCaudalUso(g, &g->celdas[ii][jj], &g->celdas[i][j], recibido_old[i * nc + j], ii, jj);
						}
						catch (int e) {
							std::cout << "error: INCREMENTAR USO" << e;
						}						
						
						if (recibido_old[i * nc + j] != 0 && !celdaSink(g->celdas[i][j])) cont++;
						if (!(i == ii && jj == j)) { 
							(recibido_new[ii * nc + jj]) += recibido_old[i * nc + j]; 
						}
						recibido_old[i * nc + j] = 0;
					}
				}
			} //for
		}// for
		//despues de una iteracion tengo que reinicialiar valores entre los caudales recibidos
		for (int i = 0; i < g->nfilas; i++) {
			for (int j = 0; j < g->ncolumnas; j++) {
				float cumuloCelda = 0.0;
				recibido_old[i * nc + j] = recibido_new[i * nc + j];

				cumuloCelda = celdaLeeCaudal(g->celdas[i][j]);
				if (cumuloCelda > auxMaxCumulo) {
					auxMaxCumulo = cumuloCelda;
				}
				recibido_new[i * nc + j] = 0;
			}
		}

		if (cont == old_cont) seguir = false;
	}//while

	g->v_maxCumulo = auxMaxCumulo;
	std::cout << " 100% --> "<< (long)auxMaxCumulo << " acumulado maximo |" << iter << " iteraciones" << endl;


	if (g->mapJunctions.size() > 0 && g->segundaVuelta) {
		iterarEstructura(g->mapJunctions, g, nombreFichero);
	}
	else {
		g->segundaVuelta = true;
		std::cout << "Fichero preparado." << endl;
	}
	

	delete[] recibido_old;
	delete[] recibido_new;
}

void iterarEstructura(std::map<std::pair<float, float>, std::vector<std::map<int, float>>> mapJunctions, Grid *g, std::string nombreFichero) {
	//std::map<int, float>
	//aux es el numero de trios maximo
	int nTrios_max = 0;
	for (auto iter = mapJunctions.begin(); iter != mapJunctions.end(); ++iter) {
		for (int i = 0; i < iter->second.size(); i++) {
			if (iter->second[i].size() > nTrios_max) {
				nTrios_max = iter->second[i].size();
			}
		}
	}

	ofstream archivo;
	archivo.open(nombreFichero.c_str(), fstream::out);

	archivo << std::fixed << setprecision(dbl::max_digits10);

	archivo << "xUTM,yUTM,Media,Numero de cambios,Maximo puntual,Numero de usos del suelo";
	//for (int  i = 0; i < nTrios_max; i++) {
		archivo << ",ID(uso_suelo),Cumulo total arrastrado, Porcentaje(%)";
	//}
	archivo << "\n";

	float aux = 0;
	float xUTM, yUTM;
	Celda celda;
	float media, maximo;
	int num_cambios, num_trios;
	float cumulo_total_arrastrado; //cumulo total que ha arrastrado hasta llegar al junction
	float cumulo_total_celda; //cumulo total que ha pasado por la celda


	if (g->generateIPFiles) {
		string directory_name(g->filename);
		//fs::current_path(fs::temp_directory_path());
		fs::create_directory("OUTPUT_FILE/" + directory_name) ? std::cout << "\nDirectorio creado - OUTPUT_FILE/" << directory_name << endl :
			std::cout << "create_directory() failed" << endl;
	}

	int nIP = 0;
	for (auto iter = mapJunctions.begin(); iter != mapJunctions.end(); ++iter) {
		xUTM = (iter->first.second * g->cellSize) + g->cornerX;
		yUTM = (g->nfilas - iter->first.first) * g->cellSize + g->cornerY;
		
		ofstream IP;
		if (g->generateIPFiles) {
			string file_IP = "OUTPUT_FILE/" + g->filename + "/RP_" + std::to_string(nIP) + ".csv";
			IP.open(file_IP, fstream::out);

			IP << "xUTM: " << xUTM << ",yUTM: " << yUTM << "\n";
		}

		archivo << xUTM << "," << yUTM;
		cumulo_total_arrastrado = 0;
		cumulo_total_celda = 0;
		num_trios = 0;
		for (int i = 0; i < iter->second.size(); i++) {
			for (auto iter2 = iter->second[i].begin(); iter2 != iter->second[i].end(); ++iter2) {

				if (g->generateIPFiles) { IP << iter2->first << "," << iter2->second << ","; }
				
				if (iter2->first == -1) {
					cumulo_total_celda += roundf(iter2->second * 100) / 100;
				}
				else {
					//ULTIMO cambio que se ha realizado
					if (i == iter->second.size() - 1) {
						cumulo_total_arrastrado += iter2->second;
					}
				}
					
			}
			if (g->generateIPFiles) { IP << "\n"; }
		}
		if (g->generateIPFiles) { nIP++; IP.close();}

		celda = gridLeeCelda(g, iter->first.first, iter->first.second);
		maximo = celda.max_caudal;
		num_cambios = iter->second.size();
		num_trios = celda.map_codSuelo.size();

		// para evitar una division por 0
		if (num_cambios == 0) media = 0;
		else media = cumulo_total_celda / num_cambios;

		archivo << "," << media << "," << num_cambios << "," << maximo << "," << num_trios;
		auto iterCodSuelo = celda.map_codSuelo.begin();

		string soilId = "";
		string cumulo = "";
		string percent = "";

		while (iterCodSuelo != celda.map_codSuelo.end()) {
			double p = 0.0;
			// para evitar una division por 0
			if (cumulo_total_arrastrado == 0) p = 0;
			else p = (100 * iterCodSuelo->second) / cumulo_total_arrastrado;

			//archivo << "," << iterCodSuelo->first << "," << iterCodSuelo->second << "," << (100* iterCodSuelo->second)/cumulo_total_arrastrado;
			
			soilId += to_string(iterCodSuelo->first) + ":";
			cumulo += to_string(iterCodSuelo->second) + ":";
			percent += to_string(p) + ":";

			//std::cout << soilId << ", " << cumulo << ", " << percent << endl;
			
			++iterCodSuelo;
		}

		if (soilId == "") soilId = "0"; else soilId = soilId.substr(0, soilId.size() - 1);
		if (cumulo == "") cumulo = "0"; else cumulo = cumulo.substr(0, cumulo.size() - 1);
		if (percent == "") percent = "0"; else percent = percent.substr(0, percent.size() - 1);
		
		archivo << "," << soilId << "," << cumulo << "," << percent;
		archivo << "\n";
	}
	std::cout << "archivo terminado" << endl;
	archivo.close();
	std::cout << "\n[" << nombreFichero << "] Fichero de salida de junctions creado correctamente." << endl;
	
}

/** Incrementa caudal al uso del suelo correspondiente, tiene en cuenta la celda a la que se le incrementa
	y toma el uso del suelo de la celda anterior para llevar un cumulo dependiendo de la proveniencia*/
void celdaIncrCaudalUso(Grid* g, Celda* c_actual, Celda* c_anterior, float caudal, int ii, int jj) {
	
	/** Calculo del porcentaje de absorción - controlar la pendiente con la formula del arcotangente */
	float caudalSegunUso = caudal;
	float difAltura = c_anterior->h - c_actual->h; //la celda anterior siempre va a tener mas altura que la actual
	std::vector<int>::iterator it;
	int porcentaje;
	std::map<int, float>::const_iterator pos = g->mapPorcAbsorcion.find(c_actual->uso_suelo);
	porcentaje = pos->second;
	caudalSegunUso = calculoCaudal(difAltura, caudal, porcentaje, g->cellSize);

	c_actual->cumulo += caudalSegunUso;
	if (c_actual->cumulo > c_actual->max_caudal) {
		c_actual->max_caudal = c_actual->cumulo;
	}

	// Se añade el nuevo cumulo de la celda anterior
	std::map<int, float>::iterator actual = c_actual->map_codSuelo.find(c_anterior->uso_suelo);
	if (actual != c_actual->map_codSuelo.end()) {
		actual->second += caudalSegunUso;
	}
	else {
		c_actual->map_codSuelo.insert({ c_anterior->uso_suelo ,caudalSegunUso });
	}

	// Se pasa el cumulo restante de la celda anterior a la actual
	auto iter_anterior = c_anterior->map_codSuelo.begin();
	while (iter_anterior != c_anterior->map_codSuelo.end()) {
		
		// si existe
		if (c_actual->map_codSuelo.find(iter_anterior->first) != c_actual->map_codSuelo.end()) {
			c_actual->map_codSuelo.find(iter_anterior->first)->second += iter_anterior->second;
			iter_anterior->second = 0;
		}
		else {
			c_actual->map_codSuelo.insert({ iter_anterior->first, iter_anterior->second });
		}
		++iter_anterior;

	}//while

	/** Mapa de JUNCTIONS */
	if (g->mapJunctions.count(std::pair<float, float>(ii, jj))) {
		auto it = g->mapJunctions.find(std::pair<float, float>(ii, jj));
		std::map<int, float> m = c_actual->map_codSuelo;
		m.insert({-1, c_actual->cumulo});
		it->second.push_back(m);
	}

	c_anterior->cumulo = 0;	
}

/** Calculo del porcentaje final de absorción teniendo en cuenta la pendiente entre dos celdas */
float calculoCaudal(float difAltura, float caudal, float porc_usoX, int tamanioCelda) {

	float angulo = atan(difAltura / tamanioCelda) * 180/M_PI; // 57.29578 para pasar de radianes a grados

	const int v_max = 90; // angulo maximo, no puede llegar a 90 pero el limite tiende a 90.

	float normalizado = ((v_max - angulo) * porc_usoX) / v_max;
	float porcentajeFinal = caudal* (1 - (normalizado / 100));

	return porcentajeFinal;
}