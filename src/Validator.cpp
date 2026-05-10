#include "Validator.h"
#include <ctime>

bool Validator::isLeapYear(int year)
{
    if (year % 400 == 0)
    {
        return true;
    }
    if (year % 100 == 0)
    {
        return false;
    }
    return (year % 4 == 0);
}

bool Validator::isNumeric(const char* text)
{
    if (text == nullptr || text[0] == '\0')
    {
        return false;
    }

    int i = 0;
    while (text[i] != '\0')
    {
        if (text[i] < '0' || text[i] > '9')
        {
            return false;
        }
        i++;
    }
    return true;
}

int Validator::getCurrentYear()
{
    //time_t now = time(nullptr);
    //tm* local = localtime(&now);
    //return 1900 + local->tm_year;
    time_t now = time(nullptr);
    tm local{};

    localtime_s(&local, &now);
    return 1900 + local.tm_year;
}

bool Validator::isValidId(const char* text)
{
    return isNumeric(text);
}

bool Validator::isValidDate(const char* text)
{
    if (text == nullptr)
    {
        return false;
    }

    int len = 0;
    while (text[len] != '\0')
    {
        len++;
    }

    if (len != 10)
    {
        return false;
    }

    if (text[2] != '-' || text[5] != '-')
    {
        return false;
    }

    if (!isNumeric(text) && !(text[0] >= '0' && text[0] <= '9'))
    {
        return false;
    }

    char dayText[3];
    char monthText[3];
    char yearText[5];

    dayText[0] = text[0];
    dayText[1] = text[1];
    dayText[2] = '\0';

    monthText[0] = text[3];
    monthText[1] = text[4];
    monthText[2] = '\0';

    yearText[0] = text[6];
    yearText[1] = text[7];
    yearText[2] = text[8];
    yearText[3] = text[9];
    yearText[4] = '\0';

    if (!isNumeric(dayText) || !isNumeric(monthText) || !isNumeric(yearText))
    {
        return false;
    }

    int day = (dayText[0] - '0') * 10 + (dayText[1] - '0');
    int month = (monthText[0] - '0') * 10 + (monthText[1] - '0');
    int year = (yearText[0] - '0') * 1000 + (yearText[1] - '0') * 100 + (yearText[2] - '0') * 10 + (yearText[3] - '0');

    if (month < 1 || month > 12)
    {
        return false;
    }

    if (year < getCurrentYear())
    {
        return false;
    }

    int maxDay = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        maxDay = 30;
    }
    else if (month == 2)
    {
        maxDay = isLeapYear(year) ? 29 : 28;
    }

    if (day < 1 || day > maxDay)
    {
        return false;
    }

    return true;
}

bool Validator::isValidTimeSlot(const char* text)
{
    if (text == nullptr)
    {
        return false;
    }

    const char* validSlots[] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00" };

    for (int i = 0; i < 8; i++)
    {
        CustomString slot(validSlots[i]);
        if (slot.equals(text))
        {
            return true;
        }
    }

    return false;
}

bool Validator::isValidContact(const char* text)
{
    if (text == nullptr)
    {
        return false;
    }

    int len = 0;
    while (text[len] != '\0')
    {
        if (text[len] < '0' || text[len] > '9')
        {
            return false;
        }
        len++;
    }

    return len == 11;
}

bool Validator::isValidPassword(const char* text)
{
    if (text == nullptr)
    {
        return false;
    }

    int len = 0;
    while (text[len] != '\0')
    {
        len++;
    }

    return len >= 6;
}

bool Validator::isValidPositiveFloat(const char* text)
{
    if (text == nullptr || text[0] == '\0')
    {
        return false;
    }

    bool hasDecimal = false;
    int i = 0;
    while (text[i] != '\0')
    {
        if (text[i] == '.')
        {
            if (hasDecimal)
            {
                return false;
            }
            hasDecimal = true;
        }
        else if (text[i] < '0' || text[i] > '9')
        {
            return false;
        }
        i++;
    }

    if (text[0] == '0' && text[1] == '\0')
    {
        return false;
    }

    return true;
}

bool Validator::isValidMenuChoice(const char* text, int minChoice, int maxChoice)
{
    if (!isNumeric(text))
    {
        return false;
    }

    int value = 0;
    int i = 0;
    while (text[i] != '\0')
    {
        value = value * 10 + (text[i] - '0');
        i++;
    }

    return value >= minChoice && value <= maxChoice;
}

bool Validator::isValidSpecialization(const char* text)
{
    return text != nullptr && text[0] != '\0';
}

bool Validator::isValidName(const char* text, int maxLength)
{
    if (text == nullptr || text[0] == '\0')
    {
        return false;
    }

    int len = 0;
    while (text[len] != '\0')
    {
        len++;
    }

    return len <= maxLength;
}

bool Validator::isValidNotesLength(const char* text, int maxLength)
{
    if (text == nullptr)
    {
        return false;
    }

    int len = 0;
    while (text[len] != '\0')
    {
        len++;
    }

    return len <= maxLength;
}