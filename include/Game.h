#pragma once
#include "raylib.h"
#include "Ball.h"
#include "PhysicsManager.h"
#include "AudioManager.h"
#include <vector>
#include "Geometry.h"


struct Flipper
{
    Flipper(Vector2 rotP, float len, Color c, int dir);
    void Draw();
    Vector2 rotPos; // rotation point
    float length;
    Vector2 tipPos;
    const float rotRadius = 20.0f;
    const float tipRadius = 10.0f;
    Circle circleRot;
    Circle circleTip;
    Line lineUp;
    Line lineDown;
    Color color;

    float angle;
    const float minAngle = -PI / 4.0f;
    const float maxAngle = PI / 4.0f;
    float angularSpeed = 4.0f; // rad per sec
    int direction;             // 1: left flipper, -1: right flipper
};

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
    std::vector<Ball> balls;
    std::vector<Line> lines;
    std::vector<Flipper> flippers;
    float time;
};