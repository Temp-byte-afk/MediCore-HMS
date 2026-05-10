#ifndef VIEWBILLSSCREEN_H
#define VIEWBILLSSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "Label.h"
#include "MessagePanel.h"

class ViewBillsScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_patientLabel;
    Label m_headerLabel;
    Label m_totalLabel;

    Button m_backButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    int compareBillsForDisplay(const Bill& first, const Bill& second) const;
    void renderBillList(sf::RenderWindow& window) const;

public:
    ViewBillsScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif