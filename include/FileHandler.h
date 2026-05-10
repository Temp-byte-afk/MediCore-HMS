#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileNotFoundException.h"

class FileHandler
{
private:
    static const int m_maxLineLength = 1024;
    static const int m_maxFieldLength = 512;

    static void trimNewLine(char* text);
    static void clearText(char* text, int size);
    static void copyField(const char* source, char* destination, int maxLength);
    static void getField(const char* line, int fieldIndex, char* output, int maxLength);
    static int toInt(const char* text);
    static double toDouble(const char* text);
    static void writePatientLine(std::ofstream& file, const Patient& patient);
    static void writeDoctorLine(std::ofstream& file, const Doctor& doctor);
    static void writeAdminLine(std::ofstream& file, const Admin& admin);
    static void writeAppointmentLine(std::ofstream& file, const Appointment& appointment);
    static void writeBillLine(std::ofstream& file, const Bill& bill);
    static void writePrescriptionLine(std::ofstream& file, const Prescription& prescription);

public:
    FileHandler();

    static bool fileExists(const char* fileName);

    static void loadPatients(const char* fileName, Storage<Patient>& patients);
    static void loadDoctors(const char* fileName, Storage<Doctor>& doctors);
    static void loadAdmins(const char* fileName, Storage<Admin>& admins);
    static void loadAppointments(const char* fileName, Storage<Appointment>& appointments);
    static void loadBills(const char* fileName, Storage<Bill>& bills);
    static void loadPrescriptions(const char* fileName, Storage<Prescription>& prescriptions);

    static void savePatients(const char* fileName, const Storage<Patient>& patients);
    static void saveDoctors(const char* fileName, const Storage<Doctor>& doctors);
    static void saveAdmins(const char* fileName, const Storage<Admin>& admins);
    static void saveAppointments(const char* fileName, const Storage<Appointment>& appointments);
    static void saveBills(const char* fileName, const Storage<Bill>& bills);
    static void savePrescriptions(const char* fileName, const Storage<Prescription>& prescriptions);

    static void appendPatient(const char* fileName, const Patient& patient);
    static void appendDoctor(const char* fileName, const Doctor& doctor);
    static void appendAdmin(const char* fileName, const Admin& admin);
    static void appendAppointment(const char* fileName, const Appointment& appointment);
    static void appendBill(const char* fileName, const Bill& bill);
    static void appendPrescription(const char* fileName, const Prescription& prescription);
    static void appendLine(const char* fileName, const char* line);

    static bool deletePatientById(const char* fileName, int patientId);
    static bool deleteDoctorById(const char* fileName, int doctorId);
    static bool deleteAdminById(const char* fileName, int adminId);
    static bool deleteAppointmentById(const char* fileName, int appointmentId);
    static bool deleteBillById(const char* fileName, int billId);
    static bool deletePrescriptionById(const char* fileName, int prescriptionId);

    static bool updatePatientById(const char* fileName, const Patient& updatedPatient);
    static bool updateDoctorById(const char* fileName, const Doctor& updatedDoctor);
    static bool updateAdminById(const char* fileName, const Admin& updatedAdmin);
    static bool updateAppointmentById(const char* fileName, const Appointment& updatedAppointment);
    static bool updateBillById(const char* fileName, const Bill& updatedBill);
    static bool updatePrescriptionById(const char* fileName, const Prescription& updatedPrescription);

    static int getNextPatientId(const char* fileName);
    static int getNextDoctorId(const char* fileName);
    static int getNextAdminId(const char* fileName);
    static int getNextAppointmentId(const char* fileName);
    static int getNextBillId(const char* fileName);
    static int getNextPrescriptionId(const char* fileName);

    static void logSecurityEvent(const char* fileName, const char* timestamp, const char* role, int enteredId, const char* result);

    static bool displayTextFile(const char* fileName);

    static bool loadTextFile(const char* fileName, CustomString& output);
};

#endif