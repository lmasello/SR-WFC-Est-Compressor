#include "HashMap.h"

const int TABLE_SIZE = 256*256*256;

HashMap::HashMap(){
	table = new HashEntry*[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++)
		table[i] = NULL;
}

HashMap::~HashMap(){
	for (int i = 0; i < TABLE_SIZE; i++)
		if (table[i])
			delete table[i];
	delete[] table;
}

unsigned int HashMap::fhash(char primero, char segundo, char tercero){
	return (unsigned char)primero * 256 * 256 + (unsigned char) segundo * 256 + (unsigned char) tercero;
}

list<unsigned int>* HashMap::get(char first, char second, char third){
	unsigned int hash = fhash(first, second, third);
	if (!table[hash]){
		list <unsigned int>* lista = new list <unsigned int>;
		return lista;
	}
	return (table[hash]->getValue());
}

void HashMap::put(char first, char second, char third, unsigned int value){
	unsigned int hash = fhash(first, second, third);
	if(!table[hash]){
		list <unsigned int>* lista = new list <unsigned int>;
		lista->push_front(value);
		table[hash] = new HashEntry(lista);
	}
	else{
		table[hash]->addValue(value);
	}
}
