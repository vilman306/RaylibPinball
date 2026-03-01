#pragma once
#include "Ball.h"

class PhysicsManager
{
public:
    static constexpr float GRAVITY = 100.0f;
    static constexpr float MAX_BALL_SPEED = 1000.0f; // Length (norm) of speed vector, not length squared
    static constexpr float COLLISION_DAMPING = 0.9f;
    static constexpr float dt = 1/120;
    
    struct PhysicsEvents {
        bool ballBounce = false;
    };

    PhysicsEvents Update(Ball &ball, float dt); // Handles collisions, contact forces etc. Returns information about collisions etc.
};