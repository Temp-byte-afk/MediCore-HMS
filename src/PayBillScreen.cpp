#include "PayBillScreen.h"
#include <iostream>

PayBillScreen::PayBillScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_billIdInput()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::PayBill;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Pay Bill", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Only unpaid bills are shown below.", 20, sf::Vector2f(70.f, 80.f));
    m_patientLabel = Label(m_font, "Patient: ", 22, sf::Vector2f(70.f, 115.f));
    m_headerLabel = Label(m_font, "Bill ID | Appointment ID | Amount (PKR) | Date", 18, sf::Vector2f(70.f, 180.f));

    m_billIdInput = InputBox(
        m_font,
        "Enter bill ID",
        sf::Vector2f(70.f, 560.f),
        sf::Vector2f(380.f, 45.f),
        20,
        false,
        20
    );

    m_payButton = Button(m_font, "Pay", sf::Vector2f(70.f, 625.f), sf::Vector2f(180.f, 45.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(270.f, 625.f), sf::Vector2f(180.f, 45.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 490.f), sf::Vector2f(540.f, 55.f), 18);
}

void PayBillScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void PayBillScreen::appendIntToBuffer(int value, char* buffer, int& index)
{
    if (value == 0)
    {
        buffer[index++] = '0';
        return;
    }

    char reversed[32];
    int r = 0;

    while (value > 0 && r < 31)
    {
        reversed[r++] = static_cast<char>('0' + (value % 10));
        value /= 10;
    }

    for (int i = r - 1; i >= 0; i--)
    {
        buffer[index++] = reversed[i];
    }
}

void PayBillScreen::appendDoubleToBuffer(double value, char* buffer, int maxLength)
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

    appendIntToBuffer(wholePart, buffer, index);

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

void PayBillScreen::submitPayment()
{
    const CustomString& billIdText = m_billIdInput.getText();

    if (billIdText.isEmpty())
    {
        m_messagePanel.setMessage("Please enter a bill ID.");
        return;
    }

    if (!Validator::isValidId(billIdText.cStr()))
    {
        m_messagePanel.setMessage("Invalid bill ID.");
        return;
    }

    int billId = 0;
    const char* idChars = billIdText.cStr();
    int i = 0;
    while (idChars[i] != '\0')
    {
        billId = billId * 10 + (idChars[i] - '0');
        i++;
    }

    int patientId = m_backend.getCurrentUserId();

    try
    {
        double remainingBalance = m_backend.payBill(patientId, billId);

        char balanceText[64];
        appendDoubleToBuffer(remainingBalance, balanceText, 64);

        CustomString message("Bill paid successfully. Remaining balance: PKR ");
        message += balanceText;

        m_messagePanel.setMessage(message.cStr());
    }
    catch (const HospitalException& ex)
    {
        m_messagePanel.setMessage(ex.what());
    }
}

void PayBillScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_billIdInput.handleEvent(event, window);
    m_payButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_payButton.isClicked(event, window))
    {
        submitPayment();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::PatientDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        submitPayment();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::PatientDashboard);
    }
}

void PayBillScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
    m_billIdInput.update();
}

void PayBillScreen::renderUnpaidBills(sf::RenderWindow& window) const
{
    const int patientId = m_backend.getCurrentUserId();
    const Bill* billData = m_backend.getBills().getAll();
    const Doctor* doctorData = m_backend.getDoctors().getAll();

    bool found = false;
    float y = 220.f;

    for (int i = 0; i < m_backend.getBills().size(); i++)
    {
        const Bill& bill = billData[i];

        if (bill.getPatientId() == patientId && bill.isUnpaid())
        {
            sf::Text row;
            row.setFont(m_font);
            row.setCharacterSize(17);
            row.setFillColor(sf::Color::White);
            row.setPosition(70.f, y);

            CustomString line;
            line.setText("");

            char billIdText[32];
            char appointmentIdText[32];
            char amountText[64];

            int billId = bill.getBillId();
            int appointmentId = bill.getAppointmentId();

            int idx1 = 0;
            if (billId == 0)
            {
                billIdText[idx1++] = '0';
            }
            else
            {
                char reversed1[32];
                int r1 = 0;
                while (billId > 0 && r1 < 31)
                {
                    reversed1[r1++] = static_cast<char>('0' + (billId % 10));
                    billId /= 10;
                }
                for (int j = r1 - 1; j >= 0; j--)
                {
                    billIdText[idx1++] = reversed1[j];
                }
            }
            billIdText[idx1] = '\0';

            int idx2 = 0;
            if (appointmentId == 0)
            {
                appointmentIdText[idx2++] = '0';
            }
            else
            {
                char reversed2[32];
                int r2 = 0;
                while (appointmentId > 0 && r2 < 31)
                {
                    reversed2[r2++] = static_cast<char>('0' + (appointmentId % 10));
                    appointmentId /= 10;
                }
                for (int j = r2 - 1; j >= 0; j--)
                {
                    appointmentIdText[idx2++] = reversed2[j];
                }
            }
            appointmentIdText[idx2] = '\0';

            appendDoubleToBuffer(bill.getAmount(), amountText, 64);

            line += billIdText;
            line += " | ";
            line += appointmentIdText;
            line += " | ";
            line += amountText;
            line += " | ";
            line += bill.getDate().cStr();

            row.setString(line.cStr());
            window.draw(row);

            y += 28.f;
            found = true;
        }
    }

    if (!found)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(70.f, 220.f);
        none.setString("No unpaid bills.");
        window.draw(none);
    }
}

void PayBillScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);

    CustomString patientText("Patient: ");
    patientText += m_backend.getCurrentUserName();
    m_patientLabel.setText(patientText.cStr());
    m_patientLabel.draw(window);

    m_headerLabel.draw(window);

    renderUnpaidBills(window);

    m_billIdInput.draw(window);
    m_payButton.draw(window);
    m_backButton.draw(window);
    m_messagePanel.draw(window);
}

bool PayBillScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState PayBillScreen::getRequestedState() const
{
    return m_requestedState;
}

void PayBillScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}