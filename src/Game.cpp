#include "raylib.h"
#include "raymath.h"
#include <string>
#include "math.h"
#include "Game.h"
#include "Config.h"
#include "Vec2Extensions.h"
#include <iostream>
#include "Geometry.h"

// #define DEBUG


Game::Game()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, Config::windowTitle);
    // ToggleFullscreen();
    InitAudioDevice();
    SetTargetFPS(400);
    renderTexture = LoadRenderTexture(Config::gameWidth, Config::gameHeight);
    audioManager.Load();

    // Line *line = new Line({150.0f, 200.0f},
    //           {300.0f, 100.0f},
    //           VIOLET);
    // lines.push_back(line);

    Ball *ball = new Ball({Config::gameWidth / 2.0f - 480.0f, 350.0f}, 10.0f, {0.0f, 0.0f}, BLUE);
    balls.push_back(ball);

    Ball *ball2 = new Ball({Config::gameWidth / 2.0f, 300.0f}, 15.0f, {0.0f, 0.0f}, RED);
    balls.push_back(ball2);

    float flipperLen = 80.0f;
    float flipperSepDistX = 0.0f;
    Flipper *flipperL = new Flipper({Config::gameWidth / 2.0f - (flipperLen + flipperSepDistX), 300.0f}, // Store on heap so that adding to "flippers" won't change memory location of "flipperL"
                                    flipperLen, VIOLET, 1);
    flippers.push_back(flipperL);
    lines.push_back(&flipperL->lineUp);
    lines.push_back(&flipperL->lineDown);
    circles.push_back(&flipperL->circleRot);
    circles.push_back(&flipperL->circleTip);

    Flipper *flipperR = new Flipper({Config::gameWidth / 2.0f + (flipperLen + flipperSepDistX), 300.0f},
                                    flipperLen, VIOLET, -1);
    flippers.push_back(flipperR);
    lines.push_back(&flipperR->lineUp);
    lines.push_back(&flipperR->lineDown);
    circles.push_back(&flipperR->circleRot);
    circles.push_back(&flipperR->circleTip);

    // Circle circle({Config::gameWidth / 2.0f, 10.0f}, 40.0f, BLACK);
    // circles.push_back(circle);
}

Game::~Game()
{
    for (Flipper *flipper : flippers)
        delete flipper;
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
        balls[0]->physicalPosition = {Config::gameWidth / 2.0f - 120.0f, 420.0f};
        balls[0]->velocity = {100.0f, 0.0f};
    }

    bool leftDown = IsKeyDown(KEY_LEFT);
    bool rightDown = IsKeyDown(KEY_RIGHT);
    for (Flipper *flipper : flippers)
    {
        if (flipper->direction == 1)
            flipper->rotateUp = leftDown;
        else
            flipper->rotateUp = rightDown;
    }

    static double prevTime = GetTime();
    double time = GetTime();
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
        physicsEventsPerBall = physicsManager.Update(balls, lines, circles);
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
    for (Ball *ball : balls)
    {
        ball->circle.position = Vector2Lerp(ball->prevPhysicalPosition, ball->physicalPosition, lerpFactor);
    }
    // Lerp flipper angle
    for (Flipper *flipper : flippers)
    {
        flipper->visualAngle = Lerp(flipper->prevPhysicalAngle, flipper->physicalAngle, lerpFactor);
    }


    prevTime = time;
}

void Game::Draw()
{
    BeginTextureMode(renderTexture);
    // Draw content:
    // -----------
    ClearBackground(DARKGRAY);
    // Show fps
    std::string fps = std::to_string(GetFPS());
    DrawText(fps.c_str(), 10, 10, 15, PURPLE);
    // Show time
    time = GetTime();
    DrawText(std::to_string(time).c_str(), Config::gameWidth - 100, 10, 15, PURPLE);

    for (Ball *ball : balls)
        ball->Draw();
    
    // // Only for testing
    // for (Line *line : lines) {
    //     line->Draw();
    // }

    for (Flipper *flipper : flippers)
        flipper->Draw();

    // // Only for testing
    // for (Circle *circle : circles)
    //     circle->Draw();

    // for (Flipper &flipper : flippers)
    //     flipper.Draw();

    // -----------

    EndTextureMode();
    DrawScaledRenderTexture();
}

void Game::DrawScaledRenderTexture()
{
    BeginDrawing();
    ClearBackground(BLACK);
    float scaleX = (float)GetScreenWidth() / Config::gameWidth;
    float scaleY = (float)GetScreenHeight() / Config::gameHeight;
    float scale = std::fmin(scaleX, scaleY); // keep aspect ratio
    float scaledWidth = Config::gameWidth * scale;
    float scaledHeight = Config::gameHeight * scale;
    float offsetX = (GetScreenWidth() - scaledWidth) * 0.5f;
    float offsetY = (GetScreenHeight() - scaledHeight) * 0.5f;
    DrawTexturePro(renderTexture.texture, {0, 0, (float)renderTexture.texture.width, -(float)renderTexture.texture.height}, {offsetX, offsetY, scaledWidth, scaledHeight}, {0, 0}, 0.0f, WHITE);
    EndDrawing();
}