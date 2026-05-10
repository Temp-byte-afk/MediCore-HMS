#ifndef MARKCOMPLETESCREEN_H
#define MARKCOMPLETESCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"
#include "MessagePanel.h"

class MarkCompleteScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_doctorLabel;
    Label m_idLabel;

    InputBox m_appointmentIdInput;

    Button m_submitButton;
    Button m_backButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void submitCompletion();

public:
    MarkCompleteScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif