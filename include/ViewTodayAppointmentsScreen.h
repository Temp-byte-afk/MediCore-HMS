#ifndef VIEWTODAYAPPOINTMENTSSCREEN_H
#define VIEWTODAYAPPOINTMENTSSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "Label.h"
#include "MessagePanel.h"

class ViewTodayAppointmentsScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_doctorLabel;
    Label m_headerLabel;

    Button m_backButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    int compareTimeSlots(const CustomString& first, const CustomString& second) const;
    void renderAppointmentList(sf::RenderWindow& window) const;

public:
    ViewTodayAppointmentsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif