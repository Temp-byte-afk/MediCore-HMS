#include "MediCoreSystem.h"
//#include <ctime>
#include "DateUtils.h"
#include <iostream>

namespace
{
    void appendIntToText(CustomString& text, int value)
    {
        char buffer[32];
        int index = 0;

        if (value == 0)
        {
            buffer[index++] = '0';
            buffer[index] = '\0';
            text.append(buffer);
            return;
        }

        if (value < 0)
        {
            buffer[index++] = '-';
            value = -value;
        }

        char reversed[32];
        int reverseIndex = 0;
        while (value > 0 && reverseIndex < 31)
        {
            reversed[reverseIndex++] = static_cast<char>('0' + (value % 10));
            value /= 10;
        }

        for (int i = reverseIndex - 1; i >= 0; i--)
        {
            buffer[index++] = reversed[i];
        }

        buffer[index] = '\0';
        text.append(buffer);
    }

    void appendDoubleToText(CustomString& text, double value)
    {
        char buffer[64];
        int index = 0;

        if (value < 0.0)
        {
            buffer[index++] = '-';
            value = -value;
        }

        int wholePart = static_cast<int>(value);
        int fractionalPart = static_cast<int>((value - wholePart) * 100.0 + 0.5);

        if (fractionalPart >= 100)
        {
            wholePart += 1;
            fractionalPart -= 100;
        }

        if (wholePart == 0)
        {
            buffer[index++] = '0';
        }
        else
        {
            char reversed[32];
            int reverseIndex = 0;
            while (wholePart > 0 && reverseIndex < 31)
            {
                reversed[reverseIndex++] = static_cast<char>('0' + (wholePart % 10));
                wholePart /= 10;
            }

            for (int i = reverseIndex - 1; i >= 0; i--)
            {
                buffer[index++] = reversed[i];
            }
        }

        buffer[index++] = '.';
        buffer[index++] = static_cast<char>('0' + (fractionalPart / 10));
        buffer[index++] = static_cast<char>('0' + (fractionalPart % 10));
        buffer[index] = '\0';

        text.append(buffer);
    }

    void appendLine(CustomString& text, const char* line)
    {
        text.append(line);
        text.append("\n");
    }
}

MediCoreSystem::MediCoreSystem()
{
    m_patientsFile = "data/patients.txt";
    m_doctorsFile = "data/doctors.txt";
    m_adminsFile = "data/admin.txt";
    m_appointmentsFile = "data/appointments.txt";
    m_prescriptionsFile = "data/prescriptions.txt";
    m_billsFile = "data/bills.txt";
    m_securityLogFile = "data/security_log.txt";
    m_dischargedFile = "data/discharged.txt";

    m_currentRole = RoleType::None;
    m_currentUserId = 0;
    m_currentUserName.setText("");
}

void MediCoreSystem::initialize()
{
    loadAllData();
    clearSession();
}

void MediCoreSystem::loadAllData()
{
    FileHandler::loadPatients(m_patientsFile, m_patients);
    FileHandler::loadDoctors(m_doctorsFile, m_doctors);
    FileHandler::loadAdmins(m_adminsFile, m_admins);
    FileHandler::loadAppointments(m_appointmentsFile, m_appointments);
    FileHandler::loadBills(m_billsFile, m_bills);
    FileHandler::loadPrescriptions(m_prescriptionsFile, m_prescriptions);
}

void MediCoreSystem::saveAllData()
{
    FileHandler::savePatients(m_patientsFile, m_patients);
    FileHandler::saveDoctors(m_doctorsFile, m_doctors);
    FileHandler::saveAdmins(m_adminsFile, m_admins);
    FileHandler::saveAppointments(m_appointmentsFile, m_appointments);
    FileHandler::saveBills(m_billsFile, m_bills);
    FileHandler::savePrescriptions(m_prescriptionsFile, m_prescriptions);
}

void MediCoreSystem::clearSession()
{
    m_currentRole = RoleType::None;
    m_currentUserId = 0;
    m_currentUserName.setText("");
}

void MediCoreSystem::setSession(RoleType role, int userId, const char* userName)
{
    m_currentRole = role;
    m_currentUserId = userId;
    m_currentUserName.setText(userName);
}

void MediCoreSystem::logout()
{
    clearSession();
}

bool MediCoreSystem::passwordMatches(const CustomString& storedPassword, const char* enteredPassword) const
{
    return storedPassword.equals(enteredPassword);
}

void MediCoreSystem::logSecurityEvent(const char* enteredRole, int enteredId, const char* result)
{
    char timestamp[64];
    getCurrentTimestamp(timestamp, 64);
    FileHandler::logSecurityEvent(m_securityLogFile, timestamp, enteredRole, enteredId, result);
}

void MediCoreSystem::getCurrentTimestamp(char* buffer, int maxLength) const
{
    DateUtils::getCurrentTimestamp(buffer, maxLength);
}

void MediCoreSystem::getTodayDate(char* buffer, int maxLength) const
{
    DateUtils::getCurrentDate(buffer, maxLength);
}

Patient* MediCoreSystem::findPatientById(int id)
{
    return m_patients.findById(id);
}

Doctor* MediCoreSystem::findDoctorById(int id)
{
    return m_doctors.findById(id);
}

Admin* MediCoreSystem::findAdminById(int id)
{
    return m_admins.findById(id);
}

Appointment* MediCoreSystem::findAppointmentById(int id)
{
    return m_appointments.findById(id);
}

Bill* MediCoreSystem::findBillById(int id)
{
    return m_bills.findById(id);
}

Prescription* MediCoreSystem::findPrescriptionById(int id)
{
    return m_prescriptions.findById(id);
}

const Patient* MediCoreSystem::findPatientById(int id) const
{
    return m_patients.findById(id);
}

const Doctor* MediCoreSystem::findDoctorById(int id) const
{
    return m_doctors.findById(id);
}

const Admin* MediCoreSystem::findAdminById(int id) const
{
    return m_admins.findById(id);
}

const Appointment* MediCoreSystem::findAppointmentById(int id) const
{
    return m_appointments.findById(id);
}

