#include "raylib.h"
#include "raymath.h"
#include "PhysicsManager.h"
#include "Ball.h"
#include "Vec2Extensions.h"
#include "Config.h"

PhysicsEvents PhysicsManager::Update(Ball &ball, std::vector<Line> &lines)
{
    PhysicsEvents events;
    
    Vector2 ballAcc = {0, GRAVITY};          // Virtual ball acceleration
    Vector2 ballVel = ball.velocity;         // Virtual ball velocity
    Vector2 ballPos = ball.physicalPosition; // Virtual ball position
    
    ballVel += ballAcc * dt;
    ballPos += ballVel * dt;

    // Ball - edge collision:
    if (ballPos.y + ball.radius > Config::gameHeight) // Ball - bottom edge
    {
        events.ballBounce = true;
        ballPos.y = Config::gameHeight - ball.radius;
        ballVel.y *= -1;
    }
    // if (ball.position.y - ball.radius < 0.0f) // Ball - upper edge
    // {
        //     events.ballBounce = true;
    //     ball.position.y = ball.radius;
    //     ball.velocity.y *= -1;
    // }
    if (ballPos.x + ball.radius > Config::gameWidth) // Ball - right edge
    {
        events.ballBounce = true;
        ballPos.x = Config::gameWidth - ball.radius;
        ballVel.x *= -1;
    }
    if (ballPos.x - ball.radius < 0) // Ball - left edge
    {
        events.ballBounce = true;
        ballPos.x = ball.radius;
        ballVel.x *= -1;
    }

    // Ball - line collision
    for (Line &line : lines)
    {
        Vector2 normal = line.normal;
        Vector2 v1 = ballPos - line.pos1;
        Vector2 v2 = line.pos2 - line.pos1;
        float t = Clamp(Vector2DotProduct(v1, v2) / Vector2LengthSqr(v2), 0.0f, 1.0f);
        Vector2 p = line.pos1 + t * v2; // Ball's "clamped" projection on line
        float d = Vector2Length(ballPos - p);
        if (d < ball.radius)
        {
            events.ballBounce = true;
            ballPos = p + normal * ball.radius;
            Vector2 velN = normal * Vector2DotProduct(ballVel, normal);
            ballVel = ballVel - 2.0f * velN;
        }
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