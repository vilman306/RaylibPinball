#include "raylib.h"
#include "raymath.h"
#include "PhysicsManager.h"
#include "Ball.h"
#include "Vec2Extensions.h"
#include "Config.h"

PhysicsEvents PhysicsManager::Update(Ball &ball, float dtGame)
{
    PhysicsEvents events;

    // Perform a physics step 1/dt times per second
    dtGameSum += dtGame;
    while (dtGameSum >= dt)
    {
        PhysicsManager::PhysicsStep(ball, events);
        dtGameSum -= dt;
    }

    return events;
    
}

void PhysicsManager::PhysicsStep(Ball &ball, PhysicsEvents &events)
{
    Vector2 ballAcc = {0, GRAVITY};  // Virtual ball acceleration
    Vector2 ballVel = ball.velocity; // Virtual ball velocity
    Vector2 ballPos = ball.position; // Virtual ball position

    ballVel += ballAcc * dt;

    // Ball - edge collision:
    if (ballPos.y + ball.radius >= Config::gameHeight) // Ball - bottom edge
    {
        ballPos.y = Config::gameHeight - ball.radius;
        ballVel.y *= -1;
        events.ballBounce = true;
    }
    // if (ball.position.y - ball.radius <= 0.0f) // Ball - upper edge
    // {
    //     ball.position.y = ball.radius;
    //     ball.velocity.y *= -1;
    //     events.ballBounce = true;
    // }
    if (ballPos.x + ball.radius >= Config::gameWidth) // Ball - right edge
    {
        ballPos.x = Config::gameWidth - ball.radius;
        ballVel.x *= -1;
        events.ballBounce = true;
    }
    if (ballPos.x - ball.radius <= 0) // Ball - left edge
    {
        ballPos.x = ball.radius;
        ballVel.x *= -1;
        events.ballBounce = true;
    }
    if (events.ballBounce == true)
    {
        ballVel *= COLLISION_DAMPING; // Apply damping
    }

    // Limit ball speed
    const float currentBallSpeed = Vector2Length(ballVel);
    if (currentBallSpeed >= MAX_BALL_SPEED)
    {
        ballVel = Vector2Normalize(ballVel) * MAX_BALL_SPEED;
    }

    ballPos += ballVel * dt;

    // Assign the ball's new velocity and position
    ball.velocity = ballVel;
    ball.position = ballPos;
}