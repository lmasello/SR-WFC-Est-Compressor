
#include "Compresor.h"

Compresor::Compresor()
{
    sr           = new SymbolRanking(ORDEN);
    estructurado = new Estructurado();
}

Compresor::~Compresor()
{
    delete sr;
    delete estructurado;
}

pair<char *, unsigned int> Compresor::comprimir(char * buffer,
        unsigned int                                   size)
{
    calculoEntropia(buffer, size);

    short * salida = new short[size];

    if (salida == NULL)
    {
        fputs("Memory error", stderr);
        exit(2);
    }

    cout << "Comenzando proceso de compresion por SymbolRanking de orden: " << ORDEN << endl;

    // Comprimimos con SR.
    sr -> comprimir(buffer, salida, size);

    cout << "SymbolRanking ha finalizado el proceso de compresion correctamente" << endl;

    calculoEntropiaSalidaSR(salida, size);

    cout << "Comenzando el proceso de compresion por Estructurado" << endl;

    // Comprimimos con Estructurado y retornamos.
    pair<char *, unsigned int> resultado = estructurado -> comprimir(salida, size);

    delete[] salida;

    return resultado;
}

pair<char *, unsigned int> Compresor::descomprimir(char * entrada,
        unsigned int                                      size)
{
    cout << "Comenzando el proceso de descompresion por Estructurado" << endl;

    // Descomprimimos con Estructurado
    pair<unsigned short *, unsigned int> aDescomprimir = estructurado -> descomprimir(entrada, size);

    cout << "Estructurado ha finalizado el proceso de descompresion correctamente" << endl;

    char * salida = new char[aDescomprimir.second];

    if (salida == nullptr)
    {
        fputs("Memory error", stderr);
        exit(2);
    }

    calculoEntropiaSalidaSR((short *) aDescomprimir.first, aDescomprimir.second);

    cout << "Comenzando el proceso de descompresion por SymbolRanking" << endl;

    // Descomprimimos con SR.
    sr -> descomprimir(aDescomprimir.first, salida, aDescomprimir.second);

    delete[] aDescomprimir.first;

    pair<char *, unsigned int> parSalida(salida, aDescomprimir.second);

    return parSalida;
}

void Compresor::calculoEntropia(char *       buffer,
                                unsigned int size)
{
    float entropia = 0;
    float Pi       = 0;    // Probabilidad de i.
    int   fcaracteres[256];

    for (int i = 0; i < 256; i++)
    {
        fcaracteres[i] = 0;
    }

    for (unsigned int i = 0; i < size; i++)
    {
        fcaracteres[(unsigned char) buffer[i]]++;
    }

    for (int i = 0; i < 256; i++)
    {
        if (fcaracteres[i] == 0)
        {
            continue;
        }

        Pi = fcaracteres[i] / (float) size;

        float elLog = (-(log(Pi) / log(2)));

        entropia += Pi * elLog;
    }

    cout << "Entropia de entrada: H = " << entropia << endl;
}

void Compresor::calculoEntropiaSalidaSR(short * salida,
        unsigned int                            size)
{
    float entropia = 0;
    float Pi;
    int   fcaracteres[MAX_NUMBER];
    int   fniveles[MAX_LEVELS];

    for (int i = 0; i < MAX_LEVELS; i++)
    {
        fniveles[i] = 0;
    }

    for (int i = 0; i < MAX_NUMBER; i++)
    {
        fcaracteres[i] = 0;
    }

    for (unsigned int i = 0; i < size; i++)
    {
        fcaracteres[salida[i]]++;

        if (salida[i] == 0)
        {
            fniveles[0]++;
        }
        else if (salida[i] == 1)
        {
            fniveles[1]++;
        }
        else if (salida[i] < 4)
        {
            fniveles[2]++;
        }
        else if (salida[i] < 8)
        {
            fniveles[3]++;
        }
        else if (salida[i] < 16)
        {
            fniveles[4]++;
        }
        else if (salida[i] < 32)
        {
            fniveles[5]++;
        }
        else if (salida[i] < 64)
        {
            fniveles[6]++;
        }
        else if (salida[i] < 128)
        {
            fniveles[7]++;
        }
        else if (salida[i] < 256)
        {
            fniveles[8]++;
        }
        else if (salida[i] < 512)
        {
            fniveles[9]++;
        }
        else
        {
            fniveles[10]++;
        }
    }

    int contador = 0;

    for (int i = 0; i < MAX_LEVELS; i++)
    {
        cout << "Nivel" << i << " - " << fniveles[i] << endl;

        contador += fniveles[i];
    }

    entropia = 0;
    Pi       = 0;

    for (int i = 0; i < MAX_NUMBER; i++)
    {
        if (fcaracteres[i] == 0)
        {
            continue;
        }

        Pi = fcaracteres[i] / (float) contador;

        float elLog = (-(log(Pi) / log(2)));

        entropia += Pi * elLog;
    }

    cout << "Entropia de salida: H = " << entropia << endl;
}
