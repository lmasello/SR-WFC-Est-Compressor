#ifndef COMPRESOR_H
#define COMPRESOR_H

/*Compresor tendra la logica sobre la compresion y descompresion del archivo de
 * entrada. */
class Compresor{
	private:

	public:
		Compresor();
		Compresor(char vector[]);
		void comprimir();
		void descomprimir();
};

#endif
