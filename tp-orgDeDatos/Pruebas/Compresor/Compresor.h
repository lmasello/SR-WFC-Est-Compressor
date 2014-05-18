#ifndef COMPRESOR_H
#define COMPRESOR_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstddef>


/*Compresor tendra la logica sobre la compresion y descompresion del archivo de entrada. */
class Compresor{
	public:
		Compresor();

		/* A partir del buffer obtenido, se generara el archivo comprimido mediante los metodos de Symbol Ranking
		 * + MTF + Estructurado. Maneja la logica general del proceso y realiza las llamadas a los metodos de compresion
		 * de cada parte del proceso. */
		void comprimir(char* buffer, unsigned long size);


		/* A partir del buffer obtenido, se genera el archivo descomprimido siguiendo los pasos y logica inversa del
		 * metodo comprimir. Todavia esta por definirse el tipo de datos del buffer que recibe este metodo (por eso
		 * no lo puse). */
		void descomprimir();
};

#endif
