#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"
#include <iostream>

class Patient : public Person
{
private:
    int m_age;
    CustomString m_gender;
    CustomString m_contact;
    double m_balance;

public:
    Patient();
    Patient(int id, const char* name, int age, const char* gender, const char* contact, const char* password, double balance);
    Patient(const Patient& other);
    virtual ~Patient();

    Patient& operator=(const Patient& other);

    Patient& operator+=(double amount);
    Patient& operator-=(double amount);
    bool operator==(const Patient& other) const;

    int getAge() const;
    const CustomString& getGender() const;
    const CustomString& getContact() const;
    double getBalance() const;

    void setAge(int age);
    void setGender(const char* gender);
    void setContact(const char* contact);
    void setBalance(double balance);

    bool canAfford(double amount) const;

    virtual CustomString getRole() const override;
    virtual void display() const override;

    friend std::ostream& operator<<(std::ostream& out, const Patient& patient);
};

#endif