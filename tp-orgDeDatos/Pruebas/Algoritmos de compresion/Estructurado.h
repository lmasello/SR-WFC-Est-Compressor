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
    protected:
        void emitirEscape();
        void emitirNro(int nro);
    private:
        double inicio_segmento;
        double fin_segmento;
        nivel_t* niveles;
};

#endif // ESTRUCTURADO_H
