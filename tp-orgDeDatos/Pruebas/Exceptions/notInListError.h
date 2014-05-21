/*
 * notInListError.h
 *
 *  Created on: May 21, 2014
 *      Author: leandro
 */

#ifndef NOTINLISTERROR_H_
#define NOTINLISTERROR_H_

//using standard exceptions
#include <iostream>
#include <exception>
using namespace std;

class notInListError: public exception{
public:
	virtual const char* what() const throw();
};



#endif /* NOTINLISTERROR_H_ */
