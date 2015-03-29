
#ifndef CONSTANTES_H_
#define CONSTANTES_H_

/*
 * /----------------------------------------------------------------------------------------&
 * / Description:
 * / File with constants and data types utilized in the compressor.
 * /----------------------------------------------------------------------------------------&
 */

/* ******************CONSTANTS FOR THE ESTRUCTURED MODEL ************************* */
#define Code_value_bits 16              // Amount of bits.
#define Top_value 65535         		// Top value of 2⁶.
#define First_qtr (Top_value/4+1)       // Size to the third quarter.
#define Half (2*First_qtr)              // Size to half.
#define Third_qtr (3*First_qtr)         // Size to the third quarter.

/* Max accumulated frequency */
#define Max_frequency 16383                     //2¹⁴-1

/* Data type */
typedef std::bitset<8> Byte;

#define LEVEL_AMOUNT 11
#define OCURRENCIAS_INICIAL 1
#define NRO_ESCAPE -1
#define NIVEL_INICIAL 0


#endif /* CONSTANTES_H_ */
