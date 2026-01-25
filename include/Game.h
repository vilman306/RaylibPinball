#pragma once
#include "raylib.h"
#include "Player.h"
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
    const int virtualWidth = 200;
    const int virtualHeight = 100;
    RenderTexture2D renderTexture; // Texture to draw on (which then is scaled to actual window size)
    Player player;
    Level level;
    float time;
};