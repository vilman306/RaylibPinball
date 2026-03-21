#include "raylib.h"
#include "raymath.h"
#include "PhysicsManager.h"
#include "Ball.h"
#include "Vec2Extensions.h"
#include "Config.h"
#include "Flipper.h"

std::vector<PhysicsEvents> PhysicsManager::Update(std::vector<Ball> &balls, std::vector<Line*> &lines, std::vector<Circle*> &circles)
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
        for (Line *line : lines)
        {
            Vector2 normal = line->normal;
            Vector2 v1 = ballPos - line->pos1;
            Vector2 v2 = line->pos2 - line->pos1;
            float t = Clamp(Vector2DotProduct(v1, v2) / Vector2LengthSqr(v2), 0.0f, 1.0f);
            Vector2 p = line->pos1 + t * v2; // Ball's "clamped" projection on line
            float d = Vector2Length(ballPos - p);
            if (d < ballRad)
            {
                events.ballBounce = true;
                ballPos = p + normal * ballRad;
                Vector2 velN = normal * Vector2DotProduct(ballVel, normal);
                ballVel = ballVel - 2.0f * velN;
                ballVel *= BOUNCE_DAMPING;
                if (line->owner != nullptr)
                {
                    // If line owner is flipper and it's rotating, add velocity in normal direction with speed angularSpeed * (length from rotation point)
                    Flipper* flipper = static_cast<Flipper*>(line->owner);
                    if (flipper->angle < flipper->maxAngle && flipper->angle > flipper->minAngle)
                        ballVel += normal * flipper->angularSpeed * flipper->length * t;
                }
            }
        }

        // Ball - ball collision
        for (int j = i + 1; j < balls.size(); j++)
        {
            Ball &ballB = balls[j];
            Vector2 &ballBPos = ballB.physicalPosition;
            Vector2 &ballBVel = ballB.velocity;
            float ballBRad = ballB.circle.radius;

            Vector2 posDiff = ballPos - ballBPos;
            float posDiffLen = Vector2Length(posDiff);
            
            if (posDiffLen < ballRad + ballBRad)
            {
                events.ballBounce = true;
                Vector2 normal = Vector2Normalize(posDiff);
                float penetration = ballRad + ballBRad - posDiffLen;
                ballPos += normal * 0.5f * penetration;
                ballBPos -= normal * 0.5f * penetration;
                Vector2 velN = normal * Vector2DotProduct(ballVel, normal);
                Vector2 velNB = normal * Vector2DotProduct(ballBVel, normal);
                ballVel += (velNB - velN);
                ballBVel += (velN - velNB);
                ballVel *= BOUNCE_DAMPING;
                ballBVel *= BOUNCE_DAMPING;
                
            }
        }

        // Ball - circle collision
        for (Circle *circle : circles)
        {
            Vector2 circlePos = circle->position;
            float circleRad = circle->radius;
            Vector2 posDiff = ballPos - circlePos;
            float posDiffLen = Vector2Length(posDiff);
            if (posDiffLen < ballRad + circleRad)
            {
                events.ballBounce = true;
                Vector2 normal = Vector2Normalize(posDiff);
                ballPos = circlePos + normal * (ballRad + circleRad);
                Vector2 velN = normal * Vector2DotProduct(ballVel, normal);
                ballVel -= 2.0f * velN;
                ballVel *= BOUNCE_DAMPING;
                if (circle->owner != nullptr)
                {
                    // If line owner is flipper and it's rotating, add velocity in normal direction with speed angularSpeed * (length from rotation point)
                    Flipper *flipper = static_cast<Flipper *>(circle->owner);
                    if (flipper->angle < flipper->maxAngle && flipper->angle > flipper->minAngle)
                        ballVel += normal * flipper->angularSpeed * flipper->length;
                }
            }
        }


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