const Bill* MediCoreSystem::findBillById(int id) const
{
    return m_bills.findById(id);
}

const Prescription* MediCoreSystem::findPrescriptionById(int id) const
{
    return m_prescriptions.findById(id);
}

bool MediCoreSystem::loginPatient(int id, const char* password)
{
    Patient* patient = findPatientById(id);

    if (patient != nullptr && passwordMatches(patient->getPassword(), password))
    {
        setSession(RoleType::Patient, id, patient->getName().cStr());
        logSecurityEvent("Patient", id, "SUCCESS");
        return true;
    }

    logSecurityEvent("Patient", id, "FAILED");
    return false;
}

bool MediCoreSystem::loginDoctor(int id, const char* password)
{
    Doctor* doctor = findDoctorById(id);

    if (doctor != nullptr && passwordMatches(doctor->getPassword(), password))
    {
        setSession(RoleType::Doctor, id, doctor->getName().cStr());
        logSecurityEvent("Doctor", id, "SUCCESS");
        return true;
    }

    logSecurityEvent("Doctor", id, "FAILED");
    return false;
}

bool MediCoreSystem::loginAdmin(int id, const char* password)
{
    Admin* admin = findAdminById(id);

    if (admin != nullptr && passwordMatches(admin->getPassword(), password))
    {
        setSession(RoleType::Admin, id, admin->getName().cStr());
        logSecurityEvent("Admin", id, "SUCCESS");
        return true;
    }

    logSecurityEvent("Admin", id, "FAILED");
    return false;
}

MediCoreSystem::RoleType MediCoreSystem::getCurrentRole() const
{
    return m_currentRole;
}

int MediCoreSystem::getCurrentUserId() const
{
    return m_currentUserId;
}

const CustomString& MediCoreSystem::getCurrentUserName() const
{
    return m_currentUserName;
}

Storage<Patient>& MediCoreSystem::getPatients()
{
    return m_patients;
}

Storage<Doctor>& MediCoreSystem::getDoctors()
{
    return m_doctors;
}

Storage<Admin>& MediCoreSystem::getAdmins()
{
    return m_admins;
}

Storage<Appointment>& MediCoreSystem::getAppointments()
{
    return m_appointments;
}

Storage<Bill>& MediCoreSystem::getBills()
{
    return m_bills;
}

Storage<Prescription>& MediCoreSystem::getPrescriptions()
{
    return m_prescriptions;
}

const Storage<Patient>& MediCoreSystem::getPatients() const
{
    return m_patients;
}

const Storage<Doctor>& MediCoreSystem::getDoctors() const
{
    return m_doctors;
}

const Storage<Admin>& MediCoreSystem::getAdmins() const
{
    return m_admins;
}

const Storage<Appointment>& MediCoreSystem::getAppointments() const
{
    return m_appointments;
}

const Storage<Bill>& MediCoreSystem::getBills() const
{
    return m_bills;
}

const Storage<Prescription>& MediCoreSystem::getPrescriptions() const
{
    return m_prescriptions;
}

bool MediCoreSystem::isPatientLoggedIn() const
{
    return m_currentRole == RoleType::Patient;
}

bool MediCoreSystem::isDoctorLoggedIn() const
{
    return m_currentRole == RoleType::Doctor;
}

bool MediCoreSystem::isAdminLoggedIn() const
{
    return m_currentRole == RoleType::Admin;
}

int MediCoreSystem::getDoctorCountForSpecialization(const char* specialization) const
{
    int count = 0;
    const Doctor* data = m_doctors.getAll();

    for (int i = 0; i < m_doctors.size(); i++)
    {
        if (data[i].getSpecialization().equalsIgnoreCase(specialization))
        {
            count++;
        }
    }

    return count;
}

int MediCoreSystem::getUnpaidBillCountForPatient(int patientId) const
{
    int count = 0;
    const Bill* data = m_bills.getAll();

    for (int i = 0; i < m_bills.size(); i++)
    {
        if (data[i].getPatientId() == patientId && data[i].isUnpaid())
        {
            count++;
        }
    }

    return count;
}

bool MediCoreSystem::isSlotAvailable(int doctorId, const char* dateText, const char* timeSlot) const
{
    const Appointment* data = m_appointments.getAll();

    for (int i = 0; i < m_appointments.size(); i++)
    {
        const Appointment& appointment = data[i];

        if (appointment.getDoctorId() == doctorId &&
            appointment.getDate().equals(dateText) &&
            appointment.getTimeSlot().equals(timeSlot) &&
            !appointment.isCancelled())
        {
            return false;
        }
    }

    return true;
}


int MediCoreSystem::bookAppointment(int patientId, int doctorId, const char* dateText, const char* timeSlot)
{
    if (!Validator::isValidDate(dateText))
    {
        throw InvalidInputException("Invalid date. Use format DD-MM-YYYY.");
    }

    if (!Validator::isValidTimeSlot(timeSlot))
    {
        throw InvalidInputException("Invalid time slot.");
    }

    Patient* patient = findPatientById(patientId);
    if (patient == nullptr)
    {
        throw InvalidInputException("Patient not found.");
    }

    Doctor* doctor = findDoctorById(doctorId);
    if (doctor == nullptr)
    {
        throw InvalidInputException("Doctor not found.");
    }

    if (!isSlotAvailable(doctorId, dateText, timeSlot))
    {
        throw SlotUnavailableException("Selected slot is unavailable.");
    }

    double fee = doctor->getFee();

    if (!patient->canAfford(fee))
    {
        throw InsufficientFundsException("Insufficient funds.");
    }

    int newAppointmentId = FileHandler::getNextAppointmentId(m_appointmentsFile);
    int newBillId = FileHandler::getNextBillId(m_billsFile);

    Appointment appointment(
        newAppointmentId,
        patientId,
        doctorId,
        dateText,
        timeSlot,
        "pending"
    );

    Bill bill(
        newBillId,
        patientId,
        newAppointmentId,
        fee,
        "unpaid",
        dateText
    );

    *patient -= fee;

    if (!m_appointments.add(appointment))
    {
        throw HospitalException("Unable to store appointment.");
    }

    if (!m_bills.add(bill))
    {
        throw HospitalException("Unable to store bill.");
    }

    FileHandler::appendAppointment(m_appointmentsFile, appointment);
    FileHandler::appendBill(m_billsFile, bill);
    FileHandler::updatePatientById(m_patientsFile, *patient);

    return newAppointmentId;
}


