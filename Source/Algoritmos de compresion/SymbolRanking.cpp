#include "SymbolRanking.h"

SymbolRanking::SymbolRanking(unsigned short ctxorder)
{
    maxOrder = ctxorder;
}

SymbolRanking::~SymbolRanking()
{

}

void SymbolRanking::compress(char *       input,
                             short *      output,
                             unsigned int size)
{
    /*
     * Searches the  context with a max order. If it returns (true, x) the x is stores in the
     * array and the process continues. If it return (false, x), the process repeats with an
     * order n-1.
     * In the case of non ocurrencies, an iteration is done till context 0, at which point
     * the ASCII value correspongin to the literal, plus the number of non ocurrencies are stored
     * in the array.
     */

    unsigned short ctxActual = maxOrder;
    char charToRank;
    unsigned short amountOfNonOcurrencies; //Number of non-ocurrencies till the symbol is found.
    pair<bool, unsigned short> my_pair;

    //First characters [0,order-1]
    for(int posCharToRank = 0; posCharToRank < maxOrder; posCharToRank++)
    {
        if (posCharToRank > 2)
        {
            hash(input[posCharToRank-3], input[posCharToRank-2],input[posCharToRank-1], posCharToRank-3);
        }

        unsigned char charToProcess = input[posCharToRank];

        output[posCharToRank] = wfc.compress(charToProcess);
    }


    //Following characters
    for (unsigned int posCharToRank = maxOrder; posCharToRank < size; posCharToRank++)
    {
        if (posCharToRank%204800 == 0)
        {
            cout << posCharToRank/1024 << "Kb" << endl;
        }

        charToRank = input[posCharToRank];
        exclusionList.clear();
        amountOfNonOcurrencies = 0;

        while(ctxActual > 2)
        {
            //The last parameters is not used for compressing, so a 0 is passed.
            my_pair = seekInContext(ctxActual,posCharToRank, input,'c',0);
            amountOfNonOcurrencies += my_pair.second;
            if (my_pair.first)
            {
                break;
            }
            ctxActual--;
        }
        while (ctxActual <= 2 && ctxActual > 0)
        {
        	my_pair = linearSearchInContext(posCharToRank,ctxActual, input,'c',0);
            amountOfNonOcurrencies += my_pair.second;
            if (!my_pair.first)
            {
                if(ctxActual == 1)
                {
                    //Context 0 case. The actual number is compressed according to the WFC method.
                    amountOfNonOcurrencies += wfc.compress(charToRank);
                }
                ctxActual--;
            }
            else break;
        }
        if(my_pair.first)
        {
            wfc.increaseFreq(charToRank);
        }

        hash(input[posCharToRank-3], input[posCharToRank-2],input[posCharToRank-1], posCharToRank-3);
        output[posCharToRank] = amountOfNonOcurrencies;
        ctxActual = maxOrder;
    }
}

void SymbolRanking::decompress(unsigned short * toDecompress,
                               char *           output,
                               unsigned int     size)
{
    unsigned short ctxActual = maxOrder;
    unsigned short rankToChar;
    pair<bool,unsigned short> my_pair;

    //Primeros rankings [0,orden-1]
    for(unsigned int posRankToChar = 0; posRankToChar < maxOrder; posRankToChar++)
    {
        if (posRankToChar == size)
        {
            break;
        }
        if (posRankToChar > 2)
        {
            hash(output[posRankToChar-3], output[posRankToChar-2],output[posRankToChar-1], posRankToChar-3);
        }
        rankToChar = toDecompress[posRankToChar];
        output[posRankToChar] = wfc.decompress(rankToChar);
    }

    // Next characters.
    for (unsigned int posRankToChar = maxOrder; posRankToChar< size; posRankToChar++)
    {
        if (posRankToChar%102400 == 0)
        {
            cout << posRankToChar/1024 << "Kb" << endl;
        }
        rankToChar = toDecompress[posRankToChar];
        exclusionList.clear();

        while(ctxActual > 2)
        {
            my_pair = seekInContext(ctxActual, posRankToChar, output, 'd', rankToChar);
            if (my_pair.first)
            {
                break;
            }
            rankToChar -= my_pair.second;
            ctxActual--;
        }
        while (ctxActual <= 2 && ctxActual > 0)
        {
        	my_pair = linearSearchInContext(posRankToChar, ctxActual, output, 'd', rankToChar);
            if (!my_pair.first)
            {
                rankToChar -= my_pair.second;
                if(ctxActual == 1)
                {
                	//Case of context zero. The actual number is compressed according to the WFC method.
                    output[posRankToChar] = wfc.decompress(rankToChar);
                }
                ctxActual--;
            }
            else break;
        }
        // Note: when pair return True, it means the second element is the offered character by an
        // existing context whichc matched. For that reason, it will always fit a char.
        if(my_pair.first)
        {
            output[posRankToChar] = (char) my_pair.second;
            wfc.increaseFreq(output[posRankToChar]);
        }
        hash(output[posRankToChar-3], output[posRankToChar-2],output[posRankToChar-1], posRankToChar-3);
        ctxActual = maxOrder;
    }
}

pair<bool,unsigned short> SymbolRanking::seekInContext(unsigned short order,
													   unsigned int   posCharToRank,
													   char *         buffer,
												       char           operation,
												   	   unsigned short ranking)
{
    unsigned int indexFirstChar = posCharToRank-order;
    unsigned int hashingAComparar = FHASH(buffer[indexFirstChar], buffer[indexFirstChar+1], buffer[indexFirstChar+2]);
    unsigned short amountOfNonOcurrencies = 0;
    unsigned short offsetDelHash = 3;
    pair<bool, unsigned short> my_pair;
    list<unsigned int>* listOfPositions = getListOfPositions(buffer, posCharToRank-3);

    if(listOfPositions)
    {
        for (auto posDeMatch : *listOfPositions)
        {
            bool match;
            if (posDeMatch < unsigned (order-offsetDelHash))
            {
                match = false;
            }
            else
            {
                match = sameHashing(hashingAComparar, posDeMatch+offsetDelHash-order, buffer);
            }

            if (match)
            {
                if(charNotExcluded(buffer[posDeMatch+offsetDelHash]))
                {
                    if(operation=='c')
                    {
                        bool esElBuscado = sameChars(posDeMatch + offsetDelHash, buffer[posCharToRank], buffer);
                        if (esElBuscado)
                        {
                        	my_pair.first = true;
                        	my_pair.second = amountOfNonOcurrencies;
                            return my_pair;
                        }
                    }
                    else if(operation=='d')
                    {
                        if(ranking==0)
                        {
                            //The offered char if the decompressed one!
                            unsigned short charDelRanking = (unsigned short) buffer[posDeMatch+offsetDelHash];
                            my_pair.first = true;
                            my_pair.second = charDelRanking;
                            return my_pair;
                        }
                        ranking--;
                    }
                    else throw ParameterError();
                    exclusionList.push_front(buffer[posDeMatch+offsetDelHash]);
                    amountOfNonOcurrencies++;
                }
            }
        }
    }
    my_pair.first = false;
    my_pair.second = amountOfNonOcurrencies;
    return my_pair;
}

pair<bool,unsigned short> SymbolRanking::linearSearchInContext(unsigned int   posCharToRank,
															   unsigned short context,
															   char *         buffer,
															   char           operation,
															   unsigned short ranking)
{
	pair<bool, unsigned short> my_pair;
    unsigned int contextCharToRank = posCharToRank-context;
    unsigned short amountOfNonOcurrencies = 0;

    size_t max = MAX_POS;
    if(posCharToRank < MAX_POS) max = posCharToRank;

    for(unsigned int i = context+1; i <= max; i++)
    {
        unsigned int contextAComparar = posCharToRank-i;
        bool match=sameContext(contextAComparar,contextCharToRank,buffer,context);
        if (match)
        {
            if(charNotExcluded(buffer[contextAComparar+context]))
            {
                if(operation=='c')
                {
                    bool esElBuscado = sameChars(contextAComparar+context, buffer[posCharToRank], buffer);
                    if(esElBuscado)
                    {
                    	my_pair.first = true;
                    	my_pair.second = amountOfNonOcurrencies;
                        return my_pair;
                    }
                }
                else if(operation=='d')
                {
                    if(ranking==0)
                    {
                        //The offered char is to be decompressed.
                        unsigned short charDelRanking = (unsigned short) buffer[contextAComparar+context];
                        my_pair.first = true;
                        my_pair.second = charDelRanking;
                        return my_pair;
                    }
                    ranking--;
                }
                else throw ParameterError();
                exclusionList.push_front(buffer[contextAComparar+context]);
                amountOfNonOcurrencies++;
            }
        }
    }
    my_pair.first = false;
    my_pair.second = amountOfNonOcurrencies;
    return my_pair;
}

list<unsigned int>* SymbolRanking::getListOfPositions(char *       buffer,
        											  unsigned int posFirst)
{
    return hashmap.get(buffer[posFirst], buffer[posFirst+1],buffer[posFirst+2]);
}

void SymbolRanking::hash(unsigned char       symbol1,
                         unsigned char       symbol2,
                         unsigned char       symbol3,
                         unsigned int indexFirstChar)
{
    hashmap.put(symbol1, symbol2,symbol3, indexFirstChar);
}

bool SymbolRanking::sameHashing(unsigned int hashing1,
                                unsigned int pos,
                                char *       buffer)
{
    return (hashing1 == FHASH(buffer[pos], buffer[pos+1], buffer[pos+2]));
}

bool SymbolRanking::sameContext(unsigned int   indexA,
                                unsigned int   indexB,
                                char *         buffer,
                                unsigned short order)
{
    int begginingOfContext = indexA;
    if (begginingOfContext<0)
    {
        // This is the case in which the position 0 is in the hashmap and due to the actual context
        // it generates underflow.
        return false;
    }
    for (unsigned short i=0; i < order; i++)
    {
        if (buffer[begginingOfContext+i]!=buffer[indexB+i])
        {
            return false;
        }
    }
    return true;
}

bool SymbolRanking::sameChars(unsigned int index,
                              char         charToCompare,
                              char *       buffer)
{
    if(buffer[index]==charToCompare)
    {
        return true;
    }
    return false;
}

bool SymbolRanking::charNotExcluded(char charToFind)
{
    auto it = find(exclusionList.begin(), exclusionList.end(), charToFind);
    if (it == exclusionList.end())
    {
        return true;
    }
    return false;
}
