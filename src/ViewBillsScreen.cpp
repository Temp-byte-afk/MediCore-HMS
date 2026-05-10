#include "ViewBillsScreen.h"
#include <iostream>
#include "DateUtils.h"

ViewBillsScreen::ViewBillsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::ViewBills;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "My Bills", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "All bills for your account", 20, sf::Vector2f(70.f, 80.f));
    m_patientLabel = Label(m_font, "Patient: ", 22, sf::Vector2f(70.f, 115.f));
    m_headerLabel = Label(m_font, "Bill ID | Appointment ID | Amount (PKR) | Status | Date", 18, sf::Vector2f(70.f, 180.f));
    m_totalLabel = Label(m_font, "Total outstanding unpaid amount: PKR 0", 22, sf::Vector2f(70.f, 610.f));

    m_backButton = Button(m_font, "Back", sf::Vector2f(70.f, 650.f), sf::Vector2f(180.f, 45.f));
    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 560.f), sf::Vector2f(650.f, 40.f), 18);
}

void ViewBillsScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

int ViewBillsScreen::compareBillsForDisplay(const Bill& first, const Bill& second) const
{
    int dateCompare = DateUtils::compareDates(first.getDate().cStr(), second.getDate().cStr());

    if (dateCompare != 0)
    {
        return -dateCompare;
    }

    if (first.getBillId() > second.getBillId())
    {
        return -1;
    }
    if (first.getBillId() < second.getBillId())
    {
        return 1;
    }

    return 0;
}

void ViewBillsScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_backButton.handleEvent(event, window);

    if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::PatientDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::PatientDashboard);
    }
}

void ViewBillsScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void ViewBillsScreen::renderBillList(sf::RenderWindow& window) const
{
    const int patientId = m_backend.getCurrentUserId();
    const Bill* billData = m_backend.getBills().getAll();

    int count = 0;
    double totalUnpaid = 0.0;

    for (int i = 0; i < m_backend.getBills().size(); i++)
    {
        if (billData[i].getPatientId() == patientId)
        {
            count++;
            if (billData[i].isUnpaid())
            {
                totalUnpaid += billData[i].getAmount();
            }
        }
    }

    CustomString totalText("Total outstanding unpaid amount: PKR ");
    {
        char amountText[64];
        int whole = static_cast<int>(totalUnpaid);

        int idx = 0;
        if (whole == 0)
        {
            amountText[idx++] = '0';
        }
        else
        {
            char reversed[32];
            int r = 0;
            while (whole > 0 && r < 31)
            {
                reversed[r++] = static_cast<char>('0' + (whole % 10));
                whole /= 10;
            }

            for (int j = r - 1; j >= 0; j--)
            {
                amountText[idx++] = reversed[j];
            }
        }
        amountText[idx] = '\0';
        totalText += amountText;
    }

    if (count == 0)
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(70.f, 220.f);
        none.setString("No bills found.");
        window.draw(none);

        const_cast<ViewBillsScreen*>(this)->m_totalLabel.setText(totalText.cStr());
        return;
    }

    const Bill** sortedBills = new const Bill * [count];
    int index = 0;

    for (int i = 0; i < m_backend.getBills().size(); i++)
    {
        if (billData[i].getPatientId() == patientId)
        {
            sortedBills[index] = &billData[i];
            index++;
        }
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (compareBillsForDisplay(*sortedBills[j], *sortedBills[j + 1]) > 0)
            {
                const Bill* temp = sortedBills[j];
                sortedBills[j] = sortedBills[j + 1];
                sortedBills[j + 1] = temp;
            }
        }
    }

    float y = 220.f;
    int visibleCount = 0;

    for (int i = 0; i < count; i++)
    {
        const Bill& bill = *sortedBills[i];

        sf::Text row;
        row.setFont(m_font);
        row.setCharacterSize(17);
        row.setFillColor(sf::Color::White);
        row.setPosition(70.f, y);

        CustomString line;
        line.setText("");

        char billIdText[32];
        char appointmentIdText[32];

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

        line += billIdText;
        line += " | ";
        line += appointmentIdText;
        line += " | ";
        line += bill.getAmount();
        line += " | ";
        line += bill.getStatus().cStr();
        line += " | ";
        line += bill.getDate().cStr();

        row.setString(line.cStr());
        window.draw(row);

        y += 28.f;
        visibleCount++;

        if (visibleCount >= 12)
        {
            break;
        }
    }

    delete[] sortedBills;

    const_cast<ViewBillsScreen*>(this)->m_totalLabel.setText(totalText.cStr());
}

void ViewBillsScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);

    CustomString patientText("Patient: ");
    patientText += m_backend.getCurrentUserName();
    m_patientLabel.setText(patientText.cStr());
    m_patientLabel.draw(window);

    m_headerLabel.draw(window);

    renderBillList(window);

    m_totalLabel.draw(window);
    m_backButton.draw(window);
    m_messagePanel.draw(window);
}

bool ViewBillsScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState ViewBillsScreen::getRequestedState() const
{
    return m_requestedState;
}

void ViewBillsScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}