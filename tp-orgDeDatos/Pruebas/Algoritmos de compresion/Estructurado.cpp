#include "Estructurado.h"

typedef struct _par{
    int numero;
    int ocurrencias;
} par_t;

struct _nivel{
    unsigned long int total_ocurrencias;
    list<par_t*> cant_por_nro;
    unsigned short numeroMaximoDelNivel;
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
    unsigned long int total_ocurrencias = 1;

    if (nro_nivel == 0){
        nivel->cant_por_nro.push_back(par_crear(0));
        nivel->numeroMaximoDelNivel = 0;
        total_ocurrencias++;
    } else {
        for (int i = pow(2, nro_nivel-1); i < pow(2, nro_nivel); i++){
            nivel->cant_por_nro.push_back(par_crear(i));
            nivel->numeroMaximoDelNivel = pow(2, nro_nivel)-1;
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
	code = 0;
	posEnStrEntrada = 0;
	resultado = new string;
	strEntrada = new string; //Se usa solo para la descompresion.
    high = 0xffff; //16 bits
    low = 0x0000;  //16 bits
    underflow = 0;

    niveles = new nivel_t[CANT_NIVELES];
    for (int i = 0; i < CANT_NIVELES; i++){
        nivel_t& nuevo = nivel_crear(i);
        niveles[i] = nuevo;
    }
}

Estructurado::~Estructurado(){
//    for (int i = 0; i < CANT_NIVELES; i++)
//        nivel_destruir(niveles[i]);
    delete[] niveles;
}

pair<unsigned short*, unsigned int> Estructurado::descomprimir(char* entrada, unsigned int size){
	pair <unsigned short*, unsigned int> par;
	generarEntrada(entrada, size);
	prepararDescompresion();
	while(true){
		int nivel_act = NIVEL_INICIAL;
		int emitido = NRO_ESCAPE;
		while(emitido == NRO_ESCAPE){
			emitido = obtenerNro(nivel_act);
			nivel_act++;
			if ((nivel_act == CANT_NIVELES -1) && (emitido == NRO_ESCAPE)) break;
		}
		if ((nivel_act == CANT_NIVELES -1) && (emitido == NRO_ESCAPE)) break;
		*resultado += emitido;
	}
	par = generar_resultado_d();
    return par;
}

pair<char*, unsigned int> Estructurado::comprimir(short* aComprimir, unsigned int size){
	prepararCompresion();

    for (unsigned int i = 0; i < size; i++){
        int nivel_indice;
        unsigned short numeroAComprimir = aComprimir[i];

        if(numeroAComprimir == 0) nivel_indice = 0;
        else nivel_indice = int(log2(numeroAComprimir)) + 1;

        int nivel_act = NIVEL_INICIAL;
        for (; nivel_act < nivel_indice; nivel_act++){
            emitirEscape(nivel_act, i);
        }
        emitirNro(nivel_act, numeroAComprimir, i);
    }
    emitirEOF(0);
    return generar_resultado_c();
}

void Estructurado::emitirEscape(int nivel, int i){
    emitirNro(nivel, NRO_ESCAPE, i);
}

void Estructurado::emitirEOF(int j){
    for (int i = 0; i < CANT_NIVELES; i++)
        emitirEscape(i, j);
}

pair<char*, unsigned int> Estructurado::generar_resultado_c(){
	size_t tam = resultado->length();
	char* salida = new char[tam/8];
	for (unsigned int i=0; i<tam; ++i){
		char aGuardar = 0x00;
		for(int j = 0; j<8; j++, i++){
			if(i == tam) break;
			char actual = (*resultado)[i];
			aGuardar += (*resultado)[i];
			aGuardar <<= 1;
		}
		i--;
		salida[i/8] = aGuardar;
	}
	pair <char*, unsigned int> par (salida, resultado->length()/8);
	return par;
}

pair<unsigned short*, unsigned int> Estructurado::generar_resultado_d(){
	unsigned short* salida = new unsigned short[resultado->length()];
	for(unsigned int i = 0; i < resultado->length(); i++){
		salida[i] = (*resultado)[i];
	}
	pair <unsigned short*, unsigned int> par (salida, resultado->length());
	return par;
}

void Estructurado::emitirNro(int nro_nivel, int nro, int i){
    nivel_t& nivel = niveles[nro_nivel];
    //Evaluamos el numero siguiente al numero a comprimir
    int nro_sig;
    if ((nro_nivel <= 2)&&(nro == NRO_ESCAPE)) nro_sig = nro_nivel;
    else nro_sig = (nro == NRO_ESCAPE) ? (pow(2, nro_nivel-1)) : nro+1;

    //Calculamos las frecuencias
    unsigned short frecuenciaTechoDelNumeroAComprimir = frecuenciaAcumuladaHastaElNumero(nivel,nro_nivel,nro_sig, i);
    unsigned short frecuenciaPisoDelNumeroAComprimir = frecuenciaAcumuladaHastaElNumero(nivel,nro_nivel,nro, i);
    unsigned short frecuenciaTotal = nivel.total_ocurrencias;

    //Calculamos los valores de frontera
    unsigned int range = (high-low) + 1;
    high = low + ((range*frecuenciaTechoDelNumeroAComprimir)/frecuenciaTotal)-1;
    low = low + ((range*frecuenciaPisoDelNumeroAComprimir)/frecuenciaTotal);

    //Caso de fin de archivo
    if ((nro_nivel == (CANT_NIVELES-1)) && (nro == NRO_ESCAPE)){
        finalizarCompresion(low);
    }

    //Ciclo hasta que no hayan mas bits para que sean emitidos o para aplicar shifts
    while(true){
        bool msbOfHigh = ((high >> 15) != 0);   //Efectua la comparacion para convertir el short en un bool
        bool msbOfLow = ((low >> 15) != 0);
        bool secondMsbOfHigh = (((high & 0x4000) >> 14) != 0);
        bool secondMsbOfLow = (((low & 0x4000) >> 14) != 0);

        if (msbOfHigh == msbOfLow){
            emitirBit(msbOfLow);
            while(underflow>0){
                emitirBit(~msbOfLow);
                underflow--;
            }
        }
        // Contemplar Underflow
        else if((secondMsbOfLow==1)&&(secondMsbOfHigh==0)){
            underflow++;
            low = low & 0x3fff;
            high = high | 0x4000;
        }
        // Si no hay msb iguales ni tampoco Underflow, sale del ciclo
        else break;
        // Realiza los shift finales
        high = (high << 1) | 0x0001;
        low = low << 1;
    }
    //Actualizar las frecuencias
    incrementarFrecuencias(nivel,nro);
}

void Estructurado::emitirBit(bool bit){
	resultado->push_back(bit);
}

int Estructurado::obtenerNro(int nro_nivel){
    nivel_t& nivel = niveles[nro_nivel];
    //Vemos las frecuencias del nivel
    unsigned short frecuenciaTotal = nivel.total_ocurrencias;

    //Nos fijamos en donde cae el numero
    unsigned int range = (high - low) + 1;
	unsigned short temp =(((code - low)+ 1) * frecuenciaTotal-1)/range;

    //Vemos a que simbolo corresponde temp
	int simbolo;
    unsigned short frecuenciaTechoDelSimbolo; //Simbolo es al que apunta temp. Es decir temp se encontrara entre [frecPisoDelSimbolo,frecTechoDelSimbolo)
    unsigned short frecuenciaPisoDelSimbolo;
    for (short numeroAEvaluar = NRO_ESCAPE; numeroAEvaluar<=nivel.numeroMaximoDelNivel; numeroAEvaluar++){
        //Logica para obtener el numero siguiente al numeroAEvaluar
        short nro_sig;
        if ((nro_nivel <= 2)&&(numeroAEvaluar == NRO_ESCAPE)) nro_sig = nro_nivel;
        else nro_sig = (numeroAEvaluar == NRO_ESCAPE) ? (pow(2, nro_nivel-1)) : numeroAEvaluar+1;

        // temp esta en el rango del numeroAEvaluar?
        unsigned short frecuenciaTechoDelNumero = frecuenciaAcumuladaHastaElNumero(nivel,nro_nivel,nro_sig, posEnStrEntrada); //Luego de debbugear quitar posEnStrEntrada
        if (frecuenciaTechoDelNumero > temp) {
        	simbolo = numeroAEvaluar;
            frecuenciaTechoDelSimbolo = frecuenciaTechoDelNumero;
            frecuenciaPisoDelSimbolo = frecuenciaAcumuladaHastaElNumero(nivel,nro_nivel,numeroAEvaluar, posEnStrEntrada);
            break;
        }
    }
    //Recalcula valores de frontera. Misma formula que en el compresor
    high = low + ((range * frecuenciaTechoDelSimbolo) / frecuenciaTotal) - 1;
    low = low + (range * frecuenciaPisoDelSimbolo)/frecuenciaTotal;

    //Itera hasta que no hayan mas simbolos posibles por decodificar
    while(true){
        bool msbOfHigh = ((high >> 15) != 0);   //Efectua la comparacion para convertir el short en un bool
        bool msbOfLow = ((low >> 15) != 0);
        bool secondMsbOfHigh = (((high & 0x4000) >> 14) != 0);
        bool secondMsbOfLow = (((low & 0x4000) >> 14) != 0);

        if (msbOfHigh != msbOfLow){
            if((secondMsbOfLow==1)&&(secondMsbOfHigh==0)){
                code = code ^ 0x4000;
                low = low & 0x3fff;
                high = high | 0x4000;
            }
            else break;
        }
        //Shifts
        low = low << 1;
        high = (high << 1) | 0x0001;
        bool nextBit = leerBit();
        code = (code << 1) | nextBit;
    }
    //Actualizar las frecuencias
    incrementarFrecuencias(nivel,simbolo);
    return simbolo;
}

bool Estructurado::leerBit(){
	posEnStrEntrada++;
	return (bool)(*strEntrada)[posEnStrEntrada-1];
}

unsigned short Estructurado::frecuenciaAcumuladaHastaElNumero(nivel_t& nivel,int nro_nivel,int nro, int i){
    unsigned short frecuenciaPisoDelNumero=0; //Con el verificarFrecuencias, nos aseguramos que no tendremos frecuencias mayores a 16 bits

    //Caso de que se pida la frec del max numero del nivel
    if(nro == (nivel.numeroMaximoDelNivel + 1)) return nivel.total_ocurrencias;

    //Caso generico
    list<par_t*>::iterator it = nivel.cant_por_nro.begin();
    for (; (*it)->numero != nro; it++){
        frecuenciaPisoDelNumero += (*it)->ocurrencias;
    }
    return frecuenciaPisoDelNumero;
}

void Estructurado::incrementarFrecuencias(nivel_t& nivel, int nro){
    // Incrementa las ocurrencias totales del nivel
    nivel.total_ocurrencias++;

    // Incrementa la frecuencia de nro en el nivel determinado
    list<par_t*>::iterator it = nivel.cant_por_nro.begin();
    for (; (*it)->numero != nro; it++); // Hace que el iterator se posicione en la posicion del nro
    (*it)->ocurrencias++;
    verificarFrecuencias(nivel);
}

void Estructurado::finalizarCompresion(unsigned short low){
    //Emite msb de Low
    bool msbOfLow = ((low >> 15) != 0);
    emitirBit(msbOfLow);

    //Emite underflows
    while(underflow>0){
        emitirBit(~msbOfLow);
        underflow--;
    }

    //Emite los demas bits del low
    for(short shift = 14;shift<0;shift--){
        unsigned short mask = (2^(shift+1))-1;
        bool bit = (bool) (low & mask) >> shift;
        emitirBit(bit);
    }
}

void Estructurado::verificarFrecuencias(nivel_t& nivel){

	//Si no se supera el limite de frecuencias, salimos del metodo
	if (nivel.total_ocurrencias < LIMITE_FRECUENCIAS) return;

	unsigned short frecuenciasTotales=0;

	//Si se supera el limite, procedemos a normalizar las frecuencias del nivel, reduciendolas a la mitad
    list<par_t*>::iterator numeroDelNivel = nivel.cant_por_nro.begin();
    for (; numeroDelNivel != nivel.cant_por_nro.end(); numeroDelNivel++){

    	(*numeroDelNivel)->ocurrencias/=2;
    	//SI el numero normalizado queda en cero, lo seteamos a 1
    	if ((*numeroDelNivel)->ocurrencias == 0) (*numeroDelNivel)->ocurrencias = 1;

    	frecuenciasTotales+= (*numeroDelNivel)->ocurrencias;
    }

	//Actualiza las frecuencias totales del nivel
	nivel.total_ocurrencias = frecuenciasTotales;
}

void Estructurado::generarEntrada(char* entrada, unsigned int size){
	for(unsigned int i = 0; i < size; i++){
		for(int j = 0; j<8; j++){
			unsigned char actual = entrada[i];
			actual <<= j;
			actual >>= 7;
			*strEntrada += (int) actual;
		}
	}
}

void Estructurado::prepararCompresion(){
    high = 0xffff; //16 bits
    low = 0x0000;  //16 bits
    underflow = 0;
}

void Estructurado::prepararDescompresion(){
    high = 0xffff; //16 bits
    low = 0x0000;  //16 bits
    code = 0;

    //Leemos los primeros 16bits del archivo
    for(register unsigned short i = 0; i<16; i++)
    	code = (code << 1) + leerBit();
}
