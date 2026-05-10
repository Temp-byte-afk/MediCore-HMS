#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"
#include "MessagePanel.h"

class LoginScreen : public BaseScreen
{
private:
    enum class LoginRole
    {
        None,
        Patient,
        Doctor,
        Admin
    };

    sf::Font m_font;

    Label m_titleLabel;
    Label m_roleLabel;
    Label m_idLabel;
    Label m_passwordLabel;

    Button m_patientButton;
    Button m_doctorButton;
    Button m_adminButton;
    Button m_loginButton;
    Button m_exitButton;

    InputBox m_idInput;
    InputBox m_passwordInput;

    MessagePanel m_messagePanel;

    LoginRole m_selectedRole;
    bool m_loginRequested;
    ScreenState m_requestedState;
    bool m_navigationRequested;

    void attemptLogin();
    void setRole(LoginRole role);
    const char* getRoleText() const;

public:
    LoginScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const;
    ScreenState getRequestedState() const;
    void clearNavigationRequest();
};

#endif