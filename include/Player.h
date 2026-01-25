#pragma once
#include "raylib.h"
class Player
{
public:
    Player(int gameWidth, int gameHeight);
    void Update();
    void Draw();
private:
    int gameWidth;
    int gameHeight;
    Vector2 position;
    Vector2 velocity;
    Color color;
};