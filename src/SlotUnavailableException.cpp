#include "SlotUnavailableException.h"

SlotUnavailableException::SlotUnavailableException()
    : HospitalException("Selected slot is unavailable.")
{
}

SlotUnavailableException::SlotUnavailableException(const char* message)
    : HospitalException(message)
{
}