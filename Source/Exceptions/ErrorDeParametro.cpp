/*
 * ErrorDeParametro.cpp
 *
 *  Created on: May 28, 2014
 *      Author: leandro
 */
#include "ErrorDeParametro.h"

const char* ErrorDeParametro::what() const throw(){
	return "Ha pasado un parametro incorrecto";
}






