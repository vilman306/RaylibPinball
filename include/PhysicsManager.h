#pragma once
#include <vector>

struct Ball;
struct LineCollider;
struct CircleCollider;
struct ColliderOwner;

struct PhysicsMaterial {
  float restitution = 1.0f;
};

struct PhysicsEvents // For use with PhysicsManager::Update
{
    Ball* ball;
    std::vector<ColliderOwner*> hitOwners; // contains the owners of the colliders that the ball collided with
};

class PhysicsManager
{
public:
    static constexpr float GRAVITY = 1000.0f;
    // static constexpr float MAX_BALL_SPEED = 2000.0f; // Length (norm) of speed vector, not length squared
    // static constexpr float BOUNCE_DAMPING = 0.6f;
    static constexpr double dt = 1.0/600.0; // Fixed delta time
    
    const float ballFlipperTuning = 0.08f;

    std::vector<PhysicsEvents> Update(std::vector<Ball*> &balls, std::vector<LineCollider*> &lineColliders, std::vector<CircleCollider*> &circles);

private:
    int stepCount = 0; // Number of physics steps performed
};