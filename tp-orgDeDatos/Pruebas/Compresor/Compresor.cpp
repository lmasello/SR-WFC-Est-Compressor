#include "Compresor.h"
#include "../Algoritmos de compresion/SymbolRanking.h"

Compresor::Compresor(){}

void Compresor::comprimir(char* buffer, unsigned int size){

	float entropia = 0;
	float Pi;
	int contador = 0;
	int fniveles[10];
	int fcaracteres[255];

	for(unsigned int i = 0; i<256;i++) fcaracteres[i] = 0;

	for(unsigned int j = 0; j < size; j++) fcaracteres[(int)buffer[j]]++;

	entropia = 0;
	Pi = 0;
	for(int i = 0; i < 255; i++){
		if(fcaracteres[i] == 0) continue;
		Pi = fcaracteres[i]/(float)size;
		float elLog = (-(log(Pi)/log(2)));
		entropia += Pi*elLog;
	}

	cout << "Ahora voy a calcular la entropia: H = ";
	cout << entropia << endl;

	SymbolRanking sr (ORDEN);

	short *salida = (short*) malloc (sizeof(short) * size);

	cout<<"Comenzando proceso de compresion por SymbolRanking de orden: " << ORDEN << endl;

	sr.comprimir(buffer, salida, size);

	cout<<"SymbolRanking ha finalizado el proceso de compresion correctamente" << endl << "Salida:" <<endl;
	//for(unsigned int i = 0; i < size; i++)cout<<salida[i]<<' ';
//	cout<<endl;


	for(int i = 0; i < 10; i++) fniveles[i] = 0;
	for(int i = 0; i < 256; i++) fcaracteres[i] = 0;

	for(unsigned int i = 0; i < size; i++){

		fcaracteres[(unsigned char) salida[i]]++;

		if     ((unsigned char) salida[i] == 0)  fniveles[0]++;
		else if((unsigned char) salida[i] == 1)  fniveles[1]++;
		else if((unsigned char) salida[i] < 4)   fniveles[2]++;
		else if((unsigned char) salida[i] < 8)   fniveles[3]++;
		else if((unsigned char) salida[i] < 16)  fniveles[4]++;
		else if((unsigned char) salida[i] < 32)  fniveles[5]++;
		else if((unsigned char) salida[i] < 64)  fniveles[6]++;
		else if((unsigned char) salida[i] < 128) fniveles[7]++;
		else if((unsigned char) salida[i] < 256) fniveles[8]++;
	}

	contador = 0;
	for(int i = 0; i<10; i++){
		cout << "Nivel" << i << " - " << fniveles[i] << endl;
		contador += fniveles[i];
	}
	entropia = 0;
	Pi = 0;
	for(int i = 0; i < 255; i++){
		if(fcaracteres[i] == 0) continue;
		Pi = fcaracteres[i]/(float)contador;
		float elLog = (-(log(Pi)/log(2)));
		entropia += Pi*elLog;
	}

	cout << "Ahora voy a calcular la entropia: H = ";
	cout << entropia;
}

void Compresor::descomprimir(unsigned short* entrada, unsigned int size){
//Deberia ser una sucesion de pasos inversa a la de comprimir
	SymbolRanking sr(ORDEN);

	char *salida = (char*) malloc (sizeof(char) * size);

	cout << "Comenzando el proceso de descompresion por SymbolRanking" << endl;

	sr.descomprimir(entrada, salida, size);

	cout << "SymbolRanking ha finalizado el proceso de descompresion correctamente"<<endl;
	cout << "Salida:"<<endl;
	for(unsigned int i = 0; i < size; i++){
		cout<< salida[i] << ' ';
	}

}
