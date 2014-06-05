#ifndef ESTRUCTURADO_H
#define ESTRUCTURADO_H

#include <list>
#include <string>

typedef struct _nivel nivel_t;

class Estructurado {
    public:
        Estructurado();
        virtual ~Estructurado();
        double comprimir(std::string &indices);
        std::string& descomprimir(double nro_comprimido);
    protected:
        void emitirEscape(int nivel);
        void emitirNro(int nro_nivel, int nro);
        void emitirEOF();
        int obtenerNro(int nro_nivel, double nro_comprimido);
    private:
        double inicio_segmento;
        double fin_segmento;
        nivel_t* niveles;
};

#endif // ESTRUCTURADO_H
