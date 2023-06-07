#include <iostream>
#include <fstream>
#include <cmath>
#include "mates.h"
#include "tin.h"

using namespace std;


/** Operaciones de Tin*/

Tin* TinNuevo(unsigned nfilas, unsigned ncolumnas) {
    Tin* t = new Tin();
    if (!t) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
    t->nvertices = t->nv = nfilas * ncolumnas;
    t->vertices = new Vertice[t->nvertices];
    if (!t->vertices) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
    t->ejes = new Eje[3 * t->nvertices];
    if (!t->ejes) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
    t->triangulos = new Triangulo[3 * t->nvertices];
    if (!t->triangulos) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
    t->nejes = t->ne = 0;
    t->ntriangulos = t->nt = 0;
    return t;
}

Tin* TinNuevo(char* namefile, unsigned tamaVertices) {

    Tin* t = new Tin();
    if (!t) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
    t->vertices = new Vertice[tamaVertices];
    if (!t->vertices) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
    t->ejes = new Eje[3 * tamaVertices];
    if (!t->ejes) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
    t->triangulos = new Triangulo[3 * tamaVertices];
    if (!t->triangulos) { std::cout << "Error al inicializar memoria" << endl; exit(1); }
    t->nejes = 0;
    t->ntriangulos = 0;
    t->nvertices = 0;
    float x, y, h;
    long int xx, yy, ayy;
    char c;
    unsigned cont = 0;
    bool flag = true; //hacer algo diferente la primera vez

    ifstream f(namefile, ifstream::in);
    if (!f) {
        std::cout << "Error en la apertura del fichero";
        exit(1);
    }

    while (!f.eof()) {
        f >> x >> c >> y >> c >> h;
        xx = (long int)x;
        yy = (long int)y;
        Vertice v = verticeNuevo(xx, yy, h);
        t->vertices[t->nvertices] = v; t->nvertices++;
        //std::cout << setw(8)  << xx << setw(12)  <<  yy << setw(8)  << h << endl;

        //la primera vez no existe el anterior
        if (flag) flag = false;
        else {
            // averiguramos filas y columnas
            if (ayy == yy) cont++;
            else { t->nfilas++; t->ncolumnas = cont; cont = 0; }
        }
        ayy = yy;
    }
    t->nfilas++; t->ncolumnas++;
    std::cout << "nfilas=" << t->nfilas << " ncolumnas=" << t->ncolumnas << endl;
    std::cout << "numero vertices:" << t->nvertices << endl;

    //indices de los triangulos vecinos a cada uno de estos ejes
    int tvecino_e1, tvecino_e2, tvecino_e4, tvecino_e5;
    for (unsigned i = 0; i < t->nfilas - 1; i++) {
        //std::cout << "i="<<i<<"  "<<endl;
        for (unsigned j = 0; j < t->ncolumnas - 1; j++) {
            /*añadimos los dos triangulos del cuadrado (i,j)-(i+1,j+1)
             * el vértice i tiene sus tres ejes en las posiciones 3i,3i+1,3i+2
             */
             // si el vertice es exterior, exteriorizamos
            if (i == 0 || j == 0)
                verticeExteriorizar(&(t->vertices[i * t->ncolumnas + j]));
            // definimos los triangulos incompletos
            Triangulo t1 = trianguloNuevo(); t->triangulos[t->ntriangulos] = t1; t->ntriangulos++;
            Triangulo t2 = trianguloNuevo(); t->triangulos[t->ntriangulos] = t2; t->ntriangulos++;
            unsigned ntri = t->ntriangulos;
            int it1 = ntri - 2;
            int it2 = ntri - 1;
            Eje eje1, eje2, eje3, eje4, eje5;
            int ieje1, ieje2, ieje3, ieje4, ieje5; //los índices
            // actualizo el triángulo vecino del vértice
            verticeModifT(&t->vertices[i * t->ncolumnas + j], it1);
            //--------------ejes del triangulo t1
            //definimos eje1 horizontal: e1
            //el triángulo de arriba está en la fila anterior; hay 2 tiangulos por cada vértice; +1 porque es el t2
            if (i == 0) { //si está en la fila de arriba, creo el eje, sino ya existe
                tvecino_e1 = -1;
                ejeSet(&eje1, i * t->ncolumnas + j, i * t->ncolumnas + (j + 1), tvecino_e1, it1);
                t->ejes[t->nejes] = eje1; t->nejes++; ieje1 = t->nejes - 1;
            }
            else { //ya existe el eje
                tvecino_e1 = 2 * ((i - 1) * (t->ncolumnas - 1) + j) + 1;
                ieje1 = trianguloE2(t->triangulos[tvecino_e1]);
            }
            //definimos eje2 vertical: e2
            //el triangulo de la izquierda es el vecino
            if (j == 0) { //si está en la fila de arriba, creo el eje, sino ya existe
                tvecino_e2 = -1;
                ejeSet(&eje2, i * t->ncolumnas + j, (i + 1) * t->ncolumnas + j, tvecino_e2, it1);
                t->ejes[t->nejes] = eje2; t->nejes++; ieje2 = t->nejes - 1;
            }
            else { //ya existe el eje
                tvecino_e2 = 2 * ((i) * (t->ncolumnas - 1) + j) - 1;
                ieje2 = trianguloE3(t->triangulos[tvecino_e2]);
            }
            //definimos eje3 oblicuo: e3
            //el triangulo t2 es el vecino
            ejeSet(&eje3, (i + 1) * t->ncolumnas + j, (i)*t->ncolumnas + (j + 1), it1, it2);
            t->ejes[t->nejes] = eje3; t->nejes++; ieje3 = t->nejes - 1;
            //modificamos t1
            trianguloModifE1(&t->triangulos[it1], ieje1);  //posicion de e1
            trianguloModifE2(&t->triangulos[it1], ieje2);  //posicion de e2
            trianguloModifE3(&t->triangulos[it1], ieje3);  //posicion de e3

            //---------------ejes del triangulo t2
            //definimos eje4=e2 horizontal
            //el triángulo de abajo no está aún, pero lo estará en el futuro si no es el último
            if (i == t->nfilas - 2) { //si está en la fila de arriba, creo el eje, sino ya existe
                tvecino_e4 = -1;
                verticeModifT(&t->vertices[(i + 1) * t->ncolumnas + j], it1);
            }
            else tvecino_e4 = 2 * ((i + 1) * (t->ncolumnas - 1) + j);
            ejeSet(&eje4, (i + 1) * t->ncolumnas + j, (i + 1) * t->ncolumnas + (j + 1), it2, tvecino_e4);
            t->ejes[t->nejes] = eje4; t->nejes++; ieje4 = t->nejes - 1;
            //definimos eje5=e3 vertical
            //el triángulo de la derecha no está aún, pero lo estará en el futuro si no es el último
            if (j == t->ncolumnas - 2) { //si está en la fila de arriba, creo el eje, sino ya existe
                tvecino_e5 = -1;
                verticeModifT(&t->vertices[(i)*t->ncolumnas + (j + 1)], it1);
            }
            else tvecino_e5 = 2 * ((i) * (t->ncolumnas - 1) + (j + 1));
            ejeSet(&eje5, (i)*t->ncolumnas + (j + 1), (i + 1) * t->ncolumnas + (j + 1), it2, tvecino_e5);
            t->ejes[t->nejes] = eje5; t->nejes++; ieje5 = t->nejes - 1;
            //tocar el último vértice de todos
            if (i == t->nfilas - 2 && j == t->ncolumnas - 2) verticeModifT(&t->vertices[(i + 1) * t->ncolumnas + (j + 1)], it2);

            //modificamos t2
            trianguloModifE1(&t->triangulos[it2], ieje3); //posicion de e1
            trianguloModifE2(&t->triangulos[it2], ieje4); //posicion de e2
            trianguloModifE3(&t->triangulos[it2], ieje5); //posicion de e3

        }
    }
    //exteriorizar los vertices de los extremos no hechos antes

    for (unsigned i = 0; i < t->nfilas; i++) verticeExteriorizar(&t->vertices[i * t->ncolumnas + t->ncolumnas - 1]);
    for (unsigned j = 0; j < t->ncolumnas; j++) verticeExteriorizar(&t->vertices[(t->nfilas - 1) * t->ncolumnas + j]);
    t->nv = t->nvertices;
    t->ne = t->nejes;
    t->nt = t->ntriangulos;
    return t;
}


