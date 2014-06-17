#ifndef CONSTANTES_H_
#define CONSTANTES_H_

/**
//----------------------------------------------------------------------------------------&
// Descripcion:
// Archivo con constantes y tipos de datos utilizados en el compresor
//----------------------------------------------------------------------------------------&
*/

/*******************CONSTANTES PARA MODELO ESTRUCTURADO **************************/

#define Code_value_bits 16		//Cantidad de bits
#define Top_value 65535     	//Maximo valor 2⁶

#define First_qtr (Top_value/4+1)	//Tamaño hasta el primer cuarto
#define Half (2*First_qtr)			//Tamaño hasta la mitad
#define Third_qtr (3*First_qtr)		//Tamaño hasta el tercer cuarto

/* Maxima frecuencia acumulada */
#define Max_frequency 16383			//2¹⁴-1

/*  Tipos de datos  */
typedef std::bitset<8> Byte;

/* Variables del estructurado */
#define CANT_NIVELES 11
#define OCURRENCIAS_INICIAL 1
#define NRO_ESCAPE -1
#define NIVEL_INICIAL 0


#endif /* CONSTANTES_H_ */
