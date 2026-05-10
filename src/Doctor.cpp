#include "Doctor.h"

Doctor::Doctor() : Person()
{
    m_specialization.setText("");
    m_contact.setText("");
    m_fee = 0.0;
}

Doctor::Doctor(int id, const char* name, const char* specialization, const char* contact, const char* password, double fee)
    : Person(id, name, password)
{
    m_specialization.setText(specialization);
    m_contact.setText(contact);
    m_fee = fee;
}

Doctor::Doctor(const Doctor& other) : Person(other)
{
    m_specialization = other.m_specialization;
    m_contact = other.m_contact;
    m_fee = other.m_fee;
}

Doctor::~Doctor()
{
}

Doctor& Doctor::operator=(const Doctor& other)
{
    if (this != &other)
    {
        Person::operator=(other);
        m_specialization = other.m_specialization;
        m_contact = other.m_contact;
        m_fee = other.m_fee;
    }
    return *this;
}

bool Doctor::operator==(const Doctor& other) const
{
    return m_id == other.m_id;
}

const CustomString& Doctor::getSpecialization() const
{
    return m_specialization;
}

const CustomString& Doctor::getContact() const
{
    return m_contact;
}

double Doctor::getFee() const
{
    return m_fee;
}

void Doctor::setSpecialization(const char* specialization)
{
    m_specialization.setText(specialization);
}

void Doctor::setContact(const char* contact)
{
    m_contact.setText(contact);
}

void Doctor::setFee(double fee)
{
    m_fee = fee;
}

CustomString Doctor::getRole() const
{
    return CustomString("Doctor");
}

void Doctor::display() const
{
    std::cout << *this;
}

std::ostream& operator<<(std::ostream& out, const Doctor& doctor)
{
    out << "Doctor ID: " << doctor.m_id
        << " | Name: " << doctor.m_name
        << " | Specialization: " << doctor.m_specialization
        << " | Contact: " << doctor.m_contact
        << " | Fee: PKR " << doctor.m_fee;
    return out;
}