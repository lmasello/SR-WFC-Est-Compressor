#include "../../headers/Compression Algorithms/Estructured.h"

typedef struct _pair {
    int number;
    int occurrences;
} pair_t;


struct _level {
    unsigned short totalOccurrences;
    list<pair_t *> amountPerNumber;
    unsigned short maxNumber;
};


pair_t *pair_create(int number) {
    pair_t *pair = new pair_t;

    pair->number = number;
    pair->occurrences = INITIAL_OCURRENCES;

    return pair;
}

level_t level_create(int level_number) {
    level_t level;

    level.amountPerNumber.push_back(pair_create(NRO_ESCAPE));

    unsigned short totalOccurrences = 1;

    if (level_number == 0) {
        level.amountPerNumber.push_back(pair_create(0));

        level.maxNumber = 0;

        totalOccurrences++;
    }
    else {
        for (int i = pow(2, level_number - 1); i < pow(2, level_number); i++) {
            level.amountPerNumber.push_back(pair_create(i));

            level.maxNumber = pow(2, level_number) - 1;

            totalOccurrences++;
        }
    }

    level.totalOccurrences = totalOccurrences;

    return level;
}

void level_destroy(level_t *level) {
    list<pair_t *>::iterator it;

    it = level->amountPerNumber.begin();

    while (it != level->amountPerNumber.end()) {
        pair_destroy(*it);

        delete *it;

        it++;
    }

    level->amountPerNumber.clear();
}

Estructured::Estructured() {
    value = 0;
    posInInputStr = 0;
    high = 0xffff;    // 16 bits
    low = 0x0000;    // 16 bits
    underflow = 0;
    bitCounter = 0;
    result_d = nullptr;
    levels = new level_t[LEVEL_AMOUNT];

    for (int i = 0; i < LEVEL_AMOUNT; i++) {
        level_t new_level = level_create(i);

        levels[i] = new_level;
    }
}

Estructured::~Estructured() {
    for (int i = 0; i < LEVEL_AMOUNT; i++) {
        level_destroy(&(levels[i]));
    }

    delete[] levels;
    delete result_d;
}

void Estructured::prepareCompression() {
    high = 0xffff;    // 16 bits
    low = 0x0000;    // 16 bits
    underflow = 0;
}

pair<char *, unsigned int> Estructured::compress(short *input,
                                                 unsigned int size) {
    prepareCompression();

    for (unsigned int i = 0; i < size; i++) {
        int level_index;
        short numberToCompress = input[i];

        if (numberToCompress == 0) {
            level_index = 0;
        }
        else {
            level_index = int(log2(numberToCompress)) + 1;
        }

        int act_level = NIVEL_INICIAL;

        for (; act_level < level_index; act_level++) {
            emitESC(act_level);
        }

        emitNumber(act_level, numberToCompress);
    }

    emitEOF();

    return generateCompressionResult();
}

void Estructured::emitNumber(int level_number,
                             int nro) {
    level_t &level = levels[level_number];

    // The number next to the one being compressed is evaluated.
    int next_number;

    if ((level_number <= 2) && (nro == NRO_ESCAPE)) {
        next_number = level_number;
    }
    else {
        next_number = (nro == NRO_ESCAPE) ? (pow(2, level_number - 1)) : nro + 1;
    }

    // The frequencies are calculated.
    unsigned short numberToCompressTopFrequency = accumulatedFreqTillNumber(level, next_number);
    unsigned short numberToCompressLowFrequency = accumulatedFreqTillNumber(level, nro);
    unsigned short totalFrequency = level.totalOccurrences;

    // Frontier values are calculated.
    unsigned short range = (high - low) + 1;

    high = low + ((range * numberToCompressTopFrequency) / totalFrequency) - 1;
    low = low + ((range * numberToCompressLowFrequency) / totalFrequency);

    // EOF case.
    if ((level_number == (LEVEL_AMOUNT - 1)) && (nro == NRO_ESCAPE)) {
        endCompression(low);
    }

    for (; ;) {
        if (high < Half) {
            emitBit(0);

            while (underflow > 0) {
                emitBit(1);
                underflow--;
            }
        }
        else if (low >= Half) {
            emitBit(1);

            while (underflow > 0) {
                emitBit(0);
                underflow--;
            }

            low -= Half;
            high -= Half;
        }
        else if ((low >= First_qtr) && (high < Third_qtr)) {
            underflow++;

            low -= First_qtr;
            high -= First_qtr;
        }
        else {
            break;
        }

        low = 2 * low;
        high = 2 * high + 1;
    }

    // Update frequencies.
    increaseFrequency(level, nro);
}

