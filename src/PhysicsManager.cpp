#include "raylib.h"
#include "raymath.h"
#include "PhysicsManager.h"
#include "Ball.h"
#include "Vec2Extensions.h"
#include "Config.h"

PhysicsEvents PhysicsManager::Update(Ball &ball)
{
    PhysicsEvents events;
    
    Vector2 ballAcc = {0, GRAVITY};          // Virtual ball acceleration
    Vector2 ballVel = ball.velocity;         // Virtual ball velocity
    Vector2 ballPos = ball.physicalPosition; // Virtual ball position
    
    ballVel += ballAcc * dt;
    ballPos += ballVel * dt;

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

    // Apply damping if ball bounced
    if (events.ballBounce == true)
    {
        ballVel *= BOUNCE_DAMPING;
    }

    // Limit ball speed
    const float currentBallSpeed = Vector2Length(ballVel);
    if (currentBallSpeed >= MAX_BALL_SPEED)
    {
        ballVel = Vector2Normalize(ballVel) * MAX_BALL_SPEED;
    }


    // Assign the ball's new position and velocity
    ball.UpdatePhysicalPosition(ballPos);
    ball.velocity = ballVel;

    stepCount += 1;
    return events;
}