#pragma once
#include "raylib.h"
#include <vector>
#include "PhysicsManager.h"

class AudioManager // Not used (yet)
{
public:
    Sound ballBounce;
    
    void Load(); // Load audio files to memory
    void Unload(); // Unload audio files from memory
};