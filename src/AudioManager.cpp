#include "AudioManager.h"
#include "Bumper.h"
#include "Ball.h"
#include "Wall.h"
#include "Flipper.h"

#include <iostream>
void AudioManager::Load()
{
    ballBounce = LoadSound("assets/audio/BallBounce.ogg");
}

void AudioManager::Unload()
{
    UnloadSound(ballBounce);
}
