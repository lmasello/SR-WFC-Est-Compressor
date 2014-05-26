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

	cout<<"Comienza el proceso de compresion por Symbol Ranking de orden " << orden << endl;

	//Primeros caracteres [0,orden-1]
	for(int i = 0; i < orden; i++){
		if (i > 1){
			hashear(aComprimir[i-2], aComprimir[i-1], i-2);
		}
		char charAProcesar = aComprimir[i];
		salida[i] = wfc.comprimir(charAProcesar);

		cout<<"El caracter " << charAProcesar << " lo procesa como " << salida[i] << endl;
	}
	//Siguientes caracteres
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
			tupla = buscarEnContextoUno(charToRank, i, aComprimir);
			nroNoOcurrencias += get<1> (tupla);
			if (!get<0> (tupla)){
				cout << "EL numero total de ofertas negativas fue de: "<<nroNoOcurrencias << endl;
				nroNoOcurrencias += wfc.comprimir(charToRank);	  // Caso de contexto = 0. Se comprime el numero actual de acuerdo al metodo WFC.
			}
		}
		hashear(aComprimir[i-2], aComprimir[i-1], i-2);
		salida[i] = nroNoOcurrencias;
		cout<<"Salida: " << salida[i] << endl<<endl;
		ctxActual = orden;
	}
}

tuple<bool,unsigned short> SymbolRanking::buscarEnContexto(int orden, char caracter, unsigned long pos, char* buffer){
	unsigned long indexFirstChar = pos-orden;
	unsigned short nroNoOcurrencias = 0;
	tuple<bool, unsigned short> tupla;
	list<unsigned long> listOfPositions = getListOfPositions(buffer, pos-2);

	cout<<"Se realiza la busqueda de contextos iguales de orden " << orden << ", para el caracter a ranquear: " << caracter << " , " << pos << endl;

	for(list<unsigned long>::iterator iterator = listOfPositions.begin();
		iterator != listOfPositions.end(); ++iterator){							//*iterator seria un unsigned long, indicando una posicion de la lista de posiciones
		bool hayMatch = contextosIguales(*iterator,indexFirstChar,buffer,orden);
		if (hayMatch){

			cout<<"Hay match de orden " << orden << " entre las posiciones: " << *iterator+(2-orden) << " y " << *iterator+1 << endl;

			if(charNoExcluido(buffer[*iterator+2])){
				bool esElBuscado = charsIguales(*iterator + 2, caracter, buffer);
				if (esElBuscado){
					get<0> (tupla) = true;
					get<1> (tupla) = nroNoOcurrencias;

					cout<<"El caracter ofrecido del contexto " << *iterator << " matchea con el caracter a rankear" << endl;
					cout<<"El numero de no ocurrencias hasta encontrar el match fue de: " << nroNoOcurrencias << endl;

					return tupla;
				}

				cout<<"El caracter ofrecido no es el buscado, por lo tanto se agrega "<<buffer[*iterator+2]<< " a la lista de exclusion" << endl;

				exclusionList.push_front(buffer[*iterator+2]);
				nroNoOcurrencias++;
			}
			else{
				cout << "El caracter ofrecido fue excluido previamente." << endl;
			}
		}
	}
	cout<<"Se han realizado " << nroNoOcurrencias << " ofertas insatisfactorias" << endl;

	get<0> (tupla) = false;
	get<1> (tupla) = nroNoOcurrencias;
	return tupla;
}


tuple<bool,unsigned short> SymbolRanking::buscarEnContextoUno(char charToRank, unsigned long pos, char* buffer){
	tuple<bool, unsigned short> tupla;
	unsigned long contextPos = pos-1;

	cout<<"Se comienza la busqueda de contextos iguales de orden 1, para: "<< charToRank <<" , "<<pos<< endl;

	unsigned short nroNoOcurrencias = 0;

	for(unsigned long i = 2; i <= pos; i++){
		if(buffer[pos-i] == buffer[contextPos]){
			cout<<"Hay match de contexto uno en la posicion "<<(pos-i) << endl;
			if(charNoExcluido(buffer[pos-i+1])){
				bool esElBuscado = charsIguales(pos-i+1, buffer[pos], buffer);
				if(esElBuscado){
					get<0> (tupla) = true;
					get<1> (tupla) = nroNoOcurrencias;
					return tupla;
				}
				cout<<"El caracter ofrecido no es el buscado, por lo tanto se agrega "<<buffer[pos-i+1]<< " a la lista de exclusion" << endl;
				exclusionList.push_front(buffer[pos-i+1]);
				nroNoOcurrencias++;
			}
			else{
				cout << "El caracter ofrecido fue excluido previamente." << endl;
			}
		}
	}
	cout<<"Se han realizado " << nroNoOcurrencias << " ofertas insatisfactorias" << endl;

	get<0> (tupla) = false;
	get<1> (tupla) = nroNoOcurrencias;
	return tupla;
}

list<unsigned long> SymbolRanking::getListOfPositions(char* buffer, unsigned long posFirst){
	return hashmap.get(buffer[posFirst], buffer[posFirst+1]);
}

void SymbolRanking::hashear(char symbol1, char symbol2, unsigned long indexFirstChar){
	hashmap.put(symbol1, symbol2, indexFirstChar);
}

bool SymbolRanking::contextosIguales(unsigned long indexA, unsigned long indexB, char* buffer,int orden){
	unsigned long comienzoDeContextoAComparar = indexA+2-orden; // Ubica al index en la posicion inicial del string a comparar de acuerdo al orden. Como se hashea para los ultimos 2 chars, la posicion incial se obtiene como (2-orden)
	for (unsigned short i=0; i<orden; i++){
		if (buffer[comienzoDeContextoAComparar+i]!=buffer[indexB+i])return false;
	}
	return true;
}

bool SymbolRanking::charsIguales(unsigned long index,char charToCompare,char* buffer){
	if(buffer[index]==charToCompare)return true;
	return false;
}

// Nota, copiado de cplusplus
bool SymbolRanking::charNoExcluido(char charToFind){
	auto it = find(exclusionList.begin(), exclusionList.end(), charToFind);
	if (it == exclusionList.end()) return true;
	return false;
}
