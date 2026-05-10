#include "TopUpScreen.h"
#include <iostream>

TopUpScreen::TopUpScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_amountInput()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::TopUpBalance;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Top Up Balance", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Enter a positive amount to add to your balance.", 20, sf::Vector2f(70.f, 80.f));
    m_patientLabel = Label(m_font, "Patient: ", 22, sf::Vector2f(70.f, 115.f));
    m_amountLabel = Label(m_font, "Amount (PKR)", 22, sf::Vector2f(70.f, 250.f));
    m_balanceLabel = Label(m_font, "Current Balance: PKR 0.00", 22, sf::Vector2f(70.f, 180.f));

    m_amountInput = InputBox(
        m_font,
        "Enter amount",
        sf::Vector2f(70.f, 285.f),
        sf::Vector2f(380.f, 45.f),
        20,
        false,
        30
    );

    m_submitButton = Button(m_font, "Add Balance", sf::Vector2f(70.f, 365.f), sf::Vector2f(180.f, 50.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(270.f, 365.f), sf::Vector2f(180.f, 50.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 455.f), sf::Vector2f(540.f, 120.f), 18);
}

void TopUpScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void TopUpScreen::appendDoubleToBuffer(double value, char* buffer, int maxLength)
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

    if (index < maxLength - 1)
    {
        buffer[index++] = '.';
    }

    if (fractionalPart < 10)
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = '0';
        }
        if (index < maxLength - 1)
        {
            buffer[index++] = static_cast<char>('0' + fractionalPart);
        }
    }
    else
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = static_cast<char>('0' + (fractionalPart / 10));
        }
        if (index < maxLength - 1)
        {
            buffer[index++] = static_cast<char>('0' + (fractionalPart % 10));
        }
    }

    buffer[index] = '\0';
}

void TopUpScreen::submitTopUp()
{
    const CustomString& amountText = m_amountInput.getText();

    if (amountText.isEmpty())
    {
        m_messagePanel.setMessage("Please enter an amount.");
        return;
    }

    int patientId = m_backend.getCurrentUserId();

    try
    {
        double newBalance = m_backend.topUpBalance(patientId, amountText.cStr());

        char balanceText[64];
        appendDoubleToBuffer(newBalance, balanceText, 64);

        CustomString message("Balance updated. New balance: PKR ");
        message += balanceText;

        m_messagePanel.setMessage(message.cStr());
    }
    catch (const HospitalException& ex)
    {
        m_messagePanel.setMessage(ex.what());
    }
}

void TopUpScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_amountInput.handleEvent(event, window);
    m_submitButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_submitButton.isClicked(event, window))
    {
        submitTopUp();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::PatientDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        submitTopUp();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::PatientDashboard);
    }
}

void TopUpScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
    m_amountInput.update();
}

//void TopUpScreen::render(sf::RenderWindow& window)
//{
//    window.clear(sf::Color(22, 22, 22));
//
//    m_titleLabel.draw(window);
//    m_hintLabel.draw(window);
//
//    CustomString patientText("Patient: ");
//    patientText += m_backend.getCurrentUserName();
//    m_patientLabel.setText(patientText.cStr());
//    m_patientLabel.draw(window);
//
//    char balanceText[64];
//    appendDoubleToBuffer(m_backend.getPatients().findById(m_backend.getCurrentUserId())->getBalance(), balanceText, 64);
//
//    CustomString currentBalance("Current Balance: PKR ");
//    currentBalance += balanceText;
//    m_balanceLabel.setText(currentBalance.cStr());
//    m_balanceLabel.draw(window);
//
//    m_amountLabel.draw(window);
//    m_amountInput.draw(window);
//
//    m_submitButton.draw(window);
//    m_backButton.draw(window);
//
//    m_messagePanel.draw(window);
//}

void TopUpScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);

    CustomString patientText("Patient: ");
    patientText += m_backend.getCurrentUserName();
    m_patientLabel.setText(patientText.cStr());
    m_patientLabel.draw(window);

    const Patient* patient = m_backend.getPatients().findById(m_backend.getCurrentUserId());
    double balance = 0.0;

    if (patient != nullptr)
    {
        balance = patient->getBalance();
    }

    char balanceText[64];
    appendDoubleToBuffer(balance, balanceText, 64);

    CustomString currentBalance("Current Balance: PKR ");
    currentBalance += balanceText;
    m_balanceLabel.setText(currentBalance.cStr());
    m_balanceLabel.draw(window);

    m_amountLabel.draw(window);
    m_amountInput.draw(window);

    m_submitButton.draw(window);
    m_backButton.draw(window);

    m_messagePanel.draw(window);
}

bool TopUpScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState TopUpScreen::getRequestedState() const
{
    return m_requestedState;
}

void TopUpScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}