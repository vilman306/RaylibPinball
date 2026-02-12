#pragma once
#include "raylib.h"
class Player
{
public:
    Player();
    void Update(float dt);
    void Draw();
private:
    Vector2 position;
    Vector2 velocity;
    Color color;
};