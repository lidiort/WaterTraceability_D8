#include <iostream>
#include "tipos.h"
#include "grid.h"

using namespace std;

/** Operaciones de vertice*/
Vertice verticeNuevo(void) {
	Vertice v;
	v.x = 0;
	v.y = 0;
	v.h = 0.0;
	v.t = 0;
	v.frontera = false;
	v.error = -1.0;
	return v;
}

Vertice verticeNuevo(long int xx, long int yy, float hh, int tt) {
	Vertice v;
	v.x = xx;
	v.y = yy;
	v.h = hh;
	v.t = tt;
	v.frontera = false;
	v.error = -1.0;
	return v;
}

Vertice verticeNuevo(long int xx, long int yy, float hh) {
	Vertice v;
	v.x = xx;
	v.y = yy;
	v.h = hh;
	v.t = 0;
	v.error = -1.0;
	v.frontera = false;
	return v;
}

void verticeSet(Vertice* v, long int xx, long int yy, float hh) {
	v->x = xx; v->y = yy; v->h = hh; v->t = 0; v->frontera = false;
}

double verticeX(Vertice v) {
	return v.x;
}

double verticeY(Vertice v) {
	return v.y;
}

double verticeH(Vertice v) {
	return v.h;
}

double verticeT(Vertice v) {
	return v.t;
}

float verticeError(Vertice v) {
	return v.error;
}

void verticeModifT(Vertice* v, int tt) {
	v->t = tt;
}

void verticeBorra(Vertice* v) {
	v->t = -1;
}

void verticeModifError(Vertice* v, float error) {
	v->error = error;
}

bool verticeEliminado(Vertice v) {
	return (v.t == -1);
}

void verticeExteriorizar(Vertice* v) {
	v->frontera = true;
}

bool verticeExterior(Vertice v) {
	return v.frontera;
}

void verticeCopy(Vertice* v1, Vertice* v2) {
	v1->x = v2->x; v1->y = v2->y;
	v1->h = v2->h; v1->t = v2->t;
	v1->frontera = v2->frontera;
}


/** Operaciones de eje */

Eje ejeNuevo(void) {
	Eje e;
	e.v1 = 0; e.v2 = 0;
	e.t1 = 0; e.t2 = 0;
	e.borrado = false;
	return e;
}

Eje ejeNuevo(unsigned ver1, unsigned ver2, int tri1, int tri2) {
	Eje e;
	e.v1 = ver1; e.v2 = ver2;
	e.t1 = tri1; e.t2 = tri2;
	e.borrado = false;
	return e;
}

void ejeSet(Eje* e, unsigned ver1, unsigned ver2, int tri1, int tri2) {
	e->v1 = ver1; e->v2 = ver2;
	e->t1 = tri1; e->t2 = tri2;
	e->borrado = false;
}

unsigned ejeV1(Eje e) {
	return e.v1;
}

unsigned ejeV2(Eje e) {
	return e.v2;
}

int ejeT1(Eje e) {
	return e.t1;
}

int ejeT2(Eje e) {
	return e.t2;
}

void ejeModifT1(Eje* e, int t) {
	e->t1 = t;
}

void ejeModifT2(Eje* e, int t) {
	e->t2 = t;
}

bool ejeContiene(Eje e, unsigned v) {
	return (e.v1 == v || e.v2 == v);
}

bool ejeExtremo(Eje e) {
	return (e.t1 == -1 || e.t2 == -1);
}

void ejeElimina(Eje* e) {
	e->borrado = true;
}

bool ejeEliminado(Eje e) {
	return e.borrado;
}


/** Operaciones de triangulo */
Triangulo trianguloNuevo(void) {
	Triangulo t;
	t.e1 = 0; t.e2 = 0; t.e3 = 0;
	t.borrado = false;
	return t;
}

Triangulo trianguloNuevo(unsigned eje1, unsigned eje2, unsigned eje3) {
	Triangulo t;
	t.e1 = eje1; t.e2 = eje2; t.e3 = eje3;
	t.borrado = false;
	return t;
}

