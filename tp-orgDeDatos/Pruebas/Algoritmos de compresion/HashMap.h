#include <cstddef>
#include <stdlib.h>
#include <list>
#include <iostream>
using namespace std;

#ifndef HASHMAP_H_
#define HASHMAP_H_

class HashEntry {
	private:
		list<unsigned int>* value;
	public:
		HashEntry(list<unsigned int>* value) {
			this->value = value;
		}

		list<unsigned int>* getValue() {
			return value;
		}

		void setValue(list<unsigned int>* val){
			this->value = val;
		}

		void addValue(unsigned int value){
			this->value->push_front(value);
			//Si la lista contiene 150 elementos, borra el ultimo y agrega uno al principio
			unsigned short tamanioMaximo = 1500;
			if (this->value->size() == tamanioMaximo) this->value->pop_back();
		}
};

class HashMap {
	private:
		HashEntry **table;

	public:
		HashMap();
		~HashMap();

		void imprimir();

		unsigned int fhash(char primero, char segundo, char tercero);

		void reset();

		/* Devuelve el valor asignado a la clave pasada por parametro. */
		list<unsigned int>* get(char first, char second, char third);

		/* Le asigna el valor value a la clave key */
		void put(char first, char second, char third, unsigned int value);

};

#endif /* HASHMAP_H_ */
