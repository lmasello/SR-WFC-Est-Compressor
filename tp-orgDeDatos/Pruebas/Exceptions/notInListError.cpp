#include "notInListError.h"

class notInListError: public exception
{
	virtual const char* what() const throw()
		{
			return "The element you are looking for is not in the list";
		}
}notInList;
