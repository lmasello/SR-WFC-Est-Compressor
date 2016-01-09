#ifndef CONSTANTS_H_
#define CONSTANTS_H_


/* ******************CONSTANTS FOR THE ESTRUCTURED MODEL ************************* */
#define TOP_VALUE 65535         		// Top value of 2⁶.
#define FIRST_QTR (TOP_VALUE / 4 + 1)       // Size to the third quarter.
#define HALF (2 * FIRST_QTR)              // Size to half.
#define THIRD_QTR (3 * FIRST_QTR)         // Size to the third quarter.

/* Max accumulated frequency */
#define MAX_FREQUENCY 16383                     //2¹⁴-1

#define LEVEL_AMOUNT 11
#define ESCAPE_NUM -1
#define INITIAL_LEVEL 0

#endif /* CONSTANTS_H_ */