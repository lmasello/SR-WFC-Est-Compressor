/*
 * WFC.h
 *
 *  Created on: May 21, 2014
 *      Author: leandro
 */

#ifndef WFC_H_
#define WFC_H_
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include "../Exceptions/notInListError.h"

using namespace std;

class WFC{
private:
	char weightedList[256];
	unordered_map<char,unsigned short> charFrequencies;

	/*
	 * Devuelve la posicion de un char de la weightedList
	 */
	unsigned short getIndex(char charToTransform);

	void incrementarFrecuencia(char charToIncrement);

	/*
	 * Reordena la weithedList en base a las frecuencias. EL criterio de ordenamiento
	 * es en forma de frecuencias decrecientes, quedando ubicados aquellos char mas
	 * frecuentes al comienzo de la lista
	 */
	void updateWeightedList();

	/*
	 * Algoritmo que se utiliza para ordenar la lista de pesos en orden decreciente.
	 */
	void quickSort(int left, int right);

public:

	WFC();

	/*
	 * Recibe un char y devuelve su posicion en la weithedList.
	 * Precondiciones:
	 *  - char debe encontrarse en la weithedList
	 */
	unsigned short comprimir(char charToTransform);

	/*
	 * Recibe un numero y devuelve el char correspondiente al caracter de
	 * weithedList[numberToTransform]
	 * Precondiciones:
	 *  - numberToTransform debe ser un numero entre 0 y weithedList.size()
	 */
	char descomprimir(unsigned short numberToTransform);
};

#endif /* WFC_H_ */
