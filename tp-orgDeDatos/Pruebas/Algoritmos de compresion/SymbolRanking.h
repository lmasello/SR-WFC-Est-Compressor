/*
 * SymbolRanking.h
 *
 *  Created on: 18 May 2014
 *      Author: lmasel
 */

#ifndef SYMBOLRANKING_H_
#define SYMBOLRANKING_H_

class SymbolRanking{
private:

public:
	void comprimir(unsigned int orden, char buffer[], unsigned int output[]);
	/*Dado un buffer con chars y un orden para la longitud maxima de match, se realiza el algoritmo
	 * de symbolRanking, colocando en el array de enteros positivos, un numero positivo, donde cada uno
	 *  de ellos indica el ranking con el cual fue comprimido cada char del buffer.
	 *
	 * Precondiciones:
	 *  - Orden debe ser un valor positivo (se estima que el optimo esta entre 3 y 4)
	 *  - buffer debe contener el archivo a procesar
	 * Postcondiciones:
	 *  - Modifica el array output, el cual debe encontrarse vacio, pero con cantidad de memoria suficiente
	 *    para almacenar la salida
	 */
};


#endif /* SYMBOLRANKING_H_ */
