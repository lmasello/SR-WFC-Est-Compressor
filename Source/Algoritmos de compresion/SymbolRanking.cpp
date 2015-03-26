#include "SymbolRanking.h"

SymbolRanking::SymbolRanking(unsigned short ctxorder)
{
	maxOrder = ctxorder;
}

SymbolRanking::~SymbolRanking()
{

}

void SymbolRanking::comprimir(char *       input,
							  short *      output,
							  unsigned int size)
{
	/* Busca el contexto con un orden max. Si devuelve (true,x) almacenamos x en el
	* vector y avanzamos. Si devuelve (false,x) se busca contexto de orden n-1.
	* En caso de no ocurrencias, se itera hasta llegar a contexto 0, donde se almacena
	* en el vector el numero ascii correspondiente al literal, mas el numero de no ocurrencias */

	unsigned short ctxActual = maxOrder;
	unsigned short minPosToHash = 3;
	char charToRank;
	unsigned short amountOfNonOcurrencies; //Sera el numero de no ocurrencias hasta que se encuentre el simbolo.
	tuple<bool,unsigned short> tup;

	//Primeros caracteres [0,orden-1]
	for(int posCharToRank = 0; posCharToRank < maxOrder; posCharToRank++){
		if (posCharToRank > 2)
		{
			hashear(input[posCharToRank-3], input[posCharToRank-2],input[posCharToRank-1], posCharToRank-3);
		}
		char charToProcess = input[posCharToRank];
		output[posCharToRank] = wfc.comprimir(charToProcess);
	}

	//Siguientes caracteres
	for (unsigned int posCharToRank = maxOrder; posCharToRank< size; posCharToRank++)
	{
		if (posCharToRank%102400 == 0)
		{
			cout << posCharToRank/1024 << "K" << endl;
		}

		charToRank = input[posCharToRank];
		exclusionList.clear();
		amountOfNonOcurrencies = 0;

		while(ctxActual > 2)
		{
			//El ultimo parametro (ranking) no se utiliza para el compresor, por lo tanto se lo pone en 0
			tup = buscarEnContexto(ctxActual,posCharToRank, input,'c',0);
			amountOfNonOcurrencies += get<1> (tup);
			if (get<0> (tup))
			{
				break;
			}
			ctxActual--;
		}
		while (ctxActual <= 2 && ctxActual > 0)
		{
			tup = busquedaLinealEnContexto(posCharToRank,ctxActual, input,'c',0);
			amountOfNonOcurrencies += get<1> (tup);
			if (!get<0> (tup))
			{
				if(ctxActual == 1)
				{
					//Context 0 case. The actual number is compressed according to the WFC method.
					amountOfNonOcurrencies += wfc.comprimir(charToRank);
				}
				ctxActual--;
			}
			else break;
		}
		if(get<0>(tup))
		{
			wfc.incrementarFrecuencia(charToRank);
		}
		if(posCharToRank >= minPosToHash)
		{
			hashear(input[posCharToRank-3], input[posCharToRank-2],input[posCharToRank-1], posCharToRank-3);
		}
		output[posCharToRank] = amountOfNonOcurrencies;
		ctxActual = maxOrder;
	}
}

void SymbolRanking::descomprimir(unsigned short * toDecompress,
								 char *           output,
								 unsigned int     size)
{
	unsigned short ctxActual = maxOrder;
	unsigned short minPosToHash = 3;
	unsigned short rankToChar;
	tuple<bool,unsigned short> tup;

	//Primeros rankings [0,orden-1]
	for(unsigned int posRankToChar = 0; posRankToChar < maxOrder; posRankToChar++)
	{
		if (posRankToChar == size)
		{
			break;
		}
		if (posRankToChar > 2)
		{
			hashear(output[posRankToChar-3], output[posRankToChar-2],output[posRankToChar-1], posRankToChar-3);
		}
		rankToChar = toDecompress[posRankToChar];
		output[posRankToChar] = wfc.descomprimir(rankToChar);
	}

	//Siguientes caracteres
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
			tup = buscarEnContexto(ctxActual, posRankToChar, output, 'd', rankToChar);
			if (get<0> (tup))
			{
				break;
			}
			rankToChar -= get<1> (tup);
			ctxActual--;
		}
		while (ctxActual <= 2 && ctxActual > 0)
		{
			tup = busquedaLinealEnContexto(posRankToChar, ctxActual, output, 'd', rankToChar);
			if (!get<0> (tup))
			{
				rankToChar -= get<1> (tup);
				if(ctxActual == 1)
				{
					//Caso de contexto = 0. Se comprime el numero actual de acuerdo al metodo WFC.
					output[posRankToChar] = wfc.descomprimir(rankToChar);
				}
				ctxActual--;
			}
			else break;
		}
		// Nota: cuando la tup me devuelve True, quiere decir que el segundo elemento es el caracter ofrecido por un
		// contexto existente que matcheo. Por lo tanto, esto siempre va a ser un char.
		if(get<0>(tup))
		{
			output[posRankToChar] = (char) get<1>(tup);
			wfc.incrementarFrecuencia(output[posRankToChar]);
		}
		if(posRankToChar >= minPosToHash)
		{
			hashear(output[posRankToChar-3], output[posRankToChar-2],output[posRankToChar-1], posRankToChar-3);
		}
		ctxActual = maxOrder;
	}
}

