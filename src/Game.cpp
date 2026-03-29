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


Game::Game() : referee(this)
{
    screenWidth = 1200;
    screenHeight = 800;
    float gameWidth = Config::gameWidth;
    float gameHeight = Config::gameHeight;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, Config::windowTitle);
    MaximizeWindow();
    
    InitAudioDevice();

    SetTargetFPS(Config::targetFPS);
   
    camera.target = {gameWidth / 2.0f, gameHeight / 2.0f};
    camera.rotation = 0.0f;
    
    renderTexture = LoadRenderTexture(screenWidth, screenHeight);
    shaderRenderTexture = LoadRenderTexture(screenWidth, screenHeight);
    
    shader = LoadShader(0, "../shaders/shader.fs");
    shaderResLoc = GetShaderLocation(shader, "resolution");
    
    
    Ball* ball = AddBall({gameWidth / 2.0f, 350.0f}, 10.0f, {0.0f, 0.0f}, BLUE);
    ResetBall(ball);

    float flipperLen = 80.0f;
    float flipperSepDistX = 25.0f;
    float flipperHeight = 80.0f;
    Flipper* flipperL = AddFlipper({gameWidth / 2.0f - (flipperLen + flipperSepDistX), flipperHeight}, // Store on heap so that adding to "flippers" won't change memory location of "flipperL"
                                    flipperLen, VIOLET, 1);
    
    Flipper* flipperR = AddFlipper({gameWidth / 2.0f + (flipperLen + flipperSepDistX), flipperHeight},
                                    flipperLen, VIOLET, -1);
    
    AddLevelWalls(flipperL, flipperR, MAGENTA);

    AddLevelBumpers();

}

void Game::ResetBall(Ball* ball)
{
    ball->velocity = {0.0f, 0.0f};
    Vector2 resetPos = {1395.0f, 100.0f};
    ball->resetPosition = resetPos;
    ball->circleCollider.circle.position = resetPos;
    ball->prevPhysicalPosition = resetPos;
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
    #ifdef DEBUG
        if (IsKeyPressed(KEY_D))
            balls[0]->velocity.x += 200.0f;
        if (IsKeyPressed(KEY_A))
            balls[0]->velocity.x -= 200.0f;
        if (IsKeyPressed(KEY_W))
            balls[0]->velocity.y += 200.0f;
        if (IsKeyPressed(KEY_S))
            balls[0]->velocity.y -= 200.0f;
    #endif

    // Launch/"serve" ball
    if (referee.isServing) {
        if (IsKeyPressed(KEY_SPACE)) {
            balls[0]->velocity = {0.0f, servePower};
            referee.isServing = false;
        }
    }

    // Rotate flippers
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
        referee.HandleEvents(physicsEventsPerBall);
        dtSum -= dtPhysics;
    }

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



void Game::Draw()
{
    GetScreenDimensions(); // Updates rendertextures if window dimensions changed

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
            int borderLen = 30000;
            Color borderColor = BLACK;
            DrawRectangle(-borderLen, -borderLen, borderLen, 2 * borderLen, borderColor); // Left
            DrawRectangle((int)gameWidth, -borderLen, borderLen, 2 * borderLen, borderColor);    // Right
            DrawRectangle(-borderLen, -borderLen, (int)gameWidth + borderLen, borderLen, borderColor); // Up
            DrawRectangle(-borderLen, (int)gameHeight, (int)gameWidth + borderLen, borderLen, borderColor); // Bottom

            #ifdef DEBUG
                for (LineCollider* line : lineColliders)
                    line->Draw();

                for (CircleCollider* circle : circleColliders)
                    circle->Draw();
            #endif
        EndMode2D();


        float fontScaling = std::min((float)screenWidth / gameWidth, (float)screenHeight / gameHeight);

        int fontSize1 = (int)(15 * fontScaling);
        // Show fps
        std::string fpsText = "FPS: " + std::to_string(GetFPS());
        DrawText(fpsText.c_str(), 10, 10, fontSize1, PURPLE);
        // Show time
        DrawText(std::to_string(time).c_str(), screenWidth - 130, 10, fontSize1, PURPLE);

        int fontSize2 = (int)(50 * fontScaling);
        int padding = (int)(50 * fontScaling);
        int padding2 = (int)(430 * fontScaling);
        int fontPosXLeft = padding;
        int fontPosXRight = screenWidth - padding2;
        int fontPosY = screenHeight / 2;
        // Show score
        int score = referee.score;
        std::string scoreText = "Score: " + std::to_string(score);
        DrawText(scoreText.c_str(), fontPosXLeft, fontPosY, fontSize2, PURPLE);
        // Show high score
        int highScore = referee.highScore;
        std::string highScoreText = "High score: " + std::to_string(highScore);
        DrawText(highScoreText.c_str(), fontPosXRight, fontPosY, fontSize2, PURPLE);

    EndTextureMode();

    BeginTextureMode(shaderRenderTexture);
        BeginMode2D(camera);
            ClearBackground(BLANK);
            for (Ball* ball : balls)
                ball->Draw();

            for (Wall* wall : walls)
                wall->Draw();
            
            for (Bumper* bumper : bumpers)
                bumper->Draw();
                
            for (Flipper* flipper : flippers)
                flipper->Draw();
        EndMode2D();
    EndTextureMode();

    BeginDrawing();
        DrawTexturePro(renderTexture.texture, {0, 0, (float)renderTexture.texture.width, -(float)renderTexture.texture.height}, {0, 0, (float)screenWidth, (float)screenHeight}, {0, 0}, 0.0f, WHITE);
            BeginShaderMode(shader);
                DrawTexturePro(shaderRenderTexture.texture, {0, 0, (float)shaderRenderTexture.texture.width, -(float)shaderRenderTexture.texture.height}, {0, 0, (float)screenWidth, (float)screenHeight}, {0, 0}, 0.0f, WHITE);
            EndShaderMode();
    EndDrawing();
}

