#include "Bill.h"

Bill::Bill()
{
    m_billId = 0;
    m_patientId = 0;
    m_appointmentId = 0;
    m_amount = 0.0;
    m_status.setText("");
    m_date.setText("");
}

Bill::Bill(int billId, int patientId, int appointmentId, double amount, const char* status, const char* date)
{
    m_billId = billId;
    m_patientId = patientId;
    m_appointmentId = appointmentId;
    m_amount = amount;
    m_status.setText(status);
    m_date.setText(date);
}

Bill::Bill(const Bill& other)
{
    m_billId = other.m_billId;
    m_patientId = other.m_patientId;
    m_appointmentId = other.m_appointmentId;
    m_amount = other.m_amount;
    m_status = other.m_status;
    m_date = other.m_date;
}

Bill::~Bill()
{
}

Bill& Bill::operator=(const Bill& other)
{
    if (this != &other)
    {
        m_billId = other.m_billId;
        m_patientId = other.m_patientId;
        m_appointmentId = other.m_appointmentId;
        m_amount = other.m_amount;
        m_status = other.m_status;
        m_date = other.m_date;
    }
    return *this;
}

int Bill::getBillId() const
{
    return m_billId;
}

int Bill::getPatientId() const
{
    return m_patientId;
}

int Bill::getAppointmentId() const
{
    return m_appointmentId;
}

double Bill::getAmount() const
{
    return m_amount;
}

const CustomString& Bill::getStatus() const
{
    return m_status;
}

const CustomString& Bill::getDate() const
{
    return m_date;
}

void Bill::setBillId(int billId)
{
    m_billId = billId;
}

void Bill::setPatientId(int patientId)
{
    m_patientId = patientId;
}

void Bill::setAppointmentId(int appointmentId)
{
    m_appointmentId = appointmentId;
}

void Bill::setAmount(double amount)
{
    m_amount = amount;
}

void Bill::setStatus(const char* status)
{
    m_status.setText(status);
}

void Bill::setDate(const char* date)
{
    m_date.setText(date);
}

bool Bill::isPaid() const
{
    return m_status.equalsIgnoreCase("paid");
}

bool Bill::isUnpaid() const
{
    return m_status.equalsIgnoreCase("unpaid");
}

bool Bill::isCancelled() const
{
    return m_status.equalsIgnoreCase("cancelled");
}

int Bill::getId() const
{
    return m_billId;
}

std::ostream& operator<<(std::ostream& out, const Bill& bill)
{
    out << "Bill ID: " << bill.m_billId
        << " | Patient ID: " << bill.m_patientId
        << " | Appointment ID: " << bill.m_appointmentId
        << " | Amount: PKR " << bill.m_amount
        << " | Status: " << bill.m_status
        << " | Date: " << bill.m_date;
    return out;
}