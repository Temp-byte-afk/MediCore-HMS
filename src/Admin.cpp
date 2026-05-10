#include "Admin.h"

Admin::Admin() : Person()
{
}

Admin::Admin(int id, const char* name, const char* password)
    : Person(id, name, password)
{
}

Admin::Admin(const Admin& other) : Person(other)
{
}

Admin::~Admin()
{
}

Admin& Admin::operator=(const Admin& other)
{
    if (this != &other)
    {
        Person::operator=(other);
    }
    return *this;
}

CustomString Admin::getRole() const
{
    return CustomString("Admin");
}

void Admin::display() const
{
    std::cout << *this;
}

std::ostream& operator<<(std::ostream& out, const Admin& admin)
{
    out << "Admin ID: " << admin.m_id
        << " | Name: " << admin.m_name;
    return out;
}