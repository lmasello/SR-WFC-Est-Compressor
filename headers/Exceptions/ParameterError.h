#include <iostream>
#include <exception>

#ifndef PARAMETERERROR_H_
#define PARAMETERERROR_H_

using namespace std;

class ParameterError:
    public exception {
        public:
            virtual const char *what() const throw();
};

#endif /* PARAMETERERROR_H_ */
