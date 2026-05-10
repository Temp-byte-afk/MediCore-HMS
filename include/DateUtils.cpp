#include "DateUtils.h"
#include <ctime>

bool DateUtils::isLeapYear(int year)
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

void DateUtils::appendTwoDigits(char* buffer, int& index, int value)
{
    if (value < 10)
    {
        buffer[index++] = '0';
        buffer[index++] = static_cast<char>('0' + value);
    }
    else
    {
        buffer[index++] = static_cast<char>('0' + (value / 10));
        buffer[index++] = static_cast<char>('0' + (value % 10));
    }
}

void DateUtils::appendFourDigits(char* buffer, int& index, int value)
{
    int thousands = value / 1000;
    int hundreds = (value / 100) % 10;
    int tens = (value / 10) % 10;
    int ones = value % 10;

    buffer[index++] = static_cast<char>('0' + thousands);
    buffer[index++] = static_cast<char>('0' + hundreds);
    buffer[index++] = static_cast<char>('0' + tens);
    buffer[index++] = static_cast<char>('0' + ones);
}

int DateUtils::charToInt(const char* text, int startIndex, int length)
{
    int value = 0;
    for (int i = 0; i < length; i++)
    {
        char c = text[startIndex + i];
        if (c < '0' || c > '9')
        {
            return -1;
        }
        value = value * 10 + (c - '0');
    }
    return value;
}

bool DateUtils::parseDate(const char* dateText, int& day, int& month, int& year)
{
    day = 0;
    month = 0;
    year = 0;

    if (dateText == nullptr)
    {
        return false;
    }

    int length = 0;
    while (dateText[length] != '\0')
    {
        length++;
    }

    if (length != 10)
    {
        return false;
    }

    if (dateText[2] != '-' || dateText[5] != '-')
    {
        return false;
    }

    day = charToInt(dateText, 0, 2);
    month = charToInt(dateText, 3, 2);
    year = charToInt(dateText, 6, 4);

    if (day == -1 || month == -1 || year == -1)
    {
        return false;
    }

    return true;
}

bool DateUtils::isValidDate(const char* dateText)
{
    int day = 0;
    int month = 0;
    int year = 0;

    if (!parseDate(dateText, day, month, year))
    {
        return false;
    }

    if (month < 1 || month > 12)
    {
        return false;
    }

    if (year < 1900)
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

void DateUtils::getCurrentDate(char* buffer, int maxLength)
{
    if (buffer == nullptr || maxLength <= 0)
    {
        return;
    }

    //time_t now = time(nullptr);
    //tm* localTime = localtime(&now);

    //int day = localTime->tm_mday;
    //int month = localTime->tm_mon + 1;
    //int year = localTime->tm_year + 1900;

    time_t now = time(nullptr);
    tm localTime{};

    localtime_s(&localTime, &now);

    int day = localTime.tm_mday;
    int month = localTime.tm_mon + 1;
    int year = localTime.tm_year + 1900;

    int index = 0;

    if (index < maxLength - 1)
    {
        appendTwoDigits(buffer, index, day);
    }
    if (index < maxLength - 1)
    {
        buffer[index++] = '-';
    }
    if (index < maxLength - 1)
    {
        appendTwoDigits(buffer, index, month);
    }
    if (index < maxLength - 1)
    {
        buffer[index++] = '-';
    }
    if (index < maxLength - 1)
    {
        appendFourDigits(buffer, index, year);
    }

    if (index < maxLength)
    {
        buffer[index] = '\0';
    }
    else
    {
        buffer[maxLength - 1] = '\0';
    }
}

void DateUtils::getCurrentTimestamp(char* buffer, int maxLength)
{
    if (buffer == nullptr || maxLength <= 0)
    {
        return;
    }

    //time_t now = time(nullptr);
    //tm* localTime = localtime(&now);

    //int day = localTime->tm_mday;
    //int month = localTime->tm_mon + 1;
    //int year = localTime->tm_year + 1900;
    //int hour = localTime->tm_hour;
    //int minute = localTime->tm_min;
    //int second = localTime->tm_sec;

    time_t now = time(nullptr);
    tm localTime{};

    localtime_s(&localTime, &now);

    int day = localTime.tm_mday;
    int month = localTime.tm_mon + 1;
    int year = localTime.tm_year + 1900;
    int hour = localTime.tm_hour;
    int minute = localTime.tm_min;
    int second = localTime.tm_sec;

    int index = 0;

    if (index < maxLength - 1) appendTwoDigits(buffer, index, day);
    if (index < maxLength - 1) buffer[index++] = '-';
    if (index < maxLength - 1) appendTwoDigits(buffer, index, month);
    if (index < maxLength - 1) buffer[index++] = '-';
    if (index < maxLength - 1) appendFourDigits(buffer, index, year);
    if (index < maxLength - 1) buffer[index++] = ' ';
    if (index < maxLength - 1) appendTwoDigits(buffer, index, hour);
    if (index < maxLength - 1) buffer[index++] = ':';
    if (index < maxLength - 1) appendTwoDigits(buffer, index, minute);
    if (index < maxLength - 1) buffer[index++] = ':';
    if (index < maxLength - 1) appendTwoDigits(buffer, index, second);

    if (index < maxLength)
    {
        buffer[index] = '\0';
    }
    else
    {
        buffer[maxLength - 1] = '\0';
    }
}

bool DateUtils::isToday(const char* dateText)
{
    char today[11];
    getCurrentDate(today, 11);

    int firstDay = 0;
    int firstMonth = 0;
    int firstYear = 0;

    int secondDay = 0;
    int secondMonth = 0;
    int secondYear = 0;

    if (!parseDate(dateText, firstDay, firstMonth, firstYear))
    {
        return false;
    }

    if (!parseDate(today, secondDay, secondMonth, secondYear))
    {
        return false;
    }

    return firstDay == secondDay && firstMonth == secondMonth && firstYear == secondYear;
}

int DateUtils::compareDates(const char* firstDate, const char* secondDate)
{
    int day1 = 0, month1 = 0, year1 = 0;
    int day2 = 0, month2 = 0, year2 = 0;

    if (!parseDate(firstDate, day1, month1, year1))
    {
        return 0;
    }

    if (!parseDate(secondDate, day2, month2, year2))
    {
        return 0;
    }

    if (year1 < year2) return -1;
    if (year1 > year2) return 1;

    if (month1 < month2) return -1;
    if (month1 > month2) return 1;

    if (day1 < day2) return -1;
    if (day1 > day2) return 1;

    return 0;
}

long long DateUtils::dateToSerial(const char* dateText)
{
    int day = 0;
    int month = 0;
    int year = 0;

    if (!parseDate(dateText, day, month, year))
    {
        return 0;
    }

    if (!isValidDate(dateText))
    {
        return 0;
    }

    if (month <= 2)
    {
        year--;
        month += 12;
    }

    long long serial = 365LL * year;
    serial += year / 4;
    serial -= year / 100;
    serial += year / 400;
    serial += (153LL * (month - 3) + 2) / 5;
    serial += day;

    return serial;
}

long long DateUtils::daysBetween(const char* firstDate, const char* secondDate)
{
    long long firstSerial = dateToSerial(firstDate);
    long long secondSerial = dateToSerial(secondDate);

    if (firstSerial > secondSerial)
    {
        return firstSerial - secondSerial;
    }

    return secondSerial - firstSerial;
}