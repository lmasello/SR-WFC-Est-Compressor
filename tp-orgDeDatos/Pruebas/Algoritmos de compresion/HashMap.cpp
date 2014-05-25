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
	list<unsigned long> lista;
	int hash = fhash(key);
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
	if(!table[hash]){
		lista.push_front(value);
		table[hash] = new HashEntry(key, lista);
	}
	else{
		list <unsigned long> lista = table[hash]->getValue();
		lista.push_front(value);
		table[hash] = new HashEntry(key, lista);
	}
}
