
#include "Compressor.h"

Compressor::Compressor()
{
    sr          = new SymbolRanking(ORDER);
    estructured = new Estructurado();
}

Compressor::~Compressor()
{
    delete sr;
    delete estructured;
}

pair<char *, unsigned int> Compressor::compress(char * buffer,
        unsigned int                                   size)
{
	calculateEntropy(buffer, size);

    short * output = new short[size];

    if (output == NULL)
    {
        fputs("Memory error", stderr);
        exit(2);
    }

    cout << "Beginning the process of compression with SymbolRanking of order " << ORDER << endl;

    // The compress stage with SR method begins.
    sr -> compress(buffer, output, size);

    cout << "SymbolRanking has ended the compression process succesfully." << endl;

    calculateEntropySROutput(output, size);

    cout << "Beginning the process of decompression with Estructured." << endl;

    // The compress stage with Estructured method  begins.
    pair<char *, unsigned int> result = estructured -> comprimir(output, size);

    delete[] output;

    return result;
}

pair<char *, unsigned int> Compressor::decompress(char * input,
        unsigned int                                      size)
{
    cout << "Beginning the process of decompression with Estructured." << endl;

    // The decompress stage with Estructured method begins.
    pair<unsigned short *, unsigned int> toDecompress = estructured -> descomprimir(input, size);

    cout << "The Estructured method has ended succesfully." << endl;

    char * output = new char[toDecompress.second];

    if (output == nullptr)
    {
        fputs("Memory error", stderr);
        exit(2);
    }

    calculateEntropySROutput((short *) toDecompress.first, toDecompress.second);

    cout << "Beginning the process of decompression with SymbolRanking." << endl;

    // The decompress stage with SR method begins.
    sr -> descomprimir(toDecompress.first, output, toDecompress.second);

    delete[] toDecompress.first;

    pair<char *, unsigned int> parSalida(output, toDecompress.second);

    return parSalida;
}

void Compressor::calculateEntropy(char *       buffer,
                                unsigned int size)
{
    float entropy = 0;
    float Pi      = 0;    // Probability of i.
    int   fcharacters[256];

    for (int i = 0; i < 256; i++)
    {
        fcharacters[i] = 0;
    }

    for (unsigned int i = 0; i < size; i++)
    {
        fcharacters[(unsigned char) buffer[i]]++;
    }

    for (int i = 0; i < 256; i++)
    {
        if (fcharacters[i] == 0)
        {
            continue;
        }

        Pi = fcharacters[i] / (float) size;

        float elLog = (-(log(Pi) / log(2)));

        entropy += Pi * elLog;
    }

    cout << "Input entropy: H = " << entropy << endl;
}

void Compressor::calculateEntropySROutput(short * output,
        unsigned int                            size)
{
    float entropy = 0;
    float Pi;
    int   fcharacters[MAX_NUMBER];
    int   flevels[MAX_LEVELS];

    for (int i = 0; i < MAX_LEVELS; i++)
    {
        flevels[i] = 0;
    }

    for (int i = 0; i < MAX_NUMBER; i++)
    {
        fcharacters[i] = 0;
    }

    for (unsigned int i = 0; i < size; i++)
    {
        fcharacters[output[i]]++;

        if (output[i] == 0)
        {
            flevels[0]++;
        }
        else if (output[i] == 1)
        {
            flevels[1]++;
        }
        else if (output[i] < 4)
        {
            flevels[2]++;
        }
        else if (output[i] < 8)
        {
            flevels[3]++;
        }
        else if (output[i] < 16)
        {
            flevels[4]++;
        }
        else if (output[i] < 32)
        {
            flevels[5]++;
        }
        else if (output[i] < 64)
        {
            flevels[6]++;
        }
        else if (output[i] < 128)
        {
            flevels[7]++;
        }
        else if (output[i] < 256)
        {
            flevels[8]++;
        }
        else if (output[i] < 512)
        {
            flevels[9]++;
        }
        else
        {
            flevels[10]++;
        }
    }

    int counter = 0;

    for (int i = 0; i < MAX_LEVELS; i++)
    {
        cout << "Level" << i << " - " << flevels[i] << endl;

        counter += flevels[i];
    }

    entropy = 0;
    Pi      = 0;

    for (int i = 0; i < MAX_NUMBER; i++)
    {
        if (fcharacters[i] == 0)
        {
            continue;
        }

        Pi = fcharacters[i] / (float) counter;

        float elLog = (-(log(Pi) / log(2)));

        entropy += Pi * elLog;
    }

    cout << "Output entropy: H = " << entropy << endl;
}
