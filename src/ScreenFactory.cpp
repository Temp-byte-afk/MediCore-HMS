#include "ScreenFactory.h"
#include "LoginScreen.h"
#include "PatientDashboardScreen.h"
#include "DoctorDashboardScreen.h"
#include "AdminDashboardScreen.h"
#include "BookAppointmentScreen.h"
#include "CancelAppointmentScreen.h"
#include "BaseScreen.h"
#include "ViewAppointmentsScreen.h"
#include "ViewMedicalRecordsScreen.h"
#include "ViewBillsScreen.h"
#include "PayBillScreen.h"
#include "TopUpScreen.h"
#include "ViewTodayAppointmentsScreen.h"
#include "MarkCompleteScreen.h"
#include "MarkNoShowScreen.h"
#include "WritePrescriptionScreen.h"
#include "DoctorHistoryScreen.h"
#include "AddDoctorScreen.h"
#include "RemoveDoctorScreen.h"
#include "ViewPatientsScreen.h"
#include "ViewDoctorsScreen.h"
#include "ViewAllAppointmentsScreen.h"
#include "ViewUnpaidBillsScreen.h"
#include "DischargePatientScreen.h"
#include "SecurityLogScreen.h"
#include "DailyReportScreen.h"

ScreenFactory::ScreenFactory(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : m_backend(backend), m_screenManager(screenManager), m_fontFilePath(fontFilePath)
{
}

std::unique_ptr<BaseScreen> ScreenFactory::createScreen(ScreenState state) const
{
    switch (state)
    {
    case ScreenState::Login:
        return std::make_unique<LoginScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::PatientDashboard:
        return std::make_unique<PatientDashboardScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::DoctorDashboard:
        return std::make_unique<DoctorDashboardScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::AdminDashboard:
        return std::make_unique<AdminDashboardScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::BookAppointment:
        return std::make_unique<BookAppointmentScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::CancelAppointment:
        return std::make_unique<CancelAppointmentScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::ViewAppointments:
        return std::make_unique<ViewAppointmentsScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::ViewMedicalRecords:
        return std::make_unique<ViewMedicalRecordsScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::ViewBills:
        return std::make_unique<ViewBillsScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::PayBill:
        return std::make_unique<PayBillScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::TopUpBalance:
        return std::make_unique<TopUpScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::ViewTodayAppointments:
        return std::make_unique<ViewTodayAppointmentsScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::MarkComplete:
        return std::make_unique<MarkCompleteScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::MarkNoShow:
        return std::make_unique<MarkNoShowScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::WritePrescription:
        return std::make_unique<WritePrescriptionScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::DoctorHistory:
        return std::make_unique<DoctorHistoryScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::AddDoctor:
        return std::make_unique<AddDoctorScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::RemoveDoctor:
        return std::make_unique<RemoveDoctorScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::ViewAllPatients:
        return std::make_unique<ViewPatientsScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::ViewAllDoctors:
        return std::make_unique<ViewDoctorsScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::ViewAllAppointments:
        return std::make_unique<ViewAllAppointmentsScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::ViewUnpaidBills:
        return std::make_unique<ViewUnpaidBillsScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::DischargePatient:
        return std::make_unique<DischargePatientScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::SecurityLog:
        return std::make_unique<SecurityLogScreen>(m_backend, m_screenManager, m_fontFilePath);
    case ScreenState::DailyReport:
        return std::make_unique<DailyReportScreen>(m_backend, m_screenManager, m_fontFilePath);

    default:
        return std::make_unique<LoginScreen>(m_backend, m_screenManager, m_fontFilePath);
    }
}