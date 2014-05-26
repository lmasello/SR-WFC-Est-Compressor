#include "SymbolRanking.h"

SymbolRanking::SymbolRanking(unsigned short ctxorden){
	ordenMaximo = ctxorden;
}

void SymbolRanking::comprimir(char* aComprimir, short* salida, unsigned long size){
	/* Busca el contexto con un orden maximo. Si devuelve (true,x) almacenamos x en el
	* vector y avanzamos. Si devuelve (false,x) se busca contexto de orden n-1.
	* En caso de no ocurrencias, se itera hasta llegar a contexto 0, donde se almacena
	* en el vector el numero ascii correspondiente al literal, mas el numero de no ocurrencias */

	unsigned short ctxActual = ordenMaximo;
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

		cout<<"El caracter " << charAProcesar << " lo procesa como " << salida[posCharToRank] << endl;
	}
	//Siguientes caracteres
	for (unsigned long posCharToRank = ordenMaximo; posCharToRank< size; posCharToRank++){	//i= index del char en el buffer

		charToRank = aComprimir[posCharToRank];
		exclusionList.clear(); 						//Deberia ser lo suficientemente eficiente, si pasa algo malo, referirse a la pagina 6 del 132.
		cantidadDeNoOcurrencias = 0;

 		while(ctxActual > 1){
			tupla = buscarEnContexto(ctxActual, charToRank, posCharToRank, aComprimir);
			cantidadDeNoOcurrencias += get<1> (tupla);
			if (get<0> (tupla)) break;
			ctxActual--;
		}
		if (ctxActual == 1){
			tupla = buscarEnContextoUno(charToRank, posCharToRank, aComprimir);
			cantidadDeNoOcurrencias += get<1> (tupla);
			if (!get<0> (tupla)){
				cout << "EL numero total de ofertas negativas fue de: "<<cantidadDeNoOcurrencias << endl;
				cantidadDeNoOcurrencias += wfc.comprimir(charToRank);	  // Caso de contexto = 0. Se comprime el numero actual de acuerdo al metodo WFC.
			}
		}
		hashear(aComprimir[posCharToRank-2], aComprimir[posCharToRank-1], posCharToRank-2);
		salida[posCharToRank] = cantidadDeNoOcurrencias;
		cout<<"Salida: " << salida[posCharToRank] << endl<<endl;
		ctxActual = ordenMaximo;
	}
}

tuple<bool,unsigned short> SymbolRanking::buscarEnContexto(unsigned short orden, char caracter, unsigned long posCharToRank, char* buffer){
	unsigned long indexFirstCharOfCurrentContext = posCharToRank-orden;
	unsigned short cantidadDeNoOcurrencias = 0;
	tuple<bool, unsigned short> tupla;
	list<unsigned long> listOfPositions = getListOfPositions(buffer, posCharToRank-2);

	cout<<"Se realiza la busqueda de contextos iguales de orden " << orden << ", para el caracter a ranquear: " << caracter << " , " << posCharToRank << endl;

	for(list<unsigned long>::iterator posDeMatch = listOfPositions.begin();
		posDeMatch != listOfPositions.end(); ++posDeMatch){							//*iterator seria un unsigned long, indicando una posicion de la lista de posiciones
		bool hayMatch = contextosIguales(*posDeMatch,indexFirstCharOfCurrentContext,buffer,orden);
		if (hayMatch){

			cout<<"Hay match de orden " << orden << " entre las posiciones: " << *posDeMatch+(2-orden) << " y " << *posDeMatch+1 << endl;

			if(charNoExcluido(buffer[*posDeMatch+2])){
				bool esElBuscado = charsIguales(*posDeMatch + 2, caracter, buffer);
				if (esElBuscado){
					get<0> (tupla) = true;
					get<1> (tupla) = cantidadDeNoOcurrencias;

					cout<<"El caracter ofrecido del contexto " << *posDeMatch << " matchea con el caracter a rankear" << endl;
					cout<<"El numero de no ocurrencias hasta encontrar el match fue de: " << cantidadDeNoOcurrencias << endl;

					return tupla;
				}

				cout<<"El caracter ofrecido no es el buscado, por lo tanto se agrega "<<buffer[*posDeMatch+2]<< " a la lista de exclusion" << endl;

				exclusionList.push_front(buffer[*posDeMatch+2]);
				cantidadDeNoOcurrencias++;
			}
			else{
				cout << "El caracter ofrecido fue excluido previamente." << endl;
			}
		}
	}
	cout<<"Se han realizado " << cantidadDeNoOcurrencias << " ofertas insatisfactorias" << endl;

	get<0> (tupla) = false;
	get<1> (tupla) = cantidadDeNoOcurrencias;
	return tupla;
}

tuple<bool,unsigned short> SymbolRanking::buscarEnContextoUno(char charToRank, unsigned long posCharToRank, char* buffer){
	tuple<bool, unsigned short> tupla;
	unsigned long contextCharToRank = posCharToRank-1;

	cout<<"Se comienza la busqueda de contextos iguales de orden 1, para: "<< charToRank <<" , "<<posCharToRank<< endl;

	unsigned short cantidadDeNoOcurrencias = 0;

	for(unsigned long i = 2; i <= posCharToRank; i++){
		unsigned long contextAComparar = posCharToRank-i;
		if(buffer[contextAComparar] == buffer[contextCharToRank]){
			cout<<"Hay match de contexto uno en la posicion "<<(contextAComparar) << endl;
			if(charNoExcluido(buffer[contextAComparar+1])){
				bool esElBuscado = charsIguales(contextAComparar+1, buffer[posCharToRank], buffer);
				if(esElBuscado){
					get<0> (tupla) = true;
					get<1> (tupla) = cantidadDeNoOcurrencias;
					return tupla;
				}
				cout<<"El caracter ofrecido no es el buscado, por lo tanto se agrega "<<buffer[posCharToRank-i+1]<< " a la lista de exclusion" << endl;
				exclusionList.push_front(buffer[contextAComparar+1]);
				cantidadDeNoOcurrencias++;
			}
			else{
				cout << "El caracter ofrecido fue excluido previamente." << endl;
			}
		}
	}
	cout<<"Se han realizado " << cantidadDeNoOcurrencias << " ofertas insatisfactorias" << endl;

	get<0> (tupla) = false;
	get<1> (tupla) = cantidadDeNoOcurrencias;
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
