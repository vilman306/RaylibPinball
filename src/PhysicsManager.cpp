#include "raylib.h"
#include "raymath.h"
#include "PhysicsManager.h"
#include "Ball.h"
#include "Vec2Extensions.h"
#include "Config.h"
#include "Flipper.h"
#include <iostream>
#include "Bumper.h"

std::vector<PhysicsEvents> PhysicsManager::Update(std::vector<Ball*>& balls, std::vector<LineCollider*>& lineColliders, std::vector<CircleCollider*>& circleColliders)
{
    std::vector<PhysicsEvents> eventsPerBall;

    for (int i = 0; i < balls.size(); i++)
    {
        PhysicsEvents events;

        Ball* ball = balls[i];

        events.ball = ball;

        Vector2 ballAcc = {0, -GRAVITY};
        Vector2 ballVel = ball->velocity;
        Vector2 ballPos = ball->circleCollider.circle.position;
        float ballRad = ball->circleCollider.circle.radius;


        ballVel += ballAcc * dt;
        ballPos += ballVel * dt;

        // Ball - circle collision
        for (CircleCollider* circleCollider : circleColliders)
        {
            Vector2 circlePos = circleCollider->circle.position;
            float circleRad = circleCollider->circle.radius;
            Vector2 posDiff = ballPos - circlePos;
            float posDiffLen = Vector2Length(posDiff);
            if (posDiffLen < ballRad + circleRad)
            {
                events.hitOwners.push_back(circleCollider->owner);
                
                Vector2 normal = Vector2Normalize(posDiff);
                ballPos = circlePos + normal * (ballRad + circleRad);

                float signedSpeedN = Vector2DotProduct(ballVel, normal);

                if (signedSpeedN < 0.0f)
                { // To prevent bugs if flipper moves into ball while ball is traveling from flipper
                    Vector2 velN = normal * signedSpeedN;
                    Vector2 parallel = {-normal.y, normal.x};
                    Vector2 velP = Vector2DotProduct(ballVel, parallel) * parallel;
                    float restitution = circleCollider->owner->material.restitution;
                    ballVel = velP - velN * restitution;
                    if (auto* bumper = dynamic_cast<Bumper*>(circleCollider->owner)) {
                        ballVel += bumper->impulse * normal;
                    }
                }

                // Add extra velocity to the ball if circleCollider is the tip of a Flipper
                if (circleCollider->role != CircleCollider::CircleColliderRole::FlipperTip)
                    continue;

                Flipper* flipper = static_cast<Flipper*>(circleCollider->owner);
                if (!(flipper->physicalAngle < flipper->maxAngle && flipper->physicalAngle > flipper->minAngle))
                    continue;

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
                events.hitOwners.push_back(lineCollider->owner);

                Vector2 normal = lineCollider->line.normal;
                ballPos = p + normal * ballRad;
                float signedSpeedN = Vector2DotProduct(ballVel, normal);
                if (signedSpeedN < 0.0f) { // To prevent bugs if flipper moves into ball while ball is traveling from flipper
                    Vector2 velN = normal * signedSpeedN;
                    Vector2 parallel = Vector2Normalize(v2);
                    Vector2 velP = Vector2DotProduct(ballVel, parallel) * parallel;
                    float restitution = lineCollider->owner->material.restitution;
                    ballVel = velP - velN * restitution;
                }
                
                // Add extra velocity to ball if the lineCollider belongs to a Flipper
                if (auto* flipper = dynamic_cast<Flipper*>(lineCollider->owner))
                {
                    flipper = static_cast<Flipper*>(lineCollider->owner);
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
                events.hitOwners.push_back(ballB->circleCollider.owner);

                Vector2 normal = Vector2Normalize(posDiff);
                float penetration = ballRad + ballBRad - posDiffLen;
                ballPos += normal * 0.5f * penetration;
                ballBPos -= normal * 0.5f * penetration;
                Vector2 velN = normal * Vector2DotProduct(ballVel, normal);
                Vector2 velNB = normal * Vector2DotProduct(ballBVel, normal);
                ballVel += (velNB - velN);
                ballBVel += (velN - velNB);
            }
        }

        // Assign the ball's new position and velocity
        ball->prevPhysicalPosition = ball->circleCollider.circle.position;
        ball->circleCollider.circle.position = ballPos;
        ball->velocity = ballVel;

        eventsPerBall.push_back(events);
    }

    stepCount += 1;
    return eventsPerBall;
}
