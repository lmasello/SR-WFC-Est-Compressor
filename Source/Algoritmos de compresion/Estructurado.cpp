
#include "Estructurado.h"

typedef struct _pair
{
    int numero;
    int ocurrencias;
} pair_t;


struct _level
{
    unsigned long int total_ocurrencies;
    list<pair_t *>     amout_per_number;
    unsigned short    numeroMaximoDelNivel;
};


pair_t * pair_create(int number)
{
    pair_t * pair = new pair_t;

    pair -> numero      = number;
    pair -> ocurrencias = OCURRENCIAS_INICIAL;

    return pair;
}

void pair_destroy(pair_t * par)
{
}

level_t level_create(int level_number)
{
    level_t level;

    level.amout_per_number.push_back(pair_create(NRO_ESCAPE));

    unsigned long int total_ocurrencies = 1;

    if (level_number == 0)
    {
        level.amout_per_number.push_back(pair_create(0));

        level.numeroMaximoDelNivel = 0;

        total_ocurrencies++;
    }
    else
    {
        for (int i = pow(2, level_number - 1); i < pow(2, level_number); i++)
        {
            level.amout_per_number.push_back(pair_create(i));

            level.numeroMaximoDelNivel = pow(2, level_number) - 1;

            total_ocurrencies++;
        }
    }

    level.total_ocurrencies = total_ocurrencies;

    return level;
}

void level_destroy(level_t * level)
{
    list<pair_t *>::iterator iter;

    iter = level -> amout_per_number.begin();

    while (iter != level -> amout_per_number.end())
    {
        pair_destroy(*iter);

        delete *iter;

        iter++;
    }

    level -> amout_per_number.clear();
}

Estructurado::Estructurado()
{
    value           = 0;
    posEnStrEntrada = 0;
    high            = 0xffff;    // 16 bits
    low             = 0x0000;    // 16 bits
    underflow       = 0;
    contadorBits_   = 0;
    resultado_d     = nullptr;
    niveles         = new level_t[CANT_NIVELES];

    for (int i = 0; i < CANT_NIVELES; i++)
    {
        level_t nuevo = level_create(i);

        niveles[i] = nuevo;
    }
}

Estructurado::~Estructurado()
{
    for (int i = 0; i < CANT_NIVELES; i++)
    {
        level_destroy(&(niveles[i]));
    }

    delete[] niveles;
    delete resultado_d;
}

void Estructurado::prepararCompresion()
{
    high      = 0xffff;    // 16 bits
    low       = 0x0000;    // 16 bits
    underflow = 0;
}

pair<char *, unsigned int> Estructurado::comprimir(short *      input,
												   unsigned int size)
{
    prepararCompresion();

    for (unsigned int i = 0; i < size; i++)
    {
        int            level_index;
        unsigned short numberToCompress = input[i];

        if (numberToCompress == 0)
        {
            level_index = 0;
        }
        else
        {
            level_index = int(log2(numberToCompress)) + 1;
        }

        int act_level = NIVEL_INICIAL;

        for (; act_level < level_index; act_level++)
        {
            emitirEscape(act_level);
        }

        emitirNro(act_level, numberToCompress);
    }

    emitirEOF();

    return generar_resultado_c();
}

void Estructurado::emitirNro(int level_number,
                             int nro)
{
    level_t & level = niveles[level_number];

    // Evaluamos el numero siguiente al numero a comprimir
    int next_number;

    if ((level_number <= 2) && (nro == NRO_ESCAPE))
    {
        next_number = level_number;
    }
    else
    {
        next_number = (nro == NRO_ESCAPE) ? (pow(2, level_number - 1)) : nro + 1;
    }

    // Calculamos las frecuencias
    unsigned short numberToCompressTopFrequency = frecuenciaAcumuladaHastaElNumero(level, next_number);
    unsigned short numberToCompressLowFrequency = frecuenciaAcumuladaHastaElNumero(level, nro);
    unsigned short totalFrequency               = level.total_ocurrencies;

    // Calculamos los valores de frontera
    unsigned int range = (high - low) + 1;

    high = low + ((range * numberToCompressTopFrequency) / totalFrequency) - 1;
    low  = low + ((range * numberToCompressLowFrequency) / totalFrequency);

    // Caso de fin de archivo
    if ((level_number == (CANT_NIVELES - 1)) && (nro == NRO_ESCAPE))
    {
        finalizarCompresion(low);
    }

    for (;;)
    {
        if (high < Half)
        {
            emitirBit(0);

            while (underflow > 0)
            {
                emitirBit(1);
                underflow--;
            }
        }
        else if (low >= Half)
        {
            emitirBit(1);

            while (underflow > 0)
            {
                emitirBit(0);
                underflow--;
            }

            low  -= Half;
            high -= Half;
        }
        else if ((low >= First_qtr) && (high < Third_qtr))
        {
            underflow++;

            low  -= First_qtr;
            high -= First_qtr;
        }
        else
        {
            break;
        }

        low  = 2 * low;
        high = 2 * high + 1;
    }

    // Actualizar las frecuencias
    incrementarFrecuencias(level, nro);
}