void Estructured::emitESC(int level) {
    emitNumber(level, NRO_ESCAPE);
}

void Estructured::emitEOF() {
    for (int i = 0; i < LEVEL_AMOUNT; i++) {
        emitESC(i);
    }
}

void Estructured::emitBit(bool bit) {
    bitCounter++;

    // The new bit is stored in the bytebuffer.
    byteBuffer[8 - bitCounter] = (bit) ? 1 : 0;

    // When a whole byte is completed, it is stored in the array to be saved in the compressed file.
    if (bitCounter == 8) {
        bitCounter = 0;

        unsigned long i = byteBuffer.to_ulong();
        unsigned char byteToSave = static_cast<unsigned char>(i);

        result.push_back(byteToSave);
    }
}

void Estructured::endCompression(unsigned short low) {
    // Emits msb of Low
    bool msbOfLow = ((low >> 15) != 0);

    emitBit(msbOfLow);

    // Emits underflows
    while (underflow > 0) {
        emitBit(~msbOfLow);
        underflow--;
    }

    // Emits remaaining bits of Low.
    for (short shift = 14; shift < 0; shift--) {
        unsigned short mask = (2 ^ (shift + 1)) - 1;
        bool bit = (bool) (low & mask) >> shift;

        emitBit(bit);
    }
}

pair<char *, unsigned int> Estructured::generateCompressionResult() {
    flushByteBuffer();

    size_t size = result.length();
    char *output = new char[size];

    for (unsigned int i = 0; i < size; i++) {
        char charToSave = result[i];

        output[i] = charToSave;
    }

    pair<char *, unsigned int> pair(output, result.length());

    return pair;
}

void Estructured::flushByteBuffer() {
    while (bitCounter != 0) {
        bitCounter++;

        // All the bits not used from the byte are set to 0.
        byteBuffer[8 - bitCounter] = 0;

        // If a whole byte is completed, it is stored in the array.
        if (bitCounter == 8) {
            bitCounter = 0;

            unsigned long i = byteBuffer.to_ulong();
            unsigned char byteToSave = static_cast<unsigned char>(i);

            result.push_back(byteToSave);
        }
    }
}

void Estructured::prepareDecompression() {
    high = 0xffff;    // 16 bits
    low = 0x0000;    // 16 bits
    value = 0;
    result_d = new list<unsigned short>;

    // The first 16 bits are read.
    for (register unsigned short i = 0; i < 16; i++) {
        value = value * 2 + readBit();
    }
}

void Estructured::generateInput(char *input,
                                unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        for (int j = 0; j < 8; j++) {
            unsigned char actual = input[i];

            actual <<= j;
            actual >>= 7;
            inputStr += (int) actual;
        }
    }
}

pair<unsigned short *, unsigned int> Estructured::decompress(char *input,
                                                             unsigned int size) {
    generateInput(input, size);
    prepareDecompression();

    int a = 0;

    while (true) {
        int act_level = NIVEL_INICIAL;
        int emitted = NRO_ESCAPE;

        while (emitted == NRO_ESCAPE) {
            emitted = obtainNumber(act_level);

            if ((act_level == LEVEL_AMOUNT - 1) && (emitted == NRO_ESCAPE)) {
                break;
            }

            act_level++;
        }

        if ((act_level == LEVEL_AMOUNT - 1) && (emitted == NRO_ESCAPE)) {
            break;
        }

        a++;
        result_d->push_back(emitted);
    }

    return generateDecompressionResult();
}

