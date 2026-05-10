#ifndef MEDICORESYSTEM_H
#define MEDICORESYSTEM_H

#include "Storage.h"
#include "FileHandler.h"
#include "Validator.h"
#include "HospitalException.h"
#include "InsufficientFundsException.h"
#include "InvalidInputException.h"
#include "SlotUnavailableException.h"

class MediCoreSystem
{
public:
    enum class RoleType
    {
        None,
        Patient,
        Doctor,
        Admin
    };

private:
    Storage<Patient> m_patients;
    Storage<Doctor> m_doctors;
    Storage<Admin> m_admins;
    Storage<Appointment> m_appointments;
    Storage<Bill> m_bills;
    Storage<Prescription> m_prescriptions;

    const char* m_patientsFile;
    const char* m_doctorsFile;
    const char* m_adminsFile;
    const char* m_appointmentsFile;
    const char* m_prescriptionsFile;
    const char* m_billsFile;
    const char* m_securityLogFile;
    const char* m_dischargedFile;

    RoleType m_currentRole;
    int m_currentUserId;
    CustomString m_currentUserName;

private:
    Patient* findPatientById(int id);
    Doctor* findDoctorById(int id);
    Admin* findAdminById(int id);
    Appointment* findAppointmentById(int id);
    Bill* findBillById(int id);
    Prescription* findPrescriptionById(int id);

    const Patient* findPatientById(int id) const;
    const Doctor* findDoctorById(int id) const;
    const Admin* findAdminById(int id) const;
    const Appointment* findAppointmentById(int id) const;
    const Bill* findBillById(int id) const;
    const Prescription* findPrescriptionById(int id) const;

    void clearSession();
    void setSession(RoleType role, int userId, const char* userName);

    void logSecurityEvent(const char* enteredRole, int enteredId, const char* result);
    void getCurrentTimestamp(char* buffer, int maxLength) const;
    void getTodayDate(char* buffer, int maxLength) const;

    bool passwordMatches(const CustomString& storedPassword, const char* enteredPassword) const;

    bool isSlotAvailable(int doctorId, const char* dateText, const char* timeSlot) const;

    Bill* findBillByAppointmentId(int appointmentId);
    const Bill* findBillByAppointmentId(int appointmentId) const;

    int compareAppointmentsForView(const Appointment& first, const Appointment& second) const;
    int compareTimeSlots(const CustomString& first, const CustomString& second) const;

    int comparePrescriptionsForView(const Prescription& first, const Prescription& second) const;

    static double textToDouble(const char* text);

    bool doctorHasCompletedAppointmentWithPatient(int doctorId, int patientId) const;
    Prescription* findPrescriptionByAppointmentId(int appointmentId);
    const Prescription* findPrescriptionByAppointmentId(int appointmentId) const;


    int compareAppointmentsForAdminView(const Appointment& first, const Appointment& second) const;
    bool hasPendingAppointmentsForDoctor(int doctorId) const;
    bool hasUnpaidBillsForPatient(int patientId) const;
    bool hasPendingAppointmentsForPatient(int patientId) const;

public:
    MediCoreSystem();

    void initialize();
    void loadAllData();
    void saveAllData();

    bool loginPatient(int id, const char* password);
    bool loginDoctor(int id, const char* password);
    bool loginAdmin(int id, const char* password);

    void logout();

    RoleType getCurrentRole() const;
    int getCurrentUserId() const;
    const CustomString& getCurrentUserName() const;

    Storage<Patient>& getPatients();
    Storage<Doctor>& getDoctors();
    Storage<Admin>& getAdmins();
    Storage<Appointment>& getAppointments();
    Storage<Bill>& getBills();
    Storage<Prescription>& getPrescriptions();

    const Storage<Patient>& getPatients() const;
    const Storage<Doctor>& getDoctors() const;
    const Storage<Admin>& getAdmins() const;
    const Storage<Appointment>& getAppointments() const;
    const Storage<Bill>& getBills() const;
    const Storage<Prescription>& getPrescriptions() const;

    bool isPatientLoggedIn() const;
    bool isDoctorLoggedIn() const;
    bool isAdminLoggedIn() const;

    int getDoctorCountForSpecialization(const char* specialization) const;
    int getUnpaidBillCountForPatient(int patientId) const;


    int bookAppointment(int patientId, int doctorId, const char* dateText, const char* timeSlot);

    double cancelAppointment(int patientId, int appointmentId);

    void displayPatientAppointments(int patientId) const;

    void displayPatientMedicalRecords(int patientId) const;

    double displayPatientBills(int patientId) const;
    double payBill(int patientId, int billId);
    double topUpBalance(int patientId, const char* amountText);

    void displayDoctorTodaysAppointments(int doctorId) const;
    void markAppointmentComplete(int doctorId, int appointmentId);
    void markAppointmentNoShow(int doctorId, int appointmentId);
    void writePrescription(int doctorId, int appointmentId, const char* medicines, const char* notes);
    void displayDoctorPatientHistory(int doctorId, int patientId) const;

    int addDoctor(const char* name, const char* specialization, const char* contact, const char* password, const char* feeText);
    bool removeDoctor(int doctorId);

    void displayAllPatients() const;
    void displayAllDoctors() const;
    void displayAllAppointments() const;
    double displayUnpaidBills() const;

    bool dischargePatient(int patientId);
    void displaySecurityLog() const;
    void generateDailyReport() const;

    bool getSecurityLogContent(CustomString& output) const;

    bool getDailyReportContent(CustomString& output) const;
};

#endif