float minimoError(Vertice* vertices, unsigned* vvertices, unsigned nvv) {
    float min = 9e9;
    for (unsigned i = 0; i < nvv; i++) {
        float dato = verticeError(vertices[vvertices[i]]);
        if (dato != -1 && dato < min)
            min = dato;
    }
    return min;
}


void imprimir(Tin* t) {
    std::cout << "vertices:" << endl;
    for (unsigned i = 0; i < t->nv; i++) {
        if (!verticeEliminado(t->vertices[i]))
            std::cout << verticeX(t->vertices[i]) << "  " << verticeY(t->vertices[i]) << "  " << verticeH(t->vertices[i]) << " " << verticeT(t->vertices[i]) << endl;
    }
    std::cout << "triangulos: " << endl;
    for (unsigned i = 0; i < t->nt; i++) {
        if (!trianguloEliminado(t->triangulos[i]))
            std::cout << trianguloE1(t->triangulos[i]) << "  " << trianguloE2(t->triangulos[i]) << "  " << trianguloE3(t->triangulos[i]) << endl;
    }
    std::cout << "ejes: " << endl;
    for (unsigned i = 0; i < t->ne; i++)
        if (!ejeEliminado(t->ejes[i])) {
            std::cout << ejeV1(t->ejes[i]) << "  " << ejeV2(t->ejes[i]) << "  ";
            std::cout << ejeT1(t->ejes[i]) << "  " << ejeT2(t->ejes[i]) << endl;
        }

}

