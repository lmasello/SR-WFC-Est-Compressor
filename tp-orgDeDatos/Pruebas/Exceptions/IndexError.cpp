/*
 * IndexError.cpp
 *
 *  Created on: 25 May 2014
 *      Author: lmasel
 */
#include "IndexError.h"

const char* IndexError::what() const throw(){
	return "Incorrect index";
}




