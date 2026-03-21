#include "raylib.h"
#include "raymath.h"
#include "PhysicsManager.h"
#include "Ball.h"
#include "Vec2Extensions.h"
#include "Config.h"

std::vector<PhysicsEvents> PhysicsManager::Update(std::vector<Ball> &balls, std::vector<Line> &lines)
{
    std::vector<PhysicsEvents> eventsPerBall;
    

    for (int i = 0; i < balls.size(); i++)
    {
        PhysicsEvents events;

        Ball &ball = balls[i];

        Vector2 ballAcc = {0, -GRAVITY};          // Virtual ball acceleration
        Vector2 ballVel = ball.velocity;         // Virtual ball velocity
        Vector2 ballPos = ball.physicalPosition; // Virtual ball position
        float ballRad = ball.circle.radius;

        ballVel += ballAcc * dt;
        ballPos += ballVel * dt;

        // Ball - edge collision:
        if (ballPos.y - ballRad < 0.0f) // Ball - bottom edge
        {
            events.ballBounce = true;
            ballPos.y = ballRad;
            ballVel.y *= -1;
            ballVel *= BOUNCE_DAMPING;
        }
        if (ballPos.x + ballRad > Config::gameWidth) // Ball - right edge
        {
            events.ballBounce = true;
            ballPos.x = Config::gameWidth - ballRad;
            ballVel.x *= -1;
            ballVel *= BOUNCE_DAMPING;
        }
        if (ballPos.x - ballRad < 0.0f) // Ball - left edge
        {
            events.ballBounce = true;
            ballPos.x = ballRad;
            ballVel.x *= -1;
            ballVel *= BOUNCE_DAMPING;
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
            if (d < ballRad)
            {
                events.ballBounce = true;
                ballPos = p + normal * ballRad;
                Vector2 velN = normal * Vector2DotProduct(ballVel, normal);
                ballVel = ballVel - 2.0f * velN;
                ballVel *= BOUNCE_DAMPING;
            }
        }

        // Ball - ball collision

        // Limit ball speed
        const float currentBallSpeed = Vector2Length(ballVel);
        if (currentBallSpeed >= MAX_BALL_SPEED)
        {
            ballVel = Vector2Normalize(ballVel) * MAX_BALL_SPEED;
        }

        // Assign the ball's new position and velocity
        ball.prevPhysicalPosition = ball.physicalPosition;
        ball.physicalPosition = ballPos;
        ball.velocity = ballVel;

        eventsPerBall.push_back(events);
    }
    
    

    

    stepCount += 1;
    return eventsPerBall;
}
