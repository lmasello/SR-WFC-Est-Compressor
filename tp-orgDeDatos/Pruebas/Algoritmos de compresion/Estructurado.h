#ifndef ESTRUCTURADO_H
#define ESTRUCTURADO_H

#include <list>
#include <string>
#include <utility>

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
         * de dicho nivel, contemplando que el intervalo del numero maximo se encuentra entre [techo(NumMax-1),cantTotalDeOcurrencias] */
        int frecuenciaAcumuladaHastaElNumero(nivel_t& nivel,int nro_nivel,int nro, int i);

        /* Incrementa la frecuencia del numero dentro del nivel, asi como tambien incrementa la cantidad de ocurrencias
         * de dicho nivel */
        void incrementarFrecuencias(nivel_t& nivel, int nro);

        /* Caso especial que se produce en emitirNro cuando se llega a la compresion del EOF.
         * Realiza la logica de emision de los ultimos bits del archivo comprimido */
        void finalizarCompresion(unsigned short low);

        void emitirEscape(int nivel, int i);

        void emitirNro(int nro_nivel, int nro, int i);

        void emitirEOF(int i);

        int obtenerNro(int nro_nivel, double nro_comprimido, int i);

        void emitirBit(bool bit);

        pair<char*, unsigned int> generar_resultado();
    private:
        string* resultado;
        unsigned short low;
        unsigned short high;
        unsigned short underflow;
        nivel_t* niveles;
};

#endif // ESTRUCTURADO_H
