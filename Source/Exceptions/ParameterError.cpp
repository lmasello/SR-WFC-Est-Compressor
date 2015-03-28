#include "ParameterError.h"

const char * ParameterError::what() const throw ()
{
    return "An incorrect parameter has been passed.";
}
