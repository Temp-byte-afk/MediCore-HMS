#ifndef SECURITYLOGSCREEN_H
#define SECURITYLOGSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "Label.h"
#include "MessagePanel.h"

class SecurityLogScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_headerLabel;

    Button m_backButton;

    MessagePanel m_messagePanel;

    CustomString m_logContent;
    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void loadLogContent();
    void renderLogLines(sf::RenderWindow& window) const;

public:
    SecurityLogScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void onEnter() override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif