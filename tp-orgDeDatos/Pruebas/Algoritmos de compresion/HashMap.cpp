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

void HashMap::imprimir(){
	for(int i = 0; i < 256; i++){
		for(int j = 0; j<256; j++){
			for(int k=0;k<256;k++){
				list <unsigned int> lista = table[i*256*256+j*256+k]->getValue();
				cout << lista.size();
			}
		}
		cout << endl;
	}
}

void HashMap::reset(){
	for(int i = 0; i<TABLE_SIZE; i++){
		if(!table[i]) continue;
		list <unsigned int> lista = table[i]->getValue();
		lista.clear();
		HashEntry* entry = table[i];
		entry->setValue(lista);
	}
}

list<unsigned int> HashMap::get(char first, char second, char third){
	list<unsigned int> lista;
	unsigned int hash = fhash(first, second, third);
	if (!table[hash])
		return lista;
	return table[hash]->getValue();
}

void HashMap::put(char first, char second, char third, unsigned int value){
	unsigned int hash = fhash(first, second, third);
	if(!table[hash]){
	    char* key = (char*) malloc (sizeof(char)*3);
	    key[0] = first; key[1] = second; key[2] = third;
		list <unsigned int> lista;
		lista.push_front(value);
		table[hash] = new HashEntry(key, lista);
	}
	else{
		table[hash]->addValue(value);
	}
}
