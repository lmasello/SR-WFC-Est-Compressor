#include "SymbolRanking.h"

SymbolRanking::SymbolRanking(unsigned short ctxorden){
	ordenMaximo = ctxorden;
}

void SymbolRanking::comprimir(char* aComprimir, short* salida, unsigned int size){
	/* Busca el contexto con un orden maximo. Si devuelve (true,x) almacenamos x en el
	* vector y avanzamos. Si devuelve (false,x) se busca contexto de orden n-1.
	* En caso de no ocurrencias, se itera hasta llegar a contexto 0, donde se almacena
	* en el vector el numero ascii correspondiente al literal, mas el numero de no ocurrencias */

	unsigned short ctxActual = ordenMaximo;
	unsigned short posicionMinimaParaHashear = 2;
	char charToRank;
	unsigned short cantidadDeNoOcurrencias; //Sera el numero de no ocurrencias hasta que se encuentre el simbolo.
	tuple<bool,unsigned short> tupla;

	cout<<"Comienza el proceso de compresion por Symbol Ranking de orden " << ordenMaximo << endl;

	//Primeros caracteres [0,orden-1]
	for(int posCharToRank = 0; posCharToRank < ordenMaximo; posCharToRank++){
		if (posCharToRank > 1){
			hashear(aComprimir[posCharToRank-2], aComprimir[posCharToRank-1], posCharToRank-2);
		}
		char charAProcesar = aComprimir[posCharToRank];
		salida[posCharToRank] = wfc.comprimir(charAProcesar);

//		cout<<"El caracter " << charAProcesar << " lo procesa como " << salida[posCharToRank] << endl;
	}
	//Siguientes caracteres
	for (unsigned int posCharToRank = ordenMaximo; posCharToRank< size; posCharToRank++){

		charToRank = aComprimir[posCharToRank];
		exclusionList.clear(); 						//Deberia ser lo suficientemente eficiente, si pasa algo malo, referirse a la pagina 6 del 132.
		cantidadDeNoOcurrencias = 0;
//		if(posCharToRank%100000 == 0) resetHashMap();

		while(ctxActual > 1){
			tupla = buscarEnContexto(ctxActual,posCharToRank, aComprimir,'c',0);//El ultimo parametro (ranking) no se utiliza para el compresor, por lo tanto se lo pone en 0
			cantidadDeNoOcurrencias += get<1> (tupla);
			if (get<0> (tupla)) break;
			ctxActual--;
		}
		if (ctxActual == 1){
			tupla = buscarEnContextoUno(posCharToRank, aComprimir,'c',0); //El ultimo parametro (ranking) no se utiliza para el compresor, por lo tanto se lo pone en 0
			cantidadDeNoOcurrencias += get<1> (tupla);
			if (!get<0> (tupla)){
//				cout << "EL numero total de ofertas negativas fue de: "<<cantidadDeNoOcurrencias << endl;
				cantidadDeNoOcurrencias += wfc.comprimir(charToRank);	  // Caso de contexto = 0. Se comprime el numero actual de acuerdo al metodo WFC.
			}
		}
		if(get<0>(tupla)) wfc.incrementarFrecuencia(charToRank);
		if(posCharToRank >= posicionMinimaParaHashear) hashear(aComprimir[posCharToRank-2], aComprimir[posCharToRank-1], posCharToRank-2);
		salida[posCharToRank] = cantidadDeNoOcurrencias;
//		cout<<"Salida: " << salida[posCharToRank] << endl<<endl;
		ctxActual = ordenMaximo;
	}
}

void SymbolRanking::descomprimir(unsigned short* aDescomprimir, char* salida, unsigned int size){

	unsigned short ctxActual = ordenMaximo;
	unsigned short posicionMinimaParaHashear = 2;
	unsigned short rankToChar;
	tuple<bool,unsigned short> tupla;

	cout<<"Comienza el proceso de descompresion por Symbol Ranking de orden " << ordenMaximo << endl;

	//Primeros rankings [0,orden-1]
	for(unsigned int posRankToChar = 0; posRankToChar < ordenMaximo; posRankToChar++){
		if (posRankToChar > 1){
			hashear(salida[posRankToChar-2], salida[posRankToChar-1], posRankToChar-2);
		}
		unsigned short rankToChar = aDescomprimir[posRankToChar];
		salida[posRankToChar] = wfc.descomprimir(rankToChar);

		cout<<"El rank " << rankToChar << " lo procesa como el caracter: " << salida[posRankToChar] << endl;
	}

	//Siguientes caracteres
	for (unsigned int posRankToChar = ordenMaximo; posRankToChar< size; posRankToChar++){
		rankToChar = aDescomprimir[posRankToChar];
		exclusionList.clear();

		while(ctxActual > 1){
			tupla = buscarEnContexto(ctxActual, posRankToChar, salida, 'd', rankToChar);
			if (get<0> (tupla))break;
			rankToChar -= get<1> (tupla);
			ctxActual--;
		}
		if (ctxActual == 1){
			tupla = buscarEnContextoUno(posRankToChar, salida, 'd', rankToChar); //Debe checkear en su implementacion que charToRank - NoOcurrencias sea 0. Si es 0, devuelve True, y el char siguiente al contexto. Si no es 0, sigue buscando y repite. Si se termino, devuelve False, y las NoOcurrencias.
			if (!get<0> (tupla)){
			rankToChar -= get<1> (tupla);
			salida[posRankToChar] = wfc.descomprimir(rankToChar);	  // Caso de contexto = 0. Se comprime el numero actual de acuerdo al metodo WFC.
			cout<<"El rank " << rankToChar << " lo procesa como el caracter " << salida[posRankToChar] << endl<<endl;
			}
		}
		//Nota: cuando la tupla me devuelve True, quiere decir que el segundo elemento es el caracter ofrecido por un contexto
		// existente que matcheo. Por lo tanto, esto siempre va a ser un char
		if(get<0>(tupla)){
			salida[posRankToChar] = (char) get<1>(tupla);
			wfc.incrementarFrecuencia(salida[posRankToChar]);
			cout<<"El rank " << rankToChar << " lo procesa como el caracter " << salida[posRankToChar] << endl<<endl;
		}
		if(posRankToChar >= posicionMinimaParaHashear)hashear(salida[posRankToChar-2], salida[posRankToChar-1], posRankToChar-2);
		ctxActual = ordenMaximo;
	}
}

