#include "raylib.h"
#include "raymath.h"
#include <string>
#include "math.h"
#include "Game.h"
#include "Config.h"
#include "Vec2Extensions.h"
#include <iostream>
#include "Colliders.h"
#include "Wall.h"

// #define DEBUG


Game::Game()
{
    screenWidth = 1200;
    screenHeight = 800;
    float gameWidth = Config::gameWidth;
    float gameHeight = Config::gameHeight;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, Config::windowTitle);
    // MaximizeWindow();
    
    InitAudioDevice();

    SetTargetFPS(targetFPS);

    audioManager.Load();
    
    camera.target = {gameWidth / 2.0f, gameHeight / 2.0f};
    camera.rotation = 0.0f;
    
    renderTexture = LoadRenderTexture(screenWidth, screenHeight);
    shaderRenderTexture = LoadRenderTexture(screenWidth, screenHeight);
    
    shader = LoadShader(0, "../shaders/shader.fs");
    shaderResLoc = GetShaderLocation(shader, "resolution");
    
    Ball* ball = new Ball({gameWidth / 2.0f - 480.0f, 350.0f}, 10.0f, {0.0f, 0.0f}, BLUE);
    balls.push_back(ball);
    
    Ball* ball2 = new Ball({gameWidth / 2.0f, 300.0f}, 15.0f, {0.0f, 0.0f}, RED);
    balls.push_back(ball2);
    
    float flipperLen = 80.0f;
    float flipperSepDistX = 25.0f;
    float flipperHeight = 80.0f;

    Flipper* flipperL = new Flipper({gameWidth / 2.0f - (flipperLen + flipperSepDistX), flipperHeight}, // Store on heap so that adding to "flippers" won't change memory location of "flipperL"
                                    flipperLen, VIOLET, 1);
    flippers.push_back(flipperL);
    lineColliders.push_back(&flipperL->lineUp);
    lineColliders.push_back(&flipperL->lineDown);
    circleColliders.push_back(&flipperL->circleRot);
    circleColliders.push_back(&flipperL->circleTip);
    
    Flipper* flipperR = new Flipper({gameWidth / 2.0f + (flipperLen + flipperSepDistX), flipperHeight},
                                    flipperLen, VIOLET, -1);
    flippers.push_back(flipperR);
    lineColliders.push_back(&flipperR->lineUp);
    lineColliders.push_back(&flipperR->lineDown);
    circleColliders.push_back(&flipperR->circleRot);
    circleColliders.push_back(&flipperR->circleTip);

    // // TEMPORARY, WILL CREATE WALL CLASS
    float wallDist = 200.0f;
    Color wallColor = MAGENTA;
    Vector2 wallPos1 = {gameWidth / 2.0f - wallDist, 0.8f * wallDist};
    Vector2 wallPos2 = {flipperL->rotPos.x, flipperL->rotPos.y + 0.6f * flipperL->rotRadius};
    // LineCollider* lineLeftBottom = new LineCollider({gameWidth / 2.0f - wallDist, 0.8f * wallDist},
    //                                 {flipperL->rotPos.x, flipperL->rotPos.y + 0.6f * flipperL->rotRadius});
    // lineColliders.push_back(lineLeftBottom);
    
    Wall* wall = new Wall(wallPos1, wallPos2, 0.0f, 0.0f, false, wallColor);
    lineColliders.push_back(&wall->lineCollider);
    walls.push_back(wall);
}

Game::~Game()
{
    for (Ball* ball : balls)
        delete ball;
    for (Flipper* flipper : flippers)
        delete flipper;
    for (Wall* wall : walls)
        delete wall;
    UnloadRenderTexture(renderTexture);
    UnloadShader(shader);
    audioManager.Unload();
    CloseAudioDevice();
    CloseWindow();
}

void Game::Run()
{
    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }
}

