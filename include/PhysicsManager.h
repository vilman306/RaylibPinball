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
    static constexpr float dt = 1.0f/120.0f; // Fixed delta time

    PhysicsEvents Update(Ball &ball, float dtGame); // Calls PhysicsManager::Step depending on the Game's delta time (dtGame).
                                                    // (Step should only be called 1/dt times per second).
                                                    // Returns information in a PhysicsEvents struct.
    int getStepCount() { return stepCount; };
private:
    // float dtGameSum = 0.0f; // Sum of the Game's dt (dtGame). Used in PhysicsManager::Update for calculating when a
                            // Step should be performed. Is "reset" when Step is performed (see definition of Update).
    void Step(Ball &ball, PhysicsEvents &events); // Called from PhysicsManager::Update. Handles collisions etc.
                                                  // Updates the PhysicsEvents from PhysicsManager::Update.
    int stepCount = 0; // Number of physics steps performed
};