Bill* MediCoreSystem::findBillByAppointmentId(int appointmentId)
{
    Bill* data = m_bills.getAll();

    for (int i = 0; i < m_bills.size(); i++)
    {
        if (data[i].getAppointmentId() == appointmentId)
        {
            return &data[i];
        }
    }

    return nullptr;
}

const Bill* MediCoreSystem::findBillByAppointmentId(int appointmentId) const
{
    const Bill* data = m_bills.getAll();

    for (int i = 0; i < m_bills.size(); i++)
    {
        if (data[i].getAppointmentId() == appointmentId)
        {
            return &data[i];
        }
    }

    return nullptr;
}

double MediCoreSystem::cancelAppointment(int patientId, int appointmentId)
{
    Patient* patient = findPatientById(patientId);
    if (patient == nullptr)
    {
        throw InvalidInputException("Patient not found.");
    }

    Appointment* appointment = findAppointmentById(appointmentId);
    if (appointment == nullptr)
    {
        throw InvalidInputException("Invalid appointment ID.");
    }

    if (appointment->getPatientId() != patientId)
    {
        throw InvalidInputException("Invalid appointment ID.");
    }

    if (!appointment->isPending())
    {
        throw InvalidInputException("Only pending appointments can be cancelled.");
    }

    Bill* bill = findBillByAppointmentId(appointmentId);
    if (bill == nullptr)
    {
        throw HospitalException("Corresponding bill not found.");
    }

    Doctor* doctor = findDoctorById(appointment->getDoctorId());

    double refundAmount = bill->getAmount();
    if (doctor != nullptr)
    {
        refundAmount = doctor->getFee();
    }

    *patient += refundAmount;
    appointment->setStatus("cancelled");
    bill->setStatus("cancelled");

    FileHandler::updateAppointmentById(m_appointmentsFile, *appointment);
    FileHandler::updateBillById(m_billsFile, *bill);
    FileHandler::updatePatientById(m_patientsFile, *patient);

    return refundAmount;
}


int MediCoreSystem::compareTimeSlots(const CustomString& first, const CustomString& second) const
{
    const char* a = first.cStr();
    const char* b = second.cStr();

    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] < b[i])
        {
            return -1;
        }
        if (a[i] > b[i])
        {
            return 1;
        }
        i++;
    }

    if (a[i] == '\0' && b[i] == '\0')
    {
        return 0;
    }

    return (a[i] == '\0') ? -1 : 1;
}

int MediCoreSystem::compareAppointmentsForView(const Appointment& first, const Appointment& second) const
{
    int dateCompare = DateUtils::compareDates(first.getDate().cStr(), second.getDate().cStr());

    if (dateCompare != 0)
    {
        return dateCompare;
    }

    return compareTimeSlots(first.getTimeSlot(), second.getTimeSlot());
}

void MediCoreSystem::displayPatientAppointments(int patientId) const
{
    const Appointment* appointmentData = m_appointments.getAll();
    int count = 0;

    for (int i = 0; i < m_appointments.size(); i++)
    {
        if (appointmentData[i].getPatientId() == patientId)
        {
            count++;
        }
    }

    if (count == 0)
    {
        std::cout << "No appointments found.\n";
        return;
    }

    const Appointment** sortedAppointments = new const Appointment * [count];
    int index = 0;

    for (int i = 0; i < m_appointments.size(); i++)
    {
        if (appointmentData[i].getPatientId() == patientId)
        {
            sortedAppointments[index] = &appointmentData[i];
            index++;
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (compareAppointmentsForView(*sortedAppointments[j], *sortedAppointments[j + 1]) > 0)
            {
                const Appointment* temp = sortedAppointments[j];
                sortedAppointments[j] = sortedAppointments[j + 1];
                sortedAppointments[j + 1] = temp;
            }
        }
    }

    std::cout << "ID | Doctor Name | Specialization | Date | Time Slot | Status\n";
    std::cout << "-------------------------------------------------------------\n";

    for (int i = 0; i < count; i++)
    {
        const Appointment& appointment = *sortedAppointments[i];
        const Doctor* doctor = findDoctorById(appointment.getDoctorId());

        std::cout << appointment.getAppointmentId() << " | ";

        if (doctor != nullptr)
        {
            std::cout << doctor->getName() << " | "
                << doctor->getSpecialization() << " | ";
        }
        else
        {
            std::cout << "Unknown Doctor | Unknown | ";
        }

        std::cout << appointment.getDate() << " | "
            << appointment.getTimeSlot() << " | "
            << appointment.getStatus() << "\n";
    }

    delete[] sortedAppointments;
}


int MediCoreSystem::comparePrescriptionsForView(const Prescription& first, const Prescription& second) const
{
    int dateCompare = DateUtils::compareDates(first.getDate().cStr(), second.getDate().cStr());

    if (dateCompare != 0)
    {
        return -dateCompare;
    }

    if (first.getPrescriptionId() > second.getPrescriptionId())
    {
        return -1;
    }
    if (first.getPrescriptionId() < second.getPrescriptionId())
    {
        return 1;
    }

    return 0;
}

