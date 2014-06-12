#include <iostream>
#include <iomanip>
#include "FileManager/FileManager.h"
#include "Compresor/Compresor.h"

using namespace std;

//const int megaByte = 1024*1024;
const unsigned char orden = 3;

int main(int argc,char *argv[]){

	//Variables de argv--> comprime/descomprime, archivo entrada, archivo salida
	//Ver opcion de que el usuario indique el orden del contexto maximo.

	if(argc != 4){
		cout << "Parametros: c/d FileIn FileOut" << endl;
		fputs ("Parameter error\n",stderr); exit(1);
	}

	char* operacionARealizar = argv[1];
	char* fileIn = argv[2];
	char* fileOut = argv[3];

	// Get start time
	clock_t start=clock();

	Compresor compresor;
	if (*operacionARealizar == 'c'){
		FileManager<char> fileManager;
		char* buffer = fileManager.processFile(fileIn);
		unsigned int size = fileManager.getSize(fileIn);

		cout << "El archivo ha sido abierto y colocado en memoria correctamente" << endl;

		pair<char*, unsigned int> parSalida;
		parSalida = compresor.comprimir(buffer,size);

		/* Luego de la instancia de compresion se debe llamar al fileManager
		 * para que guarde los resultados en el archivo de salida */
		fileManager.createFileOut(fileOut, parSalida.first, parSalida.second);

		cout << "El proceso de compresion a finalizado satisfactoriamente." << endl;
		free(buffer);
	}
	else if(*operacionARealizar=='d'){
		FileManager<char> fileManager;
		char* buffer = fileManager.processFile(fileIn);
		unsigned int size = fileManager.getSize(fileIn);

		cout << "El archivo ha sido abierto y colocado en memoria correctamente" << endl;

		pair<char*, unsigned int> parSalida;
		parSalida = compresor.descomprimir(buffer, size);

		cout << "El proceso de descompresion a finalizado satisfactoriamente." << endl;

		/* Luego de la instancia de descompresion se debe llamar al fileManager
		 * para que guarde los resultados en el archivo de salida */
		fileManager.createFileOut(fileOut, parSalida.first, parSalida.second);
		free(buffer);
	}

	// Report result
	printf("\n%s in %1.2f sec.\n",
		 argv[2], double(clock()-start)/CLOCKS_PER_SEC);

	return 0;
}



