#ifndef VIEWAPPOINTMENTSSCREEN_H
#define VIEWAPPOINTMENTSSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "Label.h"
#include "MessagePanel.h"

class ViewAppointmentsScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_headerLabel;
    Label m_patientLabel;

    Button m_backButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    int compareTimeSlots(const CustomString& first, const CustomString& second) const;
    int compareAppointmentsForDisplay(const Appointment& first, const Appointment& second) const;
    void renderAppointmentList(sf::RenderWindow& window) const;

public:
    ViewAppointmentsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif