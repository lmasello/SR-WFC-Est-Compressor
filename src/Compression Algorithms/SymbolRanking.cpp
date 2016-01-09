#include "../../headers/Compression Algorithms/SymbolRanking.h"

SymbolRanking::SymbolRanking(unsigned short ctxOrder) {
    currentOrder = maxOrder = ctxOrder;
}

SymbolRanking::~SymbolRanking() {

}

void SymbolRanking::compress(char *input, short *output, unsigned int size) {
    /*
     * Searches the  context with a max order. If it returns (true, x) the x is stores in the
     * array and the process continues. If it return (false, x), the process repeats with an
     * order n-1.
     * In the case of non occurrences, an iteration is done till context 0, at which point
     * the ASCII value corresponding to the literal, plus the number of non occurrences are stored
     * in the array.
     */

    char charToRank;
    unsigned short amountOfNonOccurrences; //Number of non occurrences till the symbol is found.
    pair<bool, unsigned short> my_pair;

    string ctx;

    //First characters [0,order-1]
    output[0] = wfc.compress(input[0]);
    for (int posCharToRank = 1; posCharToRank < maxOrder; posCharToRank++) {
        ctx.push_back(input[posCharToRank-1]);
        currentOrder = posCharToRank;

        char charToProcess = input[posCharToRank];
        mapRemainingContexts(ctx, charToProcess);
        output[posCharToRank] = wfc.compress(charToProcess);
    }

    currentOrder = maxOrder;

    //Following characters
    for (unsigned int posCharToRank = maxOrder; posCharToRank < size; posCharToRank++) {
        if (posCharToRank % 204800 == 0) {
            cout << posCharToRank / 1024 << "Kb" << endl;
        }

        charToRank = input[posCharToRank];
        exclusionList.clear();
        amountOfNonOccurrences = 0;

        //New algorithm
        ctx.clear();
        /*cout << "Char: " << charToRank << endl;*/
        while (currentOrder > 0) {
            //The last parameters is not used for compressing, so a 0 is passed.
            ctx = getCtx(input, posCharToRank);
            set<char> matchedChars;
            try {
                matchedChars = myMap.at(ctx);
            }
            catch(out_of_range) {
            }

            /*cout << currentOrder << " " << ctx << "   ";
            for (char current : matchedChars) {
                cout << current << " ";
            }
            cout << matchedChars.size() << endl;
*/
            my_pair = lookUpInContext(matchedChars, charToRank, 'c', 0);
            amountOfNonOccurrences += my_pair.second;
            if (my_pair.first) {
                /*cout << "Found" << endl;*/
                break;
            }
            matchedChars.insert(charToRank);
            myMap[ctx] = matchedChars;
            //myMap.insert (pair<string, set<char>>(ctx, matchedChars));
            currentOrder--;
        }

        if (my_pair.first) {
            wfc.increaseFreq(charToRank);
        }
        if (currentOrder == 0) {
            //Context 0 case. The actual number is compressed according to the WFC method.
            amountOfNonOccurrences += wfc.compress(charToRank);
        }

        mapRemainingContexts(ctx, charToRank);
        output[posCharToRank] = amountOfNonOccurrences;
        currentOrder = maxOrder;


/*
        //Old algorithm
        while (currentOrder > 2) {
            //The last parameters is not used for compressing, so a 0 is passed.
            my_pair = seekInContext(posCharToRank, input, 'c', 0);
            amountOfNonOccurrences += my_pair.second;
            if (my_pair.first) {
                break;
            }
            currentOrder--;
        }
        while (currentOrder <= 2 && currentOrder > 0) {
            my_pair = linearSearchInContext(posCharToRank, input, 'c', 0);
            amountOfNonOccurrences += my_pair.second;
            if (!my_pair.first) {
                if (currentOrder == 1) {
                    //Context 0 case. The actual number is compressed according to the WFC method.
                    amountOfNonOccurrences += wfc.compress(charToRank);
                }
                currentOrder--;
            }
            else break;
        }

        if (my_pair.first) {
            wfc.increaseFreq(charToRank);
        }

        hash(input[posCharToRank - 3], input[posCharToRank - 2], input[posCharToRank - 1], posCharToRank - 3);
        output[posCharToRank] = amountOfNonOccurrences;
        currentOrder = maxOrder;*/
    }
}

