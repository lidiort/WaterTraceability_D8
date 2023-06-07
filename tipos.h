#ifndef _TIPOS_H
#define _TIPOS_H

#define MAX_HUECO 15
#define ALTURA float

#include <vector>
#include <map>
#define MAX_USOS_SUELO 6



/** Definición de vertice */
typedef struct {
	/**coordenadas*/
	unsigned long x, y;
	/**altura del terreno*/
	float h;
	/**un triángulo vecino*/
	int t;
	/** exterior*/
	bool frontera;
	/**error local*/
	float error;
} Vertice;

/** Definición de eje */
typedef struct {
	/** los índices del vector donde están vértices que forman el eje*/
	unsigned v1, v2;
	/** los índices del vector donde están los triańgulos vecinos al eje*/
	int t1, t2;
	bool borrado;
} Eje;

/** Definición de triángulo */
typedef struct {
	/** los tres ejes que lo forman*/
	unsigned e1, e2, e3;
	bool borrado;
} Triangulo;


/**Definición de celda */
typedef struct {
	int i, j; //posiciones en la matriz

	float xUTM, yUTM;// posiciones XUTM,YUTM

	/** altura del punto en el centro de la celda*/
	float h;

	/** lo que la celda acumula finalmente de agua */

	float cumulo;
	/** valor que sirve para el proceso de cálculo de la red de drenaje; es el caudal recibido en esta iteración*/

	/** dirección: 1E 2SE 4S 8SW 16W 32NW 64N 128NE*/
	unsigned short direccion;

	/** determina si es un hoyo o hace */
	bool sink;

	/** uso del suelo */
	//std::string uso_suelo;
	int uso_suelo;

	/** porcentaje de absorción */
	float p_absorcion;

	/** la codificación del suelo es un vector de tamaño el numero de usos del suelo */
	//std::vector<float> codificacion_suelo;

	/** mapa de codificacion del uso del suelo , clave es el id del terreno y valor la cantidad de liquido que tiene dicha celda*/
	std::map<int, float> map_codSuelo;

	/** true si es junction */
	bool isJunction;

	/** maximo valor alcanzado en la celda */
	float max_caudal;


} Celda;



// Funciones de vértice
/**crea un vértice elemental */
Vertice verticeNuevo(void);
/** crear un vértice completo */
Vertice verticeNuevo(long int xx, long int yy, float hh, int tt);
/** crear un vértice completo  sin triangulo asociado*/
Vertice verticeNuevo(long int xx, long int yy, float hh);
/** modifica todos los valores de un vértice existente*/
void verticeSet(Vertice* v, long int xx, long int yy, float hh);
/** devuelve x */
double verticeX(Vertice v);
/** devuelve y */
double verticeY(Vertice v);
/** devuelve la altura del punto */
double verticeH(Vertice v);
/** devuelve un triangulo vecino */
double verticeT(Vertice v);
/** devuelve el valor del error*/
float verticeError(Vertice v);
/** modifica el triángulo colindante */
void verticeModifT(Vertice* v, int tt);
/** Modifica el valor del error */
void verticeModifError(Vertice* v, float error);
/** copia un vertice v2 en v1*/
void verticeCopy(Vertice* v1, Vertice* v2);
/** eliminado lógico del vertice */
//un vértice se considera borrado si t igual a -1
void verticeBorra(Vertice* v);
/** true si el vertice está eliminado */
bool verticeEliminado(Vertice v);
/** hace el vértice exterior */
void verticeExteriorizar(Vertice* v);
/** true si el vertice está en la periferia */
bool verticeExterior(Vertice v);


