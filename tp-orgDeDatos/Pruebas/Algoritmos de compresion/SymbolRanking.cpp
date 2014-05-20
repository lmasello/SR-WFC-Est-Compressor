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
	char charToRank;
	unsigned short numero; //Sera el numero de no ocurrencias hasta que se encuentre el simbolo.
	tuple<bool,unsigned short> tupla;

	for(int i = 0; i < orden; i++){
		if (i > 1){
			hashear(aComprimir[i-1], aComprimir[i-2]);
		}
		salida[i] = aComprimir[i];
	}

	for (unsigned long i = orden; i< size; i++){
		charToRank = aComprimir[i];
		exclusionList.erase(); //Deberia ser lo suficientemente eficiente, si paso algo malo, referirse a la pagina 6 del 132.
		nroNoOcurrencias = 0;
		while(ctxActual > 1){
			tupla = buscarEnContexto(ctxActual, charToRank, i, aComprimir);      // Es lo proximo a implementar. Hay que buscar una implementacion de tupla
			numero += tupla[1];										 			 // e implementar buscarEnContexto.
			if (tupla[0]) break;
			ctxActual--;
		}
		if (ctxActual == 1){
			tupla = buscarEnContextoUno(actual, i);
			numero += tupla[1];
			if (!tupla[0]) numero += (unsigned short) charToRank;	  // Caso de contexto = 0. Se le agrega al numero actual, el valor del char.
		}
		salida[i] = numero;
	}
}


tuple<bool,unsigned short> SymbolRanking::buscarEnContexto(int orden, char caracter, unsigned long pos, char* buffer){
	unsigned long indexFirstChar = pos-orden;
	unsigned long indexSecondChar = pos-orden+1;
	unsigned short noOcurrencias = 0;
	tuple<bool, unsigned short> tupla;

	char lastSymbols[] = {buffer[indexFirstChar], buffer[indexSecondChar]};

	list<unsigned long> listOfPositions = hashear(lastSymbols, indexFirstChar); //hashValue es la posicion en el map de la clave guardada.

	for(list<unsigned long>::iterator iterator = ++listOfPositions.begin();
		iterator != listOfPositions.end(); ++iterator){
		bool hayMatch = contextosIguales(*iterator,indexFirstChar,buffer);
		if (hayMatch){
			if(charNoExcluido(*iterator+orden)){
				bool esElBuscado = charsIguales(*iterator + orden, caracter, buffer);
				if (elCandidatoEsElBuscado){
					listOfPositions.push_front(indexFirstChar);
					return tupla(true,noOcurrencias);
				}
				exclusionList.push_front(*iterator+orden);
				noOcurrencias++;
			}
		}
	}
	return tupla(false,noOcurrencias);
}

// IMPLEMENTAR.
list<unsigned long> SymbolRanking::hashear(char* symbols, unsigned long index){
	return NULL;
}

bool SymbolRanking::contextosIguales(unsigned long indexA, unsigned long indexB, char* buffer){
	for (unsigned short i; i<orden; i++){
		if (buffer[indexA+i]!=buffer[indexB+i])return false;
	}
	return true;
}

bool SymbolRanking::charsIguales(unsigned long index,char charToCompare,char* buffer){
	if(buffer[index]==charToCompare)return true;
	return false;
}

// Nota, copiado de cplusplus
bool SymbolRanking::charNoExcluido(unsigned long pos){
	auto it = find(exclusionList.being(), exclusionList.end(), pos);
	if (it == exclusionList.end()) return true;
	return false;
}

//IMPLEMENTAR
void SymbolRanking::buscarEnContextoUno(char caracter, unsigned long pos, char* buffer){

}
