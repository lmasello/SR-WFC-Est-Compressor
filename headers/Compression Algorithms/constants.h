#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/*
 * /----------------------------------------------------------------------------------------&
 * / Description:
 * / File with constants and data types utilized in the compressor.
 * /----------------------------------------------------------------------------------------&
 */

/* ******************CONSTANTS FOR THE ESTRUCTURED MODEL ************************* */
#define Top_value 65535                // Top value of 2⁶.
#define First_qtr (Top_value/4+1)       // Size to the third quarter.
#define Half (2*First_qtr)              // Size to half.
#define Third_qtr (3*First_qtr)         // Size to the third quarter.

/* Max accumulated frequency */
#define MAX_FREQUENCY 16383                     //2¹⁴-1

/* Data type */
typedef std::bitset<8> Byte;

#define LEVEL_AMOUNT 11
#define NRO_ESCAPE -1
#define NIVEL_INICIAL 0


#endif /* CONSTANTS_H_ */
