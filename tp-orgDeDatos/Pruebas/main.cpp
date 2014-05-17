/*
 * main.cpp
 *
 *  Created on: 17 May 2014
 *      Author: lmasel
 */

#include <iostream>
#include <string>
#include "FileManager/FileManager.h"

using namespace std;

const long megaByte = 1024*1024;
const unsigned int orden = 3;

int main(int argc,char argv[]){

	/*Variables de argv--> comprime/descomprime, archivo entrada, archivo salida
	*Ver opcion de que el usuario indique el orden del contexto maximo.*/
	char operacionARealizar[] = argv[0];
	char nombreDelArchivo[] = argv[1];

	char buffer[megaByte*100];

	FileManager fileManager = new FIleManager();
	fileManager.processFile(nombreDelArchivo, buffer);

	/*Se carga el file en memoria (buffer), aparentemente por partes ya que hay que
	* tener en cuenta archivos muy grandes que no entran en memoria.
	*
	*Se instancia la clase Compresor, pasandole como parametro el vector
	*que devuelve el fileManager
	*
	*Se instancia a compresor.comprimir() o compresor.descomprimir() de
	* acuerdo al parametro pasado por el usuario
	*
	*Luego de la instancia de compresion/descompresion se llama al file
	*Manager para que guarde los resultados en el archivo de salida */

	return 0;
}



