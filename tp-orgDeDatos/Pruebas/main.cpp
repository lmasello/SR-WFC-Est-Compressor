#include <iostream>
#include <string>
#include "file-manager/FileManager.h"

using namespace std;

//const long megaByte = 1024*1024;
const unsigned char orden = 3;

int main(int argc,char *argv[]){

	//Variables de argv--> comprime/descomprime, archivo entrada, archivo salida
	//Ver opcion de que el usuario indique el orden del contexto maximo.

	char* operacionARealizar = argv[1];
	char* nombreDelArchivo = argv[2];
	FILE* fileOut;
	char* buffer;

	FileManager fileManager;
	fileManager.processFile(nombreDelArchivo, buffer);
	fileOut = fileManager.createFileOut(nombreDelArchivo);

	/*
	*Se instancia la clase Compresor, pasandole como parametro el buffer
	*
	*Se instancia a compresor.comprimir() o compresor.descomprimir() de
	* acuerdo al parametro pasado por el usuario
	*
	*Luego de la instancia de compresion/descompresion se llama al file
	*Manager para que guarde los resultados en el archivo de salida */

	return 0;
}


