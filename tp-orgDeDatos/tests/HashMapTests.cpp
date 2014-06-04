#include "gtest.h"
#include "Algoritmos de compresion/HashMap.h"

TEST(HashMapTest,inicializarHashMap){
	HashMap hMap;
	for(unsigned int i=0;i<256;i++){
		for(unsigned int j=0;j<256;j++){
			for(unsigned int k=0;k<256;k++){
				list<unsigned int> lista = hMap.get((char)i,(char)j,(char)k);
				ASSERT_EQ(0,lista.size());
			}
		}

	}
}

TEST(HashMapTest,posicionesHasheadas){
	HashMap hMap;
	for (unsigned int i=0;i<10;i++){
		hMap.put('Z','Z','Z',i);
	}

	list<unsigned int> lista = hMap.get('Z','Z','Z');
	ASSERT_EQ(10,lista.size());

	ASSERT_EQ(9,lista.front());

	unsigned int expected=9;
	for(list<unsigned int>::iterator value = lista.begin(); value != lista.end(); ++value){
		ASSERT_EQ(expected,*value);
		expected --;
	}
}


TEST(HashMapTest,multiplesHashes){
	HashMap hMap;
	for (unsigned int i=0;i<10;i++){
		hMap.put('Z','Z','Z',i);
	}

	for (unsigned int i=0;i<100;i+=10){
		hMap.put('f','o','o',i);
	}

	for (unsigned int i=10;i<20;i++){
		hMap.put('Z','Z','Z',i);
	}

	list<unsigned int> lista1 = hMap.get('Z','Z','Z');
	list<unsigned int> lista2 = hMap.get('f','o','o');

	ASSERT_EQ(19,lista1.front());
	ASSERT_EQ(90,lista2.front());

	unsigned int expected=19;
	for(list<unsigned int>::iterator value = lista1.begin(); value != lista1.end(); ++value){
		ASSERT_EQ(expected,*value);
		expected --;
	}
	expected=90;
	for(list<unsigned int>::iterator value = lista2.begin(); value != lista2.end(); ++value){
		ASSERT_EQ(expected,*value);
		expected -=10;
	}
}
