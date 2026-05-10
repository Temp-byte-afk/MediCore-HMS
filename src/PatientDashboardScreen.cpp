#include "PatientDashboardScreen.h"
#include <iostream>

PatientDashboardScreen::PatientDashboardScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_bookButton(),
    m_cancelButton(),
    m_viewAppointmentsButton(),
    m_viewRecordsButton(),
    m_viewBillsButton(),
    m_payBillButton(),
    m_topUpButton(),
    m_logoutButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::PatientDashboard;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Patient Dashboard", 34, sf::Vector2f(70.f, 30.f));
    m_welcomeLabel = Label(m_font, "Welcome, Patient", 24, sf::Vector2f(70.f, 90.f));
    m_balanceLabel = Label(m_font, "Balance: PKR 0", 22, sf::Vector2f(70.f, 130.f));

    m_bookButton = Button(m_font, "Book Appointment", sf::Vector2f(70.f, 200.f), sf::Vector2f(250.f, 45.f));
    m_cancelButton = Button(m_font, "Cancel Appointment", sf::Vector2f(70.f, 260.f), sf::Vector2f(250.f, 45.f));
    m_viewAppointmentsButton = Button(m_font, "View My Appointments", sf::Vector2f(70.f, 320.f), sf::Vector2f(250.f, 45.f));
    m_viewRecordsButton = Button(m_font, "View Medical Records", sf::Vector2f(70.f, 380.f), sf::Vector2f(250.f, 45.f));
    m_viewBillsButton = Button(m_font, "View Bills", sf::Vector2f(70.f, 440.f), sf::Vector2f(250.f, 45.f));
    m_payBillButton = Button(m_font, "Pay Bill", sf::Vector2f(70.f, 500.f), sf::Vector2f(250.f, 45.f));
    m_topUpButton = Button(m_font, "Top Up Balance", sf::Vector2f(70.f, 560.f), sf::Vector2f(250.f, 45.f));
    m_logoutButton = Button(m_font, "Logout", sf::Vector2f(70.f, 640.f), sf::Vector2f(250.f, 45.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(380.f, 200.f), sf::Vector2f(780.f, 90.f), 20);
}

void PatientDashboardScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void PatientDashboardScreen::showComingSoon(const char* message)
{
    m_messagePanel.setMessage(message);
}

void PatientDashboardScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_bookButton.handleEvent(event, window);
    m_cancelButton.handleEvent(event, window);
    m_viewAppointmentsButton.handleEvent(event, window);
    m_viewRecordsButton.handleEvent(event, window);
    m_viewBillsButton.handleEvent(event, window);
    m_payBillButton.handleEvent(event, window);
    m_topUpButton.handleEvent(event, window);
    m_logoutButton.handleEvent(event, window);

    if (m_bookButton.isClicked(event, window))
    {
        requestScreen(ScreenState::BookAppointment);
    }
    else if (m_cancelButton.isClicked(event, window))
    {
        requestScreen(ScreenState::CancelAppointment);
    }
    else if (m_viewAppointmentsButton.isClicked(event, window))
    {
        requestScreen(ScreenState::ViewAppointments);
    }
    else if (m_viewRecordsButton.isClicked(event, window))
    {
        requestScreen(ScreenState::ViewMedicalRecords);
    }
    else if (m_viewBillsButton.isClicked(event, window))
    {
        requestScreen(ScreenState::ViewBills);
    }
    else if (m_payBillButton.isClicked(event, window))
    {
        requestScreen(ScreenState::PayBill);
    }
    else if (m_topUpButton.isClicked(event, window))
    {
        requestScreen(ScreenState::TopUpBalance);
    }
    else if (m_logoutButton.isClicked(event, window))
    {
        m_backend.logout();
        requestScreen(ScreenState::Login);
    }
}

void PatientDashboardScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void PatientDashboardScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_welcomeLabel.draw(window);
    m_balanceLabel.draw(window);

    m_bookButton.draw(window);
    m_cancelButton.draw(window);
    m_viewAppointmentsButton.draw(window);
    m_viewRecordsButton.draw(window);
    m_viewBillsButton.draw(window);
    m_payBillButton.draw(window);
    m_topUpButton.draw(window);
    m_logoutButton.draw(window);

    m_messagePanel.draw(window);
}

bool PatientDashboardScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState PatientDashboardScreen::getRequestedState() const
{
    return m_requestedState;
}

void PatientDashboardScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}



void PatientDashboardScreen::appendDoubleToBuffer(double value, char* buffer, int maxLength)
{
    if (buffer == nullptr || maxLength <= 0)
    {
        return;
    }

    int index = 0;

    if (value < 0.0)
    {
        buffer[index++] = '-';
        value = -value;
    }

    int wholePart = static_cast<int>(value);
    int fractionalPart = static_cast<int>((value - wholePart) * 100.0 + 0.5);

    if (fractionalPart >= 100)
    {
        wholePart += 1;
        fractionalPart -= 100;
    }

    if (wholePart == 0)
    {
        buffer[index++] = '0';
    }
    else
    {
        char reversed[32];
        int r = 0;
        while (wholePart > 0 && r < 31)
        {
            reversed[r++] = static_cast<char>('0' + (wholePart % 10));
            wholePart /= 10;
        }
        for (int i = r - 1; i >= 0; i--)
        {
            buffer[index++] = reversed[i];
        }
    }

    buffer[index++] = '.';

    if (fractionalPart < 10)
    {
        buffer[index++] = '0';
        buffer[index++] = static_cast<char>('0' + fractionalPart);
    }
    else
    {
        buffer[index++] = static_cast<char>('0' + (fractionalPart / 10));
        buffer[index++] = static_cast<char>('0' + (fractionalPart % 10));
    }

    buffer[index] = '\0';
}

void PatientDashboardScreen::refreshPatientInfo()
{
    const Patient* patient = m_backend.getPatients().findById(m_backend.getCurrentUserId());
    if (patient == nullptr)
    {
        m_welcomeLabel.setText("Welcome, Patient");
        m_balanceLabel.setText("Balance: PKR 0.00");
        return;
    }

    CustomString welcomeText("Welcome, ");
    welcomeText += patient->getName();
    m_welcomeLabel.setText(welcomeText.cStr());

    char balanceBuffer[64];
    appendDoubleToBuffer(patient->getBalance(), balanceBuffer, 64);

    CustomString balanceText("Balance: PKR ");
    balanceText += balanceBuffer;
    m_balanceLabel.setText(balanceText.cStr());
}

void PatientDashboardScreen::onEnter()
{
    refreshPatientInfo();
}