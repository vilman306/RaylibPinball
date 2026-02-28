#pragma once
#include "Ball.h"

class PhysicsManager
{
public:
    static constexpr float GRAVITY = 9.82f * 10.0f;
    static constexpr float MAX_BALL_SPEED = 50.0f; // Length (norm) of speed vector, not length squared
    struct PhysicsEvents {
        bool ballBounce = false;
    };

    PhysicsEvents Update(Ball &ball, float dt); // Handles collisions, contact forces etc.
};