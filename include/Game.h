#pragma once
#include "raylib.h"
#include "Ball.h"
#include "PhysicsManager.h"
#include "AudioManager.h"
#include <vector>
#include "Colliders.h"
#include "Flipper.h"




class Game
{
public:
    Game();
    ~Game();
    void Run();
    void Update();
    void Draw();
    void GetScreenDimensions();
    // void DrawScaledRenderTexture();

private:
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
    std::vector<Flipper*> flippers;
    double time;
    const float targetFPS = 200;
};