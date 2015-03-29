
#ifndef WFC_H_
#define WFC_H_

#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include "../Exceptions/IndexError.h"

using namespace std;

class WFC
{
    private:
        char                                weightedList[256];
        unordered_map<char, unsigned short> charFrequencies;

        /*
         * Returns the position of some element in the weightedList.
         */
        unsigned short getIndex(char element);

        /*
         * Reordena la weithedList en base a las frecuencias. EL criterio de ordenamiento
         * es en forma de frecuencias decrecientes, quedando ubicados aquellos char mas
         * frecuentes al comienzo de la lista. Se pasa por parametro el indice del ultimo char actualizado
         * en base al cual se debe actualizar la lista.
         * Precondiciones:
         * -indexDelCharActualizado debe ser un numero entre 0 y 255. Si es 0, no corresponden actualizaciones de la
         * lista ya que el mismo es el mayor
         */
        void updateWeightedList(unsigned short newIndex);

        /*
         * Metodo que evalua si el peso del caracter que se encuentra en pos 1 es mayor estricto al peso del
         * caracter que se encuentra en la pos 2
         * Precondiciones:
         *  - pos1 y pos2 deben ser menores a 256 y mayores a 0
         */
        bool isWeighthigher(unsigned short pos1,
                            unsigned short pos2);

    public:
        WFC();

        /*
         * Recibe un char y devuelve su posicion en la weithedList.
         * Precondiciones:
         *  - char debe encontrarse en la weithedList
         */
        unsigned short compress(char charToTransform);

        /*
         * Recibe un numero y devuelve el char correspondiente al caracter de
         * weithedList[numberToTransform]
         * Precondiciones:
         *  - numberToTransform debe ser un numero entre 0 y weithedList.size()
         */
        char decompress(unsigned short numberToTransform);

        /*
         * Increases the frequency of the char in the frequency list.
         */
        void increaseFreq(char charToIncrement);
};
#endif /* WFC_H_ */
