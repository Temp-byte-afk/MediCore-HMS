#include "ViewUnpaidBillsScreen.h"
#include <iostream>
#include "DateUtils.h"

ViewUnpaidBillsScreen::ViewUnpaidBillsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::ViewUnpaidBills;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "View Unpaid Bills", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Bills older than 7 days are marked overdue", 20, sf::Vector2f(70.f, 80.f));
    m_headerLabel = Label(m_font, "Bill ID | Patient Name | Amount (PKR) | Date", 18, sf::Vector2f(70.f, 140.f));

    m_backButton = Button(m_font, "Back", sf::Vector2f(70.f, 650.f), sf::Vector2f(180.f, 45.f));
    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 590.f), sf::Vector2f(820.f, 45.f), 18);
}

void ViewUnpaidBillsScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void ViewUnpaidBillsScreen::appendIntToBuffer(int value, char* buffer, int maxLength)
{
    if (buffer == nullptr || maxLength <= 0)
    {
        return;
    }

    int index = 0;

    if (value == 0)
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = '0';
        }
        buffer[index] = '\0';
        return;
    }

    if (value < 0)
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = '-';
        }
        value = -value;
    }

    char reversed[32];
    int r = 0;
    while (value > 0 && r < 31)
    {
        reversed[r++] = static_cast<char>('0' + (value % 10));
        value /= 10;
    }

    for (int i = r - 1; i >= 0 && index < maxLength - 1; i--)
    {
        buffer[index++] = reversed[i];
    }

    buffer[index] = '\0';
}

void ViewUnpaidBillsScreen::appendDoubleToBuffer(double value, char* buffer, int maxLength)
{
    if (buffer == nullptr || maxLength <= 0)
    {
        return;
    }

    int index = 0;

    if (value < 0.0)
    {
        if (index < maxLength - 1)
        {
            buffer[index++] = '-';
        }
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
        if (index < maxLength - 1)
        {
            buffer[index++] = '0';
        }
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

        for (int i = r - 1; i >= 0 && index < maxLength - 1; i--)
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

void ViewUnpaidBillsScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_backButton.handleEvent(event, window);

    if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::AdminDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::AdminDashboard);
    }
}

void ViewUnpaidBillsScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void ViewUnpaidBillsScreen::renderBillList(sf::RenderWindow& window) const
{
    const Bill* billData = m_backend.getBills().getAll();
    const Patient* patientData = m_backend.getPatients().getAll();

    int count = 0;
    for (int i = 0; i < m_backend.getBills().size(); i++)
    {
        if (billData[i].isUnpaid())
        {
            count++;
        }
    }

    if (count == 0)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(70.f, 190.f);
        none.setString("No unpaid bills found.");
        window.draw(none);
        return;
    }

    char today[11];
    DateUtils::getCurrentDate(today, 11);
    long long todaySerial = DateUtils::dateToSerial(today);

    float y = 190.f;
    int visibleCount = 0;
    double totalOutstanding = 0.0;

    for (int i = 0; i < m_backend.getBills().size(); i++)
    {
        const Bill& bill = billData[i];

        if (!bill.isUnpaid())
        {
            continue;
        }

        const Patient* patient = nullptr;
        for (int p = 0; p < m_backend.getPatients().size(); p++)
        {
            if (patientData[p].getId() == bill.getPatientId())
            {
                patient = &patientData[p];
                break;
            }
        }

        long long billSerial = DateUtils::dateToSerial(bill.getDate().cStr());
        bool overdue = false;

        if (todaySerial > billSerial && (todaySerial - billSerial) > 7)
        {
            overdue = true;
        }

        sf::Text row;
        row.setFont(m_font);
        row.setCharacterSize(16);
        row.setFillColor(sf::Color::White);
        row.setPosition(70.f, y);

        CustomString line;
        line.setText("");

        char billIdText[32];
        char amountText[64];

        appendIntToBuffer(bill.getBillId(), billIdText, 32);
        appendDoubleToBuffer(bill.getAmount(), amountText, 64);

        line += billIdText;
        line += " | ";

        if (patient != nullptr)
        {
            line += patient->getName().cStr();
        }
        else
        {
            line += "Unknown Patient";
        }

        line += " | ";
        line += amountText;
        line += " | ";
        line += bill.getDate().cStr();

        if (overdue)
        {
            line += " [OVERDUE]";
        }

        row.setString(line.cStr());
        window.draw(row);

        y += 28.f;
        visibleCount++;
        totalOutstanding += bill.getAmount();

        if (visibleCount >= 14)
        {
            break;
        }
    }

    CustomString totalText("Total outstanding unpaid amount: PKR ");
    char totalBuffer[64];
    appendDoubleToBuffer(totalOutstanding, totalBuffer, 64);
    totalText += totalBuffer;

    sf::Text footer;
    footer.setFont(m_font);
    footer.setCharacterSize(20);
    footer.setFillColor(sf::Color::White);
    footer.setPosition(70.f, 555.f);
    footer.setString(totalText.cStr());
    window.draw(footer);
}

void ViewUnpaidBillsScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);
    m_headerLabel.draw(window);

    renderBillList(window);

    m_backButton.draw(window);
    m_messagePanel.draw(window);
}

bool ViewUnpaidBillsScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState ViewUnpaidBillsScreen::getRequestedState() const
{
    return m_requestedState;
}

void ViewUnpaidBillsScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}