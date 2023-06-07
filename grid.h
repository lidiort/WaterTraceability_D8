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


#ifndef _GRID_H
#define _GRID_H

#include <string>
#include <vector>
#include <map>
#include <QtWidgets>
#include <QtGui>
#include "tipos.h"

typedef std::map<std::pair<float, float>, std::vector<std::map<int, float>>> estructura_proyecto;

 /** Definición de grid */
typedef struct {
	unsigned nfilas, ncolumnas; //tamaño del grid, dimensiones X e Y
	unsigned cornerX, cornerY, cellSize;
	Celda** celdas;
	/** los vertices superiores e inferiores del grid*/
	Vertice supder, infizq;

	unsigned v_maxCumulo; //valor maximo que ha alcanzado una celda (cumulo de agua)

	unsigned v_maxDesnivel; //valor de maximo desnivel que se encuentra en el terreno

	std::map<int,float> mapPorcAbsorcion; //mapa que contiene todos los usos del suelo con so porcentaje de absorción del suelo

	/** mapa de clave un par de valores(coordenadas x, y de un junction)
	y valor un vector que contiene un mapa de codificacion del uso del suelo de las celdas por donde ha ido pasando.*/
	estructura_proyecto mapJunctions;

	estructura_proyecto mapRumblar;

	bool generateIPFiles;
	bool segundaVuelta;

	std::string filename;

	/** Matriz que contiene el mapa de lluvia de la zona */
	float** rainmap;
	/** Valor mínimo del mapa de lluvia */
	float minRainValue;// = FLT_MAX;
	/** Valor máximo del mapa de lluvia */
	float maxRainValue;// = 0;
} Grid;

// funciones de tin
/** constructor vacío a partir de un número de filas y columnas y separación de celdas*/
//Grid* GridNuevo(unsigned nfilas, unsigned ncolumnas, unsigned anchox, unsigned anchoy);


/**crea un grid a partir de la información de un fichero .asc*/
Grid* GridNuevo(std::string namefile_DEM, std::string namefile_usos, std::string namefile_junctions, std::string namefile_parametros, std::string rainmapFile, QApplication* a);

/** imprime el contenido de un grid en fichero */
void gridImprimir(Grid* g, std::string namefile);
void gridCumuloImprimir(Grid* g, std::string namefile);
/** indica la dirección de una celda del grid*/
unsigned short gridCeldaDir(Grid* g, unsigned i, unsigned j);
/** devuelve una celda ubicada en la posicion i,j*/
Celda gridLeeCelda(Grid* g, unsigned i, unsigned j);
/** modifica la celda situada en la posicion i, j con una nueva celda*/
void gridModiCelda(Grid* g, unsigned i, unsigned j, Celda c);
/** devuelve el vértice inferior izquierda*/
Vertice gridVerticeInfIzq(Grid* g);
/** Establece la dirección asociada a cada celda*/
void gridSetDirection(Grid* g);
/** determina todos los sink dobles */
void gridSetDobleSink(Grid* g);
/** construye una red de drenaje a partir de un grid recién creado*/
void gridRedDrenaje(Grid* g, int MAX_ITERACIONES, std::string nombreFichero);
/** destruye el tin*/
void gridDestruye(Grid** g);
/** incrementa caudal al uso del suelo correspondiente, tiene en cuenta la celda a la que se le incrementa
	y toma el uso del suelo de la celda anterior para llevar un cumulo dependiendo de la proveniencia*/
void celdaIncrCaudalUso(Grid* g, Celda* c_actual, Celda* c_anterior, float caudal, int ii, int jj);
/** cálculo del porcentaje final de absorción teniendo en cuenta la pendiente entre dos celdas */
float calculoCaudal(float difAltura, float caudal, float porc_usoX, int tamanioCelda);
/** obtiene los junctions de un fichero y son almacenados en un mapa */
void leeJunctions(Grid* g, std::string namefile_junctions);

/** lee el archivo del mapa de lluvia */
void loadRainMap(Grid* g, std::string file);

/*
* Convierte el valor de lluvia a un valor de un intervalo dado por sus valores mínimo y máximo
* g: grid
* value: el numero que se va a convertir al intervalo
* vmin: el valor minimo del intervalo
* vmax: el valor máximo del intervalo
*/
float setValueIntoRange(Grid* g, float value, float vmin, float vmax);

void iterarEstructura(std::map<std::pair<float, float>, std::vector<std::map<int, float>>> mapJunctions, Grid* g, std::string nombreFichero);

#endif