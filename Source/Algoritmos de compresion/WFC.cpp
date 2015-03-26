
#include "WFC.h"

WFC::WFC()
{
    for (unsigned short i = 0; i < 256; i++)
    {
        weightedList[i]           = ((char) i);
        charFrequencies[(char) i] = (unsigned short) 1;    // Initialices the frecuencies to 1.
    }
}

unsigned short WFC::comprimir(char charToTransform)
{
    unsigned short index = getIndex(charToTransform);

    incrementarFrecuencia(charToTransform);
    updateWeightedList(index);

    return index;
}

unsigned short WFC::getIndex(char charToTransform)
{
    unsigned short index = 0;
    unsigned short size = 256;

    for (unsigned short i = 0; i < size; i++)
    {
        if (weightedList[i] == charToTransform)
        {
            index = i;

            break;
        }
    }

    return index;
}

void WFC::incrementarFrecuencia(char charToTransform)
{
    unsigned short newIndex = getIndex(charToTransform);

    charFrequencies[charToTransform] += 1;

    updateWeightedList(newIndex);
}

void WFC::updateWeightedList(unsigned short newIndex)
{
    if (newIndex >= 256)
    {
        throw IndexError();
    }

    while (newIndex > 0)
    {
        bool pesoMayor = elPesoDeLaPosicionEsMayor(newIndex, newIndex - 1);

        if (pesoMayor)
        {
            char temp = weightedList[newIndex - 1];

            weightedList[newIndex - 1] = weightedList[newIndex];
            weightedList[newIndex]     = temp;
        }
        else
        {
            break;    // If the weight is < or = no exchange is needed.
        }

        newIndex--;
    }
}

bool WFC::elPesoDeLaPosicionEsMayor(unsigned short pos1,
        unsigned short                             pos2)
{
    unsigned short newCharWeight = charFrequencies[weightedList[pos1]];
    unsigned short previosCharWeight    = charFrequencies[weightedList[pos2]];

    if (newCharWeight > previosCharWeight)
    {
        return true;
    }

    return false;
}

char WFC::descomprimir(unsigned short numberToTransform)
{
    char outputChar = weightedList[numberToTransform];

    incrementarFrecuencia(outputChar);
    updateWeightedList(numberToTransform);

    return outputChar;
}


//~ Formatted by Jindent --- http://www.jindent.com
