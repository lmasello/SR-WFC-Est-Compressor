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
			hashear(aComprimir[i-1], aComprimir[i-2], i-2);
		}
		char charAProcesar = aComprimir[i];
		salida[i] = wfc.comprimir(charAProcesar);

		cout<<"El caracter " << charAProcesar << " lo procesa como " << salida[i] << endl;
	}
	//Siguientes caracteres
	for (unsigned long i = 3; i< size; i++){	//i= index del char en el buffer

		cout << i << size << endl;

		charToRank = aComprimir[i];
		exclusionList.clear(); 						//Deberia ser lo suficientemente eficiente, si pasa algo malo, referirse a la pagina 6 del 132.
		nroNoOcurrencias = 0;

 		hashear(aComprimir[i-1], aComprimir[i-2], i-2);
		while(ctxActual > 1){
			tupla = buscarEnContexto(ctxActual, charToRank, i, aComprimir);
			nroNoOcurrencias += get<1> (tupla);
			if (get<0> (tupla)) break;
			ctxActual--;
		}
		if (ctxActual == 1){
			tupla = buscarEnContextoUno(charToRank, i,aComprimir);
			nroNoOcurrencias += get<1> (tupla);
			if (!get<0> (tupla)) nroNoOcurrencias += wfc.comprimir(charToRank);	  // Caso de contexto = 0. Se comprime el numero actual de acuerdo al metodo WFC.
		}
		salida[i] = nroNoOcurrencias;
		ctxActual = orden;
	}
}

tuple<bool,unsigned short> SymbolRanking::buscarEnContexto(int orden, char caracter, unsigned long pos, char* buffer){
	unsigned long indexFirstChar = pos-orden;
	unsigned long indexSecondChar = pos-orden+1;
	unsigned short nroNoOcurrencias = 0;
	tuple<bool, unsigned short> tupla;

	cout<<"Se realiza la busqueda de contextos iguales, para el caracter a ranquear: " << caracter << " , " << (short) caracter << endl;

	for(list<unsigned long>::iterator iterator = ++listOfPositions.begin();
		iterator != listOfPositions.end(); ++iterator){							//*iterator seria un unsigned long, indicando una posicion de la lista de posiciones
		bool hayMatch = contextosIguales(*iterator,indexFirstChar,buffer,orden);
		if (hayMatch){

			cout<<"Hay match de orden " << orden << " en las posiciones: " << *iterator << " y " << indexFirstChar << endl;

			if(charNoExcluido(*iterator+orden)){
				bool esElBuscado = charsIguales(*iterator + orden, caracter, buffer);
				if (esElBuscado){
					listOfPositions.push_front(indexFirstChar); //Si no me equivoco, esto lo hace la funcion hashear!!!
					get<0> (tupla) = true;
					get<1> (tupla) = nroNoOcurrencias;

					cout<<"El caracter ofrecido del contexto " << *iterator << " matchea con el caracter a rankear" << endl;
					cout<<"El numero de no ocurrencias hasta encontrar el match fue de: " << nroNoOcurrencias << endl;

					return tupla;
				}

				cout<<"No hay match, por lo tanto se agrega el caracter ofrecido a la lista de exclusion" << endl;

				exclusionList.push_front(*iterator+orden);
				nroNoOcurrencias++;
			}
		}
	}
	cout<<"No hubieron matches" << endl;
	cout<<"Se han realizado " << nroNoOcurrencias << " ofertas insatisfactorias" << endl;

	get<0> (tupla) = false;
	get<1> (tupla) = nroNoOcurrencias;
	return tupla;
}

//Se sigue la idea mencionada por Fenwich en el 132
tuple<bool,unsigned short> SymbolRanking::buscarEnContextoUno(char charToRank, unsigned long pos, char* buffer){
	tuple<bool, unsigned short> tupla;
	unsigned long posicionDelContexto = pos-1;
	char charDelContexto = buffer[posicionDelContexto];

	cout<<"Se comienza la busqueda de contextos iguales de orden 1, para: "<< charToRank << endl;

	/*Ver de implementar una funcion de hash para un char y que devuelva una lista de tuplas, donde cada tupla
	  este formada por la posicion en donde aparece el contexto y el char que le sigue.
 	  De esta manera se aplica la lista de exclusion en la misma lista de ocurrencias, controlando el agregado y quitado de tuplas, haciendo
 	  que haya una sola entrada por cada char que le sigue al contexto. Si la prediccion es positiva, se remueve el ctx antiguo y se deja el actual.
	  si la prediccion es negativa, queda todo como esta.
	  Ej: Seria un map, donde la clave es el charDelContexto y tiene como valor (posicionDelContexto,charToRank)
	*/
	list<tuple<unsigned long,char>> sameContextPositions = hasheartu(charToRank, charDelContexto, posicionDelContexto);

	unsigned short nroNoOcurrencias = 0;

	for(list<tuple<unsigned long,char>>::iterator iterator = ++sameContextPositions.begin();
	iterator != sameContextPositions.end(); ++iterator){				// *iterator es una tupla (unsigned long,char) indicando la posicion del ctx y el char que le sigue

		unsigned long indexDelCaracterOfrecido = get<0>(*iterator) + 1;

		cout<<"Evalua el match para la posicion" << indexDelCaracterOfrecido << "del contexto" << endl;

		bool esElBuscado = charsIguales(indexDelCaracterOfrecido, charToRank, buffer);

		if (esElBuscado){

			cout<<"Hay match, remueve el contexto anterior y guarda el contexto actual" << endl;
			cout<<"El numero de no ocurrencias hasta encontrar el match fue de: " << nroNoOcurrencias << endl;

			sameContextPositions.remove(*iterator);		//Remueve la tupla del contexto. Ya que ya se coloco la posicion mas reciente del mismo contexto (la actual)
			get<0> (tupla) = true;
			get<1> (tupla) = nroNoOcurrencias;
			return tupla;
		}
		nroNoOcurrencias++;
	}
	cout<<"No hubieron matches" << endl;
	cout<<"Se han realizado " << nroNoOcurrencias << " ofertas insatisfactorias" << endl;

	get<0> (tupla) = false;
	get<1> (tupla) = nroNoOcurrencias;
	return tupla;
}
void SymbolRanking::hashear(char symbol1, char symbol2, unsigned long indexFirstChar){
	list<unsigned long> lista;
	char expr[2] = {symbol1, symbol2};
	string clave (expr);

	pair<string, list<unsigned long>> insertar (clave, lista);

	unordered_map<string, list<unsigned long>>::const_iterator got = mymap.find (clave);

	if(got == mymap.end())
		mymap.insert(insertar);

	mymap.at(clave).push_front(indexFirstChar);
}

// IMPLEMENTAR. Antes de que me olvide. Esto no va a funcionar.
/* Deberia funcionar de acuerdo a lo siguiente:
 * unordered_map<Key,T>::iterator it;
(*it).first;             // the key value (of type Key)
(*it).second;            // the mapped value (of type T)
(*it);                   // the "element value" (of type pair<const Key,T>)
 */
list<tuple<unsigned long,char>> SymbolRanking::hasheartu(char symbol1, char symbol2, unsigned long indexFirstChar){
	list<tuple<unsigned long,char>> lista;
	return lista;
}



bool SymbolRanking::contextosIguales(unsigned long indexA, unsigned long indexB, char* buffer,int orden){
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
