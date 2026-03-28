#pragma once
#include "raylib.h"
#include "Ball.h"
#include "PhysicsManager.h"
#include "AudioManager.h"
#include <vector>
#include "Colliders.h"
#include "Flipper.h"
#include "Wall.h"



class Game
{
public:
    Game();
    ~Game();
    void Run();
    // void DrawScaledRenderTexture();
    
private:
    void Update();
    void Draw();
    void GetScreenDimensions();
    Wall* AddWall(Vector2 pos1, Vector2 pos2, float circle1Rad, float circle2Rad, bool positionCircle1InPos1, bool positionCircle2InPos2, bool hasBackline, Color c);
    // RenderTexture2D renderTexture; // Texture to draw on (which then is scaled to actual window size)
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
    double time;
    const float targetFPS = 200;
};