#include "AddDoctorScreen.h"
#include <iostream>

AddDoctorScreen::AddDoctorScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_nameInput(),
    m_specializationInput(),
    m_contactInput(),
    m_passwordInput(),
    m_feeInput()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::AddDoctor;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Add Doctor", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Enter doctor details carefully.", 20, sf::Vector2f(70.f, 80.f));

    m_nameLabel = Label(m_font, "Name", 22, sf::Vector2f(70.f, 130.f));
    m_specializationLabel = Label(m_font, "Specialization", 22, sf::Vector2f(70.f, 220.f));
    m_contactLabel = Label(m_font, "Contact (11 digits)", 22, sf::Vector2f(70.f, 310.f));
    m_passwordLabel = Label(m_font, "Password (min 6 chars)", 22, sf::Vector2f(70.f, 400.f));
    m_feeLabel = Label(m_font, "Consultation Fee (PKR)", 22, sf::Vector2f(70.f, 490.f));

    m_nameInput = InputBox(m_font, "Enter name", sf::Vector2f(70.f, 165.f), sf::Vector2f(420.f, 45.f), 20, false, 50);
    m_specializationInput = InputBox(m_font, "Enter specialization", sf::Vector2f(70.f, 255.f), sf::Vector2f(420.f, 45.f), 20, false, 50);
    m_contactInput = InputBox(m_font, "Enter contact", sf::Vector2f(70.f, 345.f), sf::Vector2f(420.f, 45.f), 20, false, 11);
    m_passwordInput = InputBox(m_font, "Enter password", sf::Vector2f(70.f, 435.f), sf::Vector2f(420.f, 45.f), 20, true, 50);
    m_feeInput = InputBox(m_font, "Enter fee", sf::Vector2f(70.f, 525.f), sf::Vector2f(420.f, 45.f), 20, false, 20);

    m_submitButton = Button(m_font, "Add Doctor", sf::Vector2f(70.f, 610.f), sf::Vector2f(180.f, 50.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(270.f, 610.f), sf::Vector2f(180.f, 50.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(540.f, 160.f), sf::Vector2f(650.f, 130.f), 18);
}

void AddDoctorScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void AddDoctorScreen::submitDoctor()
{
    const CustomString& name = m_nameInput.getText();
    const CustomString& specialization = m_specializationInput.getText();
    const CustomString& contact = m_contactInput.getText();
    const CustomString& password = m_passwordInput.getText();
    const CustomString& fee = m_feeInput.getText();

    if (name.isEmpty() || specialization.isEmpty() || contact.isEmpty() || password.isEmpty() || fee.isEmpty())
    {
        m_messagePanel.setMessage("Please fill all fields.");
        return;
    }

    try
    {
        int newDoctorId = m_backend.addDoctor(
            name.cStr(),
            specialization.cStr(),
            contact.cStr(),
            password.cStr(),
            fee.cStr()
        );

        char idBuffer[32];
        int value = newDoctorId;
        int index = 0;

        if (value == 0)
        {
            idBuffer[index++] = '0';
        }
        else
        {
            char reversed[32];
            int r = 0;
            while (value > 0 && r < 31)
            {
                reversed[r++] = static_cast<char>('0' + (value % 10));
                value /= 10;
            }

            for (int i = r - 1; i >= 0; i--)
            {
                idBuffer[index++] = reversed[i];
            }
        }
        idBuffer[index] = '\0';

        CustomString message("Doctor added successfully. ID: ");
        message += idBuffer;
        m_messagePanel.setMessage(message.cStr());
    }
    catch (const HospitalException& ex)
    {
        m_messagePanel.setMessage(ex.what());
    }
}

void AddDoctorScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_nameInput.handleEvent(event, window);
    m_specializationInput.handleEvent(event, window);
    m_contactInput.handleEvent(event, window);
    m_passwordInput.handleEvent(event, window);
    m_feeInput.handleEvent(event, window);

    m_submitButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_submitButton.isClicked(event, window))
    {
        submitDoctor();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::AdminDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        submitDoctor();
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::AdminDashboard);
    }
}

void AddDoctorScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
    m_nameInput.update();
    m_specializationInput.update();
    m_contactInput.update();
    m_passwordInput.update();
    m_feeInput.update();
}

void AddDoctorScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);

    m_nameLabel.draw(window);
    m_specializationLabel.draw(window);
    m_contactLabel.draw(window);
    m_passwordLabel.draw(window);
    m_feeLabel.draw(window);

    m_nameInput.draw(window);
    m_specializationInput.draw(window);
    m_contactInput.draw(window);
    m_passwordInput.draw(window);
    m_feeInput.draw(window);

    m_submitButton.draw(window);
    m_backButton.draw(window);

    m_messagePanel.draw(window);
}

bool AddDoctorScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState AddDoctorScreen::getRequestedState() const
{
    return m_requestedState;
}

void AddDoctorScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}