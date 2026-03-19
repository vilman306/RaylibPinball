#pragma once
#include "Ball.h"

struct PhysicsEvents // For use with PhysicsManager::Update
{
    bool ballBounce = false; // true if ball collided with anything
};

class PhysicsManager
{
public:
    static constexpr float GRAVITY = 1000.0f;
    static constexpr float MAX_BALL_SPEED = 2000.0f; // Length (norm) of speed vector, not length squared
    static constexpr float BOUNCE_DAMPING = 0.9f;
    static constexpr double dt = 1.0/60.0; // Fixed delta time

    PhysicsEvents Update(Ball &ball);
    int getStepCount() { return stepCount; };
private:
    int stepCount = 0; // Number of physics steps performed
};