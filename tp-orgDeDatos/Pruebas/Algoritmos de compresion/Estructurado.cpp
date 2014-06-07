#include "Estructurado.h"

#include <cmath>

#define INICIO_SEGMENTO 0
#define FIN_SEGMENTO 1
#define CANT_NIVELES 9
#define OCURRENCIAS_INICIAL 1
#define NRO_ESCAPE -1
#define NRO_EOF 256
#define NIVEL_INICIAL 0

using std::list;
using std::string;

typedef struct _par{
    int numero;
    int ocurrencias;
} par_t;

struct _nivel{
    int total_ocurrencias;
    list<par_t*> cant_por_nro;
};

par_t* par_crear(int nro){
    par_t* par_nro = new par_t;
    par_nro->numero = nro;
    par_nro->ocurrencias = OCURRENCIAS_INICIAL;
    return par_nro;
}

nivel_t& nivel_crear(int nro_nivel){
    nivel_t* nivel = new nivel_t;

    nivel->cant_por_nro.push_back(par_crear(NRO_ESCAPE));
    int total_ocurrencias = 1;

    if (nro_nivel == 0){
        nivel->cant_por_nro.push_back(par_crear(0));
        total_ocurrencias++;
    } else {
        for (int i = 2^(nro_nivel - 1); i < (2^nro_nivel); i++){
            nivel->cant_por_nro.push_back(par_crear(i));
            total_ocurrencias++;
        }
    }
    nivel->total_ocurrencias = total_ocurrencias;
    return *nivel;
}

void nivel_destruir(nivel_t& nivel){
    nivel.cant_por_nro.clear();
    delete &nivel;
}

Estructurado::Estructurado(){
    inicio_segmento = INICIO_SEGMENTO;
    fin_segmento = FIN_SEGMENTO;
    niveles = new nivel_t[CANT_NIVELES];
    for (int i = 0; i < CANT_NIVELES; i++){
        nivel_t& nuevo = nivel_crear(i);
        niveles[i] = nuevo;
    }
}

Estructurado::~Estructurado(){
    for (int i = 0; i < CANT_NIVELES; i++)
        nivel_destruir(niveles[i]);
    delete[] niveles;
}

void Estructurado::emitirEscape(int nivel){
    emitirNro(nivel, NRO_ESCAPE);
}

void Estructurado::emitirNro(int nro_nivel, int nro){
    nivel_t& nivel = niveles[nro_nivel];
    int cant_parcial = 0;

    list<par_t*>::iterator it = nivel.cant_por_nro.begin();
    for (; (*it)->numero != nro; it++){
        cant_parcial += (*it)->ocurrencias;
    }
    int nuevo_seg_ini = (fin_segmento - inicio_segmento) * cant_parcial / nivel.total_ocurrencias + inicio_segmento;
    int nuevo_seg_fin = (fin_segmento - inicio_segmento) * (cant_parcial + (*it)->ocurrencias)/ nivel.total_ocurrencias + inicio_segmento;
    inicio_segmento = nuevo_seg_ini;
    fin_segmento = nuevo_seg_fin;
    (*it)->ocurrencias++;
    nivel.total_ocurrencias++;
}

void Estructurado::emitirEOF(){
    for (int i = 0; i < CANT_NIVELES; i++)
        emitirEscape(i);
}

double buscar_potencia2_rango(double inicio, double fin){
    double log_inicio = log2(inicio);
    if (log_inicio == int(log_inicio))
        return log_inicio;
    double suma = 0, proximo;
    int i = -1;
    while (suma < inicio){
        proximo = 2^i;
        if (suma + proximo < fin)
            suma += proximo;
        i--;
    }
    return suma;
}

double Estructurado::comprimir(short* salida, unsigned int size){
    for (unsigned int i = 0; i < size; i++){
        int indice = salida[i], nivel_indice = int(log2(indice)) + 1;
        int nivel_act = NIVEL_INICIAL;
        for (; nivel_act < nivel_indice; nivel_act++){
            emitirEscape(nivel_act);
        }
        emitirNro(nivel_act, indice);
    }
    emitirEOF();
    return buscar_potencia2_rango(inicio_segmento, fin_segmento);
}

int Estructurado::obtenerNro(int nro_nivel, double nro_comprimido){
    nivel_t& nivel = niveles[nro_nivel];
    int cant_parcial = 0;
    int nuevo_seg_ini = (fin_segmento - inicio_segmento) * cant_parcial / nivel.total_ocurrencias + inicio_segmento;

    list<par_t*>::iterator it = nivel.cant_por_nro.begin();
    for (; nuevo_seg_ini <= nro_comprimido; it++){
        cant_parcial += (*it)->ocurrencias;
        nuevo_seg_ini = (fin_segmento - inicio_segmento) * cant_parcial / nivel.total_ocurrencias + inicio_segmento;
    }
    it--;
    cant_parcial += (*it)->ocurrencias;
    nuevo_seg_ini = (fin_segmento - inicio_segmento) * cant_parcial / nivel.total_ocurrencias + inicio_segmento;
    int nuevo_seg_fin = (fin_segmento - inicio_segmento) * (cant_parcial + (*it)->ocurrencias)/ nivel.total_ocurrencias + inicio_segmento;
    inicio_segmento = nuevo_seg_ini;
    fin_segmento = nuevo_seg_fin;
    (*it)->ocurrencias++;
    nivel.total_ocurrencias++;
    return (*it)->numero;
}

string& Estructurado::descomprimir(double nro_comprimido){
    string& indices = *(new string);
    int emitido = NRO_ESCAPE;
    while(true){
        int nivel_act = NIVEL_INICIAL;
        for (; emitido != NRO_ESCAPE; nivel_act++){
            emitido = obtenerNro(nivel_act, nro_comprimido);
        }
        if ((nivel_act == CANT_NIVELES -1) && (emitido == NRO_ESCAPE)) break;
        indices += emitido;
    }
    return indices;
}