void imprimir(Tin* t, char* namefile) {

    ofstream f(namefile, ofstream::out);
    if (!f) { std::cout << "Error en la apertura del fichero" << endl; exit(1); }

    f << "vertices:" << endl;
    for (unsigned i = 0; i < t->nvertices; i++) {
        if (!verticeEliminado(t->vertices[i]))
            f << verticeX(t->vertices[i]) << "  " << verticeY(t->vertices[i]) << "  " << verticeH(t->vertices[i]) << " " << verticeT(t->vertices[i]) << endl;
        else f << "---" << endl;
    }
    f << "triangulos: " << endl;
    for (unsigned i = 0; i < t->ntriangulos; i++) {
        if (!trianguloEliminado(t->triangulos[i]))
            f << trianguloE1(t->triangulos[i]) << "  " << trianguloE2(t->triangulos[i]) << "  " << trianguloE3(t->triangulos[i]) << endl;
    }
    f << "ejes: " << endl;
    for (unsigned i = 0; i < t->nejes; i++)
        if (!ejeEliminado(t->ejes[i])) {
            f << ejeV1(t->ejes[i]) << "  " << ejeV2(t->ejes[i]) << "  ";
            f << ejeT1(t->ejes[i]) << "  " << ejeT2(t->ejes[i]) << endl;
        }

}

void vecinos(Tin* t, int it, int iv, int* tvecino1, unsigned* iv1, int* tvecino2, unsigned* iv2) {
    unsigned ie1 = trianguloE1(t->triangulos[it]);
    unsigned ie2 = trianguloE2(t->triangulos[it]);
    unsigned ie3 = trianguloE3(t->triangulos[it]);
    //std::cout << "ejes 1, 2 y 3" << endl << ie1 << endl << ie2 << endl << ie3 << endl;
    bool est1 = false;
    bool est2 = false;
    if (ejeContiene(t->ejes[ie1], iv)) { *tvecino1 = (ejeT1(t->ejes[ie1]) != it ? ejeT1(t->ejes[ie1]) : ejeT2(t->ejes[ie1])); *iv1 = (ejeV1(t->ejes[ie1]) == iv ? ejeV2(t->ejes[ie1]) : ejeV1(t->ejes[ie1]));   est1 = true; }
    if (ejeContiene(t->ejes[ie2], iv)) { *tvecino2 = (ejeT1(t->ejes[ie2]) != it ? ejeT1(t->ejes[ie2]) : ejeT2(t->ejes[ie2])); *iv2 = (ejeV1(t->ejes[ie2]) == iv ? ejeV2(t->ejes[ie2]) : ejeV1(t->ejes[ie2])); est2 = true; }
    if (!est1) { *tvecino1 = (ejeT1(t->ejes[ie3]) != it ? ejeT1(t->ejes[ie3]) : ejeT2(t->ejes[ie3])); *iv1 = ejeV1(t->ejes[ie3]) == iv ? ejeV2(t->ejes[ie3]) : ejeV1(t->ejes[ie3]); }
    if (!est2) { *tvecino2 = (ejeT1(t->ejes[ie3]) != it ? ejeT1(t->ejes[ie3]) : ejeT2(t->ejes[ie3])); *iv2 = ejeV1(t->ejes[ie3]) == iv ? ejeV2(t->ejes[ie3]) : ejeV1(t->ejes[ie3]); }
}

void vecinos(Tin* t, int it, int iv, int* tvecino1, unsigned* iv1, unsigned* e1, int* tvecino2, unsigned* iv2, unsigned* e2, unsigned* eOpuesto, bool* estri1) {
    unsigned ie1 = trianguloE1(t->triangulos[it]);
    unsigned ie2 = trianguloE2(t->triangulos[it]);
    unsigned ie3 = trianguloE3(t->triangulos[it]);
    //std::cout << "ejes 1, 2 y 3" << endl << ie1 << endl << ie2 << endl << ie3 << endl;
    bool est1 = false;
    bool est2 = false;
    bool est3 = false;
    bool est4 = false;
    if (ejeContiene(t->ejes[ie1], iv)) {
        *tvecino1 = (ejeT1(t->ejes[ie1]) != it ? ejeT1(t->ejes[ie1]) : ejeT2(t->ejes[ie1])); *iv1 = (ejeV1(t->ejes[ie1]) == iv ? ejeV2(t->ejes[ie1]) : ejeV1(t->ejes[ie1])); *e1 = ie1;  est1 = true;
    }
    if (ejeContiene(t->ejes[ie2], iv)) {
        *tvecino2 = (ejeT1(t->ejes[ie2]) != it ? ejeT1(t->ejes[ie2]) : ejeT2(t->ejes[ie2])); *iv2 = (ejeV1(t->ejes[ie2]) == iv ? ejeV2(t->ejes[ie2]) : ejeV1(t->ejes[ie2])); *e2 = ie2; est2 = true;
    }
    if (!est1) {
        *tvecino1 = (ejeT1(t->ejes[ie3]) != it ? ejeT1(t->ejes[ie3]) : ejeT2(t->ejes[ie3])); *iv1 = (ejeV1(t->ejes[ie3]) == iv ? ejeV2(t->ejes[ie3]) : ejeV1(t->ejes[ie3])); *e1 = ie3; est3 = true;
    }
    if (!est2) {
        *tvecino2 = (ejeT1(t->ejes[ie3]) != it ? ejeT1(t->ejes[ie3]) : ejeT2(t->ejes[ie3])); *iv2 = (ejeV1(t->ejes[ie3]) == iv ? ejeV2(t->ejes[ie3]) : ejeV1(t->ejes[ie3])); *e2 = ie3; est4 = true;
    }
    if (est1) {
        if (est2) *eOpuesto = ie3;
        else *eOpuesto = ie2;
    }
    else { *eOpuesto = ie1; }
    if (ejeT1(t->ejes[*eOpuesto]) == it) *estri1 = true;
    else *estri1 = false;
}


