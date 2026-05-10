#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include "CustomString.h"
#include <iostream>

class Prescription
{
private:
    int m_prescriptionId;
    int m_appointmentId;
    int m_patientId;
    int m_doctorId;
    CustomString m_date;
    CustomString m_medicines;
    CustomString m_notes;

public:
    Prescription();
    Prescription(int prescriptionId, int appointmentId, int patientId, int doctorId, const char* date, const char* medicines, const char* notes);
    Prescription(const Prescription& other);
    ~Prescription();

    Prescription& operator=(const Prescription& other);

    int getPrescriptionId() const;
    int getAppointmentId() const;
    int getPatientId() const;
    int getDoctorId() const;
    const CustomString& getDate() const;
    const CustomString& getMedicines() const;
    const CustomString& getNotes() const;

    void setPrescriptionId(int prescriptionId);
    void setAppointmentId(int appointmentId);
    void setPatientId(int patientId);
    void setDoctorId(int doctorId);
    void setDate(const char* date);
    void setMedicines(const char* medicines);
    void setNotes(const char* notes);

    int getId() const;

    friend std::ostream& operator<<(std::ostream& out, const Prescription& prescription);
};

#endif