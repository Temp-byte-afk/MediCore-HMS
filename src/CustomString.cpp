#include "CustomString.h"

namespace
{
    void appendIntToBuffer(int value, char* buffer, int maxLength)
    {
        if (buffer == nullptr || maxLength <= 0)
        {
            return;
        }

        int index = 0;

        if (value == 0)
        {
            if (index < maxLength - 1)
            {
                buffer[index++] = '0';
            }
            buffer[index] = '\0';
            return;
        }

        if (value < 0)
        {
            if (index < maxLength - 1)
            {
                buffer[index++] = '-';
            }
            value = -value;
        }

        char reversed[32];
        int reverseIndex = 0;
        while (value > 0 && reverseIndex < 31)
        {
            reversed[reverseIndex++] = static_cast<char>('0' + (value % 10));
            value /= 10;
        }

        for (int i = reverseIndex - 1; i >= 0 && index < maxLength - 1; i--)
        {
            buffer[index++] = reversed[i];
        }

        buffer[index] = '\0';
    }

    void appendDoubleToBuffer(double value, char* buffer, int maxLength)
    {
        if (buffer == nullptr || maxLength <= 0)
        {
            return;
        }

        int index = 0;

        if (value < 0.0)
        {
            if (index < maxLength - 1)
            {
                buffer[index++] = '-';
            }
            value = -value;
        }

        int wholePart = static_cast<int>(value);
        int fractionalPart = static_cast<int>((value - wholePart) * 100.0 + 0.5);

        if (fractionalPart >= 100)
        {
            wholePart += 1;
            fractionalPart -= 100;
        }

        char wholeBuffer[32];
        appendIntToBuffer(wholePart, wholeBuffer, 32);

        int i = 0;
        while (wholeBuffer[i] != '\0' && index < maxLength - 1)
        {
            buffer[index++] = wholeBuffer[i++];
        }

        if (index < maxLength - 1)
        {
            buffer[index++] = '.';
        }

        if (fractionalPart < 10)
        {
            if (index < maxLength - 1)
            {
                buffer[index++] = '0';
            }
            if (index < maxLength - 1)
            {
                buffer[index++] = static_cast<char>('0' + fractionalPart);
            }
        }
        else
        {
            if (index < maxLength - 1)
            {
                buffer[index++] = static_cast<char>('0' + (fractionalPart / 10));
            }
            if (index < maxLength - 1)
            {
                buffer[index++] = static_cast<char>('0' + (fractionalPart % 10));
            }
        }

        buffer[index] = '\0';
    }
}

CustomString::CustomString()
{
    m_length = 0;
    m_text = new char[1];
    m_text[0] = '\0';
}

CustomString::CustomString(const char* text)
{
    m_length = getLength(text);
    m_text = new char[m_length + 1];
    copyText(m_text, text);
}

CustomString::CustomString(const CustomString& other)
{
    m_length = other.m_length;
    m_text = new char[m_length + 1];
    copyText(m_text, other.m_text);
}

CustomString::~CustomString()
{
    delete[] m_text;
    m_text = nullptr;
    m_length = 0;
}

CustomString& CustomString::operator=(const CustomString& other)
{
    if (this != &other)
    {
        delete[] m_text;
        m_length = other.m_length;
        m_text = new char[m_length + 1];
        copyText(m_text, other.m_text);
    }
    return *this;
}

int CustomString::getLength(const char* text)
{
    if (text == nullptr)
    {
        return 0;
    }

    int length = 0;
    while (text[length] != '\0')
    {
        length++;
    }
    return length;
}

void CustomString::copyText(char* destination, const char* source)
{
    if (destination == nullptr)
    {
        return;
    }

    if (source == nullptr)
    {
        destination[0] = '\0';
        return;
    }

    int i = 0;
    while (source[i] != '\0')
    {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

bool CustomString::areEqual(const char* first, const char* second)
{
    if (first == nullptr && second == nullptr)
    {
        return true;
    }
    if (first == nullptr || second == nullptr)
    {
        return false;
    }

    int i = 0;
    while (first[i] != '\0' && second[i] != '\0')
    {
        if (first[i] != second[i])
        {
            return false;
        }
        i++;
    }

    return first[i] == '\0' && second[i] == '\0';
}

char CustomString::toLowerChar(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return static_cast<char>(c + 32);
    }
    return c;
}

bool CustomString::areEqualIgnoreCase(const char* first, const char* second)
{
    if (first == nullptr && second == nullptr)
    {
        return true;
    }
    if (first == nullptr || second == nullptr)
    {
        return false;
    }

    int i = 0;
    while (first[i] != '\0' && second[i] != '\0')
    {
        if (toLowerChar(first[i]) != toLowerChar(second[i]))
        {
            return false;
        }
        i++;
    }

    return first[i] == '\0' && second[i] == '\0';
}

int CustomString::length() const
{
    return m_length;
}

bool CustomString::isEmpty() const
{
    return m_length == 0;
}

const char* CustomString::cStr() const
{
    return m_text;
}

void CustomString::setText(const char* text)
{
    delete[] m_text;
    m_length = getLength(text);
    m_text = new char[m_length + 1];
    copyText(m_text, text);
}

void CustomString::clear()
{
    delete[] m_text;
    m_length = 0;
    m_text = new char[1];
    m_text[0] = '\0';
}

void CustomString::append(const char* text)
{
    if (text == nullptr || text[0] == '\0')
    {
        return;
    }

    int extraLength = getLength(text);
    char* newText = new char[m_length + extraLength + 1];

    int i = 0;
    while (i < m_length)
    {
        newText[i] = m_text[i];
        i++;
    }

    int j = 0;
    while (text[j] != '\0')
    {
        newText[i + j] = text[j];
        j++;
    }

    newText[i + j] = '\0';

    delete[] m_text;
    m_text = newText;
    m_length = m_length + extraLength;
}

void CustomString::truncate(int maxLength)
{
    if (maxLength < 0 || m_length <= maxLength)
    {
        return;
    }

    char* newText = new char[maxLength + 1];
    int i = 0;
    while (i < maxLength && m_text[i] != '\0')
    {
        newText[i] = m_text[i];
        i++;
    }
    newText[i] = '\0';

    delete[] m_text;
    m_text = newText;
    m_length = i;
}

void CustomString::toLowerInPlace()
{
    int i = 0;
    while (m_text[i] != '\0')
    {
        m_text[i] = toLowerChar(m_text[i]);
        i++;
    }
}

void CustomString::toUpperInPlace()
{
    int i = 0;
    while (m_text[i] != '\0')
    {
        if (m_text[i] >= 'a' && m_text[i] <= 'z')
        {
            m_text[i] = static_cast<char>(m_text[i] - 32);
        }
        i++;
    }
}

bool CustomString::equals(const char* text) const
{
    return areEqual(m_text, text);
}

bool CustomString::equalsIgnoreCase(const char* text) const
{
    return areEqualIgnoreCase(m_text, text);
}

bool CustomString::equalsIgnoreCase(const CustomString& other) const
{
    return areEqualIgnoreCase(m_text, other.m_text);
}

CustomString& CustomString::operator+=(const CustomString& other)
{
    append(other.m_text);
    return *this;
}

bool CustomString::operator==(const CustomString& other) const
{
    return areEqual(m_text, other.m_text);
}

bool CustomString::operator!=(const CustomString& other) const
{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, const CustomString& str)
{
    out << str.m_text;
    return out;
}

CustomString& CustomString::operator+=(int value)
{
    char buffer[32];
    appendIntToBuffer(value, buffer, 32);
    append(buffer);
    return *this;
}

CustomString& CustomString::operator+=(double value)
{
    char buffer[64];
    appendDoubleToBuffer(value, buffer, 64);
    append(buffer);
    return *this;
}