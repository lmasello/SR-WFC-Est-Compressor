
#include "HashMap.h"

const int TABLE_SIZE = 256 * 256 * 256;

HashMap::HashMap()
{
    table = new HashEntry *[TABLE_SIZE];

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i] = nullptr;
    }
}

HashMap::~HashMap()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (table[i])
        {
            delete table[i];
        }
    }

    delete[] table;
}

list<unsigned int> * HashMap::get(unsigned char first,
                                  unsigned char second,
                                  unsigned char third)
{
    unsigned int hash = FHASH(first, second, third);

    if (!table[hash])
    {
        return nullptr;
    }

    return (table[hash] -> getValue());
}

void HashMap::put(unsigned char first,
                  unsigned char second,
                  unsigned char third,
                  unsigned int  value)
{
    unsigned int hash = FHASH(first, second, third);

    if (!table[hash])
    {
        list<unsigned int> * new_list = new list<unsigned int>;

        new_list -> push_front(value);

        table[hash] = new HashEntry(new_list);
    }
    else
    {
        table[hash] -> addValue(value);
    }
}
