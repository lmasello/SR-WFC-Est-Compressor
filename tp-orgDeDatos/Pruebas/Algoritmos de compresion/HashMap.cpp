#include "HashMap.h"

const int TABLE_SIZE = 255*255;

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

int HashMap::fhash(char* clave){
	return clave[0]*255+clave[1];
}

list<unsigned long> HashMap::get(char first, char second){
	char* key = (char*) malloc (sizeof(char)*2);
	key[0] = first;
	key[1] = second;
	list<long unsigned int> lista;
	int hash = fhash(key);
	while (table[hash] && table[hash]->getKey() != key) //Segun como hicimos la fhash, no deberia pasar nunca esto.
		hash = (hash + 1) % TABLE_SIZE;
	if (!table[hash])
		return lista;
	return table[hash]->getValue();
}

void HashMap::put(char first, char second, unsigned long value){
	char* key = (char*) malloc (sizeof(char)*2);
	key[0] = first;
	key[1] = second;
	list <unsigned long> lista;
	int hash = fhash(key);
	while (table[hash] && table[hash]->getKey() != key)
		hash = (hash + 1) % TABLE_SIZE;
	if(!table[hash]){
		lista.push_front(value);
		table[hash] = new HashEntry(key, lista);
	}
	else{
		table[hash]->getValue().push_front(value);
	}
}