void Game::GetScreenDimensions() // Is called when the screen resolution changes. Updates related variables.
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


void Game::AddLevelBumpers()
{
    float gameHeight = Config::gameHeight;
    float gameWidth = Config::gameWidth;
    float hC = gameHeight - 350.0f;
    float r = 30.0f;
    Color bumperColor = GREEN;
    Bumper* bumperC = AddBumper({gameWidth / 2.0f, hC}, r, bumperColor);
    float d = 150.0f;
    float hL = gameHeight - 200.0f;
    float hR = hL;
    Bumper* bumperL = AddBumper({gameWidth / 2.0f - d, hL}, r, bumperColor);
    Bumper* bumperR = AddBumper({gameWidth / 2.0f + d, hR}, r, bumperColor);
    Bumper* bumperLL = AddBumper({gameWidth / 2.0f - 2*d, hC}, r, bumperColor);
    Bumper* bumperRR = AddBumper({gameWidth / 2.0f + 2*d, hC}, r, bumperColor);
}

Bumper* Game::AddBumper(Vector2 pos, float rad, Color c)
{
    Bumper* bumper = new Bumper(pos, rad, c);
    bumpers.push_back(bumper);
    circleColliders.push_back(&bumper->circleCollider);
    return bumper;
}

void Game::AddLevelWalls(Flipper* flipperL, Flipper* flipperR, Color wallColor)
{
    float gameHeight = Config::gameHeight;
    Vector2 rotPosL = flipperL->circleRot.circle.position;
    Vector2 rotPosR = flipperR->circleRot.circle.position;
    float d1 = 200.0f;
    float h1 = 150.0f;
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
    float h5 = gameHeight - 80.0f;
    float x5L = w2L->backLineCollider->line.pos2.x - s5L;
    float s5R = 40.0f;
    float x5R = w4->GetPos1().x + s5R;
    Wall* w5L = AddWall({x5L, h5}, {x5L, 0.0f}, 0.0f, 0.0f, false, false, false, wallColor);
    Wall* w5R = AddWall({x5R, 0.0f}, {x5R, h5}, 0.0f, 0.0f, false, false, false, wallColor);
    float d6 = 50.0f;
    float h6 = gameHeight - 10.0f;
    Wall* w6L = AddWall({w5L->GetPos1().x + d6, h6}, w5L->GetPos1(), 0.0f, 0.0f, false, false, false, wallColor);
    Wall* w6R = AddWall(w5R->GetPos2(), {w5R->GetPos2().x - d6, h6}, 0.0f, 0.0f, false, false, false, wallColor);
    Wall* w7 = AddWall(w6R->GetPos2(), w6L->GetPos1(), 0.0f, 0.0f, false, false, false, wallColor);
    float h8 = 10.0f;
    Wall* w8 = AddWall({x4, h8}, {x5R, h8}, 0.0f, 0.0f, false, false, false, wallColor);
    float h9 = 50.0f;
    Wall* w9 = AddWall(w3->GetPos2(), {x5R, w3->GetPos2().y + h9}, 0.0f, 0.0f, false, false, false, wallColor);
    std::cout << w8->GetPos1().x << std::endl;
}

Flipper* Game::AddFlipper(Vector2 rotP, float len, Color c, int dir)
{
    Flipper* flipper = new Flipper(rotP, len, c, dir);
    flippers.push_back(flipper);
    lineColliders.push_back(&flipper->lineUp);
    lineColliders.push_back(&flipper->lineDown);
    circleColliders.push_back(&flipper->circleRot);
    circleColliders.push_back(&flipper->circleTip);
    return flipper;
}

Ball* Game::AddBall(Vector2 pos, float rad, Vector2 vel, Color c)
{
    Ball* ball = new Ball(pos, rad, vel, c);
    balls.push_back(ball);
    return ball;
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
    for (Bumper* bumper : bumpers)
        delete bumper;
    UnloadRenderTexture(renderTexture);
    UnloadShader(shader);
    CloseAudioDevice();
    CloseWindow();
}
