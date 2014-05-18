#include "SymbolRanking.h"

SymbolRanking::SymbolRanking(int ctxorden){
	orden = ctxorden;
}

void SymbolRanking::comprimir(char* aComprimir, short* salida, unsigned long size){
	/* Busca el contexto con un orden maximo. Si devuelve (true,x) almacenamos x en el
	* vector y avanzamos. Si devuelve (false,x) se busca contexto de orden n-1.
	* En caso de no ocurrencias, se itera hasta llegar a contexto 0, donde se almacena
	* en el vector el numero ascii correspondiente al literal, mas el numero de no ocurrencias */

	int ctxActual = orden;
	char actual;
	short numero; //Sera el numero de no ocurrencias hasta que se encuentre el simbolo.
	for (unsigned long i = 0; i< size; i++){
		/* actual = aComprimir[i];
		while(ctxActual >1){
			tuple = buscarEnContexto(ctxActual, actual, i);      // Es lo proximo a implementar. Hay que buscar una implementacion de tupla
			numero += tuple[1];								  // e implementar buscarEnContexto.
			if (tuple[0]) break;
			ctxActual--;
		}
		if (ctxActual == 1){
			tuple = buscarEnContextoUno(actual, i);
			numero += tuple[1];
			if (!tuple[0]) numero += buscarIndiceEnTabla	  // Caso de contexto = 0. Se le agrega al numero actual, el valor del char.
		}
		salida[i] = numero;*/
	}
}

/* Devuelve una tupla de la forma que indica el metodo comprimir */
void buscarEnContexto(int orden, char caracter, unsigned long pos){

}

void buscarEnContextoUno(char caracter, unsigned long pos){

}
