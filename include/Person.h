#ifndef PERSON_H
#define PERSON_H

#include "CustomString.h"

class Person
{
protected:
    int m_id;
    CustomString m_name;
    CustomString m_password;

public:
    Person();
    Person(int id, const char* name, const char* password);
    Person(const Person& other);
    virtual ~Person();

    Person& operator=(const Person& other);

    int getId() const;
    const CustomString& getName() const;
    const CustomString& getPassword() const;

    void setId(int id);
    void setName(const char* name);
    void setPassword(const char* password);

    virtual CustomString getRole() const = 0;
    virtual void display() const = 0;
};

#endif