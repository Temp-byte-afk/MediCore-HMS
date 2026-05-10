#include "LoginScreen.h"
#include <iostream>
#include "ScreenManager.h"

LoginScreen::LoginScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_patientButton(),
    m_doctorButton(),
    m_adminButton(),
    m_loginButton(),
    m_exitButton(),
    m_idInput(),
    m_passwordInput()
{
    m_selectedRole = LoginRole::None;
    m_loginRequested = false;
    m_navigationRequested = false;
    m_requestedState = ScreenState::Login;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Welcome to MediCore Hospital Management System", 32, sf::Vector2f(180.f, 40.f));
    m_roleLabel = Label(m_font, "Select Role", 24, sf::Vector2f(180.f, 120.f));
    m_idLabel = Label(m_font, "ID", 22, sf::Vector2f(180.f, 300.f));
    m_passwordLabel = Label(m_font, "Password", 22, sf::Vector2f(180.f, 400.f));

    m_patientButton = Button(m_font, "Patient", sf::Vector2f(180.f, 160.f), sf::Vector2f(150.f, 45.f));
    m_doctorButton = Button(m_font, "Doctor", sf::Vector2f(350.f, 160.f), sf::Vector2f(150.f, 45.f));
    m_adminButton = Button(m_font, "Admin", sf::Vector2f(520.f, 160.f), sf::Vector2f(150.f, 45.f));

    m_loginButton = Button(m_font, "Login", sf::Vector2f(180.f, 520.f), sf::Vector2f(180.f, 50.f));
    m_exitButton = Button(m_font, "Exit", sf::Vector2f(380.f, 520.f), sf::Vector2f(180.f, 50.f));

    m_idInput = InputBox(m_font, "Enter ID", sf::Vector2f(180.f, 340.f), sf::Vector2f(400.f, 45.f), 20, false, 20);
    m_passwordInput = InputBox(m_font, "Enter Password", sf::Vector2f(180.f, 440.f), sf::Vector2f(400.f, 45.f), 20, true, 50);

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(180.f, 610.f), sf::Vector2f(600.f, 45.f), 18);
}

void LoginScreen::setRole(LoginRole role)
{
    m_selectedRole = role;

    m_patientButton.setFillColor(sf::Color(60, 60, 60));
    m_doctorButton.setFillColor(sf::Color(60, 60, 60));
    m_adminButton.setFillColor(sf::Color(60, 60, 60));

    if (role == LoginRole::Patient)
    {
        m_patientButton.setFillColor(sf::Color(90, 90, 140));
    }
    else if (role == LoginRole::Doctor)
    {
        m_doctorButton.setFillColor(sf::Color(90, 90, 140));
    }
    else if (role == LoginRole::Admin)
    {
        m_adminButton.setFillColor(sf::Color(90, 90, 140));
    }
}

const char* LoginScreen::getRoleText() const
{
    if (m_selectedRole == LoginRole::Patient)
    {
        return "Patient";
    }
    if (m_selectedRole == LoginRole::Doctor)
    {
        return "Doctor";
    }
    if (m_selectedRole == LoginRole::Admin)
    {
        return "Admin";
    }
    return "";
}

void LoginScreen::attemptLogin()
{
    if (m_selectedRole == LoginRole::None)
    {
        m_messagePanel.setMessage("Please select a role first.");
        return;
    }

    const CustomString& idText = m_idInput.getText();
    const CustomString& passwordText = m_passwordInput.getText();

    if (!Validator::isValidId(idText.cStr()))
    {
        m_messagePanel.setMessage("Invalid ID.");
        return;
    }

    int id = 0;
    const char* idChars = idText.cStr();
    int i = 0;
    while (idChars[i] != '\0')
    {
        id = id * 10 + (idChars[i] - '0');
        i++;
    }

    bool success = false;

    if (m_selectedRole == LoginRole::Patient)
    {
        success = m_backend.loginPatient(id, passwordText.cStr());
        if (success)
        {
            m_requestedState = ScreenState::PatientDashboard;
        }
    }
    else if (m_selectedRole == LoginRole::Doctor)
    {
        success = m_backend.loginDoctor(id, passwordText.cStr());
        if (success)
        {
            m_requestedState = ScreenState::DoctorDashboard;
        }
    }
    else if (m_selectedRole == LoginRole::Admin)
    {
        success = m_backend.loginAdmin(id, passwordText.cStr());
        if (success)
        {
            m_requestedState = ScreenState::AdminDashboard;
        }
    }

    if (success)
    {
        m_navigationRequested = true;
        m_messagePanel.setMessage("Login successful.");
    }
    else
    {
        m_messagePanel.setMessage("Login failed. Check ID or password.");
    }
}

void LoginScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_patientButton.handleEvent(event, window);
    m_doctorButton.handleEvent(event, window);
    m_adminButton.handleEvent(event, window);
    m_loginButton.handleEvent(event, window);
    m_exitButton.handleEvent(event, window);

    m_idInput.handleEvent(event, window);
    m_passwordInput.handleEvent(event, window);

    if (m_patientButton.isClicked(event, window))
    {
        setRole(LoginRole::Patient);
    }
    else if (m_doctorButton.isClicked(event, window))
    {
        setRole(LoginRole::Doctor);
    }
    else if (m_adminButton.isClicked(event, window))
    {
        setRole(LoginRole::Admin);
    }
    else if (m_loginButton.isClicked(event, window))
    {
        m_loginRequested = true;
    }
    else if (m_exitButton.isClicked(event, window))
    {
        m_requestedState = ScreenState::Exit;
        m_navigationRequested = true;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        m_loginRequested = true;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        m_requestedState = ScreenState::Exit;
        m_navigationRequested = true;
    }
}

void LoginScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;

    m_idInput.update();
    m_passwordInput.update();

    if (m_loginRequested)
    {
        m_loginRequested = false;
        attemptLogin();
    }
}

void LoginScreen::render(sf::RenderWindow& window)
{
    if (m_font.getInfo().family.empty())
    {
        return;
    }

    window.clear(sf::Color(20, 20, 20));

    m_titleLabel.draw(window);
    m_roleLabel.draw(window);
    m_idLabel.draw(window);
    m_passwordLabel.draw(window);

    m_patientButton.draw(window);
    m_doctorButton.draw(window);
    m_adminButton.draw(window);
    m_loginButton.draw(window);
    m_exitButton.draw(window);

    m_idInput.draw(window);
    m_passwordInput.draw(window);

    m_messagePanel.draw(window);
}

bool LoginScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState LoginScreen::getRequestedState() const
{
    return m_requestedState;
}

void LoginScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}