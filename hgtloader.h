#ifndef HGTLOADER_H
#define HGTLOADER_H

#include <vector>

using namespace std;

class FileNotFoundException {};
class FileOutputException {};

/**
	@author Antonio Rueda <ajrueda@ujaen.es>
	Carga ficheros HGT. Cada fichero tiene 1201 por 1201 muestras de elevación del terreno en metros
		codificadas mediante enteros de 2 bytes, con el byte más significativo delante del menos significativo.
*/
class HGTLoader {
public:
	HGTLoader() {}
	void load(string file, vector<vector<int> >& elev) throw (FileNotFoundException);
	void save(string file, vector<vector<int> >& elev) throw (FileOutputException);
};

#endif