void MediCoreSystem::displayPatientMedicalRecords(int patientId) const
{
    const Prescription* prescriptionData = m_prescriptions.getAll();
    int count = 0;

    for (int i = 0; i < m_prescriptions.size(); i++)
    {
        if (prescriptionData[i].getPatientId() == patientId)
        {
            count++;
        }
    }

    if (count == 0)
    {
        std::cout << "No medical records found.\n";
        return;
    }

    const Prescription** sortedPrescriptions = new const Prescription * [count];
    int index = 0;

    for (int i = 0; i < m_prescriptions.size(); i++)
    {
        if (prescriptionData[i].getPatientId() == patientId)
        {
            sortedPrescriptions[index] = &prescriptionData[i];
            index++;
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (comparePrescriptionsForView(*sortedPrescriptions[j], *sortedPrescriptions[j + 1]) > 0)
            {
                const Prescription* temp = sortedPrescriptions[j];
                sortedPrescriptions[j] = sortedPrescriptions[j + 1];
                sortedPrescriptions[j + 1] = temp;
            }
        }
    }

    std::cout << "Date | Doctor Name | Medicines | Notes\n";
    std::cout << "--------------------------------------\n";

    for (int i = 0; i < count; i++)
    {
        const Prescription& prescription = *sortedPrescriptions[i];
        const Doctor* doctor = findDoctorById(prescription.getDoctorId());

        std::cout << prescription.getDate() << " | ";

        if (doctor != nullptr)
        {
            std::cout << doctor->getName() << " | ";
        }
        else
        {
            std::cout << "Unknown Doctor | ";
        }

        std::cout << prescription.getMedicines() << " | "
            << prescription.getNotes() << "\n";
    }

    delete[] sortedPrescriptions;
}

double MediCoreSystem::textToDouble(const char* text)
{
    if (text == nullptr || text[0] == '\0')
    {
        return 0.0;
    }

    double result = 0.0;
    double divisor = 1.0;
    bool afterDecimal = false;
    int i = 0;

    while (text[i] != '\0')
    {
        if (text[i] == '.')
        {
            afterDecimal = true;
        }
        else if (text[i] >= '0' && text[i] <= '9')
        {
            int digit = text[i] - '0';
            if (!afterDecimal)
            {
                result = result * 10.0 + digit;
            }
            else
            {
                divisor *= 10.0;
                result += digit / divisor;
            }
        }
        i++;
    }

    return result;
}

double MediCoreSystem::displayPatientBills(int patientId) const
{
    const Bill* billData = m_bills.getAll();
    int count = 0;
    double totalUnpaid = 0.0;

    for (int i = 0; i < m_bills.size(); i++)
    {
        if (billData[i].getPatientId() == patientId)
        {
            count++;
        }
    }

    if (count == 0)
    {
        std::cout << "No bills found.\n";
        return 0.0;
    }

    std::cout << "Bill ID | Appointment ID | Amount (PKR) | Status | Date\n";
    std::cout << "--------------------------------------------------------\n";

    for (int i = 0; i < m_bills.size(); i++)
    {
        if (billData[i].getPatientId() == patientId)
        {
            std::cout << billData[i].getBillId() << " | "
                << billData[i].getAppointmentId() << " | "
                << billData[i].getAmount() << " | "
                << billData[i].getStatus() << " | "
                << billData[i].getDate() << "\n";

            if (billData[i].isUnpaid())
            {
                totalUnpaid += billData[i].getAmount();
            }
        }
    }

    std::cout << "Total outstanding unpaid amount: PKR " << totalUnpaid << "\n";
    return totalUnpaid;
}

double MediCoreSystem::payBill(int patientId, int billId)
{
    Patient* patient = findPatientById(patientId);
    if (patient == nullptr)
    {
        throw InvalidInputException("Patient not found.");
    }

    Bill* bill = findBillById(billId);
    if (bill == nullptr)
    {
        throw InvalidInputException("Invalid bill ID.");
    }

    if (bill->getPatientId() != patientId)
    {
        throw InvalidInputException("Invalid bill ID.");
    }

    if (!bill->isUnpaid())
    {
        throw InvalidInputException("Bill is not unpaid.");
    }

    double amount = bill->getAmount();

    if (!patient->canAfford(amount))
    {
        throw InsufficientFundsException("Insufficient funds.");
    }

    *patient -= amount;
    bill->setStatus("paid");

    FileHandler::updateBillById(m_billsFile, *bill);
    FileHandler::updatePatientById(m_patientsFile, *patient);

    return patient->getBalance();
}

double MediCoreSystem::topUpBalance(int patientId, const char* amountText)
{
    if (!Validator::isValidPositiveFloat(amountText))
    {
        throw InvalidInputException("Invalid amount. Enter a positive number greater than 0.");
    }

    double amount = textToDouble(amountText);

    if (amount <= 0.0)
    {
        throw InvalidInputException("Invalid amount. Enter a positive number greater than 0.");
    }

    Patient* patient = findPatientById(patientId);
    if (patient == nullptr)
    {
        throw InvalidInputException("Patient not found.");
    }

    *patient += amount;
    FileHandler::updatePatientById(m_patientsFile, *patient);

    return patient->getBalance();
}


Prescription* MediCoreSystem::findPrescriptionByAppointmentId(int appointmentId)
{
    Prescription* data = m_prescriptions.getAll();

    for (int i = 0; i < m_prescriptions.size(); i++)
    {
        if (data[i].getAppointmentId() == appointmentId)
        {
            return &data[i];
        }
    }

    return nullptr;
}

const Prescription* MediCoreSystem::findPrescriptionByAppointmentId(int appointmentId) const
{
    const Prescription* data = m_prescriptions.getAll();

    for (int i = 0; i < m_prescriptions.size(); i++)
    {
        if (data[i].getAppointmentId() == appointmentId)
        {
            return &data[i];
        }
    }

    return nullptr;
}

bool MediCoreSystem::doctorHasCompletedAppointmentWithPatient(int doctorId, int patientId) const
{
    const Appointment* data = m_appointments.getAll();

    for (int i = 0; i < m_appointments.size(); i++)
    {
        if (data[i].getDoctorId() == doctorId &&
            data[i].getPatientId() == patientId &&
            data[i].isCompleted())
        {
            return true;
        }
    }

    return false;
}

void MediCoreSystem::displayDoctorTodaysAppointments(int doctorId) const
{
    const Appointment* appointmentData = m_appointments.getAll();
    int count = 0;
    char today[11];
    DateUtils::getCurrentDate(today, 11);

    for (int i = 0; i < m_appointments.size(); i++)
    {
        if (appointmentData[i].getDoctorId() == doctorId &&
            appointmentData[i].getDate().equals(today))
        {
            count++;
        }
    }

    if (count == 0)
    {
        std::cout << "No appointments scheduled for today.\n";
        return;
    }

    const Appointment** sortedAppointments = new const Appointment * [count];
    int index = 0;

    for (int i = 0; i < m_appointments.size(); i++)
    {
        if (appointmentData[i].getDoctorId() == doctorId &&
            appointmentData[i].getDate().equals(today))
        {
            sortedAppointments[index] = &appointmentData[i];
            index++;
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (compareTimeSlots(sortedAppointments[j]->getTimeSlot(), sortedAppointments[j + 1]->getTimeSlot()) > 0)
            {
                const Appointment* temp = sortedAppointments[j];
                sortedAppointments[j] = sortedAppointments[j + 1];
                sortedAppointments[j + 1] = temp;
            }
        }
    }

    std::cout << "Appointment ID | Patient Name | Time Slot | Status\n";
    std::cout << "--------------------------------------------------\n";

    for (int i = 0; i < count; i++)
    {
        const Appointment& appointment = *sortedAppointments[i];
        const Patient* patient = findPatientById(appointment.getPatientId());

        std::cout << appointment.getAppointmentId() << " | ";

        if (patient != nullptr)
        {
            std::cout << patient->getName() << " | ";
        }
        else
        {
            std::cout << "Unknown Patient | ";
        }

        std::cout << appointment.getTimeSlot() << " | "
            << appointment.getStatus() << "\n";
    }

    delete[] sortedAppointments;
}

void MediCoreSystem::markAppointmentComplete(int doctorId, int appointmentId)
{
    Appointment* appointment = findAppointmentById(appointmentId);
    if (appointment == nullptr)
    {
        throw InvalidInputException("Invalid appointment ID.");
    }

    if (appointment->getDoctorId() != doctorId)
    {
        throw InvalidInputException("Invalid appointment ID.");
    }

    char today[11];
    DateUtils::getCurrentDate(today, 11);

    if (!appointment->getDate().equals(today))
    {
        throw InvalidInputException("Only today's appointments can be updated.");
    }

    if (!appointment->isPending())
    {
        throw InvalidInputException("Only pending appointments can be marked.");
    }

    appointment->setStatus("completed");
    FileHandler::updateAppointmentById(m_appointmentsFile, *appointment);
}

void MediCoreSystem::markAppointmentNoShow(int doctorId, int appointmentId)
{
    Appointment* appointment = findAppointmentById(appointmentId);
    if (appointment == nullptr)
    {
        throw InvalidInputException("Invalid appointment ID.");
    }

    if (appointment->getDoctorId() != doctorId)
    {
        throw InvalidInputException("Invalid appointment ID.");
    }

    char today[11];
    DateUtils::getCurrentDate(today, 11);

    if (!appointment->getDate().equals(today))
    {
        throw InvalidInputException("Only today's appointments can be updated.");
    }

    if (!appointment->isPending())
    {
        throw InvalidInputException("Only pending appointments can be marked.");
    }

    Bill* bill = findBillByAppointmentId(appointmentId);

    appointment->setStatus("no-show");
    if (bill != nullptr)
    {
        bill->setStatus("cancelled");
        FileHandler::updateBillById(m_billsFile, *bill);
    }

    FileHandler::updateAppointmentById(m_appointmentsFile, *appointment);
}

void MediCoreSystem::writePrescription(int doctorId, int appointmentId, const char* medicines, const char* notes)
{
    Appointment* appointment = findAppointmentById(appointmentId);
    if (appointment == nullptr)
    {
        throw InvalidInputException("Invalid appointment ID.");
    }

    if (appointment->getDoctorId() != doctorId)
    {
        throw InvalidInputException("Invalid appointment ID.");
    }

    if (!appointment->isCompleted())
    {
        throw InvalidInputException("Prescription can only be written for completed appointments.");
    }

    if (findPrescriptionByAppointmentId(appointmentId) != nullptr)
    {
        throw InvalidInputException("Prescription already written for this appointment.");
    }

    CustomString medicinesText(medicines);
    CustomString notesText(notes);

    medicinesText.truncate(499);
    notesText.truncate(299);

    char today[11];
    DateUtils::getCurrentDate(today, 11);

    int newPrescriptionId = FileHandler::getNextPrescriptionId(m_prescriptionsFile);

    Prescription prescription(
        newPrescriptionId,
        appointmentId,
        appointment->getPatientId(),
        doctorId,
        today,
        medicinesText.cStr(),
        notesText.cStr()
    );

    if (!m_prescriptions.add(prescription))
    {
        throw HospitalException("Unable to store prescription.");
    }

    FileHandler::appendPrescription(m_prescriptionsFile, prescription);
}

void MediCoreSystem::displayDoctorPatientHistory(int doctorId, int patientId) const
{
    if (!doctorHasCompletedAppointmentWithPatient(doctorId, patientId))
    {
        std::cout << "Access denied. You can only view records of your own patients.\n";
        return;
    }

    const Prescription* prescriptionData = m_prescriptions.getAll();
    int count = 0;

    for (int i = 0; i < m_prescriptions.size(); i++)
    {
        if (prescriptionData[i].getDoctorId() == doctorId &&
            prescriptionData[i].getPatientId() == patientId)
        {
            count++;
        }
    }

    if (count == 0)
    {
        std::cout << "No medical records found.\n";
        return;
    }

    const Prescription** sortedPrescriptions = new const Prescription * [count];
    int index = 0;

    for (int i = 0; i < m_prescriptions.size(); i++)
    {
        if (prescriptionData[i].getDoctorId() == doctorId &&
            prescriptionData[i].getPatientId() == patientId)
        {
            sortedPrescriptions[index] = &prescriptionData[i];
            index++;
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            int cmp = DateUtils::compareDates(
                sortedPrescriptions[j]->getDate().cStr(),
                sortedPrescriptions[j + 1]->getDate().cStr()
            );

            if (cmp < 0)
            {
                const Prescription* temp = sortedPrescriptions[j];
                sortedPrescriptions[j] = sortedPrescriptions[j + 1];
                sortedPrescriptions[j + 1] = temp;
            }
        }
    }

    std::cout << "Date | Medicines | Notes\n";
    std::cout << "------------------------\n";

    for (int i = 0; i < count; i++)
    {
        const Prescription& prescription = *sortedPrescriptions[i];
        std::cout << prescription.getDate() << " | "
            << prescription.getMedicines() << " | "
            << prescription.getNotes() << "\n";
    }

    delete[] sortedPrescriptions;
}



