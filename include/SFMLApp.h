#ifndef SFMLAPP_H
#define SFMLAPP_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "ScreenManager.h"
#include "MediCoreSystem.h"
#include "ScreenFactory.h"

class SFMLApp
{
private:
    sf::RenderWindow m_window;
    MediCoreSystem& m_backend;
    ScreenManager m_screenManager;
    bool m_isRunning;
    ScreenFactory m_screenFactory;

public:
    SFMLApp(
        MediCoreSystem& backend,
        const char* fontFilePath,
        std::unique_ptr<BaseScreen> initialScreen = nullptr,
        unsigned int width = 1280,
        unsigned int height = 720,
        const char* title = "MediCore Hospital Management System"
    );

    void run();
    void close();

    bool isRunning() const;

    sf::RenderWindow& getWindow();
    ScreenManager& getScreenManager();
    MediCoreSystem& getBackend();
};

#endif