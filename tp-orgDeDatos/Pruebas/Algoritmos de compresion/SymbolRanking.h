#ifndef SYMBOLRANKING_H
#define SYMBOLRANKING_H

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <list>
#include <tuple>
#include "WFC.h"
#include "HashMap.h"
#include "../Exceptions/ErrorDeParametro.h"

using namespace std;

class SymbolRanking{
	private:
		WFC wfc;
		list<char> exclusionList;
		unsigned short ordenMaximo;

		//Hash (tambien llamado map) con 255*255 claves que son combinacion de dos char. Sus valores
		// son listas de las posiciones de sus apariciones de dichas claves en el buffer.
		HashMap hashmap;

		/* Evalua si hay un match en el buffer, dado un orden de contexto. En caso de que haya match de contextos.
		 * evalua si la oferta realizada es positiva. EJ: mejor caso (true,0) peor caso (false, contextosQueSeComparan).
		 * Sigue el esquema de hashing presentado en el Tech paper 132 de P. Fenwich
 		 * Precondiciones:
 		 *  - orden: Indica el orden del contexto que se debe buscar el match.
		 *  - posCharToRank: Es la posicion actual en el buffer que se esta procesando. Debe ser un unsigned int
		 *  - buffer: Contiene los datos que han sido procesados desde la posicion 0 hasta posCharToRank
		 *  - operacion: Indica si la busqueda del contexto se esta realizando para el caso del compresor ('c') o descompresor ('d).
		 *               Se debe ingresar 'c' o 'd'. En caso contrario se levanta la excepcion ErrorDeParametro
		 *  - ranking: Este parametro unicamente es utilizado para el descompresor e indica el ranking del char a descomprimir.
		 * Postcondiciones:
		 *  -tuple[0] es un bool que indica si la oferta fue positiva (true) o no (false)
		 *  -tuple[1]: Si tuple[0]==true y la operacion es 'c' --> indica la cantidad de ofertas negativas que se realizaron hasta lograr la oferta positiva
		 *  							  la operacion es 'd' --> es el caracter ofrecido casteado a unsigned short
		 *  		   Si tuple[0]==false, indica la cantidad de ofertas negativas.
		 *
		 */
		tuple<bool,unsigned short> buscarEnContexto(unsigned short orden, unsigned long posCharToRank, char* buffer,char operacion,unsigned short ranking);

		/*
		 * Evalua si hay un match de orden 1 y en dicho caso, evalua si la oferta realizada por dicho contexto
		 * es positiva.
		 * Precondiciones:
		 *  - posCharToRank: Es la posicion actual en el buffer que se esta procesando. Debe ser un unsigned int
		 *  - buffer: Contiene los datos que han sido procesados desde la posicion 0 hasta posCharToRank
		 *  - operacion: Indica si la busqueda del contexto se esta realizando para el caso del compresor ('c') o descompresor ('d).
		 *               Se debe ingresar 'c' o 'd'. En caso contrario se levanta la excepcion ErrorDeParametro
		 *  - ranking: Este parametro unicamente es utilizado para el descompresor e indica el ranking del char a descomprimir.
		 * Postcondiciones:
		 *  -tuple[0] es un bool que indica si la oferta fue positiva (true) o no (false)
		 *  -tuple[1]: Si tuple[0]==true y la operacion es 'c' --> indica la cantidad de ofertas negativas que se realizaron hasta lograr la oferta positiva
		 *  							  la operacion es 'd' --> es el caracter ofrecido casteado a unsigned short
		 *  		   Si tuple[0]==false, indica la cantidad de ofertas negativas.
		 */
		tuple<bool,unsigned short> buscarEnContextoUno(unsigned long posCharToRank, char* buffer, char operacion,unsigned short ranking);

		/* Devuelve la lista correspondiente a la clave conformada por el string entre el caracter en
		 * la posicion posFirst del buffer, y el siguiente. */
		list<unsigned long> getListOfPositions(char* buffer, unsigned long posFirst);

		/* Dado un vector con 2 char, lo hashea en el map (atributo).
		 */
		void hashear(char symbol1, char symbol2, unsigned long indexFirstChar);

		/* Dadas dos offset de un buffer, se comparan N Bytes (N = nro de orden).
		 * Si dichos Bytes son iguales, entonces dichos strings son iguales y por lo tanto los contextos son iguales.
		 * Precondiciones:
		 *  -indexA e indexB deben ser offsets validos del buffer, donde indexA < indexB
		 * Postcondiciones:
		 *  Devuelve un boolean de acuerdo a si se produce un match de contextos o no
		 */
		bool contextosIguales(unsigned long indexA, unsigned long indexB, char* buffer,int ordenDelContexto);

		/*
		 * Dado un offset de un array, verifica si el char apuntado por esa posicion es el caracter a comparar.
		 * Precondiciones:
		 *  - index debe ser un indice valido del buffer
		 * Postcondiciones:
		 *  Devuelve un boolean de acuerdo a si hay un match entre los caracteres involucrados
		 */
		bool charsIguales(unsigned long index,char charToCompare,char* buffer);

		/* Busca si el caracter ofrecido como ranking esta en la lista de exclusion.
		 * Postcondiciones:
		 *  devuelve True si no se encuentra en la lista.
		 */
		bool charNoExcluido(char charToFind);

	public:
		SymbolRanking();

		SymbolRanking(unsigned short orden);

		/*Recibe un vector de char a comprimir. El resultado se almacenara en un vector de short (necesito mas de 256 caracteres
		 * por lo que no se puede guardar en un vector de char */
		void comprimir(char* aComprimir, short* salida, unsigned long size);

		/*
		 * Recibe un vector de shorts y genera un vector de chars descomprimidos.
		 */
		void descomprimir(unsigned short* aDescomprimir, char* salida, unsigned long size);
};

#endif
