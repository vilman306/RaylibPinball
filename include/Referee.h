#pragma once
#include "PhysicsManager.h"

class Game;

class Referee
{
public:
    Referee(Game* g);
    void HandleEvents(std::vector<PhysicsEvents> physicsEventsPerBall);
    int score = 0;
    const int roundsPerGame = 3;
    int roundsPlayed = 0;
    int highScore = 0;
private:
    Game* game;
};