#include "raylib.h"
#include "raymath.h"
#include "PhysicsManager.h"
#include "Ball.h"
#include "Vec2Extensions.h"
#include "Config.h"
#include "Flipper.h"
#include <iostream>

std::vector<PhysicsEvents> PhysicsManager::Update(std::vector<Ball*>& balls, std::vector<LineCollider*>& lineColliders, std::vector<CircleCollider*>& circleColliders)
{
    std::vector<PhysicsEvents> eventsPerBall;

    for (int i = 0; i < balls.size(); i++)
    {
        PhysicsEvents events;

        Ball* ball = balls[i];

        Vector2 ballAcc = {0, -GRAVITY};          // Virtual ball acceleration
        Vector2 ballVel = ball->velocity;         // Virtual ball velocity
        Vector2 ballPos = ball->circleCollider.circle.position; // Virtual ball position
        float ballRad = ball->circleCollider.circle.radius;


        ballVel += ballAcc * dt;
        ballPos += ballVel * dt;

        // Ball - edge collision:
        if (ballPos.y - ballRad < 0.0f) // Ball - bottom edge
        {
            events.ballBounce = true;
            ballPos.y = ballRad;
            ballVel.y *= -1;
            ballVel.y *= BOUNCE_DAMPING;
        }
        if (ballPos.y + ballRad > Config::gameHeight) // Ball - upper edge
        {
            events.ballBounce = true;
            ballPos.y = Config::gameHeight - ballRad;
            ballVel.y *= -1;
            ballVel.y *= BOUNCE_DAMPING;
        }
        if (ballPos.x + ballRad > Config::gameWidth) // Ball - right edge
        {
            events.ballBounce = true;
            ballPos.x = Config::gameWidth - ballRad;
            ballVel.x *= -1;
            ballVel.x *= BOUNCE_DAMPING;
        }
        if (ballPos.x - ballRad < 0.0f) // Ball - left edge
        {
            events.ballBounce = true;
            ballPos.x = ballRad;
            ballVel.x *= -1;
            ballVel.x *= BOUNCE_DAMPING;
        }

        // Ball - circle collision
        for (CircleCollider* circleCollider : circleColliders)
        {
            Vector2 circlePos = circleCollider->circle.position;
            float circleRad = circleCollider->circle.radius;
            Vector2 posDiff = ballPos - circlePos;
            float posDiffLen = Vector2Length(posDiff);
            if (posDiffLen < ballRad + circleRad)
            {
                events.ballBounce = true;
                Vector2 normal = Vector2Normalize(posDiff);
                ballPos = circlePos + normal * (ballRad + circleRad);

                float signedSpeedN = Vector2DotProduct(ballVel, normal);
                if (signedSpeedN < 0.0f)
                { // To prevent bugs if flipper moves into ball while ball is traveling from flipper
                    Vector2 velN = normal * signedSpeedN * BOUNCE_DAMPING;
                    ballVel -= 2.0f * velN;
                }

                if (circleCollider->role != CircleCollider::CircleColliderRole::FlipperTip)
                    continue;

                Flipper* flipper = static_cast<Flipper*>(circleCollider->owner); // Warning! Will cause problems if line can have owners of different type than Flipper
                if (!(flipper->physicalAngle < flipper->maxAngle && flipper->physicalAngle > flipper->minAngle))
                    continue;

                // Implement flipper "bounce"

                Vector2 p = circlePos + normal * circleRad;
                Vector2 r = p - flipper->rotPos;
                float rLen = Vector2Length(r);

                int flipperDir = flipper->direction;
                Vector2 rNormal = {-r.y, r.x};
                rNormal = Vector2Normalize(flipperDir * rNormal);

                float rSpeed = flipper->angularSpeed * rLen;
                Vector2 rVel = rNormal * rSpeed * flipper->rotDir;

                // Add only the component that pushes the ball outward
                float push = Vector2DotProduct(rVel, normal);
                if (push > 0.0f)
                {
                    ballVel += normal * push * ballFlipperTuning;
                }
            }
        }

        // Ball - line collision
        for (LineCollider* lineCollider : lineColliders)
        {
            Vector2 lineP1 = lineCollider->line.pos1;
            Vector2 lineP2 = lineCollider->line.pos2;
            Vector2 v1 = ballPos - lineP1;
            Vector2 v2 = lineP2 - lineP1;
            float t = Clamp(Vector2DotProduct(v1, v2) / Vector2LengthSqr(v2), 0.0f, 1.0f);
            Vector2 p = lineP1 + t * v2; // Ball's "clamped" projection on line
            float d = Vector2Length(ballPos - p);
            if (d < ballRad)
            {
                events.ballBounce = true;
                Vector2 normal = lineCollider->line.normal;
                ballPos = p + normal * ballRad;
                float signedSpeedN = Vector2DotProduct(ballVel, normal);
                if (signedSpeedN < 0.0f) { // To prevent bugs if flipper moves into ball while ball is traveling from flipper
                    Vector2 velN = normal * signedSpeedN * BOUNCE_DAMPING;
                    ballVel -= 2.0f * velN;
                }
                
                if (lineCollider->owner == nullptr)
                    continue;

                Flipper* flipper = static_cast<Flipper*>(lineCollider->owner); // Warning! Will cause problems if line can have owners of different type than Flipper
                if (!(flipper->physicalAngle < flipper->maxAngle && flipper->physicalAngle > flipper->minAngle))
                    continue;

                Vector2 r = p - flipper->rotPos;
                float rLen = Vector2Length(r);
                
                int flipperSide = 1;
                if (lineCollider->role == LineCollider::LineColliderRole::FlipperDown)
                    flipperSide *= -1;

                int flipperDir = flipper->direction;
                Vector2 rNormal = Vector2Normalize({-r.y, r.x}) * flipperDir * flipperSide;

                float rSpeed = flipper->angularSpeed * rLen;
                Vector2 rVel = rNormal * rSpeed * flipper->rotDir * flipperSide;

                // Add only the component that pushes the ball outward
                float push = Vector2DotProduct(rVel, normal);
                if (push > 0.0f) {
                    ballVel += normal * push * ballFlipperTuning;
                }
            }
        }

        // Ball - ball collision
        for (int j = i + 1; j < balls.size(); j++)
        {
            Ball* ballB = balls[j];
            Vector2& ballBPos = ballB->circleCollider.circle.position;
            Vector2& ballBVel = ballB->velocity;
            float ballBRad = ballB->circleCollider.circle.radius;

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



        // // Limit ball speed
        // const float currentBallSpeed = Vector2Length(ballVel);
        // if (currentBallSpeed >= MAX_BALL_SPEED)
        // {
        //     ballVel = Vector2Normalize(ballVel) * MAX_BALL_SPEED;
        // }

        // Assign the ball's new position and velocity
        ball->prevPhysicalPosition = ball->circleCollider.circle.position;
        ball->circleCollider.circle.position = ballPos;
        ball->velocity = ballVel;

        eventsPerBall.push_back(events);
    }
    
    

    

    stepCount += 1;
    return eventsPerBall;
}
