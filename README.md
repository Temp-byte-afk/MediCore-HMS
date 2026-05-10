# MediCore Hospital Management System

MediCore is a C++ Hospital Management System with persistent file-based storage and an SFML graphical interface. It supports three user roles: Patient, Doctor, and Admin. The system manages appointments, prescriptions, billing, medical records, account balances, security logs, and patient discharge/archiving.

All records are loaded from text files at startup and saved back immediately after every change, so data survives program restarts.

## Features

### Patient
- Login with patient ID and password
- Book appointments by specialization, doctor, date, and time slot
- Cancel pending appointments
- View personal appointments
- View medical records
- View bills and unpaid totals
- Pay bills
- Top up balance

### Doctor
- Login with doctor ID and password
- View today's appointments
- Mark appointments complete
- Mark appointments as no-show
- Write prescriptions for completed appointments
- View patient medical history

### Admin
- Login with admin ID and password
- Add a doctor
- Remove a doctor
- View all patients
- View all doctors
- View all appointments
- View all unpaid bills
- Discharge and archive patients
- View security logs
- Generate daily report

## Project Rules Followed
- No `std::vector`
- No `std::string`
- No global variables
- No `goto`
- Dynamic memory used where needed
- No static arrays for user data storage
- No direct file I/O outside `FileHandler`
- No hardcoded data for program behavior
- Separate `.h` and `.cpp` files for each class
- Modular functions for input, output, validation, and file handling
- Memory cleanup handled carefully to avoid leaks

## Architecture Overview

### Backend
The backend is responsible for:
- entity classes such as `Patient`, `Doctor`, `Admin`, `Appointment`, `Bill`, and `Prescription`
- validation through `Validator`
- file operations through `FileHandler`
- system behavior through `MediCoreSystem`
- custom exceptions for error handling
- fixed-size generic storage using `Storage<T>`

### GUI
The SFML frontend is organized using:
- `SFMLApp` for the window and event loop
- `ScreenManager` for screen navigation
- `ScreenFactory` for screen creation
- `BaseScreen` for shared screen behavior
- reusable widgets such as `Button`, `InputBox`, `Label`, and `MessagePanel`

## File Structure

### Source Code
- `main.cpp`
- `MediCoreSystem.h/.cpp`
- `FileHandler.h/.cpp`
- `Validator.h/.cpp`
- `DateUtils.h/.cpp`
- `CustomString.h/.cpp`
- `Storage.h`
- `Person.h/.cpp`
- `Patient.h/.cpp`
- `Doctor.h/.cpp`
- `Admin.h/.cpp`
- `Appointment.h/.cpp`
- `Bill.h/.cpp`
- `Prescription.h/.cpp`
- `HospitalException.h/.cpp`
- `FileNotFoundException.h/.cpp`
- `InsufficientFundsException.h/.cpp`
- `InvalidInputException.h/.cpp`
- `SlotUnavailableException.h/.cpp`

### SFML GUI
- `SFMLApp.h/.cpp`
- `ScreenManager.h/.cpp`
- `ScreenFactory.h/.cpp`
- `BaseScreen.h/.cpp`
- `Button.h/.cpp`
- `InputBox.h/.cpp`
- `Label.h/.cpp`
- `MessagePanel.h/.cpp`
- `LoginScreen.h/.cpp`
- `PatientDashboardScreen.h/.cpp`
- `DoctorDashboardScreen.h/.cpp`
- `AdminDashboardScreen.h/.cpp`
- All action screens for patient, doctor, and admin workflows

### Data Files
- `patients.txt`
- `doctors.txt`
- `admin.txt`
- `appointments.txt`
- `prescriptions.txt`
- `bills.txt`
- `security_log.txt`
- `discharged.txt`

### Asset Files
- `assets/Roboto-Regular.ttf` or the font file configured in the project

## Text File Formats

### `patients.txt`
`patient_id,name,age,gender,contact,password,balance`

### `doctors.txt`
`doctor_id,name,specialization,contact,password,fee`

### `admin.txt`
`admin_id,name,password`

### `appointments.txt`
`appointment_id,patient_id,doctor_id,date,time_slot,status`

### `prescriptions.txt`
`prescription_id,appointment_id,patient_id,doctor_id,date,medicines,notes`

### `bills.txt`
`bill_id,patient_id,appointment_id,amount,status,date`

### `security_log.txt`
`timestamp,role,entered_id,result`

### `discharged.txt`
Same format as `patients.txt`

## Build Requirements
- C++ compiler with SFML support
- SFML graphics/system/window libraries
- Standard C++ file and stream support

## How to Run

1. Place all source files, headers, data files, and asset files in the project folder.
2. Ensure the font file path used by the program exists.
3. Build the project with SFML linked correctly.
4. Run the executable.
5. Log in as Patient, Doctor, or Admin using records from the text files.

## Login and Persistence
- Every login attempt is validated against the correct file.
- Failed login attempts are written to `security_log.txt`.
- Changes are saved immediately to the corresponding data files.
- Restarting the program restores the previous state from the text files.

## Important Notes
- The program is designed for individual academic use.
- Do not submit plagiarized code.
- Ensure the GitHub repository link is valid and included in this README.
- The submission zip file must be named exactly as the roll number, for example: `22L-1234.zip`

## GitHub Repository
Repository Link: `https://github.com/Temp-byte-afk/MediCore-HMS/tree/main`

## Author
Name: `Muhammad Fayyaz`  
Roll Number: `25L-0668`