tuple<bool,unsigned short> SymbolRanking::buscarEnContexto(unsigned short order,
														   unsigned int   posCharToRank,
														   char *         buffer,
														   char           operation,
														   unsigned short ranking)
{
	unsigned int indexFirstChar = posCharToRank-order;
	unsigned int hashingAComparar = FHASH(buffer[indexFirstChar], buffer[indexFirstChar+1], buffer[indexFirstChar+2]);
	unsigned short amountOfNonOcurrencies = 0;
	unsigned short offsetDelHash = 3;
	tuple<bool, unsigned short> tup;
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
				match = hashingIguales(hashingAComparar, posDeMatch+offsetDelHash-order, buffer);
			}

			if (match)
			{
				if(charNoExcluido(buffer[posDeMatch+offsetDelHash]))
				{
					if(operation=='c')
					{
						bool esElBuscado = charsIguales(posDeMatch + offsetDelHash, buffer[posCharToRank], buffer);
						if (esElBuscado)
						{
							get<0> (tup) = true;
							get<1> (tup) = amountOfNonOcurrencies;
							return tup;
						}
					}
					else if(operation=='d')
					{
						if(ranking==0)
						{ //El char ofrecido es el descomprimido!
							unsigned short charDelRanking = (unsigned short) buffer[posDeMatch+offsetDelHash];
							get<0> (tup) = true;
							get<1> (tup) = charDelRanking;
							return tup;
						}
						ranking--;
					}
					else throw ErrorDeParametro();
					exclusionList.push_front(buffer[posDeMatch+offsetDelHash]);
					amountOfNonOcurrencies++;
				}
			}
		}
	}
	get<0> (tup) = false;
	get<1> (tup) = amountOfNonOcurrencies;
	return tup;
}

tuple<bool,unsigned short> SymbolRanking::busquedaLinealEnContexto(unsigned int   posCharToRank,
																   unsigned short context,
																   char *         buffer,
																   char           operation,
																   unsigned short ranking)
{
	tuple<bool, unsigned short> tup;
	unsigned int contextCharToRank = posCharToRank-context;
	unsigned short amountOfNonOcurrencies = 0;

	size_t max = MAX_POS;
	if(posCharToRank < MAX_POS) max = posCharToRank;

	for(unsigned int i = context+1; i <= max; i++)
	{
		unsigned int contextAComparar = posCharToRank-i;
		bool match=contextosIguales(contextAComparar,contextCharToRank,buffer,context);
		if (match)
		{
			if(charNoExcluido(buffer[contextAComparar+context]))
			{
				if(operation=='c')
				{
					bool esElBuscado = charsIguales(contextAComparar+context, buffer[posCharToRank], buffer);
					if(esElBuscado)
					{
						get<0> (tup) = true;
						get<1> (tup) = amountOfNonOcurrencies;
						return tup;
					}
				}
				else if(operation=='d')
				{
					if(ranking==0)
					{ //El char ofrecido es el que hay que descomprimir
						unsigned short charDelRanking = (unsigned short) buffer[contextAComparar+context];
						get<0> (tup) = true;
						get<1> (tup) = charDelRanking;
						return tup;
					}
					ranking--;
				}
				else throw ErrorDeParametro();
				exclusionList.push_front(buffer[contextAComparar+context]);
				amountOfNonOcurrencies++;
			}
		}
	}
	get<0> (tup) = false;
	get<1> (tup) = amountOfNonOcurrencies;
	return tup;
}

list<unsigned int>* SymbolRanking::getListOfPositions(char *       buffer,
													  unsigned int posFirst)
{
	return hashmap.get(buffer[posFirst], buffer[posFirst+1],buffer[posFirst+2]);
}

void SymbolRanking::hashear(char         symbol1,
							char         symbol2,
							char         symbol3,
							unsigned int indexFirstChar)
{
	hashmap.put(symbol1, symbol2,symbol3, indexFirstChar);
}

bool SymbolRanking::hashingIguales(unsigned int hashing1,
								   unsigned int pos,
								   char *       buffer)
{
	return (hashing1 == FHASH(buffer[pos], buffer[pos+1], buffer[pos+2]));
}

bool SymbolRanking::contextosIguales(unsigned int   indexA,
									 unsigned int   indexB,
									 char *         buffer,
									 unsigned short order)
{
	int comienzoDeContextoAComparar = indexA;
	if (comienzoDeContextoAComparar<0)
	{
		// Este es el caso que en el hash tengo posicion 0, y debido al contexto actual me genera underflow.
		return false;
	}
	for (unsigned short i=0; i < order; i++)
	{
		if (buffer[comienzoDeContextoAComparar+i]!=buffer[indexB+i])
		{
			return false;
		}
	}
	return true;
}

bool SymbolRanking::charsIguales(unsigned int index,
								 char         charToCompare,
								 char *       buffer)
{
	if(buffer[index]==charToCompare)
	{
		return true;
	}
	return false;
}

bool SymbolRanking::charNoExcluido(char charToFind)
{
	auto it = find(exclusionList.begin(), exclusionList.end(), charToFind);
	if (it == exclusionList.end())
	{
		return true;
	}
	return false;
}
