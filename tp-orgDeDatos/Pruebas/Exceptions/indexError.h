/*
 * indexError.h
 *
 *  Created on: 25 May 2014
 *      Author: lmasel
 */

#ifndef INDEXERROR_H_
#define INDEXERROR_H_


//using standard exceptions
#include <iostream>
#include <exception>
using namespace std;

class IndexError: public exception{
public:
	virtual const char* what() const throw();
};



#endif /* INDEXERROR_H_ */
