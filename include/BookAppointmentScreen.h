#ifndef BOOKAPPOINTMENTSCREEN_H
#define BOOKAPPOINTMENTSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"
#include "MessagePanel.h"

class BookAppointmentScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_specializationLabel;
    Label m_doctorIdLabel;
    Label m_dateLabel;
    Label m_timeSlotLabel;

    InputBox m_specializationInput;
    InputBox m_doctorIdInput;
    InputBox m_dateInput;
    InputBox m_timeSlotInput;

    Button m_submitButton;
    Button m_backButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void submitBooking();

    bool hasDoctorsForSpecialization(const char* specialization) const;

public:
    BookAppointmentScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;

    void renderMatchingDoctors(sf::RenderWindow& window) const;
};

#endif