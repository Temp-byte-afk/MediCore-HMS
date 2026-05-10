#include "SecurityLogScreen.h"
#include <iostream>

SecurityLogScreen::SecurityLogScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::SecurityLog;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "View Security Log", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Failed and successful login events", 20, sf::Vector2f(70.f, 80.f));
    m_headerLabel = Label(m_font, "Timestamp, Role, Entered ID, Result", 18, sf::Vector2f(70.f, 140.f));

    m_backButton = Button(m_font, "Back", sf::Vector2f(70.f, 650.f), sf::Vector2f(180.f, 45.f));
    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 590.f), sf::Vector2f(900.f, 45.f), 18);
}

void SecurityLogScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void SecurityLogScreen::loadLogContent()
{
    try
    {
        if (!m_backend.getSecurityLogContent(m_logContent))
        {
            m_logContent.clear();
        }

        if (m_logContent.isEmpty())
        {
            m_messagePanel.setMessage("No security events logged.");
        }
        else
        {
            m_messagePanel.setMessage("Security log loaded.");
        }
    }
    catch (const HospitalException& ex)
    {
        m_logContent.clear();
        m_messagePanel.setMessage(ex.what());
    }
}

void SecurityLogScreen::onEnter()
{
    loadLogContent();
}

void SecurityLogScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
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

void SecurityLogScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void SecurityLogScreen::renderLogLines(sf::RenderWindow& window) const
{
    if (m_logContent.isEmpty())
    {
        return;
    }

    const char* text = m_logContent.cStr();
    int start = 0;
    float y = 190.f;
    int visibleLines = 0;

    while (text[start] != '\0' && visibleLines < 14)
    {
        char lineBuffer[512];
        int lineIndex = 0;

        while (text[start] != '\0' && text[start] != '\n' && lineIndex < 511)
        {
            lineBuffer[lineIndex++] = text[start++];
        }

        lineBuffer[lineIndex] = '\0';

        if (text[start] == '\n')
        {
            start++;
        }

        if (lineIndex > 0)
        {
            sf::Text row;
            row.setFont(m_font);
            row.setCharacterSize(16);
            row.setFillColor(sf::Color::White);
            row.setPosition(70.f, y);
            row.setString(lineBuffer);
            window.draw(row);

            y += 28.f;
            visibleLines++;
        }
    }
}

void SecurityLogScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);
    m_headerLabel.draw(window);

    renderLogLines(window);

    m_backButton.draw(window);
    m_messagePanel.draw(window);
}

bool SecurityLogScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState SecurityLogScreen::getRequestedState() const
{
    return m_requestedState;
}

void SecurityLogScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}