#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include "CustomString.h"
#include <iostream>

class Appointment
{
private:
    int m_appointmentId;
    int m_patientId;
    int m_doctorId;
    CustomString m_date;
    CustomString m_timeSlot;
    CustomString m_status;

public:
    Appointment();
    Appointment(int appointmentId, int patientId, int doctorId, const char* date, const char* timeSlot, const char* status);
    Appointment(const Appointment& other);
    ~Appointment();

    Appointment& operator=(const Appointment& other);

    bool operator==(const Appointment& other) const;

    int getAppointmentId() const;
    int getPatientId() const;
    int getDoctorId() const;
    const CustomString& getDate() const;
    const CustomString& getTimeSlot() const;
    const CustomString& getStatus() const;

    void setAppointmentId(int appointmentId);
    void setPatientId(int patientId);
    void setDoctorId(int doctorId);
    void setDate(const char* date);
    void setTimeSlot(const char* timeSlot);
    void setStatus(const char* status);

    bool isCancelled() const;
    bool isPending() const;
    bool isCompleted() const;
    bool isNoShow() const;

    int getId() const;

    friend std::ostream& operator<<(std::ostream& out, const Appointment& appointment);
};

#endif