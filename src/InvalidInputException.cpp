#include "InvalidInputException.h"

InvalidInputException::InvalidInputException()
    : HospitalException("Invalid input.")
{
}

InvalidInputException::InvalidInputException(const char* message)
    : HospitalException(message)
{
}