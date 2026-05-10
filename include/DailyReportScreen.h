#ifndef DAILYREPORTSCREEN_H
#define DAILYREPORTSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "Label.h"
#include "MessagePanel.h"

class DailyReportScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_headerLabel;

    Button m_refreshButton;
    Button m_backButton;

    MessagePanel m_messagePanel;

    CustomString m_reportContent;
    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void loadReport();
    void renderReportLines(sf::RenderWindow& window) const;

public:
    DailyReportScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void onEnter() override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif