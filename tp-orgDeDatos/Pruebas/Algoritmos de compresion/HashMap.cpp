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
void HashMap::reset(){
	for(int i = 0; i<255*255; i++){
		if(!table[i]) continue;
		list <unsigned int> lista = table[i]->getValue();
		lista.clear();
		HashEntry* entry = table[i];
		entry->setValue(lista);
	}
}

list<unsigned int> HashMap::get(char first, char second){
    char* key = (char*) malloc (sizeof(char)*2);
    key[0] = first;
    key[1] = second;
	list<unsigned int> lista;
	int hash = fhash(key);
	free(key);
	if (!table[hash])
		return lista;
	return table[hash]->getValue();
}

void HashMap::put(char first, char second, unsigned int value){
    char* key = (char*) malloc (sizeof(char)*2);
    key[0] = first;
    key[1] = second;
	list <unsigned int> lista;
	int hash = fhash(key);
	if(!table[hash]){
		lista.push_front(value);
		table[hash] = new HashEntry(key, lista);
	}
	else{
		list <unsigned int> lista = table[hash]->getValue();
		lista.push_front(value);
		HashEntry* entry = table[hash];
		entry->setValue(lista);
		free(key);
	}
}
