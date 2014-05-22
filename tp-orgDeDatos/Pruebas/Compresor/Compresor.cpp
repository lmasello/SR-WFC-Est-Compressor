#include "Compresor.h"
#include "../Algoritmos de compresion/SymbolRanking.h"

Compresor::Compresor(){}

void Compresor::comprimir(char* buffer, unsigned long size){

	SymbolRanking sr (ORDEN);

	short *salida = (short*) malloc (sizeof(buffer) * 2);

	cout<<"Comenzando proceso de compresion por SymbolRanking"<<endl;

	sr.comprimir(buffer, salida, size);

	cout<<"Symbol ranking ha finalizado el proceso de compresion correctamente";

}

void Compresor::descomprimir(){

//Deberia ser una sucesion de pasos inversa a la de comprimir

}
