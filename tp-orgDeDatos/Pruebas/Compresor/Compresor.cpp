#include "Compresor.h"
#include "../Algoritmos de compresion/SymbolRanking.h"

Compresor::Compresor(){}

void Compresor::comprimir(char* buffer, unsigned int size){

	SymbolRanking sr (ORDEN);

	short *salida = (short*) malloc (sizeof(short) * size);

	cout<<"Comenzando proceso de compresion por SymbolRanking"<<endl;

	sr.comprimir(buffer, salida, size);

	cout<<"SymbolRanking ha finalizado el proceso de compresion correctamente" << endl;

	for(unsigned int i = 0; i < size; i++){
		cout<< salida[i] << ' ';
	}

}

void Compresor::descomprimir(unsigned short* entrada, unsigned int size){
//Deberia ser una sucesion de pasos inversa a la de comprimir
	SymbolRanking sr(ORDEN);

	char *salida = (char*) malloc (sizeof(char) * size);

	cout << "Comenzando el proceso de descompresion por SymbolRanking" << endl;

	sr.descomprimir(entrada, salida, size);

	cout << "SymbolRanking ha finalizado el proceso de descompresion correctamente"<<endl;
	cout << "Salida:"<<endl;
	for(unsigned int i = 0; i < size; i++){
		cout<< salida[i] << ' ';
	}

}
