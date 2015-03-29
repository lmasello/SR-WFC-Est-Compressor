
#include <cstddef>
#include <stdlib.h>
#include <list>
#include <iostream>

using namespace std;
#define FHASH(a, b, c) (a * 256*256 + b*256 + c)

#ifndef HASHMAP_H_
#define HASHMAP_H_
class HashEntry
{
    private:
        list<unsigned int> * value;

    public:
        HashEntry(list<unsigned int> * value)
        {
            this -> value = value;
        }

        ~HashEntry()
        {
            delete value;
        }

        list<unsigned int> * getValue()
        {
            return value;
        }

        void setValue(list<unsigned int> * val)
        {
            this -> value = val;
        }

        void addValue(unsigned int value)
        {
            this -> value -> push_front(value);
        }
};


class HashMap
{
    private:
        HashEntry ** table;

    public:
        HashMap();

        ~HashMap();

        /* Returns the value to the key formed by the three char. */
        list<unsigned int> * get(char first,
                                 char second,
                                 char third);

        /* Assigns the value "value" to the key formed by the three char. */
        void put(char         first,
                 char         second,
                 char         third,
                 unsigned int value);
};
#endif /* HASHMAP_H_ */