int MediCoreSystem::compareAppointmentsForAdminView(const Appointment& first, const Appointment& second) const
{
    int dateCompare = DateUtils::compareDates(first.getDate().cStr(), second.getDate().cStr());

    if (dateCompare != 0)
    {
        return -dateCompare;
    }

    return compareTimeSlots(first.getTimeSlot(), second.getTimeSlot());
}

bool MediCoreSystem::hasPendingAppointmentsForDoctor(int doctorId) const
{
    const Appointment* data = m_appointments.getAll();

    for (int i = 0; i < m_appointments.size(); i++)
    {
        if (data[i].getDoctorId() == doctorId && data[i].isPending())
        {
            return true;
        }
    }

    return false;
}

bool MediCoreSystem::hasUnpaidBillsForPatient(int patientId) const
{
    const Bill* data = m_bills.getAll();

    for (int i = 0; i < m_bills.size(); i++)
    {
        if (data[i].getPatientId() == patientId && data[i].isUnpaid())
        {
            return true;
        }
    }

    return false;
}

bool MediCoreSystem::hasPendingAppointmentsForPatient(int patientId) const
{
    const Appointment* data = m_appointments.getAll();

    for (int i = 0; i < m_appointments.size(); i++)
    {
        if (data[i].getPatientId() == patientId && data[i].isPending())
        {
            return true;
        }
    }

    return false;
}

int MediCoreSystem::addDoctor(const char* name, const char* specialization, const char* contact, const char* password, const char* feeText)
{
    if (!Validator::isValidName(name, 50))
    {
        throw InvalidInputException("Invalid doctor name.");
    }

    if (!Validator::isValidName(specialization, 50))
    {
        throw InvalidInputException("Invalid specialization.");
    }

    if (!Validator::isValidContact(contact))
    {
        throw InvalidInputException("Invalid contact number.");
    }

    if (!Validator::isValidPassword(password))
    {
        throw InvalidInputException("Invalid password.");
    }

    if (!Validator::isValidPositiveFloat(feeText))
    {
        throw InvalidInputException("Invalid consultation fee.");
    }

    double fee = textToDouble(feeText);
    if (fee <= 0.0)
    {
        throw InvalidInputException("Invalid consultation fee.");
    }

    int newDoctorId = FileHandler::getNextDoctorId(m_doctorsFile);

    Doctor doctor(
        newDoctorId,
        name,
        specialization,
        contact,
        password,
        fee
    );

    if (!m_doctors.add(doctor))
    {
        throw HospitalException("Unable to store doctor.");
    }

    FileHandler::appendDoctor(m_doctorsFile, doctor);
    return newDoctorId;
}

bool MediCoreSystem::removeDoctor(int doctorId)
{
    Doctor* doctor = findDoctorById(doctorId);
    if (doctor == nullptr)
    {
        throw InvalidInputException("Doctor not found.");
    }

    if (hasPendingAppointmentsForDoctor(doctorId))
    {
        throw InvalidInputException("Cannot remove doctor with pending appointments. Cancel or reassign them first.");
    }

    bool removedFromMemory = m_doctors.removeById(doctorId);
    if (!removedFromMemory)
    {
        return false;
    }

    FileHandler::saveDoctors(m_doctorsFile, m_doctors);
    return true;
}

void MediCoreSystem::displayAllPatients() const
{
    const Patient* data = m_patients.getAll();

    if (m_patients.size() == 0)
    {
        std::cout << "No patients found.\n";
        return;
    }

    std::cout << "ID | Name | Age | Gender | Contact | Balance | Unpaid Bills\n";
    std::cout << "-----------------------------------------------------------\n";

    for (int i = 0; i < m_patients.size(); i++)
    {
        std::cout << data[i].getId() << " | "
            << data[i].getName() << " | "
            << data[i].getAge() << " | "
            << data[i].getGender() << " | "
            << data[i].getContact() << " | "
            << data[i].getBalance() << " | "
            << getUnpaidBillCountForPatient(data[i].getId()) << "\n";
    }
}

void MediCoreSystem::displayAllDoctors() const
{
    const Doctor* data = m_doctors.getAll();

    if (m_doctors.size() == 0)
    {
        std::cout << "No doctors found.\n";
        return;
    }

    std::cout << "ID | Name | Specialization | Contact | Fee\n";
    std::cout << "-------------------------------------------\n";

    for (int i = 0; i < m_doctors.size(); i++)
    {
        std::cout << data[i].getId() << " | "
            << data[i].getName() << " | "
            << data[i].getSpecialization() << " | "
            << data[i].getContact() << " | "
            << data[i].getFee() << "\n";
    }
}

void MediCoreSystem::displayAllAppointments() const
{
    const Appointment* appointmentData = m_appointments.getAll();
    int count = m_appointments.size();

    if (count == 0)
    {
        std::cout << "No appointments found.\n";
        return;
    }

    const Appointment** sortedAppointments = new const Appointment * [count];
    for (int i = 0; i < count; i++)
    {
        sortedAppointments[i] = &appointmentData[i];
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (compareAppointmentsForAdminView(*sortedAppointments[j], *sortedAppointments[j + 1]) > 0)
            {
                const Appointment* temp = sortedAppointments[j];
                sortedAppointments[j] = sortedAppointments[j + 1];
                sortedAppointments[j + 1] = temp;
            }
        }
    }

    std::cout << "ID | Patient Name | Doctor Name | Date | Time Slot | Status\n";
    std::cout << "-----------------------------------------------------------\n";

    for (int i = 0; i < count; i++)
    {
        const Appointment& appointment = *sortedAppointments[i];
        const Patient* patient = findPatientById(appointment.getPatientId());
        const Doctor* doctor = findDoctorById(appointment.getDoctorId());

        std::cout << appointment.getAppointmentId() << " | ";

        if (patient != nullptr)
        {
            std::cout << patient->getName() << " | ";
        }
        else
        {
            std::cout << "Unknown Patient | ";
        }

        if (doctor != nullptr)
        {
            std::cout << doctor->getName() << " | ";
        }
        else
        {
            std::cout << "Unknown Doctor | ";
        }

        std::cout << appointment.getDate() << " | "
            << appointment.getTimeSlot() << " | "
            << appointment.getStatus() << "\n";
    }

    delete[] sortedAppointments;
}

