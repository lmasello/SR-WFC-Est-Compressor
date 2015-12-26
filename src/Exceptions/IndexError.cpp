#include "../../headers/Exceptions/IndexError.h"

const char *IndexError::what() const throw() {
    return "Incorrect index";
}
