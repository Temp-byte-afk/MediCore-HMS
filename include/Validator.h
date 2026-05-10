#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "CustomString.h"

class Validator
{
private:
    static bool isLeapYear(int year);
    static bool isNumeric(const char* text);
    static int getCurrentYear();

public:
    static bool isValidId(const char* text);
    static bool isValidDate(const char* text);
    static bool isValidTimeSlot(const char* text);
    static bool isValidContact(const char* text);
    static bool isValidPassword(const char* text);
    static bool isValidPositiveFloat(const char* text);
    static bool isValidMenuChoice(const char* text, int minChoice, int maxChoice);
    static bool isValidSpecialization(const char* text);
    static bool isValidName(const char* text, int maxLength);
    static bool isValidNotesLength(const char* text, int maxLength);
};

#endif