double MediCoreSystem::displayUnpaidBills() const
{
    const Bill* data = m_bills.getAll();
    int count = 0;
    double totalOutstanding = 0.0;

    for (int i = 0; i < m_bills.size(); i++)
    {
        if (data[i].isUnpaid())
        {
            count++;
        }
    }

    if (count == 0)
    {
        std::cout << "No unpaid bills found.\n";
        return 0.0;
    }

    char today[11];
    DateUtils::getCurrentDate(today, 11);
    long long todaySerial = DateUtils::dateToSerial(today);

    std::cout << "Bill ID | Patient Name | Amount (PKR) | Date\n";
    std::cout << "---------------------------------------------\n";

    for (int i = 0; i < m_bills.size(); i++)
    {
        if (data[i].isUnpaid())
        {
            const Patient* patient = findPatientById(data[i].getPatientId());
            long long billSerial = DateUtils::dateToSerial(data[i].getDate().cStr());
            bool overdue = false;

            if (todaySerial > billSerial && (todaySerial - billSerial) > 7)
            {
                overdue = true;
            }

            std::cout << data[i].getBillId() << " | ";

            if (patient != nullptr)
            {
                std::cout << patient->getName() << " | ";
            }
            else
            {
                std::cout << "Unknown Patient | ";
            }

            std::cout << data[i].getAmount() << " | " << data[i].getDate();

            if (overdue)
            {
                std::cout << " [OVERDUE]";
            }

            std::cout << "\n";
            totalOutstanding += data[i].getAmount();
        }
    }

    std::cout << "Total outstanding unpaid amount: PKR " << totalOutstanding << "\n";
    return totalOutstanding;
}

bool MediCoreSystem::dischargePatient(int patientId)
{
    Patient* patient = findPatientById(patientId);
    if (patient == nullptr)
    {
        throw InvalidInputException("Patient not found.");
    }

    if (hasUnpaidBillsForPatient(patientId))
    {
        throw InvalidInputException("Cannot discharge patient with unpaid bills.");
    }

    if (hasPendingAppointmentsForPatient(patientId))
    {
        throw InvalidInputException("Cannot discharge patient with pending appointments.");
    }

    FileHandler::appendPatient(m_dischargedFile, *patient);

    Storage<Appointment> updatedAppointments;
    const Appointment* appointmentData = m_appointments.getAll();
    for (int i = 0; i < m_appointments.size(); i++)
    {
        if (appointmentData[i].getPatientId() != patientId)
        {
            updatedAppointments.add(appointmentData[i]);
        }
    }

    Storage<Bill> updatedBills;
    const Bill* billData = m_bills.getAll();
    for (int i = 0; i < m_bills.size(); i++)
    {
        if (billData[i].getPatientId() != patientId)
        {
            updatedBills.add(billData[i]);
        }
    }

    Storage<Prescription> updatedPrescriptions;
    const Prescription* prescriptionData = m_prescriptions.getAll();
    for (int i = 0; i < m_prescriptions.size(); i++)
    {
        if (prescriptionData[i].getPatientId() != patientId)
        {
            updatedPrescriptions.add(prescriptionData[i]);
        }
    }

    m_appointments = updatedAppointments;
    m_bills = updatedBills;
    m_prescriptions = updatedPrescriptions;

    m_patients.removeById(patientId);

    FileHandler::saveAppointments(m_appointmentsFile, m_appointments);
    FileHandler::saveBills(m_billsFile, m_bills);
    FileHandler::savePrescriptions(m_prescriptionsFile, m_prescriptions);
    FileHandler::savePatients(m_patientsFile, m_patients);

    return true;
}

void MediCoreSystem::displaySecurityLog() const
{
    bool hasContent = FileHandler::displayTextFile(m_securityLogFile);

    if (!hasContent)
    {
        std::cout << "No security events logged.\n";
    }
}