float datosVertice(Tin* t, unsigned iv, float* errorActual, unsigned* vvertices, unsigned* nv, int* vtriangulos, unsigned* nt, unsigned* vejes, unsigned* ne, unsigned* veopus, unsigned* no, bool* vest1) {

    //partir del triángulo vecino de iv e ir avanzando hasta dar la vuelta o encontrame con -1
    Vertice v = t->vertices[iv];
    float hv = verticeH(v);//altura de v
    int tv1, tv2, it1, it2, it = (int)verticeT(v);
    Triangulo tr = t->triangulos[it];
    unsigned iv1, iv2, ivv2, ie1, ie2, iee2, ieopu;
    unsigned nvv, nvt, nve, nvo;
    float error;
    bool est1;
    nvv = nvt = nve = nvo = 0;

    int itaux = it;

    vtriangulos[nvt] = it; nvt++; //el primer triangulo
    vecinos(t, it, iv, &tv1, &iv1, &ie1, &tv2, &ivv2, &iee2, &ieopu, &est1); //std::cout << "it= "<<it <<" "<<tv1<<" "<<tv2<<endl; 
    vtriangulos[nvt] = tv1; nvt++;
    vvertices[nvv] = iv1; nvv++;
    vejes[nve] = ie1; nve++;
    veopus[nvo] = ieopu; vest1[nvo] = est1; nvo++;
    //sigo el camino de tv1, si acaba en -1 tengo que seguir el de tv2
    int iteraciones = 0;
    while (it != tv1 && tv1 != -1) {
        vecinos(t, tv1, iv, &it1, &iv1, &ie1, &it2, &iv2, &ie2, &ieopu, &est1);
        //std::cout << "tv1= "<<tv1 <<" "<<it1<<" "<<it2<<endl;
        if (itaux != it1) { itaux = tv1; tv1 = it1; vvertices[nvv] = iv1; nvv++; vejes[nve] = ie1; nve++; }
        else { itaux = tv1; tv1 = it2; vvertices[nvv] = iv2; nvv++; vejes[nve] = ie2; nve++; }
        if (tv1 != -1) vtriangulos[nvt] = tv1; nvt++;
        vest1[nvo] = est1; veopus[nvo++] = ieopu;
        iteraciones++;
    }
    //	std::cout << iteraciones;
    itaux = it;
    if (tv1 == -1) {//no ha dado la vuelta y hay que seguir buscando por el otro lado; se trata de un vértice exterior
        verticeExteriorizar(&t->vertices[iv]);
        vvertices[nvv++] = ivv2;
        vtriangulos[nvt++] = tv2;
        vejes[nve++] = iee2;
        while (tv2 != -1) {
            vecinos(t, tv2, iv, &it1, &iv1, &ie1, &it2, &iv2, &ie2, &ieopu, &est1);
            if (itaux != it1) { itaux = tv2; tv2 = it1; vvertices[nvv++] = iv1; vejes[nve++] = ie1; }
            else { itaux = tv2; tv2 = it2; vvertices[nvv++] = iv2; vejes[nve++] = ie2; }
            if (tv2 != -1) vtriangulos[nvt++] = tv2;
            vest1[nvo] = est1; veopus[nvo++] = ieopu;
        }
    }
    //calculo el error asociado al vertice sin eliminarlo
    float errmax = fabs(hv - verticeH(t->vertices[vvertices[1]]));
    for (unsigned i = 1; i < nvv; i++) {
        error = fabs(hv - verticeH(t->vertices[vvertices[i]]));
        if (error > errmax) errmax = error;
    }
    *errorActual = errmax;
    // ahora tengo que calcular el error asociado a todos los vértices vecinos
    float hmin = verticeH(t->vertices[vvertices[0]]);
    float hmax = verticeH(t->vertices[vvertices[1]]);
    float aux;
    if (hmin > hmax) { aux = hmin; hmin = hmax; hmax = aux; }
    for (unsigned i = 2; i < nvv; i++) {
        int vecino = vvertices[i];
        if (verticeH(t->vertices[vecino]) < hmin) hmin = verticeH(t->vertices[vecino]);
        if (verticeH(t->vertices[vecino]) > hmax) hmax = verticeH(t->vertices[vecino]);
    }
    //std::cout << vejes.size();
    *nv = nvv; *ne = nve; *nt = nvt; *no = nvo;
    return (hmax - hmin);
}


unsigned verticeOpuesto(Tin* t, unsigned ie1, unsigned ie2) {

    if (ejeV1(t->ejes[ie1]) == ejeV1(t->ejes[ie2])) return ejeV2(t->ejes[ie2]);
    else  if (ejeV1(t->ejes[ie1]) == ejeV2(t->ejes[ie2])) return ejeV1(t->ejes[ie2]);
    else if (ejeV2(t->ejes[ie1]) == ejeV1(t->ejes[ie2])) return ejeV2(t->ejes[ie2]);
    return ejeV1(t->ejes[ie2]);
}

unsigned verticeCentral(Tin* t, unsigned ie1, unsigned ie2) {

    if (ejeV1(t->ejes[ie1]) == ejeV1(t->ejes[ie2])) return ejeV1(t->ejes[ie2]);
    else  if (ejeV1(t->ejes[ie1]) == ejeV2(t->ejes[ie2])) return ejeV2(t->ejes[ie2]);
    else if (ejeV2(t->ejes[ie1]) == ejeV1(t->ejes[ie2])) return ejeV1(t->ejes[ie2]);
    return ejeV2(t->ejes[ie2]);
}


bool convexo(Tin* t, unsigned* vvertices, unsigned tama) {
    bool vconvex, iguales;
    bool prime = true;
    for (unsigned i = 0; i < tama; i++) {
        unsigned imenos1 = (i - 1 + tama) % tama;
        unsigned imas1 = (i + 1) % tama;
        vconvex = Izquierda(verticeX(t->vertices[vvertices[imas1]]), verticeY(t->vertices[vvertices[imas1]]),
            verticeX(t->vertices[vvertices[imenos1]]), verticeY(t->vertices[vvertices[imenos1]]),
            verticeX(t->vertices[vvertices[i]]), verticeY(t->vertices[vvertices[i]]));
        if (prime) { iguales = vconvex; prime = false; }
        else if (iguales xor vconvex) return false;
    }
    return true;
}


