
#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstddef>
#include "../Algoritmos de compresion/SymbolRanking.h"
#include "../Algoritmos de compresion/Estructurado.h"

#define ORDER 6
#define MAX_LEVELS 11
#define MAX_NUMBER 1024
using namespace std;

/* Compressor manages the logic about the overall compression and decompression of the input file. */
class Compressor
{
    private:
        SymbolRanking * sr;
        Estructurado *  estructured;

        /*
         * Calculates and informs the user of the entropy of the uncompressed file.
         */
        void calculateEntropy(char *       buffer,
                             unsigned int size);

        /*
         * Calculates and informs the user of the entropy of the file after the SymbolRanking compression.
         */
        void calculateEntropySROutput(short *      salida,
                                     unsigned int size);

    public:
        Compressor();

        ~Compressor();

        /*
         * From the buffer (the original file in memory), the compressed result is generated using the Symbol Ranking
         * method + WFC + Estructured. Manages the general logic of the process and makes the calls to the different
         * compression methods from each part of the process.
         * Begins with calling the compression from the SymbolRanking compressor, and then, uses that output as input
         * for the Estructured process.
         * Informs the user of the entropy of the input and final output files.
         */
        pair<char *, unsigned int> compress(char * buffer,
                unsigned int                        size);

        /*
         * From the buffer, the decompressed result is generated using the inverse logic as the compression process.
         * First the Estructured method is called to decompress the buffer obtained from the compressed file, and then,
         * its output is used as input for the Symbol Ranking compression method.
         * Informs the user of the entropy of the input and final output files.
         */
        pair<char *, unsigned int> decompress(char * entrada,
                unsigned int                           size);
};
#endif


//~ Formatted by Jindent --- http://www.jindent.com
