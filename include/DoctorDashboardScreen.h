#ifndef DOCTORDASHBOARDSCREEN_H
#define DOCTORDASHBOARDSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "Label.h"
#include "MessagePanel.h"

class DoctorDashboardScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_welcomeLabel;
    Label m_specializationLabel;

    Button m_todayButton;
    Button m_completeButton;
    Button m_noShowButton;
    Button m_prescriptionButton;
    Button m_historyButton;
    Button m_logoutButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void showComingSoon(const char* message);

    void refreshDoctorInfo();

public:
    DoctorDashboardScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;

    void onEnter() override;
};

#endif