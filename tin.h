
#ifndef _TIN_H
#define _TIN_H

#include "tipos.h"

/** Definición de TIN */
typedef struct {
	Vertice* vertices;
	Eje* ejes;
	Triangulo* triangulos;
	unsigned nfilas, ncolumnas;
	unsigned nvertices, nejes, ntriangulos; //número real de vértices, triángulos y ejes
	unsigned nv, nt, ne; //tamaño de los arrays 
} Tin;


// funciones de tin
/** constructor vacío a partir de un número de filas y columnas*/
Tin* TinNuevo(unsigned nfilas, unsigned ncolumnas);
/**crea un tin a partir de la información de un fichero .asc*/
Tin* TinNuevo(char* namefile, unsigned tamaVertices);
/** imprime el contenido del tin en pantalla */
void imprimir(Tin* t);
/** imprime el contenido de un tin en fichero */
void imprimir(Tin* t, char* namefile);
/** dado un vertice iv y un triangulo it, devuelve dos parejas, (tvecino1, iv1) y (tvecino2, iv2) respectivamente con el triangulo y vertice vecino*/
void vecinos(Tin* t, int it, int iv, int* tvecino1, unsigned* iv1, int* tvecino2, unsigned* iv2);
/** dado un vertice iv y un triangulo it, devuelve dos parejas, (tvecino1, iv1, e1) y (tvecino2, iv2, e2) respectivamente con el triangulo y vertice vecino*/
void vecinos(Tin* t, int it, int iv, int* tvecino1, unsigned* iv1, unsigned* e1, int* tvecino2, unsigned* iv2, unsigned* e2, unsigned* eOpuesto, bool* est1);
/** determina si el vector corresponde a un polígono convexo*/
bool convexo(Tin* t, unsigned* vvertices, unsigned tama);
/** devuelve en sendos vectores, toda la información sobre vertices, ejes, triángulos y ejes opuestos a un vértice dado */
float datosVertice(Tin* t, unsigned iv, float* errorActual, unsigned* vvertices, unsigned* nv, int* vtriangulos, unsigned* nt, unsigned* vejes, unsigned* ne, unsigned* veopus, unsigned* no, bool* vest1);
/** devuelve el vertice no común de ie1 con ie2 */
unsigned verticeOpuesto(Tin* t, unsigned ie1, unsigned ie2);
/** devuelve el vertice común a los dos ejes */
unsigned verticeCentral(Tin* t, unsigned ie1, unsigned ie2);
/** determina si el vecindario tiene errores */
bool vecindarioIntegro(Tin* t, unsigned* vvertices, unsigned nvv, unsigned* vejes, unsigned nve);
/** procesa un vértice iv y es eliminado según el error introducido*/
bool procesarVertice(Tin* t, unsigned iv, float errorGlobal);
/** devuelve los vértices asociados a un triángulo*/
void leeVerticesTriangulo(Tin* t, Triangulo tt, unsigned* v1, unsigned* v2, unsigned* v3);
/** salva el terreno en formato PLY */
void salvarPLY(Tin* t, char* namefile);
/** simplifica una malla a partir del valor de un error */
void simplificaMayaCPU(Tin* t, float error);
/** encuentra el vértice óptimo a partir del cual triangular el hueco */
unsigned verticeOptimo(Tin* t, unsigned* veopus, unsigned nvo);
/** chequea la integridad del tin*/
void checkTin(Tin* t);
/** destruye el tin*/
void TinDestruye(Tin** t);

#endif
