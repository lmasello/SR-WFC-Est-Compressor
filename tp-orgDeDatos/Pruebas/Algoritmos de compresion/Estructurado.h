#ifndef ESTRUCTURADO_H
#define ESTRUCTURADO_H

#include <list>
#include <string>
#include <iostream>
#include <cmath>
#include <bitset>
#include "constantes.h"
using namespace std;

typedef struct _nivel nivel_t;

class Estructurado {
    public:
        Estructurado();
        virtual ~Estructurado();
        pair<char*, unsigned int> comprimir(short* aComprimir, unsigned int size);
        pair<unsigned short*, unsigned int> descomprimir(char* entrada, unsigned int size);
    protected:
        /* Devuelve la frecuencia piso del numero pasado por parametro.
         * En caso que el numero sea mayor al numero maximo del nivel, se devuelve la cantidad de ocurrencias
         * de dicho nivel, contemplando que el intervalo del numero maximo se encuentra entre [techo(NumMax-1),cantTotalDeOcurrencias].
         * DIcha frecuencia va a ser un numero que entre en 16 bits gracias al metodo verificarFrecuencias
         *  */
        unsigned short frecuenciaAcumuladaHastaElNumero(nivel_t& nivel,short nro_nivel,short nro);

        /* Incrementa la frecuencia del numero dentro del nivel, asi como tambien incrementa la cantidad de ocurrencias
         * de dicho nivel */
        void incrementarFrecuencias(nivel_t& nivel, int nro);

        /* Caso especial que se produce en emitirNro cuando se llega a la compresion del EOF.
         * Realiza la logica de emision de los ultimos bits del archivo comprimido */
        void finalizarCompresion(unsigned short low);

        void emitirEscape(int nivel, int i);

        /*
         * Va almacenando en el atributo resultado, los bits que representan la probabilidad con la cual se comprime el parametro nro, en el
         * nro_nivel determinado.
         * La logica de procesamiento se basa en http://www.arturocampos.com/ac_arithmetic.html
         */
        void emitirNro(int nro_nivel, int nro, int i); //Acordarse de quitar la variable i luego de debbugear

        void emitirEOF(int i);

        /*
         * Devuelve el simbolo representado por una tira de bits del archivo a descomprimir.
         * Se tiene en cuenta el proceso indicado en http://www.arturocampos.com/ac_arithmetic.html
         * Realiza modificaciones a los atributos code, low y high de acuerdo a los bits que va procesando
         */
        short decodeSymbol(int nro_nivel);

        void emitirBit(bool bit);

        /*
         * Lee el proximo bit del archivo a descomprimir
         */
        bool leerBit();

        /*
         * Metodo util para controlar la precision en la cual se realizaran las operacionse
         * de compresion y descompresion usando aritmetica de enteros.
         * Para no precisar mas de 16 bits, el total de nuestras frecuencias deberian
         * ser inforiores a 16384 (2¹⁴). Si las probabilidades (ocurrencias) son mayores a este
         * limite, se las divide por un factor de 2 o 4.
         */
        void verificarFrecuencias(nivel_t& nivel);

        pair<char*, unsigned int> generar_resultado_c();

        /*
         * Setea variables que se utilizaran para inicializar el proceso de compresion
         */
        void prepararCompresion();

        pair<unsigned short*, unsigned int> generar_resultado_d();

		/*
         * Setea variables que se utilizaran para inicializar el proceso de descompresion
         */
        void start_decoding();

        void generarEntrada(char* entrada, unsigned int size);

        void flushByteBuffer();

        short getNumeroSiguiente(short numeroAEvaluar,int nro_nivel);
    private:
        string* strEntrada;
        string* resultado;
        bitset<8>	byteBuffer;
        unsigned short contadorBits_;
    	unsigned int posEnStrEntrada;
        unsigned short underflow;
        nivel_t* niveles;

        /***************** ENCODING AND DECODING **********************/
        code_value value;			//Currently-seen code value
        code_value low;
        code_value high;				//Ends of current code region
};

#endif // ESTRUCTURADO_H
