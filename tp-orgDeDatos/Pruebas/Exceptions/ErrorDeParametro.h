/*
 * ErrorDeParametro.h
 *
 *  Created on: May 28, 2014
 *      Author: leandro
 */

#ifndef ERRORDEPARAMETRO_H_
#define ERRORDEPARAMETRO_H_

//using standard exceptions
#include <iostream>
#include <exception>
using namespace std;

class ErrorDeParametro: public exception{
public:
	virtual const char* what() const throw();
};





#endif /* ERRORDEPARAMETRO_H_ */
