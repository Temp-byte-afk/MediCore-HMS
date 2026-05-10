#ifndef PATIENTDASHBOARDSCREEN_H
#define PATIENTDASHBOARDSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "Label.h"
#include "MessagePanel.h"

class PatientDashboardScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_welcomeLabel;
    Label m_balanceLabel;

    Button m_bookButton;
    Button m_cancelButton;
    Button m_viewAppointmentsButton;
    Button m_viewRecordsButton;
    Button m_viewBillsButton;
    Button m_payBillButton;
    Button m_topUpButton;
    Button m_logoutButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void showComingSoon(const char* message);

    void refreshPatientInfo();
    static void appendDoubleToBuffer(double value, char* buffer, int maxLength);

public:
    PatientDashboardScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;

    void onEnter() override;
};

#endif