#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "BaseScreen.h"

class ScreenManager
{
private:
    std::unique_ptr<BaseScreen> m_activeScreen;
    ScreenState m_currentState;

public:
    ScreenManager();

    void setScreen(std::unique_ptr<BaseScreen> screen, ScreenState state);
    void clearScreen();

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);

    bool hasScreen() const;
    ScreenState getCurrentState() const;

    BaseScreen* getActiveScreen();
    const BaseScreen* getActiveScreen() const;
};

#endif