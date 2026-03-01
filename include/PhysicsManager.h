#pragma once
#include "Ball.h"

struct PhysicsEvents // For use with PhysicsManager::Update
{                            
    bool ballBounce = false; // true if ball collided with anything
};

class PhysicsManager
{
public:
    static constexpr const float GRAVITY = 1000.0f;
    static constexpr const float MAX_BALL_SPEED = 2000.0f; // Length (norm) of speed vector, not length squared
    static constexpr const float COLLISION_DAMPING = 0.9f;
    static constexpr const float dt = 1.0f/120.0f; // Fixed delta time

    PhysicsEvents Update(Ball &ball, float dtGame); // Calls PhysicsStep depending on the Game's delta time (dtGame).
                                                    // (PhysicsStep should only be called 1/dt times per second).
                                                    // Returns information in a PhysicsEvents struct.
private:
    float dtGameSum = 0.0f; // Sum of the Game's dt (dtGame). Used in PhysicsManager::Update for calculating when a
                            // PhysicsStep should be performed.
    void PhysicsStep(Ball &ball, PhysicsEvents &events); // Called from PhysicsManager::Update. Handles collisions etc.
                                                         // Updates the PhysicsEvents from PhysicsManager::Update.
};