#include "Compresor.h"

void Compresor::comprimir(char* buffer, unsigned long size){
/* Va a llamar a SymbolRanking comprimir. El resultado de esto se lo pasa
 * a Mtf.comprimir finalmente el resultado de esto se lo pasa a
 * Estructurado.comprimir y modifica/devuelve el archivo comprimido.*/

	SymbolRanking sr (ORDEN);

	short *salida = (short*) malloc (sizeof(buffer) * 2);

	sr.comprimir(buffer, salida, size);

}

void Compresor::descomprimir(){

//Deberia ser una sucesion de pasos inversa a la de comprimir

}
