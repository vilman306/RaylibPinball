#include "raylib.h"
#define RAYMATH_DISABLE_CPP_OPERATORS // OBS!
#include "raymath.h"
#include "PhysicsManager.h"
#include "Ball.h"
#include "Vec2Extensions.h"
#include "Config.h"

PhysicsManager::PhysicsEvents PhysicsManager::Update(Ball &ball, float dt)
{
    PhysicsEvents events;
    events.ballBounce = false;
    
    Vector2 ballAcc = {0, GRAVITY};
    ball.velocity += ballAcc * dt;

    // Ball - edge collision
    if (ball.position.y + ball.radius >= Config::height) // Ball - bottom edge
    {
        ball.position.y = Config::height - ball.radius;
        ball.velocity.y *= -1;
        events.ballBounce = true;
    }
    if (ball.position.y - ball.radius <= 0.0f) // Ball - upper edge
    {
        ball.position.y = ball.radius;
        ball.velocity.y *= -1;
        events.ballBounce = true;
    }
    if (ball.position.x + ball.radius >= Config::width) // Ball - right edge
    {
        ball.position.x = Config::width - ball.radius;
        ball.velocity.x *= -1;
        events.ballBounce = true;
    }
    if (ball.position.x - ball.radius <= 0) // Ball - left edge
    {
        ball.position.x = ball.radius;
        ball.velocity.x *= -1;
        events.ballBounce = true;
    }
    const float colDamping = 0.7f;
    if (events.ballBounce == true)
    {
        ball.velocity *= colDamping; // Damping
    }

    const float currentBallSpeed = Vector2Length(ball.velocity);
    if (currentBallSpeed >= MAX_BALL_SPEED)
    {
        ball.velocity = Vector2Normalize(ball.velocity) * MAX_BALL_SPEED;
    }
    
    ball.position += ball.velocity;
    return events;
}