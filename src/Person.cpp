#include "Person.h"

Person::Person()
{
    m_id = 0;
    m_name.setText("");
    m_password.setText("");
}

Person::Person(int id, const char* name, const char* password)
{
    m_id = id;
    m_name.setText(name);
    m_password.setText(password);
}

Person::Person(const Person& other)
{
    m_id = other.m_id;
    m_name = other.m_name;
    m_password = other.m_password;
}

Person::~Person()
{
}

Person& Person::operator=(const Person& other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_name = other.m_name;
        m_password = other.m_password;
    }
    return *this;
}

int Person::getId() const
{
    return m_id;
}

const CustomString& Person::getName() const
{
    return m_name;
}

const CustomString& Person::getPassword() const
{
    return m_password;
}

void Person::setId(int id)
{
    m_id = id;
}

void Person::setName(const char* name)
{
    m_name.setText(name);
}

void Person::setPassword(const char* password)
{
    m_password.setText(password);
}