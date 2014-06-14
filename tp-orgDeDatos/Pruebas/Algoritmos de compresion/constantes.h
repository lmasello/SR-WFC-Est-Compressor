#ifndef CONSTANTES_H_
#define CONSTANTES_H_

/**
//----------------------------------------------------------------------------------------&
// Descripcion:
// Archivo con constantes y tipos de datos utilizados en el compresor
//----------------------------------------------------------------------------------------&
*/

/*******************CONSTANTES PARA MODELO ESTRUCTURADO **************************/
/*SIZE OF ARITHMETIC CODE VALUES*/
#define Code_value_bits 16		//Number of bits in a code value
typedef long code_value;		//Type of an arithmetic code value
#define Top_value (((long)1<<Code_value_bits)-1)	//Largest code value

/* HALF AND QUARTER POINTS IN THE CODE VALUE RANGE */
#define First_qtr (Top_value/4+1)	//Point after first quarter
#define Half (2*First_qtr)			//Point after first half
#define Third_qtr (3*First_qtr)		//Point after third quarter

/* CUMULATIVE FREQUENCY */
#define Max_frequency 16383			//Maximun allowed frequency count. 2^14-1

/*  Tipos de datos  */
typedef std::bitset<8> Byte;

/* Variables del estructurado */
#define CANT_NIVELES 10
#define OCURRENCIAS_INICIAL 1
#define NRO_ESCAPE -1
#define NRO_EOF 256
#define NIVEL_INICIAL 0



/*  Constantes para el FileManager    */


#endif /* CONSTANTES_H_ */
