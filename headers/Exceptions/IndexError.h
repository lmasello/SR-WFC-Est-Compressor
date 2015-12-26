#include <iostream>
#include <exception>

#ifndef INDEXERROR_H_
#define INDEXERROR_H_

using namespace std;

class IndexError:
    public exception {
        public:
            virtual const char *what() const throw();
};

#endif /* INDEXERROR_H_ */
