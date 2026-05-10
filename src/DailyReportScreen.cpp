#include "DailyReportScreen.h"
#include <iostream>

DailyReportScreen::DailyReportScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath)
    : BaseScreen(backend, screenManager),
    m_refreshButton(),
    m_backButton()
{
    m_navigationRequested = false;
    m_requestedState = ScreenState::DailyReport;

    if (!m_font.loadFromFile(fontFilePath))
    {
        std::cout << "Warning: could not load font file.\n";
    }

    m_titleLabel = Label(m_font, "Generate Daily Report", 34, sf::Vector2f(70.f, 30.f));
    m_hintLabel = Label(m_font, "Derived from current backend data", 20, sf::Vector2f(70.f, 80.f));
    m_headerLabel = Label(m_font, "Today's summary", 18, sf::Vector2f(70.f, 140.f));

    m_refreshButton = Button(m_font, "Refresh", sf::Vector2f(70.f, 650.f), sf::Vector2f(180.f, 45.f));
    m_backButton = Button(m_font, "Back", sf::Vector2f(270.f, 650.f), sf::Vector2f(180.f, 45.f));

    m_messagePanel = MessagePanel(m_font, sf::Vector2f(70.f, 590.f), sf::Vector2f(900.f, 45.f), 18);
}

void DailyReportScreen::requestScreen(ScreenState state)
{
    m_requestedState = state;
    m_navigationRequested = true;
}

void DailyReportScreen::loadReport()
{
    if (m_backend.getDailyReportContent(m_reportContent))
    {
        m_messagePanel.setMessage("Daily report loaded.");
    }
    else
    {
        m_reportContent.setText("No report available.");
        m_messagePanel.setMessage("No report available.");
    }
}

void DailyReportScreen::onEnter()
{
    loadReport();
}

void DailyReportScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    m_refreshButton.handleEvent(event, window);
    m_backButton.handleEvent(event, window);

    if (m_refreshButton.isClicked(event, window))
    {
        loadReport();
    }
    else if (m_backButton.isClicked(event, window))
    {
        requestScreen(ScreenState::AdminDashboard);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestScreen(ScreenState::AdminDashboard);
    }
}

void DailyReportScreen::update(sf::Time deltaTime)
{
    (void)deltaTime;
}

void DailyReportScreen::renderReportLines(sf::RenderWindow& window) const
{
    if (m_reportContent.isEmpty())
    {
        sf::Text none;
        none.setFont(m_font);
        none.setCharacterSize(18);
        none.setFillColor(sf::Color::White);
        none.setPosition(70.f, 190.f);
        none.setString("No report available.");
        window.draw(none);
        return;
    }

    const char* text = m_reportContent.cStr();
    int start = 0;
    float y = 190.f;
    int visibleLines = 0;

    while (text[start] != '\0' && visibleLines < 16)
    {
        char lineBuffer[1024];
        int lineIndex = 0;

        while (text[start] != '\0' && text[start] != '\n' && lineIndex < 1023)
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

            y += 26.f;
            visibleLines++;
        }
    }
}

void DailyReportScreen::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(22, 22, 22));

    m_titleLabel.draw(window);
    m_hintLabel.draw(window);
    m_headerLabel.draw(window);

    renderReportLines(window);

    m_refreshButton.draw(window);
    m_backButton.draw(window);
    m_messagePanel.draw(window);
}

bool DailyReportScreen::hasNavigationRequest() const
{
    return m_navigationRequested;
}

ScreenState DailyReportScreen::getRequestedState() const
{
    return m_requestedState;
}

void DailyReportScreen::clearNavigationRequest()
{
    m_navigationRequested = false;
}