#include "FileHandler.h"
#include <iostream>
#include <ctime>

FileHandler::FileHandler()
{
}

void FileHandler::trimNewLine(char* text)
{
    if (text == nullptr)
    {
        return;
    }

    int i = 0;
    while (text[i] != '\0')
    {
        if (text[i] == '\n' || text[i] == '\r')
        {
            text[i] = '\0';
            return;
        }
        i++;
    }
}

void FileHandler::clearText(char* text, int size)
{
    if (text == nullptr || size <= 0)
    {
        return;
    }

    for (int i = 0; i < size; i++)
    {
        text[i] = '\0';
    }
}

void FileHandler::copyField(const char* source, char* destination, int maxLength)
{
    if (destination == nullptr || maxLength <= 0)
    {
        return;
    }

    int i = 0;
    if (source != nullptr)
    {
        while (source[i] != '\0' && i < maxLength - 1)
        {
            destination[i] = source[i];
            i++;
        }
    }
    destination[i] = '\0';
}

void FileHandler::getField(const char* line, int fieldIndex, char* output, int maxLength)
{
    clearText(output, maxLength);

    if (line == nullptr || output == nullptr)
    {
        return;
    }

    int currentField = 0;
    int outputIndex = 0;
    int i = 0;

    while (line[i] != '\0')
    {
        if (currentField == fieldIndex)
        {
            if (line[i] == ',')
            {
                break;
            }

            if (outputIndex < maxLength - 1)
            {
                output[outputIndex] = line[i];
                outputIndex++;
            }
        }

        if (line[i] == ',')
        {
            currentField++;
            if (currentField > fieldIndex)
            {
                break;
            }
        }

        i++;
    }

    output[outputIndex] = '\0';
}

int FileHandler::toInt(const char* text)
{
    if (text == nullptr || text[0] == '\0')
    {
        return 0;
    }

    int i = 0;
    int sign = 1;
    int result = 0;

    if (text[0] == '-')
    {
        sign = -1;
        i = 1;
    }

    while (text[i] != '\0')
    {
        if (text[i] >= '0' && text[i] <= '9')
        {
            result = result * 10 + (text[i] - '0');
        }
        i++;
    }

    return result * sign;
}

double FileHandler::toDouble(const char* text)
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
                result = result + (digit / divisor);
            }
        }
        i++;
    }

    return result;
}

bool FileHandler::fileExists(const char* fileName)
{
    std::ifstream file(fileName);
    return file.is_open();
}

void FileHandler::writePatientLine(std::ofstream& file, const Patient& patient)
{
    file << patient.getId() << ","
        << patient.getName() << ","
        << patient.getAge() << ","
        << patient.getGender() << ","
        << patient.getContact() << ","
        << patient.getPassword() << ","
        << patient.getBalance();
}

void FileHandler::writeDoctorLine(std::ofstream& file, const Doctor& doctor)
{
    file << doctor.getId() << ","
        << doctor.getName() << ","
        << doctor.getSpecialization() << ","
        << doctor.getContact() << ","
        << doctor.getPassword() << ","
        << doctor.getFee();
}

void FileHandler::writeAdminLine(std::ofstream& file, const Admin& admin)
{
    file << admin.getId() << ","
        << admin.getName() << ","
        << admin.getPassword();
}

void FileHandler::writeAppointmentLine(std::ofstream& file, const Appointment& appointment)
{
    file << appointment.getAppointmentId() << ","
        << appointment.getPatientId() << ","
        << appointment.getDoctorId() << ","
        << appointment.getDate() << ","
        << appointment.getTimeSlot() << ","
        << appointment.getStatus();
}

void FileHandler::writeBillLine(std::ofstream& file, const Bill& bill)
{
    file << bill.getBillId() << ","
        << bill.getPatientId() << ","
        << bill.getAppointmentId() << ","
        << bill.getAmount() << ","
        << bill.getStatus() << ","
        << bill.getDate();
}

void FileHandler::writePrescriptionLine(std::ofstream& file, const Prescription& prescription)
{
    file << prescription.getPrescriptionId() << ","
        << prescription.getAppointmentId() << ","
        << prescription.getPatientId() << ","
        << prescription.getDoctorId() << ","
        << prescription.getDate() << ","
        << prescription.getMedicines() << ","
        << prescription.getNotes();
}

void FileHandler::loadPatients(const char* fileName, Storage<Patient>& patients)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not open patients file.");
    }

    char line[m_maxLineLength];
    while (file.getline(line, m_maxLineLength))
    {
        trimNewLine(line);
        if (line[0] == '\0')
        {
            continue;
        }

        char idText[m_maxFieldLength];
        char name[m_maxFieldLength];
        char ageText[m_maxFieldLength];
        char gender[m_maxFieldLength];
        char contact[m_maxFieldLength];
        char password[m_maxFieldLength];
        char balanceText[m_maxFieldLength];

        getField(line, 0, idText, m_maxFieldLength);
        getField(line, 1, name, m_maxFieldLength);
        getField(line, 2, ageText, m_maxFieldLength);
        getField(line, 3, gender, m_maxFieldLength);
        getField(line, 4, contact, m_maxFieldLength);
        getField(line, 5, password, m_maxFieldLength);
        getField(line, 6, balanceText, m_maxFieldLength);

        Patient patient(
            toInt(idText),
            name,
            toInt(ageText),
            gender,
            contact,
            password,
            toDouble(balanceText)
        );

        patients.add(patient);
    }
}

void FileHandler::loadDoctors(const char* fileName, Storage<Doctor>& doctors)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not open doctors file.");
    }

    char line[m_maxLineLength];
    while (file.getline(line, m_maxLineLength))
    {
        trimNewLine(line);
        if (line[0] == '\0')
        {
            continue;
        }

        char idText[m_maxFieldLength];
        char name[m_maxFieldLength];
        char specialization[m_maxFieldLength];
        char contact[m_maxFieldLength];
        char password[m_maxFieldLength];
        char feeText[m_maxFieldLength];

        getField(line, 0, idText, m_maxFieldLength);
        getField(line, 1, name, m_maxFieldLength);
        getField(line, 2, specialization, m_maxFieldLength);
        getField(line, 3, contact, m_maxFieldLength);
        getField(line, 4, password, m_maxFieldLength);
        getField(line, 5, feeText, m_maxFieldLength);

        Doctor doctor(
            toInt(idText),
            name,
            specialization,
            contact,
            password,
            toDouble(feeText)
        );

        doctors.add(doctor);
    }
}

void FileHandler::loadAdmins(const char* fileName, Storage<Admin>& admins)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not open admin file.");
    }

    char line[m_maxLineLength];
    while (file.getline(line, m_maxLineLength))
    {
        trimNewLine(line);
        if (line[0] == '\0')
        {
            continue;
        }

        char idText[m_maxFieldLength];
        char name[m_maxFieldLength];
        char password[m_maxFieldLength];

        getField(line, 0, idText, m_maxFieldLength);
        getField(line, 1, name, m_maxFieldLength);
        getField(line, 2, password, m_maxFieldLength);

        Admin admin(
            toInt(idText),
            name,
            password
        );

        admins.add(admin);
    }
}

void FileHandler::loadAppointments(const char* fileName, Storage<Appointment>& appointments)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not open appointments file.");
    }

    char line[m_maxLineLength];
    while (file.getline(line, m_maxLineLength))
    {
        trimNewLine(line);
        if (line[0] == '\0')
        {
            continue;
        }

        char appointmentIdText[m_maxFieldLength];
        char patientIdText[m_maxFieldLength];
        char doctorIdText[m_maxFieldLength];
        char date[m_maxFieldLength];
        char timeSlot[m_maxFieldLength];
        char status[m_maxFieldLength];

        getField(line, 0, appointmentIdText, m_maxFieldLength);
        getField(line, 1, patientIdText, m_maxFieldLength);
        getField(line, 2, doctorIdText, m_maxFieldLength);
        getField(line, 3, date, m_maxFieldLength);
        getField(line, 4, timeSlot, m_maxFieldLength);
        getField(line, 5, status, m_maxFieldLength);

        Appointment appointment(
            toInt(appointmentIdText),
            toInt(patientIdText),
            toInt(doctorIdText),
            date,
            timeSlot,
            status
        );

        appointments.add(appointment);
    }
}

void FileHandler::loadBills(const char* fileName, Storage<Bill>& bills)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not open bills file.");
    }

    char line[m_maxLineLength];
    while (file.getline(line, m_maxLineLength))
    {
        trimNewLine(line);
        if (line[0] == '\0')
        {
            continue;
        }

        char billIdText[m_maxFieldLength];
        char patientIdText[m_maxFieldLength];
        char appointmentIdText[m_maxFieldLength];
        char amountText[m_maxFieldLength];
        char status[m_maxFieldLength];
        char date[m_maxFieldLength];

        getField(line, 0, billIdText, m_maxFieldLength);
        getField(line, 1, patientIdText, m_maxFieldLength);
        getField(line, 2, appointmentIdText, m_maxFieldLength);
        getField(line, 3, amountText, m_maxFieldLength);
        getField(line, 4, status, m_maxFieldLength);
        getField(line, 5, date, m_maxFieldLength);

        Bill bill(
            toInt(billIdText),
            toInt(patientIdText),
            toInt(appointmentIdText),
            toDouble(amountText),
            status,
            date
        );

        bills.add(bill);
    }
}