void Game::Update()
{
    if (IsKeyPressed(KEY_D))
        balls[0]->velocity.x += 200.0f;
    if (IsKeyPressed(KEY_A))
        balls[0]->velocity.x -= 200.0f;
    if (IsKeyPressed(KEY_W))
        balls[0]->velocity.y += 200.0f;
    if (IsKeyPressed(KEY_S))
        balls[0]->velocity.y -= 200.0f;
    if (IsKeyPressed(KEY_SPACE)) {
        balls[0]->circleCollider.circle.position = {Config::gameWidth / 2.0f - 120.0f, 420.0f};
        balls[0]->velocity = {100.0f, 0.0f};
    }

    bool leftDown = IsKeyDown(KEY_LEFT);
    bool rightDown = IsKeyDown(KEY_RIGHT);
    for (Flipper* flipper : flippers)
    {
        if (flipper->direction == 1)
            flipper->rotateUp = leftDown;
        else
            flipper->rotateUp = rightDown;
    }

    static double prevTime = GetTime();
    time = GetTime();
    double dt = time - prevTime;
    #ifdef DEBUG
        dt = fmin(dt, Config::dtDebug);
    #endif

    static double dtSum = 0.0;
    dtSum += dt;
    double dtPhysics = PhysicsManager::dt;

    // Update physics
    std::vector<PhysicsEvents> physicsEventsPerBall;
    while (dtSum >= dtPhysics)
    {
        for (Flipper *flipper : flippers) {
            flipper->UpdatePhysics(dtPhysics);
        }
        physicsEventsPerBall = physicsManager.Update(balls, lineColliders, circleColliders);
        dtSum -= dtPhysics;
    }

    // // Play ballBounce sound if ball collided
    // if (physicsEvents.ballBounce)
    // {
    //     float ballSpeed = Vector2Length(ball.velocity);
    //     float bounceVolume = ballSpeed / PhysicsManager::MAX_BALL_SPEED;
    //     float minSoundLevel = 0.01f;
    //     if (bounceVolume >= minSoundLevel) {
    //         SetSoundVolume(audioManager.ballBounce, bounceVolume);
    //         PlaySound(audioManager.ballBounce);
    //     }
    // }

    // Lerp ball positions between its previous and current physical positions:
    float lerpFactor = dtSum / dtPhysics;
    for (Ball* ball : balls)
    {
        ball->visualPosition = Vector2Lerp(ball->prevPhysicalPosition, ball->circleCollider.circle.position, lerpFactor);
    }
    // Lerp flipper angle
    for (Flipper* flipper : flippers)
    {
        flipper->visualAngle = Lerp(flipper->prevPhysicalAngle, flipper->physicalAngle, lerpFactor);
    }


    prevTime = time;
}

void Game::GetScreenDimensions()
{
    int newWidth = GetScreenWidth();
    int newHeight = GetScreenHeight();
    if (newWidth != screenWidth || newHeight != screenHeight)
    {
        screenWidth = newWidth;
        screenHeight = newHeight;
        UnloadRenderTexture(renderTexture);
        renderTexture = LoadRenderTexture(screenWidth, screenHeight);
        UnloadRenderTexture(shaderRenderTexture);
        shaderRenderTexture = LoadRenderTexture(screenWidth, screenHeight);
        float res[2] = { (float)screenWidth, (float)screenHeight };
        SetShaderValue(shader, shaderResLoc, res, SHADER_UNIFORM_VEC2);
    }
}

void Game::Draw()
{
    GetScreenDimensions(); // Updates rendertexture if window dimensions changed

    float gameWidth = Config::gameWidth;
    float gameHeight = Config::gameHeight;

    BeginTextureMode(renderTexture);
        ClearBackground(BLACK);

        int textureWidth = renderTexture.texture.width;
        int textureHeight = renderTexture.texture.height;
        camera.offset = {textureWidth / 2.0f, textureHeight / 2.0f};
        camera.zoom = std::min(textureWidth / gameWidth, textureHeight / gameHeight);
        BeginMode2D(camera);
            // Borders
            int borderLen = 5000;
            Color borderColor = DARKGRAY;
            DrawRectangle(-borderLen, -borderLen, borderLen, 2 * borderLen, borderColor); // Left
            DrawRectangle((int)gameWidth, -borderLen, borderLen, 2 * borderLen, borderColor);    // Right
            DrawRectangle(-borderLen, -borderLen, (int)gameWidth + borderLen, borderLen, borderColor); // Up
            DrawRectangle(-borderLen, (int)gameHeight, (int)gameWidth + borderLen, borderLen, borderColor); // Bottom

            // TEMPORARY, WILL CREATE WALL CLASS
            // for (LineCollider* line : lineColliders)
            //     line->Draw();

            for (Wall* wall : walls)
                wall->Draw();
            
        EndMode2D();

        // Show fps
        std::string fps = std::to_string(GetFPS());
        DrawText(fps.c_str(), 10, 10, 15, PURPLE);
        // Show time
        DrawText(std::to_string(time).c_str(), screenWidth - 80, 10, 15, PURPLE);
    EndTextureMode();

    BeginTextureMode(shaderRenderTexture);
        BeginMode2D(camera);
            ClearBackground(BLANK);
            for (Ball* ball : balls)
                ball->Draw();

            for (Flipper* flipper : flippers)
                flipper->Draw();
        EndMode2D();
    EndTextureMode();

    BeginDrawing();
        DrawTexturePro(renderTexture.texture, {0, 0, (float)renderTexture.texture.width, -(float)renderTexture.texture.height}, {0, 0, (float)screenWidth, (float)screenHeight}, {0, 0}, 0.0f, WHITE);
        
        // BeginShaderMode(shader);
            DrawTexturePro(shaderRenderTexture.texture, {0, 0, (float)shaderRenderTexture.texture.width, -(float)shaderRenderTexture.texture.height}, {0, 0, (float)screenWidth, (float)screenHeight}, {0, 0}, 0.0f, WHITE);
        // EndShaderMode();
    EndDrawing();
}
