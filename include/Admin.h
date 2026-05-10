#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include <iostream>

class Admin : public Person
{
public:
    Admin();
    Admin(int id, const char* name, const char* password);
    Admin(const Admin& other);
    virtual ~Admin();

    Admin& operator=(const Admin& other);

    virtual CustomString getRole() const override;
    virtual void display() const override;

    friend std::ostream& operator<<(std::ostream& out, const Admin& admin);
};

#endif