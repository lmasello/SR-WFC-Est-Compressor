#include "Compresor.h"
#include "../SymbolRanking/SymbolRanking.h"
#define ORDEN 3
using namespace std;

void Compresor::comprimir(char* buffer){
/* Va a llamar a SymbolRanking comprimir. El resultado de esto se lo pasa
 * a Mtf.comprimir finalmente el resultado de esto se lo pasa a
 * Estructurado.comprimir y modifica/devuelve el archivo comprimido.*/

	SymbolRanking sr (ORDEN);

	short *salida;

	salida = sr.comprimir(buffer);

}

void Compresor::descomprimir(){

//Deberia ser una sucesion de pasos inversa a la de comprimir

}
