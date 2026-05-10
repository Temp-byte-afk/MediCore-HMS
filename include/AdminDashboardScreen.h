#ifndef ADMINDASHBOARDSCREEN_H
#define ADMINDASHBOARDSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "Label.h"
#include "MessagePanel.h"

class AdminDashboardScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_welcomeLabel;

    Button m_addDoctorButton;
    Button m_removeDoctorButton;
    Button m_viewPatientsButton;
    Button m_viewDoctorsButton;
    Button m_viewAppointmentsButton;
    Button m_viewUnpaidBillsButton;
    Button m_dischargeButton;
    Button m_securityLogButton;
    Button m_dailyReportButton;
    Button m_logoutButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void showComingSoon(const char* message);

public:
    AdminDashboardScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif