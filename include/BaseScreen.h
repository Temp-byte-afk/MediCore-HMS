#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <SFML/Graphics.hpp>
#include "MediCoreSystem.h"

class ScreenManager;

enum class ScreenState
{
    Login,
    PatientDashboard,
    BookAppointment,
    CancelAppointment,
    ViewAppointments,
    ViewMedicalRecords,
    ViewBills,
    PayBill,
    TopUpBalance,

    DoctorDashboard,
    ViewTodayAppointments,
    MarkComplete,
    MarkNoShow,
    WritePrescription,
    DoctorHistory,

    AdminDashboard,
    AddDoctor,
    RemoveDoctor,
    ViewAllPatients,
    ViewAllDoctors,
    ViewAllAppointments,
    ViewUnpaidBills,
    DischargePatient,
    SecurityLog,
    DailyReport,

    Exit
};

class BaseScreen
{
protected:
    MediCoreSystem& m_backend;
    ScreenManager& m_screenManager;

public:
    BaseScreen(MediCoreSystem& backend, ScreenManager& screenManager);
    virtual ~BaseScreen();

    virtual void onEnter();
    virtual void onExit();

    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window) = 0;
    virtual void update(sf::Time deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    virtual bool hasNavigationRequest() const;
    virtual ScreenState getRequestedState() const;
    virtual void clearNavigationRequest();

    MediCoreSystem& getBackend();
    ScreenManager& getScreenManager();

    const MediCoreSystem& getBackend() const;
    const ScreenManager& getScreenManager() const;
};

#endif