void Estructurado::emitirEscape(int level)
{
    emitirNro(level, NRO_ESCAPE);
}

void Estructurado::emitirEOF()
{
    for (int i = 0; i < CANT_NIVELES; i++)
    {
        emitirEscape(i);
    }
}

void Estructurado::emitirBit(bool bit)
{
    contadorBits_++;

    // Almacenamos el nuevo bit en el byteBuffer
    byteBuffer[8 - contadorBits_] = (bit) ? 1 : 0;

    // En caso de completar un byte entero, lo guardamos en el archivo
    if (contadorBits_ == 8)
    {
        contadorBits_ = 0;

        unsigned long i            = byteBuffer.to_ulong();
        unsigned char byteToSave = static_cast<unsigned char>(i);

        resultado.push_back(byteToSave);
    }
}

void Estructurado::finalizarCompresion(unsigned short low)
{
    // Emite msb de Low
    bool msbOfLow = ((low >> 15) != 0);

    emitirBit(msbOfLow);

    // Emite underflows
    while (underflow > 0)
    {
        emitirBit(~msbOfLow);
        underflow--;
    }

    // Emite los demas bits del low
    for (short shift = 14; shift < 0; shift--)
    {
        unsigned short mask = (2 ^ (shift + 1)) - 1;
        bool           bit  = (bool) (low & mask) >> shift;

        emitirBit(bit);
    }
}

pair<char *, unsigned int> Estructurado::generar_resultado_c()
{
    flushByteBuffer();

    size_t size    = resultado.length();
    char * output = new char[size];

    for (unsigned int i = 0; i < size; i++)
    {
        char charToSave = resultado[i];

        output[i] = charToSave;
    }

    pair<char *, unsigned int> pair(output, resultado.length());

    return pair;
}

void Estructurado::flushByteBuffer()
{
    while (contadorBits_ != 0)
    {
        contadorBits_++;

        // Seteamos en 0 todos los bits no utilizados del byte
        byteBuffer[8 - contadorBits_] = 0;

        // En caso de completar un byte entero, lo guardamos en el archivo
        if (contadorBits_ == 8)
        {
            contadorBits_ = 0;

            unsigned long i            = byteBuffer.to_ulong();
            unsigned char byteToSave = static_cast<unsigned char>(i);

            resultado.push_back(byteToSave);
        }
    }
}

void Estructurado::prepararDescompresion()
{
    high        = 0xffff;    // 16 bits
    low         = 0x0000;    // 16 bits
    value       = 0;
    resultado_d = new list<unsigned short>;

    // Leemos los primeros 16bits del archivo
    for (register unsigned short i = 0; i < 16; i++)
    {
        value = value * 2 + leerBit();
    }
}

void Estructurado::generarEntrada(char *       input,
                                  unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            unsigned char actual = input[i];

            actual     <<= j;
            actual     >>= 7;
            strEntrada += (int) actual;
        }
    }
}

pair<unsigned short *, unsigned int> Estructurado::descomprimir(char * input,
        														unsigned int size)
{
    generarEntrada(input, size);
    prepararDescompresion();

    int a = 0;

    while (true)
    {
        int act_level = NIVEL_INICIAL;
        int emitted   = NRO_ESCAPE;

        while (emitted == NRO_ESCAPE)
        {
            emitted = obtenerNro(act_level);

            if ((act_level == CANT_NIVELES - 1) && (emitted == NRO_ESCAPE))
            {
                break;
            }

            act_level++;
        }

        if ((act_level == CANT_NIVELES - 1) && (emitted == NRO_ESCAPE))
        {
            break;
        }

        a++;
        resultado_d -> push_back(emitted);
    }

    return generar_resultado_d();
}

int Estructurado::obtenerNro(int level_number)
{
    level_t & level = niveles[level_number];

    // Vemos las frecuencias del nivel
    unsigned short totalFrequency = level.total_ocurrencies;

    // Nos fijamos en donde cae el numero
    unsigned int range = (high - low) + 1;
    int          temp  = ((value - low + 1) * totalFrequency - 1) / range;

    // Vemos a que symbol corresponde temp
    // Simbolo es al que apunta temp. Es decir temp se encontrara entre [frecPisoDelSimbolo,frecTechoDelSimbolo)
    int            symbol;
    unsigned short symbolTopFrequency = 0;
    unsigned short symbolLowFrequency = 0;
    unsigned short numbreTopFrequency = 0;
    int            next_number;

    for (int numbreToEvaluate = NRO_ESCAPE; numbreToEvaluate <= level.numeroMaximoDelNivel; numbreToEvaluate = next_number)
    {
        // Logica para obtener el numero siguiente al numbreToEvaluate
        if ((level_number <= 2) && (numbreToEvaluate == NRO_ESCAPE))
        {
            next_number = level_number;
        }
        else
        {
            next_number = (numbreToEvaluate == NRO_ESCAPE) ? (pow(2, level_number - 1)) : numbreToEvaluate + 1;
        }

        // temp esta en el rango del numbreToEvaluate?
        numbreTopFrequency = frecuenciaAcumuladaHastaElNumero(level, next_number);

        if (numbreTopFrequency > temp)
        {
            symbol                   = numbreToEvaluate;
            symbolTopFrequency = numbreTopFrequency;
            symbolLowFrequency  = frecuenciaAcumuladaHastaElNumero(level, numbreToEvaluate);

            break;
        }
    }

    // Recalcula valores de frontera. Misma formula que en el compresor
    high = low + ((range * symbolTopFrequency) / totalFrequency) - 1;
    low  = low + (range * symbolLowFrequency) / totalFrequency;

    // Itera hasta que no hayan mas symbols posibles por decodificar
    for (;;)
    {
        if (high < Half)
        {
            // Nothing                       //Expand low half
        }
        else if (low >= Half)
        {                                 // Expand high half
            value -= Half;
            low   -= Half;                // Subtract offset to top
            high  -= Half;
        }
        else if ((low >= First_qtr) && (high < Third_qtr))
        {                                 // Expand middle half
            value -= First_qtr;
            low   -= First_qtr;           // Subtract offset to middle
            high  -= First_qtr;
        }
        else
        {
            break;                        // Otherwise exit loop
        }

        low   = 2 * low;
        high  = 2 * high + 1;             // Scale up code range
        value = 2 * value + leerBit();    // Move in next input bit
    }

    // Actualizar las frecuencias
    incrementarFrecuencias(level, symbol);

    return symbol;
}

bool Estructurado::leerBit()
{
    posEnStrEntrada++;

    return (bool) strEntrada[posEnStrEntrada - 1];
}

unsigned short Estructurado::frecuenciaAcumuladaHastaElNumero(level_t & level,
        													  int       number)
{
	// Con el verificarFrecuencias, nos aseguramos que no tendremos frecuencias mayores a 16 bits
    unsigned short numbreLowFrequency = 0;

    // Caso de que se pida la frec del max numero del level
    if (number == (level.numeroMaximoDelNivel + 1))
    {
        return level.total_ocurrencies;
    }

    // Caso generico
    list<pair_t *>::iterator it = level.amout_per_number.begin();

    for (; (*it) -> numero != number; it++)
    {
        numbreLowFrequency += (*it) -> ocurrencias;
    }

    return numbreLowFrequency;
}

void Estructurado::incrementarFrecuencias(level_t & level,
										  int       number)
{
    // Incrementa las ocurrencias totales del level
    level.total_ocurrencies++;

    // Incrementa la frecuencia de number en el level determinado
    list<pair_t *>::iterator it = level.amout_per_number.begin();

    for (; (*it) -> numero != number; it++);    // Hace que el iterator se posicione en la posicion del nro

    (*it) -> ocurrencias++;
    verificarFrecuencias(level);
}

void Estructurado::verificarFrecuencias(level_t & level)
{
    // Si no se supera el limite de frecuencias, salimos del metodo
    if (level.total_ocurrencies < LIMITE_FRECUENCIAS)
    {
        return;
    }

    unsigned short totalFrequencies = 0;

    // Si se supera el limite, procedemos a normalizar las frecuencias del level, reduciendolas a la mitad
    list<pair_t *>::iterator numeroDelNivel = level.amout_per_number.begin();

    for (; numeroDelNivel != level.amout_per_number.end(); numeroDelNivel++)
    {
        (*numeroDelNivel) -> ocurrencias /= 2;

        // Si el numero normalizado queda en cero, lo seteamos a 1
        if ((*numeroDelNivel) -> ocurrencias == 0)
        {
            (*numeroDelNivel) -> ocurrencias = 1;
        }

        totalFrequencies += (*numeroDelNivel) -> ocurrencias;
    }

    // Actualiza las frecuencias totales del level
    level.total_ocurrencies = totalFrequencies;
}

pair<unsigned short *, unsigned int> Estructurado::generar_resultado_d()
{
    unsigned short * output = new unsigned short[resultado_d -> size()];
    int i = 0;

    for(auto result : *resultado_d)
    {
    	output[i] = result;
    	i++;
    }
    pair<unsigned short *, unsigned int> par(output, resultado_d -> size());

    return par;
}
