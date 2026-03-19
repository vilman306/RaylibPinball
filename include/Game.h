#pragma once
#include "raylib.h"
#include "Ball.h"
#include "Level.h"
#include "PhysicsManager.h"
#include "AudioManager.h"
#include <vector>
#include "Box.h"
#include "Line.h"

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
    PhysicsManager physicsManager;
    AudioManager audioManager;
    Ball ball;
    std::vector<Box> boxes;
    std::vector<Line> lines;
    Level level;
    float time;
    float debugtime = 0; // remove
};