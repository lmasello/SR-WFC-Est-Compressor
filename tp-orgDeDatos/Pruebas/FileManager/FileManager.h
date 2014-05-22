#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstddef>
using namespace std;

class FileManager {
	public:
		FileManager();

		char* processFile(char* filename);
		/* Dado un filename, lo abre y vuelca su contenido en el buffer.
		 * Precondiciones:
		 *  -fileIn debe ser un nombre de archivo valido, ubicado en el mismo directorio del
		 *   proyecto
		 *  -buffer todavia no contemplo la cantidad de memoria necesaria
		 *  Postcondiciones:
		 *  -El metodo modifica el array buffer, agregando el total del contenido del archivo a procesar
		 * */

		unsigned long getSize(char* filename);

		FILE* createFileOut(char* filename);
		/*
		 * Dado el nombre del archivo a procesar, ya sea a comprimir o descomprimir, se crea el archivo
		 * en donde se guararan los resultados.
		 * Precondiciones:
		 *  -filename debe ser un nombre de archivo valido, ubicado en el mismo directorio del
		 *   proyecto
		 * Postcondiciones:
		 *  -devuelve un FILE* apuntando al archivo de salida creado
		 */
};


#endif /* FILEMANAGER_H_ */
