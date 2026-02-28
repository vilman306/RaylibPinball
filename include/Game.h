#pragma once
#include "raylib.h"
#include "Ball.h"
#include "Level.h"
class Game
{
public:
    Game();
    ~Game();
    void Run();
    void Update();
    void Draw();
    void DrawScaledRenderTexture();

private:
    RenderTexture2D renderTexture; // Texture to draw on (which then is scaled to actual window size)
    Ball ball;
    Level level;
    float time;
};