bool eliminarVertice(Tin* t, unsigned iv, unsigned* vvertices, unsigned nv, int* vtriangulos, unsigned nt, unsigned* vejes, unsigned ne, unsigned* veopus, unsigned no, bool* vest1) {

    if (verticeExterior(t->vertices[iv]) || verticeEliminado(t->vertices[iv])) return false;

    //fase de eliminación
    verticeBorra(&t->vertices[iv]); t->nvertices--;
    //std::cout << "vtriangulos.size()" << vtriangulos.size() << endl;
    for (unsigned i = 0; i < nt - 1; i++) { //el -1 porque repite el último
        trianguloElimina(&t->triangulos[vtriangulos[i]]); //std::cout <<vtriangulos[i] << endl;
        t->ntriangulos--;
    }

    //std::cout << "visualizamos vejes" << endl;
    for (unsigned i = 0; i < ne; i++) {
        ejeElimina(&t->ejes[vejes[i]]); //std::cout << vejes[i] << endl;
        t->nejes--;
    }

    unsigned iopus; // indice del vector de ejes opuestos
    unsigned iv1, iv2, ivx; //indice de los vertices que forman nuevos ejes

    //fase de inserción y actualización    
    int ntri = 0, nver = 0; //número de vértices y triangulos que se irán insertando
    //conseguimos los ejes vecinos y los triángulos
    unsigned ieje1 = veopus[0];
    unsigned ieje2 = veopus[1];
    iopus = 1;

    if (vest1[0]) ejeModifT1(&t->ejes[ieje1], vtriangulos[0]);
    else ejeModifT2(&t->ejes[ieje1], vtriangulos[0]);
    /*
        int t1e1=ejeT1(t->ejes[ieje1]);
         int t2e1=ejeT2(t->ejes[ieje1]);
        if (t1e1 == -1){
                if (trianguloEliminado(t->triangulos[t2e1])) ejeModifT2(&t->ejes[ieje1], vtriangulos[0]);
                else std::cout << "No puede ser #1";
         } else if (t2e1 == -1) {
                if (trianguloEliminado(t->triangulos[t1e1])) ejeModifT1(&t->ejes[ieje1], vtriangulos[0]);
                else std::cout << "No puede ser #2";
                } else { // no están en la periferia
                    if (trianguloEliminado(t->triangulos[t2e1])) ejeModifT2(&t->ejes[ieje1], vtriangulos[0]);
                    else if (trianguloEliminado(t->triangulos[t1e1])) ejeModifT1(&t->ejes[ieje1], vtriangulos[0]);
                            else std::cout << "No puede ser #3" ;
                }
    */
    if (vest1[1]) ejeModifT1(&t->ejes[ieje2], vtriangulos[0]);
    else ejeModifT2(&t->ejes[ieje2], vtriangulos[0]);
    /*
        int t1e2=ejeT1(t->ejes[ieje2]);
         int t2e2=ejeT2(t->ejes[ieje2]);
        if (t1e2 == -1){
                if (trianguloEliminado(t->triangulos[t2e2])) ejeModifT2(&t->ejes[ieje2], vtriangulos[0]);
                else std::cout << "No puede ser #4";
         } else if (t2e2 == -1) {
                if (trianguloEliminado(t->triangulos[t1e2])) ejeModifT1(&t->ejes[ieje2], vtriangulos[0]);
                else std::cout << "No puede ser #5";
                } else { // no están en la periferia
                    if (trianguloEliminado(t->triangulos[t2e2])) ejeModifT2(&t->ejes[ieje2], vtriangulos[0]);
                    else if (trianguloEliminado(t->triangulos[t1e2])) ejeModifT1(&t->ejes[ieje2], vtriangulos[0]);
                            else std::cout << "No puede ser #6";
                }
    */
    //creamos el nuevo eje y el nuevo triángulo
    iv1 = verticeOpuesto(t, ieje2, ieje1);
    iv2 = verticeOpuesto(t, ieje1, ieje2);
    ivx = verticeCentral(t, ieje1, ieje2);
    Eje enuev = ejeNuevo(iv1, iv2, vtriangulos[0], vtriangulos[1]); //vtriangulos[1] no está pero estará en el futuro
    Triangulo tnuev = trianguloNuevo(ieje1, ieje2, vejes[0]);
    //actualizo el triángulo vecino
    verticeModifT(&t->vertices[iv1], vtriangulos[0]);
    verticeModifT(&t->vertices[iv2], vtriangulos[0]);
    verticeModifT(&t->vertices[ivx], vtriangulos[0]);//std::cout << "actualizados " << iv1 << "-" << iv2 <<"  "<< ivx<< endl;

    //cambian los papeles para seguir el proceso
    ieje1 = vejes[0];
    //insertamos el nuevo triángulo y el nuevo eje
    t->triangulos[vtriangulos[0]] = tnuev; t->ntriangulos++;
    t->ejes[vejes[0]] = enuev; t->nejes++;

    //construimos los tríángulos del centro
    for (unsigned i = 2; i < ne - 2; i++) {
        //incrementamos los índices de los ejes y triangulos a insertar
        ntri++;
        nver++;
        iopus++;
        //eje vecino es veopus[iopus]
        ieje2 = veopus[iopus];

        if (vest1[iopus]) ejeModifT1(&t->ejes[ieje2], vtriangulos[ntri]);
        else ejeModifT2(&t->ejes[ieje2], vtriangulos[ntri]);

        /*
                    t1e2=ejeT1(t->ejes[ieje2]);
                    t2e2=ejeT2(t->ejes[ieje2]);
                    if (t1e2 == -1){
                            if (trianguloEliminado(t->triangulos[t2e2])) ejeModifT2(&t->ejes[ieje2],vtriangulos[ntri]);
                            else std::cout << "No puede ser #7";
                    } else if (t2e2 == -1) {
                            if (trianguloEliminado(t->triangulos[t1e2])) ejeModifT1(&t->ejes[ieje2],vtriangulos[ntri]);
                            else std::cout << "No puede ser #8";
                            } else { // no están en la periferia
                                if (trianguloEliminado(t->triangulos[t2e2])) ejeModifT2(&t->ejes[ieje2], vtriangulos[ntri]);
                                else if (trianguloEliminado(t->triangulos[t1e2])) ejeModifT1(&t->ejes[ieje2], vtriangulos[ntri]);
                                        else std::cout << "No puede ser #9";
                            }
        */
        //creamos el nuevo eje
        iv2 = verticeOpuesto(t, ieje1, ieje2);
        verticeModifT(&t->vertices[iv2], vtriangulos[ntri]); //std::cout << "actualizados " << iv2  << endl;
        Eje enuev = ejeNuevo(iv1, iv2, vtriangulos[ntri], vtriangulos[ntri + 1]); //el siguiente estará en esta posición ntri+1
        Triangulo tnuev = trianguloNuevo(ieje1, ieje2, vejes[nver]);
        ieje1 = vejes[nver];
        //el último triángulo creado está incompleto, es vecino del triángulo actual
        //insertamos el nuevo triángulo y el nuevo eje
        t->triangulos[vtriangulos[ntri]] = tnuev; t->ntriangulos++;
        t->ejes[vejes[nver]] = enuev;	t->nejes++;
    }
    //construimos el último triángulo, el eje no es necesario
    ntri++;
    nver++;
    iopus++;
    ieje2 = veopus[iopus];
    //esta vez tenemos eje3
    unsigned ieje3 = veopus[iopus + 1]; if (iopus + 1 >= no) std::cout << "Esto es un gravísimo error" << endl;
    //actualizamos los ejes vecinos, en este caso hay dos
    if (vest1[iopus]) ejeModifT1(&t->ejes[ieje2], vtriangulos[ntri]);
    else ejeModifT2(&t->ejes[ieje2], vtriangulos[ntri]);
    /*
        t1e2=ejeT1(t->ejes[ieje2]);
         t2e2=ejeT2(t->ejes[ieje2]);
        if (t1e2 == -1){
                if (trianguloEliminado(t->triangulos[t2e2])) ejeModifT2(&t->ejes[ieje2], vtriangulos[ntri]);
                else std::cout << "No puede ser #10";
         } else if (t2e2 == -1) {
                if (trianguloEliminado(t->triangulos[t1e2])) ejeModifT1(&t->ejes[ieje2], vtriangulos[ntri]);
                else std::cout << "No puede ser #11";
                } else { // no están en la periferia
                    if (trianguloEliminado(t->triangulos[t2e2])) ejeModifT2(&t->ejes[ieje2],vtriangulos[ntri]);
                    else if (trianguloEliminado(t->triangulos[t1e2])) ejeModifT1(&t->ejes[ieje2],vtriangulos[ntri]);
                            else std::cout << "No puede ser #12";
                }
    */
    if (vest1[iopus + 1]) ejeModifT1(&t->ejes[ieje3], vtriangulos[ntri]);
    else ejeModifT2(&t->ejes[ieje3], vtriangulos[ntri]);
    /*
        int t1e3=ejeT1(t->ejes[ieje3]);
         int t2e3=ejeT2(t->ejes[ieje3]);
        if (t1e3 == -1){
                if (trianguloEliminado(t->triangulos[t2e3])) ejeModifT2(&t->ejes[ieje3],vtriangulos[ntri]);
                else std::cout << "No puede ser #13";
         } else if (t2e3 == -1) {
                if (trianguloEliminado(t->triangulos[t1e3])) ejeModifT1(&t->ejes[ieje3],vtriangulos[ntri]);
                else std::cout << "No puede ser #14";
                } else { // no están en la periferia
                    if (trianguloEliminado(t->triangulos[t2e3])) ejeModifT2(&t->ejes[ieje3],vtriangulos[ntri]);
                    else if (trianguloEliminado(t->triangulos[t1e3])) ejeModifT1(&t->ejes[ieje3],vtriangulos[ntri]);
                            else std::cout << "No puede ser #15";
                }
    */

    //este vertice también hay que actualizarlo
    ivx = verticeCentral(t, ieje2, ieje3); verticeModifT(&t->vertices[ivx], vtriangulos[ntri]); //std::cout << "actualizados" <<ivx << endl;
    //insertamos el nuevo triángulo y el nuevo eje
    tnuev = trianguloNuevo(ieje1, ieje2, ieje3);
    t->triangulos[vtriangulos[ntri]] = tnuev; t->ntriangulos++;
    return true;
}



