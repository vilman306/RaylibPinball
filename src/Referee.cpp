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
            // Increase score if ball hit bumper
            if (auto* bumper = dynamic_cast<Bumper*>(hitOwner))
                score += bumper->points;
        }
        Ball* ball = physicsEvents.ball;
        Vector2 ballPos = ball->circleCollider.circle.position;
        float ballRad = ball->circleCollider.circle.radius;
        // Reset ball/game if ball has fallen out
        if (ballPos.y < -ballRad) {
            roundsPlayed += 1;
            // Only reset game if there are no rounds left to play
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