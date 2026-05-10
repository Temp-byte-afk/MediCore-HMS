#ifndef ADDDOCTORSCREEN_H
#define ADDDOCTORSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"
#include "MessagePanel.h"

class AddDoctorScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_nameLabel;
    Label m_specializationLabel;
    Label m_contactLabel;
    Label m_passwordLabel;
    Label m_feeLabel;

    InputBox m_nameInput;
    InputBox m_specializationInput;
    InputBox m_contactInput;
    InputBox m_passwordInput;
    InputBox m_feeInput;

    Button m_submitButton;
    Button m_backButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void submitDoctor();

public:
    AddDoctorScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif