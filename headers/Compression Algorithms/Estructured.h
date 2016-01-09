
#ifndef ESTRUCTURED_H
#define ESTRUCTURED_H

#include <list>
#include <string>
#include <iostream>
#include <cmath>
#include <bitset>
#include "constants.h"

#define INITIAL_OCURRENCIES 1
#define FREQUENCIES_LIMIT MAX_FREQUENCY

// de http://www.arturocampos.com/ac_arithmetic.html
using namespace std;

typedef struct _level level_t;


class Estructured
{
    public:
        Estructured();

        ~Estructured();

        pair<char *, unsigned int> compress(short *      input,
                                            unsigned int size);

        pair<unsigned short *, unsigned int> decompress(char *       indexes,
                                                        unsigned int size);

    protected:

        /*
         *  Devuelve la frecuencia piso del numero pasado por parametro.
         * En caso que el numero sea mayor al numero maximo del nivel, se devuelve la cantidad de
         * ocurrencias de dicho nivel, contemplando que el intervalo del numero maximo se encuentra
         * entre [techo(NumMax-1), cantTotalDeOcurrencias].
         * Dicha frecuencia va a ser un numero que entre en 16 bits gracias al metodo verifyFreqs
         */
        unsigned short accumulatedFreqTillNumber(level_t & level,
                                                 int       number);

        /*
         *  Incrementa la frecuencia del numero dentro del nivel, asi como tambien incrementa la cantidad de ocurrencias
         * de dicho nivel
         */
        void increaseFreq(level_t & level,
                          int      number);

        /*
         *  Caso especial que se produce en emitNumber cuando se llega a la compresion del EOF.
         * Realiza la logica de emision de los ultimos bits del archivo comprimido
         */
        void endCompression(unsigned short low);

        /*
         * Emite un ESC
         */
        void emitESC(int nivel);

        /*
         * Va almacenando en el atributo result, los bits que representan la probabilidad con la cual se comprime
         * el parametro nro, en el nro_nivel determinado.
         * La logica de procesamiento se basa en http://www.arturocampos.com/ac_arithmetic.html
         */
        void emitNumber(int nro_nivel,
                        int nro);    // Acordarse de quitar la variable i luego de debbugear

        /*
         * Emite un EOF.
         */
        void emitEOF();

        /*
         * Devuelve el simbolo representado por una tira de bits del archivo a decompress.
         * Se tiene en cuenta el proceso indicado en http://www.arturocampos.com/ac_arithmetic.html
         * Realiza modificaciones a los atributos code, low y high de acuerdo a los bits que va procesando
         */
        int obtainNumber(int level_number);

        /*
         * Emite un bit (0 o 1) para ser guardado en el archivo comprimido.
         */
        void emitBit(bool bit);

        /*
         * Lee el proximo bit del archivo a decompress
         */
        bool readBit();

        /*
         * Metodo util para controlar la precision en la cual se realizaran las operacionse
         * de compresion y descompresion usando aritmetica de enteros.
         * Para no precisar mas de 16 bits, el total de nuestras frecuencias deberian
         * ser inforiores a 16384 (2¹⁴). Si las probabilidades (ocurrencias) son mayores a este
         * limite, se las divide por un factor de 2 o 4.
         */
        void verifyFreqs(level_t & nivel);

        /*
         * Genera el result en un char* que sera escrito en el archivo comprimido
         */
        pair<char *, unsigned int> generateCompressionResult();

        /*
         * Setea variables que se utilizaran para inicializar el proceso de compresion
         */
        void prepareCompression();

        /*
         * Genera el result en un short* que sera utilizado por el SR para su descompresion
         */
        pair<unsigned short *, unsigned int> generateDecompressionResult();

        /*
         * Setea variables que se utilizaran para inicializar el proceso de descompresion
         */
        void prepareDecompression();

        /*
         *
         */
        void generateInput(char *       input,
                           unsigned int size);

        /*
         *
         */
        void flushByteBuffer();

    private:
        string                 inputStr;
        string                 result;
        list<unsigned short> * result_d;
        bitset<8>              byteBuffer;
        unsigned short         bitCounter;
        unsigned int           posInInputStr;
        unsigned short         low;
        unsigned short         high;
        unsigned short         underflow;
        unsigned short         value;
        level_t *              levels;
};
#endif // ESTRUCTURADO_H