bool vecindarioIntegro(Tin* t, unsigned* vvertices, unsigned nvv, unsigned* vejes, unsigned nve) {
    //partir del triángulo vecino de iv e ir avanzando hasta dar la vuelta o encontrame con -1
    bool est1;

    for (unsigned i = 0; i < nvv; i++) {
        unsigned iv = vvertices[i];
        Vertice v = t->vertices[iv];
        int tv1, tv2, it1, it2, itaux;
        int it = (int)verticeT(v);
        //if (it==-1) 
        //std::cout << "paro" << endl;
        unsigned iv1, iv2, ivv2, ie1, ie2, iee2, ieopu;
        itaux = it;

        vecinos(t, it, iv, &tv1, &iv1, &ie1, &tv2, &ivv2, &iee2, &ieopu, &est1); //std::cout << "it= "<<it <<" "<<tv1<<" "<<tv2<<endl;
        //sigo el camino de tv1, si acaba en -1 tengo que seguir el de tv2
        int iteraciones = 0;
        while (it != tv1 && tv1 != -1 && iteraciones < 20) {
            vecinos(t, tv1, iv, &it1, &iv1, &ie1, &it2, &iv2, &ie2, &ieopu, &est1);
            //std::cout << "tv1= "<<tv1 <<" "<<it1<<" "<<it2<<endl;
            if (itaux != it1) { itaux = tv1; tv1 = it1; }
            else { itaux = tv1; tv1 = it2; }
            iteraciones++;
        }
        //for (int j=0; j<vvertices.size(); j++) {if (i!=j && vvertices[i]==vvertices[j]) std::cout<<"v";}
        //for (int j=0; j<vejes.size(); j++) {if (i!=j && vejes[i]==vejes[j]) std::cout<<"e";}
     //if (iteraciones <4) std::cout <<"menos que 4";
        if (iteraciones >= 20) return false;
    }

    return true;

}

