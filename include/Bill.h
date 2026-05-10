#ifndef BILL_H
#define BILL_H

#include "CustomString.h"
#include <iostream>

class Bill
{
private:
    int m_billId;
    int m_patientId;
    int m_appointmentId;
    double m_amount;
    CustomString m_status;
    CustomString m_date;

public:
    Bill();
    Bill(int billId, int patientId, int appointmentId, double amount, const char* status, const char* date);
    Bill(const Bill& other);
    ~Bill();

    Bill& operator=(const Bill& other);

    int getBillId() const;
    int getPatientId() const;
    int getAppointmentId() const;
    double getAmount() const;
    const CustomString& getStatus() const;
    const CustomString& getDate() const;

    void setBillId(int billId);
    void setPatientId(int patientId);
    void setAppointmentId(int appointmentId);
    void setAmount(double amount);
    void setStatus(const char* status);
    void setDate(const char* date);

    bool isPaid() const;
    bool isUnpaid() const;
    bool isCancelled() const;

    int getId() const;

    friend std::ostream& operator<<(std::ostream& out, const Bill& bill);
};

#endif