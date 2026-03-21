#pragma once
#include "raylib.h"
#include "Ball.h"
#include "Level.h"
#include "PhysicsManager.h"
#include "AudioManager.h"
#include <vector>
#include "Line.h"

struct Circle
{
    Circle(Vector2 pos, float rad, Color c);
    void Draw();
    Color color;
    Vector2 position;
    float radius;
};

struct Flipper
{
    Flipper(Vector2 rotP, Vector2 tipP, Color c);
    void Draw();
    Vector2 rotPos; // rotation point
    Vector2 tipPos;
    float rotRadius = 20.0f;
    float tipRadius = 10.0f;
    Circle circleRot;
    Circle circleTip;
    Line lineUp;
    Line lineDown;
    Color color;

    float angle;
    float minAngle = -PI / 4.0f;
    float maxAngle = PI / 4.0f;
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
    Ball ball;
    std::vector<Line> lines;
    std::vector<Flipper> flippers;
    Level level;
    float time;
};