void FileHandler::loadPrescriptions(const char* fileName, Storage<Prescription>& prescriptions)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not open prescriptions file.");
    }

    char line[m_maxLineLength];
    while (file.getline(line, m_maxLineLength))
    {
        trimNewLine(line);
        if (line[0] == '\0')
        {
            continue;
        }

        char prescriptionIdText[m_maxFieldLength];
        char appointmentIdText[m_maxFieldLength];
        char patientIdText[m_maxFieldLength];
        char doctorIdText[m_maxFieldLength];
        char date[m_maxFieldLength];
        char medicines[m_maxFieldLength];
        char notes[m_maxFieldLength];

        getField(line, 0, prescriptionIdText, m_maxFieldLength);
        getField(line, 1, appointmentIdText, m_maxFieldLength);
        getField(line, 2, patientIdText, m_maxFieldLength);
        getField(line, 3, doctorIdText, m_maxFieldLength);
        getField(line, 4, date, m_maxFieldLength);
        getField(line, 5, medicines, m_maxFieldLength);
        getField(line, 6, notes, m_maxFieldLength);

        Prescription prescription(
            toInt(prescriptionIdText),
            toInt(appointmentIdText),
            toInt(patientIdText),
            toInt(doctorIdText),
            date,
            medicines,
            notes
        );

        prescriptions.add(prescription);
    }
}

void FileHandler::savePatients(const char* fileName, const Storage<Patient>& patients)
{
    std::ofstream file(fileName, std::ios::trunc);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not save patients file.");
    }

    const Patient* data = patients.getAll();
    for (int i = 0; i < patients.size(); i++)
    {
        writePatientLine(file, data[i]);
        if (i < patients.size() - 1)
        {
            file << "\n";
        }
    }
}

void FileHandler::saveDoctors(const char* fileName, const Storage<Doctor>& doctors)
{
    std::ofstream file(fileName, std::ios::trunc);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not save doctors file.");
    }

    const Doctor* data = doctors.getAll();
    for (int i = 0; i < doctors.size(); i++)
    {
        writeDoctorLine(file, data[i]);
        if (i < doctors.size() - 1)
        {
            file << "\n";
        }
    }
}

void FileHandler::saveAdmins(const char* fileName, const Storage<Admin>& admins)
{
    std::ofstream file(fileName, std::ios::trunc);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not save admin file.");
    }

    const Admin* data = admins.getAll();
    for (int i = 0; i < admins.size(); i++)
    {
        writeAdminLine(file, data[i]);
        if (i < admins.size() - 1)
        {
            file << "\n";
        }
    }
}

void FileHandler::saveAppointments(const char* fileName, const Storage<Appointment>& appointments)
{
    std::ofstream file(fileName, std::ios::trunc);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not save appointments file.");
    }

    const Appointment* data = appointments.getAll();
    for (int i = 0; i < appointments.size(); i++)
    {
        writeAppointmentLine(file, data[i]);
        if (i < appointments.size() - 1)
        {
            file << "\n";
        }
    }
}

void FileHandler::saveBills(const char* fileName, const Storage<Bill>& bills)
{
    std::ofstream file(fileName, std::ios::trunc);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not save bills file.");
    }

    const Bill* data = bills.getAll();
    for (int i = 0; i < bills.size(); i++)
    {
        writeBillLine(file, data[i]);
        if (i < bills.size() - 1)
        {
            file << "\n";
        }
    }
}

void FileHandler::savePrescriptions(const char* fileName, const Storage<Prescription>& prescriptions)
{
    std::ofstream file(fileName, std::ios::trunc);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not save prescriptions file.");
    }

    const Prescription* data = prescriptions.getAll();
    for (int i = 0; i < prescriptions.size(); i++)
    {
        writePrescriptionLine(file, data[i]);
        if (i < prescriptions.size() - 1)
        {
            file << "\n";
        }
    }
}

void FileHandler::appendLine(const char* fileName, const char* line)
{
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not append to file.");
    }

    file << line << "\n";
}

