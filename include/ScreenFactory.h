#ifndef SCREENFACTORY_H
#define SCREENFACTORY_H

#include <memory>
#include "BaseScreen.h"
#include "BookAppointmentScreen.h"
#include "CancelAppointmentScreen.h"

class ScreenFactory
{
private:
    MediCoreSystem& m_backend;
    ScreenManager& m_screenManager;
    const char* m_fontFilePath;

public:
    ScreenFactory(MediCoreSystem& backend, ScreenManager& screenManager, const char* fontFilePath);

    std::unique_ptr<BaseScreen> createScreen(ScreenState state) const;
};

#endif