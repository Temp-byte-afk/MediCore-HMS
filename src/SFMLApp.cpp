#include "SFMLApp.h"
#include "ScreenManager.h"

SFMLApp::SFMLApp(
    MediCoreSystem& backend,
    const char* fontFilePath,
    std::unique_ptr<BaseScreen> initialScreen,
    unsigned int width,
    unsigned int height,
    const char* title)
    : m_window(sf::VideoMode(width, height), title),
    m_backend(backend),
    m_screenManager(),
    m_screenFactory(backend, m_screenManager, fontFilePath),
    m_isRunning(true)
{
    m_window.setFramerateLimit(60);

    if (initialScreen)
    {
        m_screenManager.setScreen(std::move(initialScreen), ScreenState::Login);
    }
    else
    {
        m_screenManager.setScreen(m_screenFactory.createScreen(ScreenState::Login), ScreenState::Login);
    }
}


void SFMLApp::run()
{
    sf::Clock clock;

    while (m_window.isOpen() && m_isRunning)
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                close();
            }

            m_screenManager.handleEvent(event, m_window);
        }

        sf::Time deltaTime = clock.restart();
        m_screenManager.update(deltaTime);

        if (m_screenManager.hasScreen())
        {
            BaseScreen* active = m_screenManager.getActiveScreen();
            if (active != nullptr && active->hasNavigationRequest())
            {
                ScreenState nextState = active->getRequestedState();
                active->clearNavigationRequest();

                if (nextState == ScreenState::Exit)
                {
                    close();
                }
                else
                {
                    m_screenManager.setScreen(m_screenFactory.createScreen(nextState), nextState);
                }
            }
        }

        m_window.clear(sf::Color::Black);
        m_screenManager.render(m_window);
        m_window.display();
    }
}

void SFMLApp::close()
{
    m_isRunning = false;
    if (m_window.isOpen())
    {
        m_window.close();
    }
}

bool SFMLApp::isRunning() const
{
    return m_isRunning;
}

sf::RenderWindow& SFMLApp::getWindow()
{
    return m_window;
}

ScreenManager& SFMLApp::getScreenManager()
{
    return m_screenManager;
}

MediCoreSystem& SFMLApp::getBackend()
{
    return m_backend;
}