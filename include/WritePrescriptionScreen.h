#ifndef WRITEPRESCRIPTIONSCREEN_H
#define WRITEPRESCRIPTIONSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"
#include "MessagePanel.h"

class WritePrescriptionScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_doctorLabel;
    Label m_appointmentIdLabel;
    Label m_medicinesLabel;
    Label m_notesLabel;
    Label m_completedListLabel;

    InputBox m_appointmentIdInput;
    InputBox m_medicinesInput;
    InputBox m_notesInput;

    Button m_submitButton;
    Button m_backButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void submitPrescription();
    void renderCompletedAppointments(sf::RenderWindow& window) const;

public:
    WritePrescriptionScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif