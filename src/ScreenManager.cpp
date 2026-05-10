#include "ScreenManager.h"

ScreenManager::ScreenManager()
{
    m_currentState = ScreenState::Login;
}

void ScreenManager::setScreen(std::unique_ptr<BaseScreen> screen, ScreenState state)
{
    if (m_activeScreen)
    {
        m_activeScreen->onExit();
    }

    m_activeScreen = std::move(screen);
    m_currentState = state;

    if (m_activeScreen)
    {
        m_activeScreen->onEnter();
    }
}

void ScreenManager::clearScreen()
{
    if (m_activeScreen)
    {
        m_activeScreen->onExit();
    }

    m_activeScreen.reset();
    m_currentState = ScreenState::Exit;
}

void ScreenManager::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if (m_activeScreen)
    {
        m_activeScreen->handleEvent(event, window);
    }
}

void ScreenManager::update(sf::Time deltaTime)
{
    if (m_activeScreen)
    {
        m_activeScreen->update(deltaTime);
    }
}

void ScreenManager::render(sf::RenderWindow& window)
{
    if (m_activeScreen)
    {
        m_activeScreen->render(window);
    }
}

bool ScreenManager::hasScreen() const
{
    return m_activeScreen != nullptr;
}

ScreenState ScreenManager::getCurrentState() const
{
    return m_currentState;
}

BaseScreen* ScreenManager::getActiveScreen()
{
    return m_activeScreen.get();
}

const BaseScreen* ScreenManager::getActiveScreen() const
{
    return m_activeScreen.get();
}