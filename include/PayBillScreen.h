#ifndef PAYBILLSCREEN_H
#define PAYBILLSCREEN_H

#include "BaseScreen.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"
#include "MessagePanel.h"

class PayBillScreen : public BaseScreen
{
private:
    sf::Font m_font;

    Label m_titleLabel;
    Label m_hintLabel;
    Label m_patientLabel;
    Label m_headerLabel;

    InputBox m_billIdInput;

    Button m_payButton;
    Button m_backButton;

    MessagePanel m_messagePanel;

    bool m_navigationRequested;
    ScreenState m_requestedState;

    void requestScreen(ScreenState state);
    void submitPayment();
    void renderUnpaidBills(sf::RenderWindow& window) const;

    static void appendIntToBuffer(int value, char* buffer, int& index);
    static void appendDoubleToBuffer(double value, char* buffer, int maxLength);

public:
    PayBillScreen(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool hasNavigationRequest() const override;
    ScreenState getRequestedState() const override;
    void clearNavigationRequest() override;
};

#endif