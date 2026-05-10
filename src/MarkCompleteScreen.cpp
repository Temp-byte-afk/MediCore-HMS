#include "MarkCompleteScreen.h"
#include <iostream>

MarkCompleteScreen::MarkCompleteScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_appointmentIdInput()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::MarkComplete;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Mark Appointment Complete", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Only today's pending appointments can be completed.", 20, sf::Vector2f(70.f, 80.f));
    m_doctorLabel = Label(m_font, "Doctor: ", 22, sf::Vector2f(70.f, 115.f));
    m_idLabel = Label(m_font, "Appointment ID", 22, sf::Vector2f(70.f, 210.f));

    m_appointmentIdInput = InputBox(
        m_font,
        "Enter appointment ID",
        sf::Vector2f(70.f, 245.f),
        sf::Vector2f(380.f, 45.f),
        20,
        false,
        20
    );

    m_submitButton = Button(m_font, "Mark Complete", sf::Vector2f(70.f, 325.f), sf::Vector2f(180.f, 50.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(270.f, 325.f), sf::Vector2f(180.f, 50.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 420.f), sf::Vector2f(560.f, 110.f), 18);
}

void MarkCompleteScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void MarkCompleteScreen::submitCompletion()
{
    const CustomString& appointmentIdText = m_appointmentIdInput.getText();

    if (appointmentIdText.isEmpty())
    {
        m_messagePanel.setMessage("Please enter an appointment ID.");
        return;
    }

    if (!Validator::isValidId(appointmentIdText.cStr()))
    {
        m_messagePanel.setMessage("Invalid appointment ID.");
        return;
    }

    int appointmentId = 0;
    const char* idChars = appointmentIdText.cStr();
    int i = 0;
    while (idChars[i] != '\0')
    {
        appointmentId = appointmentId * 10 + (idChars[i] - '0');
        i++;
    }

    int doctorId = m_backend.getCurrentUserId();

    try
    {
        m_backend.markAppointmentComplete(doctorId, appointmentId);
        m_messagePanel.setMessage("Appointment marked as completed.");
    }
    catch (const HospitalException& ex)
    {
        m_messagePanel.setMessage(ex.what());
    }
}

void MarkCompleteScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_appointmentIdInput.handleEvent(event, window);
    m_submitButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_submitButton.isClicked(event, window))
    {
        submitCompletion();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::DoctorDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        submitCompletion();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::DoctorDashboard);
    }
}

void MarkCompleteScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
    m_appointmentIdInput.update();
}

void MarkCompleteScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);

    CustomString doctorText("Doctor: ");
    doctorText += m_backend.getCurrentUserName();
    m_doctorLabel.setText(doctorText.cStr());
    m_doctorLabel.draw(window);

    m_idLabel.draw(window);
    m_appointmentIdInput.draw(window);

    m_submitButton.draw(window);
    m_backButton.draw(window);

    m_messagePanel.draw(window);
}

bool MarkCompleteScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState MarkCompleteScreen::getRequestedState() const
{
    return m_requestedState;
}

void MarkCompleteScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}