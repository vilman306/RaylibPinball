#pragma once
#include "PhysicsManager.h"

namespace Config
{
    // the game's (virtual) width and height
    const int gameWidth = 3000;
    const int gameHeight = 1400;
    
    constexpr const char* windowTitle = "Pinball Game";

    const float dtDebug = PhysicsManager::dt / 4.0;
}