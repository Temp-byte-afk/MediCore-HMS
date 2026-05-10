#ifndef VIEWMEDICALRECORDSSCREEN_H
#define VIEWMEDICALRECORDSSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "Label.h"
#include "MessagePanel.h"

class ViewMedicalRecordsScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_patientLabel;
    Label m_headerLabel;

    Button m_backButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    int comparePrescriptionsForDisplay(const Prescription& first, const Prescription& second) const;
    void renderPrescriptionList(sf::RenderWindow& window) const;

public:
    ViewMedicalRecordsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif