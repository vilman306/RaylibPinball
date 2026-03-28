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
    
    Ball* ball = new Ball({gameWidth / 2.0f, 350.0f}, 10.0f, {0.0f, 0.0f}, BLUE);
    balls.push_back(ball);
    
    // Ball* ball2 = new Ball({gameWidth / 2.0f, 300.0f}, 15.0f, {0.0f, 0.0f}, RED);
    // balls.push_back(ball2);
    
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

    Vector2 rotPosL = flipperL->circleRot.circle.position;
    Vector2 rotPosR = flipperR->circleRot.circle.position;
    Color wallColor = MAGENTA;
    float d1 = 200.0f;
    float h1 = 200.0f;
    float rotRad = flipperL->rotRadius;
    Wall* w1L = AddWall({rotPosL.x - d1, rotPosL.y + h1}, rotPosL, 0.0f, rotRad, false, true, false, wallColor); // Left
    Wall* w1R = AddWall(rotPosR, {rotPosR.x + d1, rotPosR.y + h1}, rotRad, 0.0f, true, false, false, wallColor); // Right
    float d2 = 100.0f;
    float h2 = 200.0f;
    float r2 = 5.0f;
    Wall* w2L = AddWall({w1L->GetPos1().x - d2, w1L->GetPos1().y + h2}, w1L->GetPos1(), r2, 0.0f, false, false, true, wallColor);
    Wall* w2R = AddWall(w1R->GetPos2(), {w1R->GetPos2().x + d2, w1R->GetPos2().y + h2}, 0.0f, 0.0f, false, false, false, wallColor);
    float h3 = 400.0f;
    float r3 = 5.0f;
    Wall* w3 = AddWall(w2R->GetPos2(), {w2R->GetPos2().x, w2R->GetPos2().y + h3}, 0.0f, r3, false, false, true, wallColor);
    float s4 = 40.0f;
    float x4 = w3->backLineCollider->line.pos1.x;
    Wall* w4 = AddWall({x4, w3->GetPos1().y}, {x4, 0.0f}, 0.0f, 0.0f, false, false, false, wallColor);
    float s5L = 30.0f;
    float h5 = Config::gameHeight - 80.0f;
    float x5L = w2L->backLineCollider->line.pos2.x - s5L;
    float s5R = 40.0f;
    float x5R = w4->GetPos1().x + s5R;
    Wall* w5L = AddWall({x5L, h5}, {x5L, 0.0f}, 0.0f, 0.0f, false, false, false, wallColor);
    Wall* w5R = AddWall({x5R, 0.0f}, {x5R, h5}, 0.0f, 0.0f, false, false, false, wallColor);    
}

Wall* Game::AddWall(Vector2 pos1, Vector2 pos2, float circle1Rad, float circle2Rad, bool positionCircle1InPos1, bool positionCircle2InPos2, bool hasBackline, Color c)
{
    Wall* wall = new Wall(pos1, pos2, circle1Rad, circle2Rad, positionCircle1InPos1, positionCircle2InPos2, hasBackline, c);
    walls.push_back(wall);
    lineColliders.push_back(&wall->lineCollider);
    if (wall->circle1Collider)
        circleColliders.push_back(&*wall->circle1Collider);
    if (wall->circle2Collider)
        circleColliders.push_back(&*wall->circle2Collider);
    if (wall->backLineCollider)
        lineColliders.push_back(&*wall->backLineCollider);
    return wall;
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

            // for (LineCollider* line : lineColliders)
            //     line->Draw();

            // for (CircleCollider* circle : circleColliders)
            //     circle->Draw();

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


