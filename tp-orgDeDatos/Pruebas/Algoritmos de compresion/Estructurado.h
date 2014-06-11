#ifndef ESTRUCTURADO_H
#define ESTRUCTURADO_H

#include <iostream>
#include <cmath>
#include <list>
#include <string>

#define CANT_NIVELES 9
#define OCURRENCIAS_INICIAL 1
#define NRO_ESCAPE -1
#define NRO_EOF 256
#define NIVEL_INICIAL 0

using std::list;
using std::string;

using namespace std;

typedef struct _nivel nivel_t;

class Estructurado {
    public:
        Estructurado();
        virtual ~Estructurado();
        //Documentar!!!
        void comprimir(short* aComprimir, unsigned int size);
        string& descomprimir(double nro_comprimido);
    protected:
        void emitirEscape(int nivel);
        void emitirNro(int nro_nivel, int nro);
        void emitirEOF();
        /*
         * Devuelve la frecuencia piso del numero pasado por parametro.
         * En caso que el numero sea mayor al numero maximo del nivel, se devuelve la cantidad de ocurrencias
         * de dicho nivel, contemplando que el intervalo del numero maximo se encuentra entre [techo(NumMax-1),cantTotalDeOcurrencias]
         */
        int frecuenciaAcumuladaHastaElNumero(nivel_t& nivel,int nro_nivel,int nro);
        /*
         * Incrementa la frecuencia del numero dentro del nivel, asi como tambien incrementa la cantidad de ocurrencias
         * de dicho nivel
         */
        void incrementarFrecuencias(nivel_t& nivel, int nro);
        /*
         * Caso especial que se produce en emitirNro cuando se llega a la compresion del EOF.
         * Realiza la logica de emision de los ultimos bits del archivo comprimido
         */
        void finalizarCompresion(unsigned short low);
        void emitirBit(bool bitAEmitir);

//      int obtenerNro(int nro_nivel, double nro_comprimido);
    private:
        unsigned short low;
        unsigned short high;
        unsigned short underflow;
        nivel_t* niveles;
};

#endif // ESTRUCTURADO_H