void MediCoreSystem::generateDailyReport() const
{
    //char today[11];
    //DateUtils::getCurrentDate(today, 11);

    //int totalToday = 0;
    //int pendingToday = 0;
    //int completedToday = 0;
    //int noShowToday = 0;
    //int cancelledToday = 0;
    //double revenueToday = 0.0;

    //const Appointment* appointmentData = m_appointments.getAll();
    //const Bill* billData = m_bills.getAll();

    //for (int i = 0; i < m_appointments.size(); i++)
    //{
    //    if (appointmentData[i].getDate().equals(today))
    //    {
    //        totalToday++;

    //        if (appointmentData[i].isPending())
    //        {
    //            pendingToday++;
    //        }
    //        else if (appointmentData[i].isCompleted())
    //        {
    //            completedToday++;
    //        }
    //        else if (appointmentData[i].isNoShow())
    //        {
    //            noShowToday++;
    //        }
    //        else if (appointmentData[i].isCancelled())
    //        {
    //            cancelledToday++;
    //        }
    //    }
    //}

    //for (int i = 0; i < m_bills.size(); i++)
    //{
    //    if (billData[i].getDate().equals(today) && billData[i].isPaid())
    //    {
    //        revenueToday += billData[i].getAmount();
    //    }
    //}

    //std::cout << "Total appointments today: " << totalToday
    //    << " (Pending: " << pendingToday
    //    << " Completed: " << completedToday
    //    << " No-show: " << noShowToday
    //    << " Cancelled: " << cancelledToday << ")\n";

    //std::cout << "Revenue collected today (paid bills): PKR " << revenueToday << "\n";

    //std::cout << "Patients with outstanding unpaid bills:\n";
    //std::cout << "Patient Name | Total Owed\n";
    //std::cout << "-------------------------\n";

    //const Patient* patientData = m_patients.getAll();
    //for (int i = 0; i < m_patients.size(); i++)
    //{
    //    double owed = 0.0;
    //    for (int j = 0; j < m_bills.size(); j++)
    //    {
    //        if (billData[j].getPatientId() == patientData[i].getId() && billData[j].isUnpaid())
    //        {
    //            owed += billData[j].getAmount();
    //        }
    //    }

    //    if (owed > 0.0)
    //    {
    //        std::cout << patientData[i].getName() << " | PKR " << owed << "\n";
    //    }
    //}

    //std::cout << "Doctor-wise summary for today:\n";
    //std::cout << "Doctor Name | Completed | Pending | No-show\n";
    //std::cout << "--------------------------------------------\n";

    //const Doctor* doctorData = m_doctors.getAll();
    //for (int i = 0; i < m_doctors.size(); i++)
    //{
    //    int completed = 0;
    //    int pending = 0;
    //    int noShow = 0;

    //    for (int j = 0; j < m_appointments.size(); j++)
    //    {
    //        if (appointmentData[j].getDoctorId() == doctorData[i].getId() &&
    //            appointmentData[j].getDate().equals(today))
    //        {
    //            if (appointmentData[j].isCompleted())
    //            {
    //                completed++;
    //            }
    //            else if (appointmentData[j].isPending())
    //            {
    //                pending++;
    //            }
    //            else if (appointmentData[j].isNoShow())
    //            {
    //                noShow++;
    //            }
    //        }
    //    }

    //    if (completed > 0 || pending > 0 || noShow > 0)
    //    {
    //        std::cout << doctorData[i].getName() << " | "
    //            << completed << " | "
    //            << pending << " | "
    //            << noShow << "\n";
    //    }
    //}


    CustomString report;
    getDailyReportContent(report);
    std::cout << report.cStr();

}

bool MediCoreSystem::getSecurityLogContent(CustomString& output) const
{
    return FileHandler::loadTextFile(m_securityLogFile, output);
}


bool MediCoreSystem::getDailyReportContent(CustomString& output) const
{
    output.clear();

    char today[11];
    DateUtils::getCurrentDate(today, 11);

    int totalToday = 0;
    int pendingToday = 0;
    int completedToday = 0;
    int noShowToday = 0;
    int cancelledToday = 0;
    double revenueToday = 0.0;

    const Appointment* appointmentData = m_appointments.getAll();
    const Bill* billData = m_bills.getAll();
    const Patient* patientData = m_patients.getAll();
    const Doctor* doctorData = m_doctors.getAll();

    for (int i = 0; i < m_appointments.size(); i++)
    {
        if (appointmentData[i].getDate().equals(today))
        {
            totalToday++;

            if (appointmentData[i].isPending())
            {
                pendingToday++;
            }
            else if (appointmentData[i].isCompleted())
            {
                completedToday++;
            }
            else if (appointmentData[i].isNoShow())
            {
                noShowToday++;
            }
            else if (appointmentData[i].isCancelled())
            {
                cancelledToday++;
            }
        }
    }

    for (int i = 0; i < m_bills.size(); i++)
    {
        if (billData[i].getDate().equals(today) && billData[i].isPaid())
        {
            revenueToday += billData[i].getAmount();
        }
    }

    appendLine(output, "Total appointments today: ");
    appendIntToText(output, totalToday);
    output.append(" (Pending: ");
    appendIntToText(output, pendingToday);
    output.append(" Completed: ");
    appendIntToText(output, completedToday);
    output.append(" No-show: ");
    appendIntToText(output, noShowToday);
    output.append(" Cancelled: ");
    appendIntToText(output, cancelledToday);
    output.append(")\n");

    output.append("Revenue collected today (paid bills): PKR ");
    appendDoubleToText(output, revenueToday);
    output.append("\n");

    appendLine(output, "Patients with outstanding unpaid bills:");
    appendLine(output, "Patient Name | Total Owed");

    bool anyPatientOwes = false;
    for (int i = 0; i < m_patients.size(); i++)
    {
        double owed = 0.0;
        for (int j = 0; j < m_bills.size(); j++)
        {
            if (billData[j].getPatientId() == patientData[i].getId() && billData[j].isUnpaid())
            {
                owed += billData[j].getAmount();
            }
        }

        if (owed > 0.0)
        {
            output += patientData[i].getName();
            output.append(" | PKR ");
            appendDoubleToText(output, owed);
            output.append("\n");
            anyPatientOwes = true;
        }
    }

    if (!anyPatientOwes)
    {
        appendLine(output, "None");
    }

    appendLine(output, "Doctor-wise summary for today:");
    appendLine(output, "Doctor Name | Completed | Pending | No-show");

    bool anyDoctorShown = false;
    for (int i = 0; i < m_doctors.size(); i++)
    {
        int completed = 0;
        int pending = 0;
        int noShow = 0;

        for (int j = 0; j < m_appointments.size(); j++)
        {
            if (appointmentData[j].getDoctorId() == doctorData[i].getId() &&
                appointmentData[j].getDate().equals(today))
            {
                if (appointmentData[j].isCompleted())
                {
                    completed++;
                }
                else if (appointmentData[j].isPending())
                {
                    pending++;
                }
                else if (appointmentData[j].isNoShow())
                {
                    noShow++;
                }
            }
        }

        if (completed > 0 || pending > 0 || noShow > 0)
        {
            output += doctorData[i].getName();
            output.append(" | ");
            appendIntToText(output, completed);
            output.append(" | ");
            appendIntToText(output, pending);
            output.append(" | ");
            appendIntToText(output, noShow);
            output.append("\n");
            anyDoctorShown = true;
        }
    }

    if (!anyDoctorShown)
    {
        appendLine(output, "None");
    }

    return true;
}