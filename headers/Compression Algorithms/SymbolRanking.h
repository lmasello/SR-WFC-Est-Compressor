#ifndef SYMBOLRANKING_H
#define SYMBOLRANKING_H

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <list>
#include <tuple>
#include "WFC.h"
#include "HashMap.h"
#include "../Exceptions/ParameterError.h"

using namespace std;
#define SEARCH_LIMIT 204800

class SymbolRanking {
    private:
        WFC wfc;
        list<char> exclusionList;
        unsigned int maxOrder;
        unsigned int currentOrder;

        /*
         * It's values are the lists with positions of each key in the buffer.
         */
        HashMap hashMap;

        /*
         * TODO: translate and update
         * Evalua si hay un match entre una posicion del buffer y el contexto actual, dado un orden determinado.
         * En caso de que haya match de contextos. Evalua si la oferta realizada es positiva. EJ: mejor caso (true,0)
         * peor caso (false, contextosQueSeComparan). Sigue el esquema de hashing del Tech paper 132 de P. Fenwich
         * Precondiciones:
         *  - orden: Indica el orden del contexto que se debe buscar el match.
         *  - posCharToRank: Es la posicion actual en el buffer que se esta procesando. Debe ser un unsigned int
         *  - buffer: Contiene los datos que han sido procesados desde la posicion 0 hasta posCharToRank
         *  - operacion: Indica si la busqueda del contexto se esta realizando para comprimir ('c') o decompress('d')
         *               Se debe ingresar 'c' o 'd'. En caso contrario se levanta la excepcion ErrorDeParametro
         *  - ranking: es unicamente utilizado para el descompresor e indica el ranking del char a decompress.
         * Postcondiciones:
         *  -tuple[0] es un bool que indica si la oferta fue positiva (true) o no (false)
         *  -tuple[1]: Si tuple[0]==true y la operacion es 'c' --> indica la cantidad de ofertas negativas que se
         *  realizaron hasta lograr la oferta positiva.
         *  -tuple[1]: Si tuple[0]==true y la operacion es 'd' --> es el caracter ofrecido casteado a unsigned short
         * Si tuple[0]==false, indica la cantidad de ofertas negativas.
         */
        pair<bool, unsigned short> seekInContext(unsigned int posCharToRank,
                                                 char *buffer,
                                                 char operation,
                                                 unsigned short ranking);

        /*
         * TODO: translate and update
         * Evalua si hay un match de orden 1 y en dicho caso, evalua si la oferta realizada por dicho contexto
         * es positiva.
         * Precondiciones:
         *  - posCharToRank: Es la posicion actual en el buffer que se esta procesando. Debe ser un unsigned int
         *  - buffer: Contiene los datos que han sido procesados desde la posicion 0 hasta posCharToRank
         *  - operacion: Indica si la busqueda del contexto se esta realizando para comprimir ('c') o decompress('d')
         *               Se debe ingresar 'c' o 'd'. En caso contrario se levanta la excepcion ErrorDeParametro
         *  - ranking: es unicamente utilizado para el descompresor e indica el ranking del char a decompress
         * Postcondiciones:
         *  -tuple[0] es un bool que indica si la oferta fue positiva (true) o no (false)
         *  -tuple[1]: Si tuple[0]==true y la operacion es 'c' --> indica la cantidad de ofertas negativas que se
         *  realizaron hasta lograr la oferta positiva.
         *  -tuple[1]: Si tuple[0]==true y la operacion es 'd' --> es el caracter ofrecido casteado a unsigned short
         * Si tuple[0]==false, indica la cantidad de ofertas negativas.
         */
        pair<bool, unsigned short> linearSearchInContext(unsigned int posCharToRank,
                                                         char *buffer,
                                                         char operation,
                                                         unsigned short ranking);

        /*
         * Returns the corresponding list to the key conformed by the string between the character in the
         * posFirst position from the buffer, and the next one.
         */
        list<unsigned int> *getListOfPositions(char *buffer,
                                               unsigned int posFirst);

        /*
         * Given an array with 3 chars, it hashes it in the hashmap.
         */
        void hash(unsigned char symbol1,
                  unsigned char symbol2,
                  unsigned char symbol3,
                  unsigned int indexFirstChar);

        /*
         * Given two buffer offsets, N bytes following those offsets are compared (N = currentOrder)
         * If said bytes are the same in both segments, both contexts are equal.
         * Preconditions:
         *  -indexA and indexB must be valid offsets within the buffer, where indexA < indexB
         * Postconditions:
         *  A boolean is returned indicating whether there was a match.
         */
        bool sameContext(unsigned int indexA,
                         unsigned int indexB,
                         char *buffer);

        /*
         * Checks if the actual context has the same hashing as the context with which the comparisson
         * is to be made.
         */
        bool sameHashing(unsigned int hashing1,
                         unsigned int pos,
                         char *buffer);

        /*
         * Given an offset from an array, verifies if the char poited by that position is the character to compare.
         * Preconditions:
         *  - index must ve a valid index from the buffer
         * Postconditions:
         *  Returns a boolean representing if there's a match between the characters involved.
         */
        bool sameChars(unsigned int index,
                       char charToCompare,
                       char *buffer);

        /*
         * Searches if the character offered is in the exclusion list.
         * Postconditions:
         *  Returns True if the char is in the list.
         */
        bool charExcluded(char charToFind);

    public:

        SymbolRanking(unsigned short ctxOrder);

        ~SymbolRanking();

        /*
         * Receives an array to compress.The result is stored in a short array (more than 256
         * characters are needed so a char is of no use.
         */
        void compress(char *toCompress,
                      short *output,
                      unsigned int size);

        /*
         * Receives a short array and generates a char array that represents the chars decompressed.
         */
        void decompress(unsigned short *toDecompress,
                        char *output,
                        unsigned int size);
};

#endif
