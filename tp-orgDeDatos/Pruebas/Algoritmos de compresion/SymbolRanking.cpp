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
	short numero; //Sera el numero de no ocurrencias hasta que se encuentre el simbolo.
	for (unsigned long i = 0; i< size; i++){
		charToRank = aComprimir[i];
		numero = 0;
/*		while((ctxActual >1) && (i>=orden)){
			tuple = buscarEnContexto(ctxActual, charToRank, i, aComprimir);      // Es lo proximo a implementar. Hay que buscar una implementacion de tupla
			numero += tuple[1];										 			 // e implementar buscarEnContexto.
			if (tuple[0]) break;
			ctxActual--;
		}
		if ((ctxActual == 1) || (i<orden)){
			tuple = buscarEnContextoUno(actual, i);
			numero += tuple[1];
			if (!tuple[0]) numero += buscarIndiceEnTabla	  // Caso de contexto = 0. Se le agrega al numero actual, el valor del char.
		}
		salida[i] = numero;
*/	}

}


void SymbolRanking::buscarEnContexto(int orden, char caracter, unsigned long pos, char* buffer){
	unsigned long indexFirstChar = pos-orden;
	unsigned long indexSecondChar = pos-orden+1;
	unsigned short noOcurrencias = 0;

	char lastSymbols[] = {buffer[indexFirstChar], buffer[indexSecondChar]};
	string lastTwoSymbols = string(lastSymbols);

	size_t hashValue = hashKey(lastTwoSymbols);
/*
	// get the list of positions with the same hashValue
	hashValue* listOfPositions;
	if (listOfPositions.empty()) listOfPositions.push_front(indexFirstChar); //Guarda la posicion en la lista
	else(){
		for(list<unsigned long>::iterator iterator = listOfPositions.begin();iterator != listOfPositions.end(); ++iterator){
			bool hayMatch = this.contextosIguales(*iterator,indexFirstChar,buffer);
			if (hayMatch){
				bool elCandidatoEsElBuscado = this.charsIguales(*iterator, orden, caracter, buffer);

				// Aqui habria que consultar la lista de exclusion

				if (elCandidatoEsElBuscado){
					listOfPositions.push_front(indexFirstChar);
					return (true,noOcurrencias);
				}
				noOcurrencias++;
			}
		}
		listOfPositions.push_front(indexFirstChar);
	}
	return (false,noOcurrencias);
*/
}

bool SymbolRanking::contextosIguales(unsigned long indexA, unsigned long indexB, char* buffer){
	for (unsigned short i; i<orden; i++){
		if (buffer[indexA+i]!=buffer[indexB+i])return false;
	}
	return true;
}

bool SymbolRanking::charsIguales(unsigned long index,unsigned short offsetToChar,char charToCompare,char* buffer){
	if(buffer[index+offsetToChar]==charToCompare)return true;
	return false;
}

size_t SymbolRanking::hashKey(string stringToHash){
	hash<std::string> strHash; // Si sigue tirando error, implementamos una funcion de hashing nosotros.
	return strHash(stringToHash);
}


void SymbolRanking::buscarEnContextoUno(char caracter, unsigned long pos, char* buffer){

}
