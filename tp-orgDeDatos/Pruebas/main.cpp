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

		double salida = compresor.comprimir(buffer,size);

		cout << endl << "Resultado final: " << std::setprecision(51) << salida << endl;
	}
	else if(*operacionARealizar=='d'){
//		FileManager<unsigned short> fileManager;
//		unsigned short* buffer = fileManager.processFile(nombreDelArchivo);
//		unsigned int size = fileManager.getSize(nombreDelArchivo);

		cout << "El archivo ha sido abierto y colocado en memoria correctamente" << endl;

		unsigned short buffer[24] = {97,98,99,100,101,0,0,4,0,0,4,0,112,1,0,2,0,0,0,1,0,0,0,0};
		unsigned int size = 24;
		compresor.descomprimir(buffer,size);
//		free(buffer);
	}

	// Report result
	printf("\n%s in %1.2f sec.\n",
		 argv[2], double(clock()-start)/CLOCKS_PER_SEC);
	/*
	 * Luego de la instancia de compresion/descompresion se debe llamar al file
	 * Manager para que guarde los resultados en el archivo de salida
	 */

	return 0;
}



