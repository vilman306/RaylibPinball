#pragma once
#include "raylib.h"
#include "Ball.h"
#include "PhysicsManager.h"
#include "AudioManager.h"
#include <vector>
#include "Colliders.h"
#include "Flipper.h"
#include "Wall.h"
#include "Bumper.h"
#include "Referee.h"


class Game
{
public:
    Game();
    ~Game();
    void Run();
    void ResetBall(Ball* ball);
    
private:
    void Update();
    void Draw();
    void GetScreenDimensions();
    Ball* AddBall(Vector2 pos, float rad, Vector2 vel, Color c);
    Flipper* AddFlipper(Vector2 rotP, float len, Color c, int dir);
    void AddLevelWalls(Flipper* flipperL, Flipper* flipperR, Color wallColor);
    Wall* AddWall(Vector2 pos1, Vector2 pos2, float circle1Rad, float circle2Rad, bool positionCircle1InPos1, bool positionCircle2InPos2, bool hasBackline, Color c);
    void AddLevelBumpers();
    Bumper* AddBumper(Vector2 pos, float rad, Color c);
    Camera2D camera;
    PhysicsManager physicsManager;
    AudioManager audioManager;
    RenderTexture renderTexture;
    RenderTexture shaderRenderTexture;
    Shader shader;
    int shaderResLoc; // Location to shader's resolution uniform
    int screenWidth;
    int screenHeight;
    std::vector<Ball*> balls; // movable
    std::vector<LineCollider*> lineColliders; // stationary (or attached to flippers)
    std::vector<CircleCollider*> circleColliders; // stationary (or attached to flippers)
    std::vector<Wall*> walls;
    std::vector<Flipper*> flippers;
    std::vector<Bumper*> bumpers;
    Referee referee;
    double time;
    
    Vector2 ballResetPosition;
};