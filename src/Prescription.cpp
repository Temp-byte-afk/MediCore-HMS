#include "Prescription.h"

Prescription::Prescription()
{
    m_prescriptionId = 0;
    m_appointmentId = 0;
    m_patientId = 0;
    m_doctorId = 0;
    m_date.setText("");
    m_medicines.setText("");
    m_notes.setText("");
}

Prescription::Prescription(int prescriptionId, int appointmentId, int patientId, int doctorId, const char* date, const char* medicines, const char* notes)
{
    m_prescriptionId = prescriptionId;
    m_appointmentId = appointmentId;
    m_patientId = patientId;
    m_doctorId = doctorId;
    m_date.setText(date);
    m_medicines.setText(medicines);
    m_notes.setText(notes);
}

Prescription::Prescription(const Prescription& other)
{
    m_prescriptionId = other.m_prescriptionId;
    m_appointmentId = other.m_appointmentId;
    m_patientId = other.m_patientId;
    m_doctorId = other.m_doctorId;
    m_date = other.m_date;
    m_medicines = other.m_medicines;
    m_notes = other.m_notes;
}

Prescription::~Prescription()
{
}

Prescription& Prescription::operator=(const Prescription& other)
{
    if (this != &other)
    {
        m_prescriptionId = other.m_prescriptionId;
        m_appointmentId = other.m_appointmentId;
        m_patientId = other.m_patientId;
        m_doctorId = other.m_doctorId;
        m_date = other.m_date;
        m_medicines = other.m_medicines;
        m_notes = other.m_notes;
    }
    return *this;
}

int Prescription::getPrescriptionId() const
{
    return m_prescriptionId;
}

int Prescription::getAppointmentId() const
{
    return m_appointmentId;
}

int Prescription::getPatientId() const
{
    return m_patientId;
}

int Prescription::getDoctorId() const
{
    return m_doctorId;
}

const CustomString& Prescription::getDate() const
{
    return m_date;
}

const CustomString& Prescription::getMedicines() const
{
    return m_medicines;
}

const CustomString& Prescription::getNotes() const
{
    return m_notes;
}

void Prescription::setPrescriptionId(int prescriptionId)
{
    m_prescriptionId = prescriptionId;
}

void Prescription::setAppointmentId(int appointmentId)
{
    m_appointmentId = appointmentId;
}

void Prescription::setPatientId(int patientId)
{
    m_patientId = patientId;
}

void Prescription::setDoctorId(int doctorId)
{
    m_doctorId = doctorId;
}

void Prescription::setDate(const char* date)
{
    m_date.setText(date);
}

void Prescription::setMedicines(const char* medicines)
{
    m_medicines.setText(medicines);
}

void Prescription::setNotes(const char* notes)
{
    m_notes.setText(notes);
}

int Prescription::getId() const
{
    return m_prescriptionId;
}

std::ostream& operator<<(std::ostream& out, const Prescription& prescription)
{
    out << "Prescription ID: " << prescription.m_prescriptionId
        << " | Appointment ID: " << prescription.m_appointmentId
        << " | Patient ID: " << prescription.m_patientId
        << " | Doctor ID: " << prescription.m_doctorId
        << " | Date: " << prescription.m_date
        << " | Medicines: " << prescription.m_medicines
        << " | Notes: " << prescription.m_notes;
    return out;
}