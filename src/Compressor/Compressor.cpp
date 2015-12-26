#include <c++/cmath>
#include "../../headers/Compressor/Compressor.h"

Compressor::Compressor() {
    // Don't use an ORDER < 4 for reasons
    sr = new SymbolRanking(ORDER);
    estructured = new Estructured();
}

Compressor::~Compressor() {
    delete sr;
    delete estructured;
}

pair<char *, unsigned int> Compressor::compress(char *buffer,
                                                unsigned int size) {

    short *output = new short[size];

    cout << "Beginning the process of compression with SymbolRanking of order " << ORDER << endl;

    // The compress stage with SR method begins.
    sr->compress(buffer, output, size);

    cout << "SymbolRanking has ended the compression process succesfully." << endl;

    cout << "Beginning the process of decompression with Estructured." << endl;

    // The compress stage with Estructured method  begins.
    pair<char *, unsigned int> result = estructured->compress(output, size);

    delete[] output;

    return result;
}

pair<char *, unsigned int> Compressor::decompress(char *input,
                                                  unsigned int size) {
    cout << "Beginning the process of decompression with Estructured." << endl;

    // The decompress stage with Estructured method begins.
    pair<unsigned short *, unsigned int> toDecompress = estructured->decompress(input, size);

    cout << "The Estructured method has ended succesfully." << endl;

    char *output = new char[toDecompress.second];

    cout << "Beginning the process of decompression with SymbolRanking." << endl;

    // The decompress stage with SR method begins.
    sr->decompress(toDecompress.first, output, toDecompress.second);

    delete[] toDecompress.first;

    pair<char *, unsigned int> outputPair(output, toDecompress.second);

    return outputPair;
}

void Compressor::calculateEntropy(char *buffer,
                                  unsigned int size) {
    double entropy = 0;
    double Pi = 0;    // Probability of i.
    int fCharacters[256];

    for (int i = 0; i < 256; i++) fCharacters[i] = 0;

    for (unsigned int i = 0; i < size; i++) fCharacters[(unsigned char) buffer[i]]++;

    for (int i = 0; i < 256; i++) {
        if (fCharacters[i] == 0) continue;

        Pi = fCharacters[i] / (float) size;

        double iLog = - log2(Pi);

        entropy += Pi * iLog;
    }

    cout << "Input entropy: H = " << entropy << endl;
}

void Compressor::calculateEntropySROutput(short *output,
                                          unsigned int size) {
    double entropy = 0;
    double Pi = 0;
    int fCharacters[MAX_NUMBER];
    int fLevels[MAX_LEVELS];

    for (int i = 0; i < MAX_LEVELS; i++) fLevels[i] = 0;

    for (int i = 0; i < MAX_NUMBER; i++) fCharacters[i] = 0;

    for (unsigned int i = 0; i < size; i++) {
        fCharacters[output[i]]++;

        if (output[i] == 0) fLevels[0]++;

        else if (output[i] == 1) fLevels[1]++;

        else fLevels[(int)log2(output[i])]++;
    }

    int counter = 0;

    for (int i = 0; i < MAX_LEVELS; i++) {
        cout << "Level" << i << " - " << fLevels[i] << endl;
        counter += fLevels[i];
    }

    for (int i = 0; i < MAX_NUMBER; i++) {
        if (fCharacters[i] == 0) continue;

        Pi = fCharacters[i] / (float) counter;

        double iLog = (-(log(Pi) / log(2)));

        entropy += Pi * iLog;
    }

    cout << "Output entropy: H = " << entropy << endl;
}
