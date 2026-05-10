#include "Patient.h"

Patient::Patient() : Person()
{
    m_age = 0;
    m_gender.setText("");
    m_contact.setText("");
    m_balance = 0.0;
}

Patient::Patient(int id, const char* name, int age, const char* gender, const char* contact, const char* password, double balance)
    : Person(id, name, password)
{
    m_age = age;
    m_gender.setText(gender);
    m_contact.setText(contact);
    m_balance = balance;
}

Patient::Patient(const Patient& other) : Person(other)
{
    m_age = other.m_age;
    m_gender = other.m_gender;
    m_contact = other.m_contact;
    m_balance = other.m_balance;
}

Patient::~Patient()
{
}

Patient& Patient::operator=(const Patient& other)
{
    if (this != &other)
    {
        Person::operator=(other);
        m_age = other.m_age;
        m_gender = other.m_gender;
        m_contact = other.m_contact;
        m_balance = other.m_balance;
    }
    return *this;
}

Patient& Patient::operator+=(double amount)
{
    if (amount > 0)
    {
        m_balance += amount;
    }
    return *this;
}

Patient& Patient::operator-=(double amount)
{
    if (amount > 0)
    {
        m_balance -= amount;
        if (m_balance < 0)
        {
            m_balance = 0;
        }
    }
    return *this;
}

bool Patient::operator==(const Patient& other) const
{
    return m_id == other.m_id;
}

int Patient::getAge() const
{
    return m_age;
}

const CustomString& Patient::getGender() const
{
    return m_gender;
}

const CustomString& Patient::getContact() const
{
    return m_contact;
}

double Patient::getBalance() const
{
    return m_balance;
}

void Patient::setAge(int age)
{
    m_age = age;
}

void Patient::setGender(const char* gender)
{
    m_gender.setText(gender);
}

void Patient::setContact(const char* contact)
{
    m_contact.setText(contact);
}

void Patient::setBalance(double balance)
{
    m_balance = balance;
}

bool Patient::canAfford(double amount) const
{
    return m_balance >= amount;
}

CustomString Patient::getRole() const
{
    return CustomString("Patient");
}

void Patient::display() const
{
    std::cout << *this;
}

std::ostream& operator<<(std::ostream& out, const Patient& patient)
{
    out << "Patient ID: " << patient.m_id
        << " | Name: " << patient.m_name
        << " | Age: " << patient.m_age
        << " | Gender: " << patient.m_gender
        << " | Contact: " << patient.m_contact
        << " | Balance: PKR " << patient.m_balance;
    return out;
}