#include <iostream>
#include "FileManager/FileManager.h"
#include "Compresor/Compresor.h"

using namespace std;

//const long megaByte = 1024*1024;
const unsigned char orden = 3;

int main(int argc,char *argv[]){

	//Variables de argv--> comprime/descomprime, archivo entrada, archivo salida
	//Ver opcion de que el usuario indique el orden del contexto maximo.

	char* operacionARealizar = argv[1];
	char* nombreDelArchivo = argv[2];

	Compresor compresor;
	if (*operacionARealizar == 'c'){
		FileManager<char> fileManager;
		char* buffer = fileManager.processFile(nombreDelArchivo);
		unsigned long size = fileManager.getSize(nombreDelArchivo);

		cout << "El archivo ha sido abierto y colocado en memoria correctamente" << endl;

		compresor.comprimir(buffer,size);
		free(buffer);
	}
	else if(*operacionARealizar=='d'){
//		FileManager<unsigned short> fileManager;
//		unsigned short* buffer = fileManager.processFile(nombreDelArchivo);
//		unsigned long size = fileManager.getSize(nombreDelArchivo);

		cout << "El archivo ha sido abierto y colocado en memoria correctamente" << endl;

		unsigned short buffer[24] = {97,98,99,100,101,0,0,4,0,0,4,0,112,1,0,2,0,0,0,1,0,0,0,0};
		unsigned long size = 24;
		compresor.descomprimir(buffer,size);
//		free(buffer);
	}

	/*
	 * Luego de la instancia de compresion/descompresion se debe llamar al file
	 * Manager para que guarde los resultados en el archivo de salida
	 */

	return 0;
}



