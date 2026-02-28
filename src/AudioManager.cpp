#include "AudioManager.h"

void AudioManager::Load()
{
    ballBounce = LoadSound("assets/audio/BallBounce.ogg");
}

void AudioManager::Unload()
{
    UnloadSound(ballBounce);
}