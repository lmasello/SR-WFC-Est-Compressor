#ifndef SYMBOLRANKING_H
#define SYMBOLRANKING_H

class SymbolRanking{
	private:
		char tabla[]; //Con los valores ASCII
		int orden; //Orden maximo de contexto.

		/* Dado un orden de contexto y un char busca en el vector y devuelve
		* true or false segun si lo encontro o no y el numero de no ocurrencias.
		* EJ: mejor caso (true,0) peor caso (false, contextosQueSeComparan)
		*/
		void buscarEnContexto(int orden, char caracter);

		/* Realiza la busqueda del caracter para el caso de contexto = 1 */
		void buscarEnContextoUno(char caracter, unsigned long pos);

	public:
		SymbolRanking();

		SymbolRanking(int orden);

		/*Recibe un vector de char a comprimir. El resultado se almacenara en un vector de short (necesito mas de 256 caracteres
		 * por lo que no se puede guardar en un vector de char */
		void comprimir(char* aComprimir, short* salida, unsigned long size);
};

#endif


