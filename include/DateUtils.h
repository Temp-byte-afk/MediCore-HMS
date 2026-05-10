#ifndef DATEUTILS_H
#define DATEUTILS_H

class DateUtils
{
private:
    static bool isLeapYear(int year);
    static void appendTwoDigits(char* buffer, int& index, int value);
    static void appendFourDigits(char* buffer, int& index, int value);
    static int charToInt(const char* text, int startIndex, int length);

public:
    static bool parseDate(const char* dateText, int& day, int& month, int& year);
    static bool isValidDate(const char* dateText);
    static bool isToday(const char* dateText);
    static int compareDates(const char* firstDate, const char* secondDate);
    static long long dateToSerial(const char* dateText);
    static long long daysBetween(const char* firstDate, const char* secondDate);

    static void getCurrentDate(char* buffer, int maxLength);
    static void getCurrentTimestamp(char* buffer, int maxLength);
};

#endif