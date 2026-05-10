#include "BaseScreen.h"
#include "ScreenManager.h"

BaseScreen::BaseScreen(MediCoreSystem& backend, ScreenManager& screenManager)
    : m_backend(backend), m_screenManager(screenManager)
{
}

BaseScreen::~BaseScreen()
{
}

void BaseScreen::onEnter()
{
}

void BaseScreen::onExit()
{
}

bool BaseScreen::hasNavigationRequest() const
{
    return false;
}

ScreenState BaseScreen::getRequestedState() const
{
    return ScreenState::Exit;
}

void BaseScreen::clearNavigationRequest()
{
}

MediCoreSystem& BaseScreen::getBackend()
{
    return m_backend;
}

ScreenManager& BaseScreen::getScreenManager()
{
    return m_screenManager;
}

const MediCoreSystem& BaseScreen::getBackend() const
{
    return m_backend;
}

const ScreenManager& BaseScreen::getScreenManager() const
{
    return m_screenManager;
}