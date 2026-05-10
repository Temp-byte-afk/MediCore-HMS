#ifndef DOCTORHISTORYSCREEN_H
#define DOCTORHISTORYSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"
#include "MessagePanel.h"

class DoctorHistoryScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_doctorLabel;
    Label m_patientIdLabel;
    Label m_headerLabel;
    Label m_accessLabel;

    InputBox m_patientIdInput;

    Button m_submitButton;
    Button m_backButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void submitLookup();
    bool doctorHasAccessToPatient(int doctorId, int patientId) const;
    int comparePrescriptionsForDisplay(const Prescription& first, const Prescription& second) const;
    void renderPrescriptionList(sf::RenderWindow& window, int patientId) const;

public:
    DoctorHistoryScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif