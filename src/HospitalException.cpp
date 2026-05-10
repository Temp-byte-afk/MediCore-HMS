#include "HospitalException.h"

HospitalException::HospitalException()
{
    m_message[0] = '\0';
}

HospitalException::HospitalException(const char* message)
{
    int i = 0;
    if (message != nullptr)
    {
        while (message[i] != '\0' && i < 199)
        {
            m_message[i] = message[i];
            i++;
        }
    }
    m_message[i] = '\0';
}

HospitalException::HospitalException(const HospitalException& other)
{
    int i = 0;
    while (other.m_message[i] != '\0')
    {
        m_message[i] = other.m_message[i];
        i++;
    }
    m_message[i] = '\0';
}

HospitalException::~HospitalException()
{
}

HospitalException& HospitalException::operator=(const HospitalException& other)
{
    if (this != &other)
    {
        int i = 0;
        while (other.m_message[i] != '\0')
        {
            m_message[i] = other.m_message[i];
            i++;
        }
        m_message[i] = '\0';
    }
    return *this;
}

const char* HospitalException::what() const
{
    return m_message;
}