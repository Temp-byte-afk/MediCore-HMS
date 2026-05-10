#include "Appointment.h"

Appointment::Appointment()
{
    m_appointmentId = 0;
    m_patientId = 0;
    m_doctorId = 0;
    m_date.setText("");
    m_timeSlot.setText("");
    m_status.setText("");
}

Appointment::Appointment(int appointmentId, int patientId, int doctorId, const char* date, const char* timeSlot, const char* status)
{
    m_appointmentId = appointmentId;
    m_patientId = patientId;
    m_doctorId = doctorId;
    m_date.setText(date);
    m_timeSlot.setText(timeSlot);
    m_status.setText(status);
}

Appointment::Appointment(const Appointment& other)
{
    m_appointmentId = other.m_appointmentId;
    m_patientId = other.m_patientId;
    m_doctorId = other.m_doctorId;
    m_date = other.m_date;
    m_timeSlot = other.m_timeSlot;
    m_status = other.m_status;
}

Appointment::~Appointment()
{
}

Appointment& Appointment::operator=(const Appointment& other)
{
    if (this != &other)
    {
        m_appointmentId = other.m_appointmentId;
        m_patientId = other.m_patientId;
        m_doctorId = other.m_doctorId;
        m_date = other.m_date;
        m_timeSlot = other.m_timeSlot;
        m_status = other.m_status;
    }
    return *this;
}

bool Appointment::operator==(const Appointment& other) const
{
    if (isCancelled() || other.isCancelled())
    {
        return false;
    }

    return m_doctorId == other.m_doctorId
        && m_date == other.m_date
        && m_timeSlot == other.m_timeSlot;
}

int Appointment::getAppointmentId() const
{
    return m_appointmentId;
}

int Appointment::getPatientId() const
{
    return m_patientId;
}

int Appointment::getDoctorId() const
{
    return m_doctorId;
}

const CustomString& Appointment::getDate() const
{
    return m_date;
}

const CustomString& Appointment::getTimeSlot() const
{
    return m_timeSlot;
}

const CustomString& Appointment::getStatus() const
{
    return m_status;
}

void Appointment::setAppointmentId(int appointmentId)
{
    m_appointmentId = appointmentId;
}

void Appointment::setPatientId(int patientId)
{
    m_patientId = patientId;
}

void Appointment::setDoctorId(int doctorId)
{
    m_doctorId = doctorId;
}

void Appointment::setDate(const char* date)
{
    m_date.setText(date);
}

void Appointment::setTimeSlot(const char* timeSlot)
{
    m_timeSlot.setText(timeSlot);
}

void Appointment::setStatus(const char* status)
{
    m_status.setText(status);
}

bool Appointment::isCancelled() const
{
    return m_status.equalsIgnoreCase("cancelled");
}

bool Appointment::isPending() const
{
    return m_status.equalsIgnoreCase("pending");
}

bool Appointment::isCompleted() const
{
    return m_status.equalsIgnoreCase("completed");
}

bool Appointment::isNoShow() const
{
    return m_status.equalsIgnoreCase("no-show");
}

int Appointment::getId() const
{
    return m_appointmentId;
}

std::ostream& operator<<(std::ostream& out, const Appointment& appointment)
{
    out << "Appointment ID: " << appointment.m_appointmentId
        << " | Patient ID: " << appointment.m_patientId
        << " | Doctor ID: " << appointment.m_doctorId
        << " | Date: " << appointment.m_date
        << " | Time: " << appointment.m_timeSlot
        << " | Status: " << appointment.m_status;
    return out;
}