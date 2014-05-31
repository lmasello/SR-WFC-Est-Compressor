#include <cstddef>
#include <stdlib.h>
#include <list>
#include <iostream>
using namespace std;

#ifndef HASHMAP_H_
#define HASHMAP_H_

class HashEntry {
	private:
		char* key;
		list<unsigned int> value;
	public:
		HashEntry(char* key, list<unsigned int> value) {
			this->key = key;
			this->value = value;
		}

		char* getKey() {
			return key;
		}

		list<unsigned int> getValue() {
			return value;
		}

		void setValue(list<unsigned int> val){
			this->value = val;
		}
};

class HashMap {
	private:
		HashEntry **table;

		int fhash(char* chars);
	public:
		HashMap();
		~HashMap();

		void imprimir();

		void reset();

		/* Devuelve el valor asignado a la clave pasada por parametro. */
		list<unsigned int> get(char first, char second);

		/* Le asigna el valor value a la clave key */
		void put(char first, char second, unsigned int value);

};

#endif /* HASHMAP_H_ */