//Funciones de eje
/** crea un eje elemental */
Eje ejeNuevo(void);
/** crea un eje completo */
Eje ejeNuevo(unsigned ver1, unsigned ver2, int tri1, int tri2);
/** modifica los valores del eje*/
void ejeSet(Eje* e, unsigned ver1, unsigned ver2, int tri1, int tri2);
/** devuelve el vertice v1*/
unsigned ejeV1(Eje e);
/** devuelve el vertice v2*/
unsigned ejeV2(Eje e);
/** devuelve el triangulo t1*/
int ejeT1(Eje e);
/**devuelve el triangulo t2*/
int ejeT2(Eje e);
/** modifica el triangulo t1*/
void ejeModifT1(Eje* e, int t);
/** modifica el triangulo t2*/
void ejeModifT2(Eje* e, int t);
/** dice si contiene el vertice v*/
bool ejeContiene(Eje e, unsigned v);
/** dice si el eje es un eje de la periferia*/
bool ejeExtremo(Eje e);
/** elimina el eje de forma lógica*/
void ejeElimina(Eje* e);
/** dice si el eje está eliminado */
bool ejeEliminado(Eje e);

//Funciones de triangulo
/** crea un triangulo elemental*/
Triangulo trianguloNuevo(void);
/** crea un triangulo completo */
Triangulo trianguloNuevo(unsigned eje1, unsigned eje2, unsigned eje3);
/** modifica todos los valores de un triangulo existente */
void trianguloSet(Triangulo* t, unsigned eje1, unsigned eje2, unsigned eje3);
/** modifica el eje e1*/
void trianguloModifE1(Triangulo* t, int e);
/** modifica el eje e2*/
void trianguloModifE2(Triangulo* t, int e);
/** modifica el eje e3*/
void trianguloModifE3(Triangulo* t, int e);
/** devuelve el eje e1 */
unsigned trianguloE1(Triangulo t);
/** devuelve el eje e2 */
unsigned trianguloE2(Triangulo t);
/** devuelve el eje e3 */
unsigned trianguloE3(Triangulo t);
/** elimina el triangulo de modo lógico*/
void trianguloElimina(Triangulo* t);
/** lo deja activo después de haber estado borrado */
void trianguloActivar(Triangulo* t);
/** dice si el triángulo está eliminado */
bool trianguloEliminado(Triangulo t);
/** imprime el triángulo en pantalla */
void trianguloImprimir(Triangulo t);

//Funciones de celda
/** Crea una nueva celda elemental*/
Celda celdaNuevo(void);
/** Crea una celda a partir de una altura*/
Celda celdaNuevo(float hh, bool junction, float i, float j);
/** Actualiza la dirección de la celda; el valor cero significa no establecido o exterior*/
void celdaSetDir(Celda* c, unsigned dir);
/** Actualiza el uso del suelo al que corresponde la celda*/
void celdaSetUsoSuelo(Celda* c, std::string uso);
/** Determina si la dirección de la celda está o no establecida*/
bool celdaDirValida(Celda c);
/** Crea una celda a partir de una altura y un cúmulo de agua*/
Celda celdaNuevo(float hh, float cumul);
/** Aumenta el caudal de agua recibida */
void celdaIncrCaudal(Celda* c, float caudal);
/** establece un caudal */
void celdaSetCaudal(Celda* c, float caudal);
/** devuelve el caudal de una celda*/
float celdaLeeCaudal(Celda c);
/** devuelve la altura de la celda */
float celdaAltura(Celda c);
/** Actualiza la altura */
void celdaSetAltura(Celda* c, float h);
/** devuelve el caudal total de la celda*/
float celdaCaudal(Celda* c);
/** devuelve la dirección de la celda*/
unsigned celdaDir(Celda c);
/** devuelve el recién recibido */
//float celdaRecibido (Celda *c);
/** imprime en pantalla una celda*/
void celdaImprimir(Celda c);
/** imprime solo altura celda */
void celdaImprimirAltura(Celda c);
/** dice si la celda c es un hoyo*/
bool celdaSink(Celda c);
/** establece una celda como hoyo*/
void celdaSetSink(Celda* c);
/** dado un porcentaje devuelve el nuevo caudal*/
float calculoCaudalSegunUso(float porcentaje, float caudal);

#endif