void trianguloSet(Triangulo* t, unsigned eje1, unsigned eje2, unsigned eje3) {
	t->e1 = eje1;
	t->e2 = eje2;
	t->e3 = eje3;
	t->borrado = false;
}

void trianguloModifE1(Triangulo* t, int e) {
	t->e1 = e;
}

void trianguloModifE2(Triangulo* t, int e) {
	t->e2 = e;
}

void trianguloModifE3(Triangulo* t, int e) {
	t->e3 = e;
}

unsigned trianguloE1(Triangulo t) {
	return t.e1;
}

unsigned trianguloE2(Triangulo t) {
	return t.e2;
}

unsigned trianguloE3(Triangulo t) {
	return t.e3;
}

void trianguloElimina(Triangulo* t) {
	t->borrado = true;
}

void trianguloActivar(Triangulo* t) {
	t->borrado = false;
}

bool trianguloEliminado(Triangulo t) {
	return (t.borrado == true);
}

void trianguloImprimir(Triangulo t) {
	std::cout << "e1:" << t.e1 << " e2:" << t.e2 << " e3:" << t.e3 << endl;
}

/** Operaciones de Celda */

Celda celdaNuevo(void) {
	Celda c;
	c.h = 0;
	c.cumulo = 0.0;
	//c.recibido = 0;
	c.direccion = 0;
	c.sink = false;
	c.p_absorcion = 0;
	c.uso_suelo = -1;
	//c.codificacion_suelo.resize(MAX_USOS_SUELO);
	return c;
}

Celda celdaNuevo(float hh, bool junction, float i, float j) {
	Celda c;
	c.h = hh;
	c.isJunction = junction;
	c.cumulo = 0.0;
	c.direccion = 0;
	c.sink = false;
	c.p_absorcion = 0;
	c.uso_suelo = -1;
	c.max_caudal = 0;
	c.xUTM = i;
	c.yUTM = j;
	//c.codificacion_suelo.resize(MAX_USOS_SUELO);

	return c;
}

Celda celdaNuevo(float hh, float cumul) {
	Celda c;
	c.h = hh;
	c.cumulo = cumul;
	//c.recibido = 0;
	c.direccion = 0;
	c.sink = false;
	c.p_absorcion = 0;
	c.uso_suelo = -1;
	//c.codificacion_suelo.resize(MAX_USOS_SUELO);
	return c;
}

void celdaSetDir(Celda* c, unsigned dir) {
	c->direccion = dir;
}

void celdaSetUsoSuelo(Celda *c, int uso) {
	c->uso_suelo = uso;
}

bool celdaDirValida(Celda c) {
	return (c.direccion != 0);
}

/*
void celdaIncrRecibida (Celda *c, float caudal){
	c->recibido += caudal;
}
*/

void celdaIncrCaudal(Celda* c, float caudal) {
	c->cumulo += caudal;
	if (c->cumulo > c->max_caudal) {
		c->max_caudal = c->cumulo;
	}
}

void celdaSetCaudal(Celda* c, float caudal) {
	c->cumulo = caudal;
}

float celdaAltura(Celda c) {
	return c.h;
}

void celdaSetAltura(Celda* c, float h) {
	c->h = h;
}

float celdaCaudal(Celda c) {
	return c.cumulo;
}

/*
float celdaRecibido (Celda c){
	return c.recibido;
}
*/

float celdaLeeCaudal(Celda c) {
	return c.cumulo;
}

unsigned celdaDir(Celda c) {
	return c.direccion;
}

void celdaImprimir(Celda c) {
	printf("%5.2f-%d ", c.h, c.direccion);
}

void celdaImprimirAltura(Celda c) {
	printf("%3.0f ", c.h);
}

bool celdaSink(Celda c) {
	return c.sink;
}

void celdaSetSink(Celda* c) {
	c->sink = true;
}

float calculoCaudalSegunUso(float porcentaje, float caudal) {
	float _caudal = caudal;

	_caudal = caudal * (1 - (porcentaje / 100));

	return _caudal;
}
