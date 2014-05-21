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
	unsigned short nroNoOcurrencias; //Sera el numero de no ocurrencias hasta que se encuentre el simbolo.
	tuple<bool,unsigned short> tupla;

	for(int i = 0; i < orden; i++){
		if (i > 1){
			hashear(aComprimir[i-1], aComprimir[i-2], i-2);	//Ojo, diferencias de parametros con el hashear de buscarEnContexto
		}
		salida[i] = aComprimir[i];
	}

	for (unsigned long i = orden; i< size; i++){	//i= index del char en el buffer
		charToRank = aComprimir[i];
		exclusionList.clear(); 						//Deberia ser lo suficientemente eficiente, si pasa algo malo, referirse a la pagina 6 del 132.
		nroNoOcurrencias = 0;
		while(ctxActual > 1){
			tupla = buscarEnContexto(ctxActual, charToRank, i, aComprimir);
			nroNoOcurrencias += get<1> (tupla);
			if (get<0> (tupla)) break;
			ctxActual--;
		}
		if (ctxActual == 1){
			tupla = buscarEnContextoUno(charToRank, i,aComprimir);
			nroNoOcurrencias += get<1> (tupla);
			if (!get<0> (tupla)) nroNoOcurrencias += (unsigned short) charToRank;	  // Caso de contexto = 0. Se le agrega al numero actual, el valor del char.
		}
		salida[i] = nroNoOcurrencias;
	}
}


tuple<bool,unsigned short> SymbolRanking::buscarEnContexto(int orden, char caracter, unsigned long pos, char* buffer){
	unsigned long indexFirstChar = pos-orden;
	unsigned long indexSecondChar = pos-orden+1;
	unsigned short nroNoOcurrencias = 0;
	tuple<bool, unsigned short> tupla;

	list<unsigned long> listOfPositions = hashear(buffer[indexFirstChar], buffer[indexSecondChar], indexFirstChar);

	for(list<unsigned long>::iterator iterator = ++listOfPositions.begin();
		iterator != listOfPositions.end(); ++iterator){							//*iterator seria un unsigned long, indicando una posicion de la lista de posiciones
		bool hayMatch = contextosIguales(*iterator,indexFirstChar,buffer);
		if (hayMatch){
			if(charNoExcluido(*iterator+orden)){
				bool esElBuscado = charsIguales(*iterator + orden, caracter, buffer);
				if (esElBuscado){
					listOfPositions.push_front(indexFirstChar); //Si no me equivoco, esto lo hace la funcion hashear!!!
					get<0> (tupla) = true;
					get<1> (tupla) = nroNoOcurrencias;
					return tupla;
				}
				exclusionList.push_front(*iterator+orden);
				nroNoOcurrencias++;
			}
		}
	}
	get<0> (tupla) = false;
	get<1> (tupla) = nroNoOcurrencias;
	return tupla;
}

//Se sigue la idea mencionada por Fenwich en el 132
tuple<bool,unsigned short> SymbolRanking::buscarEnContextoUno(char charToRank, unsigned long pos, char* buffer){
	tuple<bool, unsigned short> tupla;
/*

 	 unsigned long posicionDelContexto = pos-1;
 	 char charDelContexto = buffer[posicionDelContexto];

 	 //Ver de implementar una funcion de hash para un char y que devuelva una lista de tuplas, donde cada tupla
 	 // este formada por la posicion en donde aparece el contexto y el char que le sigue.
 	 // De esta manera se aplica la lista de exclusion en la misma lista, controlan el agregado y quitado de tuplas, haciendo
 	 // que haya una sola entrada por cada char que le sigue al contexto. Si la prediccion es positiva, se remueve el ctx antiguo y se deja el actual.
 	 // si la prediccion es negativa, queda todo como esta.
	 //Ej: Seria un map, donde la clave es el charDelContexto y tiene como valor (posicionDelContexto,charToRank)
	 list<tuple<unsigned long,char>> sameContextPositions = hashear (charToRank, charDelContexto, posicionDelContexto);

     unsigned short noOcurrencias = 0;

	 for(list<unsigned long>::iterator iterator = ++sameContextPositions.begin();
		iterator != sameContextPositions.end(); ++iterator){				// *iterator es una tupla (unsigned long,char) indicando la posicion del ctx y el char que le sigue

		unsigned long indexDelCaracterOfrecido = *iterator[0] + 1;
		bool esElBuscado = charsIguales(indexDelCaracterOfrecido, charToRank, buffer);

		if (esElBuscado){
			sameContextPositions.remove(*iterator);		//Renueva la tupla del contexto. Ya que ya se coloco la posicion mas reciente del mismo contexto (la actual)
			return tupla(true,noOcurrencias);
		}
		noOcurrencias++;
	}
	return tupla(false,noOcurrencias);

*/
	return tupla;
}

// IMPLEMENTAR.
list<unsigned long> SymbolRanking::hashear(char symbol1, char symbol2, unsigned long indexFirstChar){
	list<unsigned long> lista;
	return lista;
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
	auto it = std::find(exclusionList.begin(), exclusionList.end(), pos);
	if (it == exclusionList.end()) return true;
	return false;
}