void SymbolRanking::decompress(unsigned short *toDecompress,
                               char *output,
                               unsigned int size) {
    unsigned short rankToChar;
    pair<bool, unsigned short> my_pair;

    //Firsts rankings [0,order-1]
    for (unsigned int posRankToChar = 0; posRankToChar < maxOrder; posRankToChar++) {
        if (posRankToChar == size) {
            break;
        }
        if (posRankToChar > 2) {
            hash(output[posRankToChar - 3], output[posRankToChar - 2], output[posRankToChar - 1], posRankToChar - 3);
        }
        rankToChar = toDecompress[posRankToChar];
        output[posRankToChar] = wfc.decompress(rankToChar);
    }

    // Next characters.
    for (unsigned int posRankToChar = maxOrder; posRankToChar < size; posRankToChar++) {
        if (posRankToChar % 102400 == 0) {
            cout << posRankToChar / 1024 << "Kb" << endl;
        }
        rankToChar = toDecompress[posRankToChar];
        exclusionList.clear();

        while (currentOrder > 2) {
            my_pair = seekInContext(posRankToChar, output, 'd', rankToChar);
            if (my_pair.first) {
                break;
            }
            rankToChar -= my_pair.second;
            currentOrder--;
        }
        while (currentOrder <= 2 && currentOrder > 0) {
            my_pair = linearSearchInContext(posRankToChar, output, 'd', rankToChar);
            if (!my_pair.first) {
                rankToChar -= my_pair.second;
                if (currentOrder == 1) {
                    //Case of context zero. The actual number is compressed according to the WFC method.
                    output[posRankToChar] = wfc.decompress(rankToChar);
                }
                currentOrder--;
            }
            else break;
        }
        // Note: when pair return True, it means the second element is the offered character by an
        // existing context whichc matched. For that reason, it will always fit a char.
        if (my_pair.first) {
            output[posRankToChar] = ( char ) my_pair.second;
            wfc.increaseFreq(output[posRankToChar]);
        }
        hash(output[posRankToChar - 3], output[posRankToChar - 2], output[posRankToChar - 1], posRankToChar - 3);
        currentOrder = maxOrder;
    }
}

pair<bool, unsigned short> SymbolRanking::seekInContext(unsigned int posCharToRank,
                                                        char *buffer,
                                                        char operation,
                                                        unsigned short ranking) {
    unsigned int indexFirstChar = posCharToRank - currentOrder;
    unsigned int
        hashingToCompare = FHASH(buffer[indexFirstChar], buffer[indexFirstChar + 1], buffer[indexFirstChar + 2]);
    unsigned short nonOccurrences = 0;
    unsigned short offsetDelHash = 3;
    pair<bool, unsigned short> my_pair;
    list<unsigned int> *listOfPositions = getListOfPositions(buffer, posCharToRank - 3);

    if (listOfPositions) {
        for (auto matchPos : *listOfPositions) {
            bool match;
            if (matchPos < currentOrder - offsetDelHash) {
                match = false;
            }
            else {
                match = sameHashing(hashingToCompare, matchPos + offsetDelHash - currentOrder, buffer);
            }

            if (!match) continue;

            if (charExcluded(buffer[matchPos + offsetDelHash])) continue;

            if (operation == 'c') {
                bool found = sameChars(matchPos + offsetDelHash, buffer[posCharToRank], buffer);
                if (found) {
                    my_pair.first = true;
                    my_pair.second = nonOccurrences;
                    return my_pair;
                }
            }
            else if (operation == 'd') {
                if (ranking == 0) {
                    //The offered char if the decompressed one!
                    unsigned short charDelRanking = ( unsigned short ) buffer[matchPos + offsetDelHash];
                    my_pair.first = true;
                    my_pair.second = charDelRanking;
                    return my_pair;
                }
                ranking--;
            }
            else {
                throw ParameterError(); //Should never happen.
            }
            exclusionList.push_front(buffer[matchPos + offsetDelHash]);
            nonOccurrences++;
        }
    }
    my_pair.first = false;
    my_pair.second = nonOccurrences;
    return my_pair;
}

pair<bool, unsigned short> SymbolRanking::linearSearchInContext(unsigned int posCharToRank,
                                                                char *buffer,
                                                                char operation,
                                                                unsigned short ranking) {
    pair<bool, unsigned short> my_pair;
    unsigned int contextCharToRank = posCharToRank - currentOrder;
    unsigned short nonOccurrences = 0;

    size_t max = SEARCH_LIMIT;
    if (posCharToRank < SEARCH_LIMIT) max = posCharToRank;

    for (unsigned int i = currentOrder + 1; i <= max; i++) {
        unsigned int contextToCompare = posCharToRank - i;
        bool match = sameContext(contextToCompare, contextCharToRank, buffer);

        if (!match) continue;

        if (charExcluded(buffer[contextToCompare + currentOrder])) continue;

        if (operation == 'c') {
            bool found = sameChars(contextToCompare + currentOrder, buffer[posCharToRank], buffer);
            if (found) {
                my_pair.first = true;
                my_pair.second = nonOccurrences;
                return my_pair;
            }
        }
        else if (operation == 'd') {
            if (ranking == 0) {
                //The offered char is to be decompressed.
                unsigned short charDelRanking = ( unsigned short ) buffer[contextToCompare + currentOrder];
                my_pair.first = true;
                my_pair.second = charDelRanking;
                return my_pair;
            }
            ranking--;
        }
        else throw ParameterError();
        exclusionList.push_front(buffer[contextToCompare + currentOrder]);
        nonOccurrences++;
    }
    my_pair.first = false;
    my_pair.second = nonOccurrences;
    return my_pair;
}

list<unsigned int> *SymbolRanking::getListOfPositions(char *buffer,
                                                      unsigned int posFirst) {
    return hashMap.get(buffer[posFirst], buffer[posFirst + 1], buffer[posFirst + 2]);
}

void SymbolRanking::hash(unsigned char symbol1,
                         unsigned char symbol2,
                         unsigned char symbol3,
                         unsigned int indexFirstChar) {
    hashMap.put(symbol1, symbol2, symbol3, indexFirstChar);
}

bool SymbolRanking::sameHashing(unsigned int hashing1,
                                unsigned int pos,
                                char *buffer) {
    return (hashing1 == FHASH(buffer[pos], buffer[pos + 1], buffer[pos + 2]));
}

bool SymbolRanking::sameContext(unsigned int indexA,
                                unsigned int indexB,
                                char *buffer) {
    int beginningOfContext = indexA;
    if (beginningOfContext < 0) {
        // This is the case in which the position 0 is in the hashmap and due to the actual context
        // it generates underflow.
        return false;
    }
    for (unsigned short i = 0; i < currentOrder; i++) {
        if (buffer[beginningOfContext + i] != buffer[indexB + i]) {
            return false;
        }
    }
    return true;
}

bool SymbolRanking::sameChars(unsigned int index,
                              char charToCompare,
                              char *buffer) {
    return buffer[index] == charToCompare;
}

bool SymbolRanking::charExcluded(char charToFind) {
    auto it = find(exclusionList.begin(), exclusionList.end(), charToFind);
    return it != exclusionList.end();
}

string SymbolRanking::getCtx(char *buffer, unsigned int posCharToRank) {
    unsigned int pos = posCharToRank - currentOrder;
    string context;
    for (int i = 0; i < currentOrder; i++) {
        context.push_back(buffer[pos+i]);
    }
    return context;
}

void SymbolRanking::mapRemainingContexts(string context, char charToRank) {
    string currentCtx = context;
    /*cout << "Map remaining ctx" << endl;*/
    for(int i = 0; i < currentOrder; i++) {
        currentCtx = context.substr(i);
       /* cout << i << " currentCtx: " << currentCtx << " to rank " << charToRank;*/
        set<char> matchedChars;
        try {
            matchedChars = myMap.at(currentCtx);
        }
        catch(out_of_range) {
        }
        /*cout << "   Value of map before: ";
        for (char current : matchedChars) {
            cout << current << " ";
        }
        cout << matchedChars.size();*/

        matchedChars.insert(charToRank);

        /*cout << "   Value of map updated to: ";
        for (char current : matchedChars) {
            cout << current << " ";
        }
        cout << matchedChars.size() << endl;*/
        myMap[currentCtx] = matchedChars;
        //myMap.insert (pair<string, set<char>>(currentCtx, matchedChars));
    }
}

pair<bool, unsigned short> SymbolRanking::lookUpInContext(set<char> listOfChars,
                                                          char charToRank,
                                                          char operation,
                                                          unsigned short ranking) {
    unsigned short nonOccurrences = 0;
    pair<bool, unsigned short> my_pair;

    for (char character : listOfChars) {

        if (charExcluded(character)) continue;

        if (operation == 'c') {
            bool found = (character == charToRank);
            if (found) {
                my_pair.first = true;
                my_pair.second = nonOccurrences;
                return my_pair;
            }
        }
        else if (operation == 'd') {
            if (ranking == 0) {
                //The offered char if the decompressed one!
                unsigned short charDelRanking = ( unsigned short ) charToRank;
                my_pair.first = true;
                my_pair.second = charDelRanking;
                return my_pair;
            }
            ranking--;
        }
        else {
            throw ParameterError(); //Should never happen.
        }
        exclusionList.push_front(character);
        nonOccurrences++;
    }

    my_pair.first = false;
    my_pair.second = nonOccurrences;
    return my_pair;
}