bool procesarVertice(Tin* t, unsigned iv, float errorGlobal) {
    unsigned* vvertices, * vejes, * veopus;
    int* vtriangulos;
    bool* vest1;

    //limpiamos por si traen información
    vvertices = new unsigned[MAX_HUECO];
    vtriangulos = new int[MAX_HUECO];
    vejes = new unsigned[MAX_HUECO];
    veopus = new unsigned[MAX_HUECO];
    vest1 = new bool[MAX_HUECO];
    unsigned nvv, nvt, nve, nvo;

    float errorLocal;
    bool eliminado = false;
    //si el vértice no es exterior, está por debajo del umbral permitido y es mayor que el error actual, lo elimino

    if (!verticeExterior(t->vertices[iv])) {
        float error = datosVertice(t, iv, &errorLocal, vvertices, &nvv, vtriangulos, &nvt, vejes, &nve, veopus, &nvo, vest1);
        //	float minError = minimoError (t->vertices, vvertices, nvv);
        //if (errorLocal <= minError){
        bool integridad = convexo(t, vvertices, nvv);
        if (integridad && error<errorGlobal && error>errorLocal) {
            if (eliminarVertice(t, iv, vvertices, nvv, vtriangulos, nvt, vejes, nve, veopus, nvo, vest1)) eliminado = true;
            std::cout << "X";
        }
        if (!integridad) std::cout << "O"; //<<iv; 
        else if (!eliminado) std::cout << ".";
        //} 
    }
    else std::cout << "-";
    delete[] vvertices;
    delete[] vtriangulos;
    delete[] vejes;
    delete[] veopus;
    delete[] vest1;
    return eliminado;
}

void leeVerticesTriangulo(Tin* t, Triangulo tt, unsigned* v1, unsigned* v2, unsigned* v3) {
    unsigned e1, e2, e3;
    e1 = trianguloE1(tt);
    e2 = trianguloE2(tt);
    e3 = trianguloE3(tt);
    *v1 = ejeV1(t->ejes[e1]);
    *v2 = ejeV2(t->ejes[e1]);
    if (ejeV1(t->ejes[e2]) != *v1 && ejeV1(t->ejes[e2]) != *v2) *v3 = ejeV1(t->ejes[e2]);
    else *v3 = ejeV2(t->ejes[e2]);
}

void salvarPLY(Tin* t, char* namefile) {

    ofstream f(namefile, ofstream::out);
    if (!f) {
        std::cout << "Error en la apertura del fichero" << endl;
        exit(1);
    }

    if (t->ntriangulos == 0) std::cout << "error: malla no creada" << endl;


    f << "ply" << endl;
    f << "format ascii 1.0" << endl;
    f << "comment created by lidia" << endl;
    f << "element vertex " << t->nv << endl; //ponemos todos los vertices y sólo las caras válidas
    f << "property float32 x" << endl << "property float32 y" << endl << "property float32 z" << endl;
    f << "element face " << t->nt << endl;
    f << "property list uint8 int32 vertex_indices" << endl;
    f << "end_header" << endl;

    for (unsigned i = 0; i < t->nv; i++) {
        f << verticeX(t->vertices[i]) << " " << verticeY(t->vertices[i]) << " " << verticeH(t->vertices[i]) << " " << endl;
    }

    unsigned t1, t2, t3;
    for (unsigned i = 0; i < t->nt; i++) {
        if (!trianguloEliminado(t->triangulos[i])) {
            leeVerticesTriangulo(t, t->triangulos[i], &t1, &t2, &t3);
            f << 3 << " " << t1 << " " << t2 << " " << t3 << " " << endl;
        }
    }
    f.close();

}

void simplificaMayaCPU(Tin* t, float error) {
    for (unsigned i = 0; i < t->nv; i++) {
        //		std::cout << i << " ";
        procesarVertice(t, i, error);
        if (i % t->ncolumnas == 0)  std::cout << endl;
    }
}


float distancia2(Vertice& v1, Vertice& v2) {
    return (verticeX(v1) - verticeX(v2)) * (verticeX(v1) - verticeX(v2)) + (verticeY(v1) - verticeY(v2)) * (verticeY(v1) - verticeY(v2));
}


unsigned verticeOptimo(Tin* t, unsigned* veopus, unsigned nvo) {
    unsigned imax = 0;
    float dismax, dis, dismin = 9e9;
    for (unsigned i = 0; i < nvo; i++) {
        dismax = 0.0;
        for (unsigned j = 0; j < nvo; j++) {
            if (i == j && fabs(i - j) != 1 && fabs(i - j) != nvo - 1) { // si no es el mismo vértice o el vecino
                dis = distancia2(t->vertices[veopus[i]], t->vertices[veopus[j]]);
                if (dis > dismax) { dismax = dis; }
            }
        }
        if (dismax < dismin) { dismin = dismax; imax = i; }
    }
    return imax;
}


void checkTin(Tin* t) {
    for (unsigned i = 0; i < t->ne; i++) {
        if (!ejeEliminado(t->ejes[i])) {
            if (ejeT1(t->ejes[i]) == ejeT2(t->ejes[i]))
                std::cout << "Error en el chequeo: dos vecinos iguales para un eje" << endl;
        }
    }
}

void TinDestruye(Tin** t) {
    if ((*t)->vertices) delete[](*t)->vertices;
    if ((*t)->triangulos) delete[](*t)->triangulos;
    if ((*t)->ejes) delete[](*t)->ejes;
    if (*t) delete (*t);
}
