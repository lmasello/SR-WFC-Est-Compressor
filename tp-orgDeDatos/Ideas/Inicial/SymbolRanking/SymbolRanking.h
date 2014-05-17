#ifndef SYMBOLRANKING_H
#define SYMBOLRANKING_H

class SymbolRanking{
	private:
		char tabla[]; //Con los valores ASCII
		void buscarEnContexto(unsigned char orden, char caracter);
	public:
		SymbolRanking();
		SymbolRanking(unsigned char orden);
		void comprimir(char aComprimir[], int salida[]);
};

#endif

 
