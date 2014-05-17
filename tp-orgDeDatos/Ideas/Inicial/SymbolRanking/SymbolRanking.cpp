#include "SymbolRanking.h"

SymbolRanking::SymbolRanking(){

}

SymbolRanking::SymbolRanking(unsigned char orden){

}

/*Recibe un vector dinamico donde se va a ir almacenando los resultados del SR y el vector a comprimir

* Busca el contexto con un orden maximo. Si devuelve (true,x) almacenamos x en el
* vector y avanzamos. Si devuelve (false,x) se busca contexto de orden n-1.
* En caso de no ocurrencias, se itera hasta llegar a contexto 0, donde se almacena
* en el vector el numero ascii correspondiente al literal, mas el numero de no ocurrencias

*/
void SymbolRanking::comprimir(char aComprimir[], int salida[]){

}

/* Dado un orden de contexto y un char busca en el vector y devuelve
* true or false segun si lo encontro o no y el numero de no ocurrencias.
* EJ: mejor caso (true,0) peor caso (false, contextosQueSeComparan)
*/
void buscarEnContexto(unsigned int orden, char caracter){

}
