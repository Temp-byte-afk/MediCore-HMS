#ifndef DOCTOR_H
#define DOCTOR_H

#include "Person.h"
#include <iostream>

class Doctor : public Person
{
private:
    CustomString m_specialization;
    CustomString m_contact;
    double m_fee;

public:
    Doctor();
    Doctor(int id, const char* name, const char* specialization, const char* contact, const char* password, double fee);
    Doctor(const Doctor& other);
    virtual ~Doctor();

    Doctor& operator=(const Doctor& other);

    bool operator==(const Doctor& other) const;

    const CustomString& getSpecialization() const;
    const CustomString& getContact() const;
    double getFee() const;

    void setSpecialization(const char* specialization);
    void setContact(const char* contact);
    void setFee(double fee);

    virtual CustomString getRole() const override;
    virtual void display() const override;

    friend std::ostream& operator<<(std::ostream& out, const Doctor& doctor);
};

#endif