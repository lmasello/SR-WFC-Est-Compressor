#include <cstddef>
#include <stdlib.h>
#include <list>

using namespace std;

#ifndef HASHMAP_H_
#define HASHMAP_H_

class HashEntry {
	private:
		char* key;
		list<unsigned long> value;
	public:
		HashEntry(char* key, list<unsigned long> value) {
			this->key = key;
			this->value = value;
		}

		char* getKey() {
			return key;
		}

		list<unsigned long> getValue() {
			return value;
		}
};

class HashMap {
	private:
		HashEntry **table;

		int fhash(char* chars);
	public:
		HashMap();
		~HashMap();

		/* Devuelve el valor asignado a la clave pasada por parametro. */
		list<unsigned long> get(char first, char second);

		/* Le asigna el valor value a la clave key */
		void put(char first, char second, unsigned long value);

};

#endif /* HASHMAP_H_ */
