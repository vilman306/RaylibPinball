#include "Referee.h"
#include "Colliders.h"
#include "Bumper.h"
#include "Ball.h"
#include "Game.h"

Referee::Referee(Game* g) : game(g)
{
}

void Referee::HandleEvents(std::vector<PhysicsEvents> physicsEventsPerBall)
{
    for (PhysicsEvents& physicsEvents : physicsEventsPerBall)
    {
        for (ColliderOwner* hitOwner : physicsEvents.hitOwners)
        {
            if (auto* bumper = dynamic_cast<Bumper*>(hitOwner))
                score += bumper->points;
        }

        Ball* ball = physicsEvents.ball;
        Vector2 ballPos = ball->circleCollider.circle.position;
        float ballRad = ball->circleCollider.circle.radius;
        if (ballPos.y < -ballRad) {
            roundsPlayed += 1;
            if (roundsPlayed == roundsPerGame) {
                roundsPlayed = 0;
                highScore = (score > highScore) ? score : highScore;
                score = 0;
            }
            isServing = true;
            game->ResetBall(ball);   
        }
    }
}