#pragma once
#include "Ball.h"
#include <vector>
#include "Colliders.h"

struct PhysicsEvents // For use with PhysicsManager::Update
{
    bool ballBounce = false; // true if ball collided with anything
};

class PhysicsManager
{
public:
    static constexpr float GRAVITY = 1000.0f;
    // static constexpr float MAX_BALL_SPEED = 2000.0f; // Length (norm) of speed vector, not length squared
    static constexpr float BOUNCE_DAMPING = 0.6f;
    static constexpr double dt = 1.0/600.0; // Fixed delta time
    
    const float ballFlipperTuning = 0.05f;

    std::vector<PhysicsEvents> Update(std::vector<Ball*> &balls, std::vector<LineCollider*> &lineColliders, std::vector<CircleCollider*> &circles);

private:
    int stepCount = 0; // Number of physics steps performed
};