void FileHandler::appendPatient(const char* fileName, const Patient& patient)
{
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not append patient.");
    }
    writePatientLine(file, patient);
    file << "\n";
}

void FileHandler::appendDoctor(const char* fileName, const Doctor& doctor)
{
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not append doctor.");
    }
    writeDoctorLine(file, doctor);
    file << "\n";
}

void FileHandler::appendAdmin(const char* fileName, const Admin& admin)
{
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not append admin.");
    }
    writeAdminLine(file, admin);
    file << "\n";
}

void FileHandler::appendAppointment(const char* fileName, const Appointment& appointment)
{
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not append appointment.");
    }
    writeAppointmentLine(file, appointment);
    file << "\n";
}

void FileHandler::appendBill(const char* fileName, const Bill& bill)
{
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not append bill.");
    }
    writeBillLine(file, bill);
    file << "\n";
}

void FileHandler::appendPrescription(const char* fileName, const Prescription& prescription)
{
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not append prescription.");
    }
    writePrescriptionLine(file, prescription);
    file << "\n";
}

bool FileHandler::deletePatientById(const char* fileName, int patientId)
{
    Storage<Patient> temp;
    loadPatients(fileName, temp);

    bool removed = false;
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp.getAll()[i].getId() == patientId)
        {
            temp.removeById(patientId);
            removed = true;
            break;
        }
    }

    if (removed)
    {
        savePatients(fileName, temp);
    }

    return removed;
}

bool FileHandler::deleteDoctorById(const char* fileName, int doctorId)
{
    Storage<Doctor> temp;
    loadDoctors(fileName, temp);

    bool removed = false;
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp.getAll()[i].getId() == doctorId)
        {
            temp.removeById(doctorId);
            removed = true;
            break;
        }
    }

    if (removed)
    {
        saveDoctors(fileName, temp);
    }

    return removed;
}

bool FileHandler::deleteAdminById(const char* fileName, int adminId)
{
    Storage<Admin> temp;
    loadAdmins(fileName, temp);

    bool removed = false;
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp.getAll()[i].getId() == adminId)
        {
            temp.removeById(adminId);
            removed = true;
            break;
        }
    }

    if (removed)
    {
        saveAdmins(fileName, temp);
    }

    return removed;
}

bool FileHandler::deleteAppointmentById(const char* fileName, int appointmentId)
{
    Storage<Appointment> temp;
    loadAppointments(fileName, temp);

    bool removed = false;
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp.getAll()[i].getAppointmentId() == appointmentId)
        {
            temp.removeById(appointmentId);
            removed = true;
            break;
        }
    }

    if (removed)
    {
        saveAppointments(fileName, temp);
    }

    return removed;
}

bool FileHandler::deleteBillById(const char* fileName, int billId)
{
    Storage<Bill> temp;
    loadBills(fileName, temp);

    bool removed = false;
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp.getAll()[i].getBillId() == billId)
        {
            temp.removeById(billId);
            removed = true;
            break;
        }
    }

    if (removed)
    {
        saveBills(fileName, temp);
    }

    return removed;
}

bool FileHandler::deletePrescriptionById(const char* fileName, int prescriptionId)
{
    Storage<Prescription> temp;
    loadPrescriptions(fileName, temp);

    bool removed = false;
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp.getAll()[i].getPrescriptionId() == prescriptionId)
        {
            temp.removeById(prescriptionId);
            removed = true;
            break;
        }
    }

    if (removed)
    {
        savePrescriptions(fileName, temp);
    }

    return removed;
}

bool FileHandler::updatePatientById(const char* fileName, const Patient& updatedPatient)
{
    Storage<Patient> temp;
    loadPatients(fileName, temp);

    bool found = false;
    Patient* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getId() == updatedPatient.getId())
        {
            data[i] = updatedPatient;
            found = true;
            break;
        }
    }

    if (found)
    {
        savePatients(fileName, temp);
    }

    return found;
}

bool FileHandler::updateDoctorById(const char* fileName, const Doctor& updatedDoctor)
{
    Storage<Doctor> temp;
    loadDoctors(fileName, temp);

    bool found = false;
    Doctor* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getId() == updatedDoctor.getId())
        {
            data[i] = updatedDoctor;
            found = true;
            break;
        }
    }

    if (found)
    {
        saveDoctors(fileName, temp);
    }

    return found;
}