int Estructured::obtainNumber(int level_number) {
    level_t &level = levels[level_number];

    // The frequencies of the level.
    unsigned short totalFrequency = level.totalOccurrences;

    // It is evaluated where the number falls.
    unsigned int range = (high - low) + 1;
    int temp = ((value - low + 1) * totalFrequency - 1) / range;

    // Symbol is the one temp is pointing to. temp will be in the range [symbolLowFrequency,symbolTopFrequency)
    int symbol = 0;
    unsigned short symbolTopFrequency = 0;
    unsigned short symbolLowFrequency = 0;
    unsigned short numberTopFrequency = 0;
    int next_number;

    for (int numberToEvaluate = NRO_ESCAPE; numberToEvaluate <= level.maxNumber; numberToEvaluate = next_number) {
        // Logic used to obtaing the number next to numberToEvaluate
        if ((level_number <= 2) && (numberToEvaluate == NRO_ESCAPE)) {
            next_number = level_number;
        }
        else {
            next_number = (numberToEvaluate == NRO_ESCAPE) ? (pow(2, level_number - 1)) : numberToEvaluate + 1;
        }

        // Is temp in the numberToEvaluate range?
        numberTopFrequency = accumulatedFreqTillNumber(level, next_number);

        if (numberTopFrequency > temp) {
            symbol = numberToEvaluate;
            symbolTopFrequency = numberTopFrequency;
            symbolLowFrequency = accumulatedFreqTillNumber(level, numberToEvaluate);

            break;
        }
    }

    // Recalculates the frontier values.
    high = low + ((range * symbolTopFrequency) / totalFrequency) - 1;
    low = low + (range * symbolLowFrequency) / totalFrequency;

    // Iters till there are no more possible symbols to decode.
    for (; ;) {
        if (high < Half) {
            // Nothing                       //Expand low half
        }
        else if (low >= Half) {
            // Expand high half
            value -= Half;
            low -= Half;                // Subtract offset to top
            high -= Half;
        }
        else if ((low >= First_qtr) && (high < Third_qtr)) {
            // Expand middle half
            value -= First_qtr;
            low -= First_qtr;           // Subtract offset to middle
            high -= First_qtr;
        }
        else {
            break;                        // Otherwise exit loop
        }

        low = 2 * low;
        high = 2 * high + 1;             // Scale up code range
        value = 2 * value + readBit();    // Move in next input bit
    }

    // Updates frequencies.
    increaseFrequency(level, symbol);

    return symbol;
}

bool Estructured::readBit() {
    posInInputStr++;

    return (bool) inputStr[posInInputStr - 1];
}

unsigned short Estructured::accumulatedFreqTillNumber(level_t &level,
                                                      int number) {
    // With verifyFreqs, no frequencies higher than 16 bits get through.
    unsigned short numbreLowFrequency = 0;

    // Case in which the frequencie of the max number of the level is asked for.
    if (number == (level.maxNumber + 1)) {
        return level.totalOccurrences;
    }

    // Generic case.
    list<pair_t *>::iterator it = level.amountPerNumber.begin();

    for (; (*it)->number != number; it++) {
        numbreLowFrequency += (*it)->occurrences;
    }

    return numbreLowFrequency;
}

void Estructured::increaseFrequency(level_t &level,
                                    int number) {
    // Level total frequencies are updated.
    level.totalOccurrences++;

    // Increases the frequencies of the number in the determined level.
    list<pair_t *>::iterator it = level.amountPerNumber.begin();

    for (; (*it)->number != number; it++);    // The iterator is positioned in the number position.

    (*it)->occurrences++;
    verifyFrequencies(level);
}

void Estructured::verifyFrequencies(level_t &level) {
    // If the frequency limit is not surpassed, it exits.
    if (level.totalOccurrences < MAX_FREQUENCY) {
        return;
    }

    unsigned short totalFrequencies = 0;

    // If the frequency limit is surpassed, the level frequencies are normalized, reducing them to half.
    list<pair_t *>::iterator levelNumber = level.amountPerNumber.begin();

    for (; levelNumber != level.amountPerNumber.end(); levelNumber++) {
        (*levelNumber)->occurrences /= 2;

        // If the normalized number is 0, it is set to 1.
        if ((*levelNumber)->occurrences == 0) {
            (*levelNumber)->occurrences = 1;
        }

        totalFrequencies += (*levelNumber)->occurrences;
    }

    // Update all the level frequencies.
    level.totalOccurrences = totalFrequencies;
}

pair<unsigned short *, unsigned int> Estructured::generateDecompressionResult() {
    unsigned short *output = new unsigned short[result_d->size()];
    int i = 0;

    for (auto result : *result_d) {
        output[i] = result;
        i++;
    }
    pair<unsigned short *, unsigned int> par(output, result_d->size());

    return par;
}
