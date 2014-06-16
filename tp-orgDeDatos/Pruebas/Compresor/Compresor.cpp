#include "Compresor.h"

Compresor::Compresor(){
	sr = new SymbolRanking(ORDEN);
	estructurado = new Estructurado();
}

Compresor::~Compresor(){
	delete sr;
	delete estructurado;
}

pair<char*, unsigned int> Compresor::comprimir(char* buffer, unsigned int size){

	calculoEntropia(buffer,size);

	short* salida = new short[size];
	if (salida == NULL) {fputs ("Memory error",stderr); exit (2);}

	cout<<"Comenzando proceso de compresion por SymbolRanking de orden: " << ORDEN << endl;

	//Comprimimos con SR.
	sr->comprimir(buffer, salida, size);

	cout<<"SymbolRanking ha finalizado el proceso de compresion correctamente" << endl;

	calculoEntropiaSalidaSR(salida,size);

	cout << "Comenzando el proceso de compresion por Estructurado" << endl;

	//Comprimimos con Estructurado y retornamos.
	return estructurado->comprimir(salida, size);
}

pair<char*, unsigned int> Compresor::descomprimir(char* entrada, unsigned int size){

	cout << "Comenzando el proceso de descompresion por Estructurado" << endl;

	//Descomprimimos con Estructurado
	pair<unsigned short*, unsigned int> aDescomprimir = estructurado->descomprimir(entrada, size);
	cout<<"Estructurado ha finalizado el proceso de descompresion correctamente" << endl;

	char* salida = new char[aDescomprimir.second];
	if (salida == NULL) {fputs ("Memory error",stderr); exit (2);}

	calculoEntropiaSalidaSR((short*) aDescomprimir.first, aDescomprimir.second);

	cout << "Comenzando el proceso de descompresion por SymbolRanking" << endl;
	//Descomprimimos con SR.

	sr->descomprimir(aDescomprimir.first, salida, aDescomprimir.second);
	cout << "SymbolRanking ha finalizado el proceso de descompresion correctamente"<<endl;

	pair<char*, unsigned int> parSalida (salida, aDescomprimir.second);
	return parSalida;
}

void Compresor::calculoEntropia(char* buffer,unsigned int size){
	float entropia = 0;
	float Pi;
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
	cout << "Entropia de entrada: H = " << entropia << endl;
}
void Compresor::calculoEntropiaSalidaSR(short* salida,unsigned int size){
	float entropia = 0;
	float Pi = 0;  //Probabilidad de i.
	int fcaracteres[512];
	for(int i = 0; i < 512; i++) fcaracteres[i] = 0;

	for(unsigned int i = 0; i < size; i++)
		fcaracteres[(unsigned char) salida[i]]++;

	for(int i = 0; i < 512; i++){
		if(fcaracteres[i] == 0) continue;
		Pi = fcaracteres[i]/(float)size;
		float elLog = (-(log(Pi)/log(2)));
		entropia += Pi*elLog;
	}
	cout << "Entropia de salida: H = " << entropia << endl;
}
