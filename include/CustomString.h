#ifndef CUSTOMSTRING_H
#define CUSTOMSTRING_H

#include <iostream>

class CustomString
{
private:
    char* m_text;
    int m_length;

    static int getLength(const char* text);
    static void copyText(char* destination, const char* source);
    static bool areEqual(const char* first, const char* second);
    static bool areEqualIgnoreCase(const char* first, const char* second);
    static char toLowerChar(char c);

public:
    CustomString();
    CustomString(const char* text);
    CustomString(const CustomString& other);
    ~CustomString();

    CustomString& operator=(const CustomString& other);

    int length() const;
    bool isEmpty() const;
    const char* cStr() const;

    void setText(const char* text);
    void clear();
    void append(const char* text);
    void truncate(int maxLength);
    void toLowerInPlace();
    void toUpperInPlace();

    bool equals(const char* text) const;
    bool equalsIgnoreCase(const char* text) const;
    bool equalsIgnoreCase(const CustomString& other) const;

    CustomString& operator+=(const CustomString& other);
    bool operator==(const CustomString& other) const;
    bool operator!=(const CustomString& other) const;

    friend std::ostream& operator<<(std::ostream& out, const CustomString& str);

    CustomString& operator+=(int value);
    CustomString& operator+=(double value);
};

#endif