tuple<bool,unsigned short> SymbolRanking::buscarEnContexto(unsigned short orden, unsigned int posCharToRank, char* buffer, char operacion,unsigned short ranking){
	unsigned int indexFirstCharOfCurrentContext = posCharToRank-orden;
	unsigned short cantidadDeNoOcurrencias = 0;
	tuple<bool, unsigned short> tupla;

	list<unsigned int> listOfPositions = getListOfPositions(buffer, posCharToRank-2);

	for(list<unsigned int>::iterator posDeMatch = listOfPositions.begin(); posDeMatch != listOfPositions.end(); ++posDeMatch){

		bool hayMatch = contextosIguales(*posDeMatch,indexFirstCharOfCurrentContext,buffer,orden);
		if (hayMatch){
			if(charNoExcluido(buffer[*posDeMatch+2])){
			if(operacion=='c'){
				bool esElBuscado = charsIguales(*posDeMatch + 2, buffer[posCharToRank], buffer);
				if (esElBuscado){
					get<0> (tupla) = true;
					get<1> (tupla) = cantidadDeNoOcurrencias;
					return tupla;
				}
			}
			else if(operacion=='d'){
				if(ranking==0){ //El char ofrecido es el descomprimido!
					unsigned short charDelRanking = (unsigned short) buffer[*posDeMatch+2];
					get<0> (tupla) = true;
					get<1> (tupla) = charDelRanking;
					return tupla;
				}
				ranking--;
			}
			else throw ErrorDeParametro();
			exclusionList.push_front(buffer[*posDeMatch+2]);
			cantidadDeNoOcurrencias++;
			}
		}
	}
	get<0> (tupla) = false;
	get<1> (tupla) = cantidadDeNoOcurrencias;
	return tupla;
}

tuple<bool,unsigned short> SymbolRanking::buscarEnContextoUno(unsigned int posCharToRank, char* buffer, char operacion,unsigned short ranking){
	tuple<bool, unsigned short> tupla;
	unsigned int contextCharToRank = posCharToRank-1;
	unsigned short cantidadDeNoOcurrencias = 0;

	for(unsigned int i = 2; i <= posCharToRank; i++){
		unsigned int contextAComparar = posCharToRank-i;
		if(buffer[contextAComparar] == buffer[contextCharToRank]){
			if(charNoExcluido(buffer[contextAComparar+1])){

				if(operacion=='c'){
					bool esElBuscado = charsIguales(contextAComparar+1, buffer[posCharToRank], buffer);
					if(esElBuscado){
						get<0> (tupla) = true;
						get<1> (tupla) = cantidadDeNoOcurrencias;
						return tupla;
					}
				}
				else if(operacion=='d'){
					if(ranking==0){ //El char ofrecido es el que hay que descomprimir
						unsigned short charDelRanking = (unsigned short) buffer[contextAComparar+1];
						get<0> (tupla) = true;
						get<1> (tupla) = charDelRanking;
						return tupla;
					}
					ranking--;
				}
				else throw ErrorDeParametro();
				exclusionList.push_front(buffer[contextAComparar+1]);
				cantidadDeNoOcurrencias++;
			}
		}
	}
	get<0> (tupla) = false;
	get<1> (tupla) = cantidadDeNoOcurrencias;
	return tupla;
}

void SymbolRanking::resetHashMap(){
	hashmap.reset();
}

list<unsigned int> SymbolRanking::getListOfPositions(char* buffer, unsigned int posFirst){
	return hashmap.get(buffer[posFirst], buffer[posFirst+1]);
}

void SymbolRanking::hashear(char symbol1, char symbol2, unsigned int indexFirstChar){
	hashmap.put(symbol1, symbol2, indexFirstChar);
}

bool SymbolRanking::contextosIguales(unsigned int indexA, unsigned int indexB, char* buffer,int orden){
	int comienzoDeContextoAComparar = indexA+2-orden; // Ubica al index en la posicion inicial del string a comparar de acuerdo al orden. Como se hashea para los ultimos 2 chars, la posicion incial se obtiene como (2-orden)
	if (comienzoDeContextoAComparar<0) return false; //Este es el caso que en el hash tengo posicion 0, y debido al contexto actual me genera underflow.
	for (unsigned short i=0; i<orden; i++){
		if (buffer[comienzoDeContextoAComparar+i]!=buffer[indexB+i])return false;
	}
	return true;
}

bool SymbolRanking::charsIguales(unsigned int index,char charToCompare,char* buffer){
	if(buffer[index]==charToCompare)return true;
	return false;
}
// Nota, copiado de cplusplus
bool SymbolRanking::charNoExcluido(char charToFind){
	auto it = find(exclusionList.begin(), exclusionList.end(), charToFind);
	if (it == exclusionList.end()) return true;
	return false;
}
