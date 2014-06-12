#ifndef ESTRUCTURADO_H
#define ESTRUCTURADO_H

#include <list>
#include <string>
#include <iostream>
#include <cmath>

#define CANT_NIVELES 10
#define OCURRENCIAS_INICIAL 1
#define NRO_ESCAPE -1
#define NRO_EOF 256
#define NIVEL_INICIAL 0
#define LIMITE_FRECUENCIAS 16384 // 2 ^14

using namespace std;

typedef struct _nivel nivel_t;

class Estructurado {
    public:
        Estructurado();
        virtual ~Estructurado();
        pair<char*, unsigned int> comprimir(short* aComprimir, unsigned int size);
        pair<unsigned short*, unsigned int> descomprimir(char* indices, unsigned int size);
    protected:
        /* Devuelve la frecuencia piso del numero pasado por parametro.
         * En caso que el numero sea mayor al numero maximo del nivel, se devuelve la cantidad de ocurrencias
         * de dicho nivel, contemplando que el intervalo del numero maximo se encuentra entre [techo(NumMax-1),cantTotalDeOcurrencias].
         * DIcha frecuencia va a ser un numero que entre en 16 bits gracias al metodo verificarFrecuencias
         *  */
        unsigned short frecuenciaAcumuladaHastaElNumero(nivel_t& nivel,int nro_nivel,int nro, int i);

        /* Incrementa la frecuencia del numero dentro del nivel, asi como tambien incrementa la cantidad de ocurrencias
         * de dicho nivel */
        void incrementarFrecuencias(nivel_t& nivel, int nro);

        /* Caso especial que se produce en emitirNro cuando se llega a la compresion del EOF.
         * Realiza la logica de emision de los ultimos bits del archivo comprimido */
        void finalizarCompresion(unsigned short low);

        void emitirEscape(int nivel, int i);

        void emitirNro(int nro_nivel, int nro, int i);

        void emitirEOF(int i);

        void obtenerNro(int nro_nivel);

        void emitirBit(bool bit);

        bool leerBit();

        /*
         * Metodo util para controlar la precision en la cual se realizaran las operacionse
         * de compresion y descompresion usando aritmetica de enteros.
         * Para no precisar mas de 16 bits, el total de nuestras frecuencias deberian
         * ser inforiores a 16384 (2¹⁴). Si las probabilidades (ocurrencias) son mayores a este
         * limite, se las divide por un factor de 2 o 4.
         */
        void verificarFrecuencias(nivel_t& nivel);

        pair<char*, unsigned int> generar_resultado();

        void prepararDescompresion();

        void generarEntrada(char* entrada, unsigned int size);
    private:
        string* strEntrada;
        string* resultado;
    	unsigned int posEnStrEntrada;
        unsigned short low;
        unsigned short high;
        unsigned short underflow;
        unsigned short code;
        nivel_t* niveles;
};

#endif // ESTRUCTURADO_H
