#include <iostream>
#include <iomanip>
#include "FileManager/FileManager.h"
#include "Compresor/Compresor.h"
#define GRP_NUMBER ".11"

using namespace std;

const char* crearFileOutC(const char* fileIn){
	string strFileOut(fileIn);
	strFileOut += GRP_NUMBER;
	return &strFileOut[0];
}

const char* crearFileOutD(const char* fileIn){
	string strFileOut(fileIn);
	strFileOut.erase(strFileOut.end()-3, strFileOut.end());
	return &strFileOut[0];
}

void help(){
	cout << "Parametros: TpGrupo11 c/d FileIn" << endl;
	fputs ("Parameter error\n",stderr); exit(1);
}

int main(int argc,char *argv[]){

	//Variables de argv--> comprime/descomprime, archivo entrada, archivo salida
	//Ver opcion de que el usuario indique el orden del contexto maximo.

	if(argc != 3){
		help();
	}

	const char* operacionARealizar = argv[1];
	const char* fileIn = argv[2];
	const char* fileOut;

	// Get start time
	clock_t start=clock();

	FileManager<char> fileManager;
	char* buffer = fileManager.processFile(fileIn);
	unsigned int size = fileManager.getSize(fileIn);

	cout << "El archivo ha sido abierto y colocado en memoria correctamente" << endl;

	pair<char*, unsigned int> parSalida;

	Compresor *compresor = new Compresor();

	if (*operacionARealizar == 'c'){

		parSalida = compresor->comprimir(buffer,size);

		fileOut = crearFileOutC(fileIn);

		/* Luego de la instancia de compresion se debe llamar al fileManager
		 * para que guarde los resultados en el archivo de salida */
		fileManager.createFileOut(fileOut, parSalida.first, parSalida.second);

		cout << "El proceso de compresion a finalizado satisfactoriamente." << endl;
	}

	else if(*operacionARealizar=='d'){

		parSalida = compresor->descomprimir(buffer, size);

		fileOut = crearFileOutD(fileIn);

		/* Luego de la instancia de descompresion se debe llamar al fileManager
		 * para que guarde los resultados en el archivo de salida */
		fileManager.createFileOut(fileOut, parSalida.first, parSalida.second);

		cout << "El proceso de descompresion a finalizado satisfactoriamente." << endl;
	}

	else help();

	delete[] buffer;
	delete[] parSalida.first;
	delete compresor;

	// Report result
	printf("\n%s to %s in %1.2f sec.\n",
		 argv[2], fileOut, double(clock()-start)/CLOCKS_PER_SEC);

	return 0;
}
