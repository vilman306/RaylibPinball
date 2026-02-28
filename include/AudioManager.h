#pragma once
#include "raylib.h"

class AudioManager
{
public:
    Sound ballBounce;
    
    void Load(); // Load audio files to memory
    void Unload(); // Unload audio files from memory
};