
#ifndef PARAMETERERROR_H_
#define PARAMETERERROR_H_

// using standard exceptions

#include <iostream>
#include <exception>

using namespace std;

class ParameterError:
    public exception
{
    public:
        virtual const char * what() const throw ();
};
#endif /* ERRORDEPARAMETRO_H_ */