bool FileHandler::updateAdminById(const char* fileName, const Admin& updatedAdmin)
{
    Storage<Admin> temp;
    loadAdmins(fileName, temp);

    bool found = false;
    Admin* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getId() == updatedAdmin.getId())
        {
            data[i] = updatedAdmin;
            found = true;
            break;
        }
    }

    if (found)
    {
        saveAdmins(fileName, temp);
    }

    return found;
}

bool FileHandler::updateAppointmentById(const char* fileName, const Appointment& updatedAppointment)
{
    Storage<Appointment> temp;
    loadAppointments(fileName, temp);

    bool found = false;
    Appointment* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getAppointmentId() == updatedAppointment.getAppointmentId())
        {
            data[i] = updatedAppointment;
            found = true;
            break;
        }
    }

    if (found)
    {
        saveAppointments(fileName, temp);
    }

    return found;
}

bool FileHandler::updateBillById(const char* fileName, const Bill& updatedBill)
{
    Storage<Bill> temp;
    loadBills(fileName, temp);

    bool found = false;
    Bill* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getBillId() == updatedBill.getBillId())
        {
            data[i] = updatedBill;
            found = true;
            break;
        }
    }

    if (found)
    {
        saveBills(fileName, temp);
    }

    return found;
}

bool FileHandler::updatePrescriptionById(const char* fileName, const Prescription& updatedPrescription)
{
    Storage<Prescription> temp;
    loadPrescriptions(fileName, temp);

    bool found = false;
    Prescription* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getPrescriptionId() == updatedPrescription.getPrescriptionId())
        {
            data[i] = updatedPrescription;
            found = true;
            break;
        }
    }

    if (found)
    {
        savePrescriptions(fileName, temp);
    }

    return found;
}

int FileHandler::getNextPatientId(const char* fileName)
{
    Storage<Patient> temp;
    loadPatients(fileName, temp);

    int maxId = 0;
    const Patient* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getId() > maxId)
        {
            maxId = data[i].getId();
        }
    }
    return maxId + 1;
}

int FileHandler::getNextDoctorId(const char* fileName)
{
    Storage<Doctor> temp;
    loadDoctors(fileName, temp);

    int maxId = 0;
    const Doctor* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getId() > maxId)
        {
            maxId = data[i].getId();
        }
    }
    return maxId + 1;
}

int FileHandler::getNextAdminId(const char* fileName)
{
    Storage<Admin> temp;
    loadAdmins(fileName, temp);

    int maxId = 0;
    const Admin* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getId() > maxId)
        {
            maxId = data[i].getId();
        }
    }
    return maxId + 1;
}

int FileHandler::getNextAppointmentId(const char* fileName)
{
    Storage<Appointment> temp;
    loadAppointments(fileName, temp);

    int maxId = 0;
    const Appointment* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getAppointmentId() > maxId)
        {
            maxId = data[i].getAppointmentId();
        }
    }
    return maxId + 1;
}

int FileHandler::getNextBillId(const char* fileName)
{
    Storage<Bill> temp;
    loadBills(fileName, temp);

    int maxId = 0;
    const Bill* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getBillId() > maxId)
        {
            maxId = data[i].getBillId();
        }
    }
    return maxId + 1;
}

int FileHandler::getNextPrescriptionId(const char* fileName)
{
    Storage<Prescription> temp;
    loadPrescriptions(fileName, temp);

    int maxId = 0;
    const Prescription* data = temp.getAll();
    for (int i = 0; i < temp.size(); i++)
    {
        if (data[i].getPrescriptionId() > maxId)
        {
            maxId = data[i].getPrescriptionId();
        }
    }
    return maxId + 1;
}

void FileHandler::logSecurityEvent(const char* fileName, const char* timestamp, const char* role, int enteredId, const char* result)
{
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not open security log.");
    }

    file << timestamp << ","
        << role << ","
        << enteredId << ","
        << result << "\n";
}

bool FileHandler::displayTextFile(const char* fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not open file.");
    }

    char line[m_maxLineLength];
    bool hasContent = false;

    while (file.getline(line, m_maxLineLength))
    {
        trimNewLine(line);
        if (line[0] == '\0')
        {
            continue;
        }

        std::cout << line << "\n";
        hasContent = true;
    }

    return hasContent;
}

bool FileHandler::loadTextFile(const char* fileName, CustomString& output)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException("Could not open file.");
    }

    output.clear();

    char line[m_maxLineLength];
    bool hasContent = false;

    while (file.getline(line, m_maxLineLength))
    {
        trimNewLine(line);

        if (line[0] == '\0')
        {
            continue;
        }

        output += line;
        output += "\n";
        hasContent = true;
    }

    return hasContent;
}